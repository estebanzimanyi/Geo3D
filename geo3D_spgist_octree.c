/*-------------------------------------------------------------------------
 *
 * spgoctreeproc.c
 *	  implementation of octree over points for SP-GiST
 *
 *
 * Portions Copyright (c) 1996-2015, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * IDENTIFICATION
 *			src/backend/access/spgist/spgoctreeproc.c
 *
 *-------------------------------------------------------------------------
 */

#include <postgres.h>

#include <access/spgist.h>
#include <access/stratnum.h>
#include <catalog/pg_type.h>
#include <utils/builtins.h>
#include "geo3D_decls.h"

static int spgist_octree_inner_consistent_point3D(Point3D *centroid, Point3D *query, StrategyNumber strategy);
static int spgist_octree_inner_consistent_lseg3D(Point3D *centroid, Lseg3D *query, StrategyNumber strategy);
static int spgist_octree_inner_consistent_line3D(Point3D *centroid, Line3D *query, StrategyNumber strategy);
static int spgist_octree_inner_consistent_box3D(Point3D *centroid, Box3D *query, StrategyNumber strategy);
static int spgist_octree_inner_consistent_path3D(Point3D *centroid, Path3D *query, StrategyNumber strategy);
static int spgist_octree_inner_consistent_poly3D(Point3D *centroid, Polygon3D *query, StrategyNumber strategy);
static int spgist_octree_inner_consistent_sphere(Point3D *centroid, Sphere *query, StrategyNumber strategy);

static bool spgist_octree_leaf_consistent_point3D(Point3D *key, Point3D *query, StrategyNumber strategy);
static bool spgist_octree_leaf_consistent_lseg3D(Point3D *key, Lseg3D *query, StrategyNumber strategy);
static bool spgist_octree_leaf_consistent_line3D(Point3D *key, Line3D *query, StrategyNumber strategy);
static bool spgist_octree_leaf_consistent_box3D(Point3D *key, Box3D *query, StrategyNumber strategy);
static bool spgist_octree_leaf_consistent_path3D(Point3D *key, Path3D *query, StrategyNumber strategy);
static bool spgist_octree_leaf_consistent_poly3D(Point3D *key, Polygon3D *query, StrategyNumber strategy);
static bool spgist_octree_leaf_consistent_sphere(Point3D *key, Sphere *query, StrategyNumber strategy);


Datum
spgist_octree_config(PG_FUNCTION_ARGS)
{
	/* spgConfigIn *cfgin = (spgConfigIn *) PG_GETARG_POINTER(0); */
	spgConfigOut *cfg = (spgConfigOut *) PG_GETARG_POINTER(1);

	cfg->prefixType = point3d_oid() ;
	cfg->labelType = VOIDOID;	/* we don't need node labels */
	cfg->canReturnData = true;
	cfg->longValuesOK = false;
	PG_RETURN_VOID();
}

PG_FUNCTION_INFO_V1(spgist_octree_config);

#define SPTEST(f, x, y) \
	DatumGetBool(DirectFunctionCall2(f, Point3DPGetDatum(x), Point3DPGetDatum(y)))

/*
 * Determine which octant a point falls into, relative to the centroid.
 *
 * Octants are identified like this:
 *
 *	 4	|  1      8  |  5
 *	----+-----   ----+-----
 *	 3	|  2      7  |  6
 *    FRONT         BACK
 *
 * Points on one of the axes are taken to lie in the lowest-numbered
 * adjacent octant.
 */
static int16
getOctant(Point3D *centroid, Point3D *tst)
{
	if (
		(SPTEST(front_point3D_point3D, tst, centroid) ||
		 SPTEST(point3D_perpendicular, tst, centroid)) &&
		(SPTEST(above_point3D_point3D, tst, centroid) ||
		 SPTEST(point3D_horizontal, tst, centroid)) &&
		(SPTEST(right_point3D_point3D, tst, centroid) ||
		 SPTEST(point3D_vertical, tst, centroid))
		 )
		return 1;

	if (
		(SPTEST(front_point3D_point3D, tst, centroid) ||
		 SPTEST(point3D_perpendicular, tst, centroid)) &&
		 SPTEST(below_point3D_point3D, tst, centroid) &&
		(SPTEST(right_point3D_point3D, tst, centroid) ||
		 SPTEST(point3D_vertical, tst, centroid))
		 )
		return 2;

	if (
		(SPTEST(front_point3D_point3D, tst, centroid) ||
		 SPTEST(point3D_perpendicular, tst, centroid)) &&
		(SPTEST(below_point3D_point3D, tst, centroid) ||
		 SPTEST(point3D_horizontal, tst, centroid)) &&
		 SPTEST(left_point3D_point3D, tst, centroid)
		)
		return 3;

	if (
		(SPTEST(front_point3D_point3D, tst, centroid) ||
		 SPTEST(point3D_perpendicular, tst, centroid)) &&
		SPTEST(above_point3D_point3D, tst, centroid) &&
		SPTEST(left_point3D_point3D, tst, centroid)
		)
		return 4;

	if (
		(SPTEST(above_point3D_point3D, tst, centroid) ||
		 SPTEST(point3D_horizontal, tst, centroid)) &&
		(SPTEST(right_point3D_point3D, tst, centroid) ||
		 SPTEST(point3D_vertical, tst, centroid))
		)
		return 5;

	if (
		SPTEST(below_point3D_point3D, tst, centroid) &&
		(SPTEST(right_point3D_point3D, tst, centroid) ||
		 SPTEST(point3D_vertical, tst, centroid))
		)
		return 6;

	if (
		(SPTEST(below_point3D_point3D, tst, centroid) ||
		 SPTEST(point3D_horizontal, tst, centroid)) &&
		SPTEST(left_point3D_point3D, tst, centroid)
		)
		return 7;

	if (
		SPTEST(above_point3D_point3D, tst, centroid) &&
		SPTEST(left_point3D_point3D, tst, centroid)
		)
		return 8;

	elog(ERROR, "getOctant: impossible case");
	return 0;
}


Datum
spgist_octree_choose(PG_FUNCTION_ARGS)
{
	spgChooseIn *in = (spgChooseIn *) PG_GETARG_POINTER(0);
	spgChooseOut *out = (spgChooseOut *) PG_GETARG_POINTER(1);
	Point3D	   *inPoint = DatumGetPoint3DP(in->datum),
			   *centroid;

	if (in->allTheSame)
	{
		out->resultType = spgMatchNode;
		/* nodeN will be set by core */
		out->result.matchNode.levelAdd = 0;
		out->result.matchNode.restDatum = Point3DPGetDatum(inPoint);
		PG_RETURN_VOID();
	}

	Assert(in->hasPrefix);
	centroid = DatumGetPoint3DP(in->prefixDatum);

	Assert(in->nNodes == 8);

	out->resultType = spgMatchNode;
	out->result.matchNode.nodeN = getOctant(centroid, inPoint) - 1;
	out->result.matchNode.levelAdd = 0;
	out->result.matchNode.restDatum = Point3DPGetDatum(inPoint);

	PG_RETURN_VOID();
}

PG_FUNCTION_INFO_V1(spgist_octree_choose);

#ifdef USE_MEDIAN
static int
x_cmp(const void *a, const void *b, void *arg)
{
	Point3D	   *pa = *(Point3D **) a;
	Point3D	   *pb = *(Point3D **) b;

	if (pa->x == pb->x)
		return 0;
	return (pa->x > pb->x) ? 1 : -1;
}

static int
y_cmp(const void *a, const void *b, void *arg)
{
	Point3D	   *pa = *(Point3D **) a;
	Point3D	   *pb = *(Point3D **) b;

	if (pa->y == pb->y)
		return 0;
	return (pa->y > pb->y) ? 1 : -1;
}

static int
z_cmp(const void *a, const void *b, void *arg)
{
	Point3D	   *pa = *(Point3D **) a;
	Point3D	   *pb = *(Point3D **) b;

	if (pa->z == pb->z)
		return 0;
	return (pa->z > pb->z) ? 1 : -1;
}
#endif

Datum
spgist_octree_picksplit(PG_FUNCTION_ARGS)
{
	spgPickSplitIn *in = (spgPickSplitIn *) PG_GETARG_POINTER(0);
	spgPickSplitOut *out = (spgPickSplitOut *) PG_GETARG_POINTER(1);
	int			i;
	Point3D	   *centroid;

#ifdef USE_MEDIAN
	/* Use the median values of x, y, and z as the centroid point */
	Point3D	  **sorted;

	sorted = palloc(sizeof(*sorted) * in->nTuples);
	for (i = 0; i < in->nTuples; i++)
		sorted[i] = DatumGetPoint3DP(in->datums[i]);

	centroid = palloc(sizeof(*centroid));

	qsort(sorted, in->nTuples, sizeof(*sorted), x_cmp);
	centroid->x = sorted[in->nTuples >> 1]->x;
	qsort(sorted, in->nTuples, sizeof(*sorted), y_cmp);
	centroid->y = sorted[in->nTuples >> 1]->y;
	qsort(sorted, in->nTuples, sizeof(*sorted), z_cmp);
	centroid->z = sorted[in->nTuples >> 1]->z;
#else
	/* Use the average values of x and y as the centroid point */
	centroid = palloc0(sizeof(*centroid));

	for (i = 0; i < in->nTuples; i++)
	{
		centroid->x += DatumGetPoint3DP(in->datums[i])->x;
		centroid->y += DatumGetPoint3DP(in->datums[i])->y;
		centroid->z += DatumGetPoint3DP(in->datums[i])->z;
	}

	centroid->x /= in->nTuples;
	centroid->y /= in->nTuples;
	centroid->z /= in->nTuples;
#endif

	out->hasPrefix = true;
	out->prefixDatum = Point3DPGetDatum(centroid);

	out->nNodes = 8;
	out->nodeLabels = NULL;		/* we don't need node labels */

	out->mapTuplesToNodes = palloc(sizeof(int) * in->nTuples);
	out->leafTupleDatums = palloc(sizeof(Datum) * in->nTuples);

	for (i = 0; i < in->nTuples; i++)
	{
		Point3D	   *p = DatumGetPoint3DP(in->datums[i]);
		int			octant = getOctant(centroid, p) - 1;

		out->leafTupleDatums[i] = Point3DPGetDatum(p);
		out->mapTuplesToNodes[i] = octant;
	}

	PG_RETURN_VOID();
}

PG_FUNCTION_INFO_V1(spgist_octree_picksplit);

Datum
spgist_octree_inner_consistent(PG_FUNCTION_ARGS)
{
	spgInnerConsistentIn *in = (spgInnerConsistentIn *) PG_GETARG_POINTER(0);
	spgInnerConsistentOut *out = (spgInnerConsistentOut *) PG_GETARG_POINTER(1);
	Point3D	   *centroid;
	int			which;
	int			i;

	Assert(in->hasPrefix);
	centroid = DatumGetPoint3DP(in->prefixDatum);

	if (in->allTheSame)
	{
		/* Report that all nodes should be visited */
		out->nNodes = in->nNodes;
		out->nodeNumbers = (int *) palloc(sizeof(int) * in->nNodes);
		for (i = 0; i < in->nNodes; i++)
			out->nodeNumbers[i] = i;
		PG_RETURN_VOID();
	}

	Assert(in->nNodes == 8);
    /* "which" is a bitmask of octants that satisfy all constraints */
    which = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8);

	for (i = 0; i < in->nkeys; i++)
	{
        StrategyNumber strategy = in->scankeys[i].sk_strategy;
        StrategyNumber strategyGroup = strategy / Geo3DStrategyNumberOffset;
        
		switch (strategyGroup)
		{
            case Point3DStrategyNumberGroup:
                which = spgist_octree_inner_consistent_point3D(centroid, DatumGetPoint3DP(in->scankeys[i].sk_argument),
                    strategy % Geo3DStrategyNumberOffset);
                break;
            case Lseg3DStrategyNumberGroup:
                which = spgist_octree_inner_consistent_lseg3D(centroid, DatumGetLseg3DP(in->scankeys[i].sk_argument),
                    strategy % Geo3DStrategyNumberOffset);
                break;
            case Line3DStrategyNumberGroup:
                which = spgist_octree_inner_consistent_line3D(centroid, DatumGetLine3DP(in->scankeys[i].sk_argument),
                    strategy % Geo3DStrategyNumberOffset);
                break;
            case Box3DStrategyNumberGroup:
                which = spgist_octree_inner_consistent_box3D(centroid, DatumGetBox3DP(in->scankeys[i].sk_argument),
                    strategy % Geo3DStrategyNumberOffset);
                break;
            case Path3DStrategyNumberGroup:
                which = spgist_octree_inner_consistent_path3D(centroid, DatumGetPath3DP(in->scankeys[i].sk_argument),
                    strategy % Geo3DStrategyNumberOffset);
                break;
            case Polygon3DStrategyNumberGroup:
                which = spgist_octree_inner_consistent_poly3D(centroid, DatumGetPolygon3DP(in->scankeys[i].sk_argument),
                    strategy % Geo3DStrategyNumberOffset);
                break;
            case SphereStrategyNumberGroup:
                which = spgist_octree_inner_consistent_sphere(centroid, DatumGetSphereP(in->scankeys[i].sk_argument),
                    strategy % Geo3DStrategyNumberOffset);
                break;
		}

		if (which == 0)
			break;				/* no need to consider remaining conditions */
	}

	/* We must descend into the octant(s) identified by which */
	out->nodeNumbers = (int *) palloc(sizeof(int) * 8);
	out->nNodes = 0;
	for (i = 1; i <= 8; i++)
	{
		if (which & (1 << i))
			out->nodeNumbers[out->nNodes++] = i - 1;
	}

	PG_RETURN_VOID();
}

PG_FUNCTION_INFO_V1(spgist_octree_inner_consistent);

Datum
spgist_octree_leaf_consistent(PG_FUNCTION_ARGS)
{
	spgLeafConsistentIn *in = (spgLeafConsistentIn *) PG_GETARG_POINTER(0);
	spgLeafConsistentOut *out = (spgLeafConsistentOut *) PG_GETARG_POINTER(1);
	Point3D	   *datum = DatumGetPoint3DP(in->leafDatum);
	bool		res;
	int			i;

	/* all tests are exact */
	out->recheck = false;

	/* leafDatum is what it is... */
	out->leafValue = in->leafDatum;

	/* Perform the required comparison(s) */
	res = true;
	for (i = 0; i < in->nkeys; i++)
	{
        StrategyNumber strategy = in->scankeys[i].sk_strategy;
        StrategyNumber strategyGroup = strategy / Geo3DStrategyNumberOffset;

		switch (strategyGroup)
		{
            case Point3DStrategyNumberGroup:
                res = spgist_octree_leaf_consistent_point3D(datum, DatumGetPoint3DP(in->scankeys[i].sk_argument),
                        strategy % Geo3DStrategyNumberOffset);
                break;
            case Lseg3DStrategyNumberGroup:
                res = spgist_octree_leaf_consistent_lseg3D(datum, DatumGetLseg3DP(in->scankeys[i].sk_argument),
                        strategy % Geo3DStrategyNumberOffset);
                break;
            case Line3DStrategyNumberGroup:
                res = spgist_octree_leaf_consistent_line3D(datum, DatumGetLine3DP(in->scankeys[i].sk_argument),
                        strategy % Geo3DStrategyNumberOffset);
                break;
            case Box3DStrategyNumberGroup:
                res = spgist_octree_leaf_consistent_box3D(datum, DatumGetBox3DP(in->scankeys[i].sk_argument),
                        strategy % Geo3DStrategyNumberOffset);
                break;
            case Path3DStrategyNumberGroup:
                res = spgist_octree_leaf_consistent_path3D(datum, DatumGetPath3DP(in->scankeys[i].sk_argument),
                        strategy % Geo3DStrategyNumberOffset);
                break;
            case Polygon3DStrategyNumberGroup:
                res = spgist_octree_leaf_consistent_poly3D(datum, DatumGetPolygon3DP(in->scankeys[i].sk_argument),
                        strategy % Geo3DStrategyNumberOffset);
                break;
            case SphereStrategyNumberGroup:
                res = spgist_octree_leaf_consistent_sphere(datum, DatumGetSphereP(in->scankeys[i].sk_argument),
                        strategy % Geo3DStrategyNumberOffset);
                break;
		}

		if (!res)
			break;
	}

	PG_RETURN_BOOL(res);
}

PG_FUNCTION_INFO_V1(spgist_octree_leaf_consistent);

/*
 *  Inner consistency for Point3D
 */
static int
spgist_octree_inner_consistent_point3D(Point3D *centroid, Point3D *query, StrategyNumber strategy)
{
    int			which;
    
    /* "which" is a bitmask of octants that satisfy all constraints */
    which = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8);
    
    switch (strategy)
    {
        case RTLeftStrategyNumber:
            if (right_point3D_point3D_internal(centroid, query))
                which &= (1 << 3) | (1 << 4) | (1 << 7) | (1 << 8);
            break;
        case RTRightStrategyNumber:
            if (left_point3D_point3D_internal(centroid, query))
                which &= (1 << 1) | (1 << 2) | (1 << 5) | (1 << 6);
            break;
        case RTBelowStrategyNumber:
            if (above_point3D_point3D_internal(centroid, query))
                which &= (1 << 2) | (1 << 3) | (1 << 6) | (1 << 7);
            break;
        case RTAboveStrategyNumber:
            if (below_point3D_point3D_internal(centroid, query))
                which &= (1 << 1) | (1 << 4) | (1 << 5) | (1 << 8);
            break;
        case RTFrontStrategyNumber:
            if (back_point3D_point3D_internal(centroid, query))
                which &= (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4);
            break;
        case RTBackStrategyNumber:
            if (front_point3D_point3D_internal(centroid, query))
                which &= (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8);
            break;
        case RTSameStrategyNumber:
            which &= (1 << getOctant(centroid, query));
            break;
        default:
            elog(ERROR, "unrecognized strategy number: %d", strategy);
            which = 0;
            break;
    }
    
    return which;
}

/*
 *  Inner consistency for Lseg3D
 */
static int
spgist_octree_inner_consistent_lseg3D(Point3D *centroid, Lseg3D *query, StrategyNumber strategy)
{
    int			which;
    
    /* "which" is a bitmask of octants that satisfy all constraints */
    which = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8);
    
    switch (strategy)
    {
        case RTLeftStrategyNumber:
            if (right_point3D_lseg3D_internal(centroid, query))
                which &= (1 << 3) | (1 << 4) | (1 << 7) | (1 << 8);
            break;
        case RTRightStrategyNumber:
            if (left_point3D_lseg3D_internal(centroid, query))
                which &= (1 << 1) | (1 << 2) | (1 << 5) | (1 << 6);
            break;
        case RTBelowStrategyNumber:
            if (above_point3D_lseg3D_internal(centroid, query))
                which &= (1 << 2) | (1 << 3) | (1 << 6) | (1 << 7);
            break;
        case RTAboveStrategyNumber:
            if (below_point3D_lseg3D_internal(centroid, query))
                which &= (1 << 1) | (1 << 4) | (1 << 5) | (1 << 8);
            break;
        case RTFrontStrategyNumber:
            if (back_point3D_lseg3D_internal(centroid, query))
                which &= (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4);
            break;
        case RTBackStrategyNumber:
            if (front_point3D_lseg3D_internal(centroid, query))
                which &= (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8);
            break;
        case RTContainedByStrategyNumber:
        {
            double      x, y, z;
            double      x0 = query->p[0].x, x1 = query->p[1].x;
            double      y0 = query->p[0].y, y1 = query->p[1].y;
            double      z0 = query->p[0].z, z1 = query->p[1].z;
            int         r = (1 << getOctant(centroid, &query->p[0]) | (1 << getOctant(centroid, &query->p[1])));
            
            /* if centroid is contained in lseg */
            if(contain_lseg3D_point3D_internal(query, centroid))
            {
                r |= (1 << 1);
                which &= r;
                break;
            }
            
            if((FPle(x0, centroid->x) && FPge(x1, centroid->x)) || (FPge(x0, centroid->x) && FPle(x1, centroid->x)))
            {
                /* the crossing point with surface x = centroid->x */
                x = centroid->x;
                y = y0 + (y1 - y0) * (x - x0) / (x1 - x0);
                z = z0 + (z1 - z0) * (x - x0) / (x1 - x0);
                
                /* if the acrossing point is on the axis */
                if(FPeq(y, centroid->y) && FPgt(z, centroid->z))
                    r |= (1 << 5 | 1 << 6 | 1 << 7 | 1 << 8);
                else if(FPeq(y, centroid->y) && FPlt(z, centroid->z))
                    r |= (1 << 1 | 1 << 2 | 1 << 3 | 1 << 4);
                else if(FPgt(y, centroid->y) && FPeq(z, centroid->z))
                    r |= (1 << 1 | 1 << 4 | 1 << 5 | 1 << 8);
                else if(FPlt(y, centroid->y) && FPeq(z, centroid->z))
                    r |= (1 << 2 | 1 << 3 | 1 << 6 | 1 << 7);
                
                /* otherwise */
                else if(FPgt(y, centroid->y) && FPgt(z, centroid->z))
                    r |= (1 << 5 | 1 << 8);
                else if(FPgt(y, centroid->y) && FPlt(z, centroid->z))
                    r |= (1 << 1 | 1 << 4);
                else if(FPlt(y, centroid->y) && FPgt(z, centroid->z))
                    r |= (1 << 6 | 1 << 7);
                else if(FPlt(y, centroid->y) && FPlt(z, centroid->z))
                    r |= (1 << 2 | 1 << 3);
            }
            
            if((FPle(y0, centroid->y) && FPge(y1, centroid->y)) || (FPge(y0, centroid->y) && FPle(y1, centroid->y)))
            {
                /* the crossing point with surface y = centroid->y */
                y = centroid->y;
                x = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
                z = z0 + (z1 - z0) * (y - y0) / (y1 - y0);
                
                /* if the crossing point is on the axis */
                if(FPeq(x, centroid->x) && FPgt(z, centroid->z))
                    r |= (1 << 5 | 1 << 6 | 1 << 7 | 1 << 8);
                else if(FPeq(x, centroid->x) && FPlt(z, centroid->z))
                    r |= (1 << 1 | 1 << 2 | 1 << 3 | 1 << 4);
                else if(FPgt(x, centroid->x) && FPeq(z, centroid->z))
                    r |= (1 << 1 | 1 << 2 | 1 << 5 | 1 << 6);
                else if(FPlt(x, centroid->x) && FPeq(z, centroid->z))
                    r |= (1 << 3 | 1 << 4 | 1 << 7 | 1 << 8);
                
                /* otherwise */
                else if(FPgt(x, centroid->x) && FPgt(z, centroid->z))
                    r |= (1 << 5 | 1 << 6);
                else if(FPgt(x, centroid->x) && FPlt(z, centroid->z))
                    r |= (1 << 1 | 1 << 2);
                else if(FPlt(x, centroid->x) && FPgt(z, centroid->z))
                    r |= (1 << 7 | 1 << 8);
                else if(FPlt(x, centroid->x) && FPlt(z, centroid->z))
                    r |= (1 << 3 | 1 << 4);
            }
            
            if((FPle(z0, centroid->z) && FPge(z1, centroid->z)) || (FPge(z0, centroid->z) && FPle(z1, centroid->z)))
            {
                /* the crossing point with surface z = centroid->z */
                z = centroid->z;
                x = x0 + (x1 - x0) * (z - z0) / (z1 - z0);
                y = y0 + (y1 - y0) * (z - z0) / (z1 - z0);
                
                /* if the crossing point is on the axis */
                if(FPeq(x, centroid->x) && FPgt(y, centroid->y))
                    r |= (1 << 1 | 1 << 4 | 1 << 5 | 1 << 8);
                else if(FPeq(x, centroid->x) && FPlt(y, centroid->y))
                    r |= (1 << 2 | 1 << 3 | 1 << 6 | 1 << 7);
                else if(FPgt(x, centroid->x) && FPeq(y, centroid->y))
                    r |= (1 << 1 | 1 << 2 | 1 << 5 | 1 << 6);
                else if(FPlt(x, centroid->x) && FPeq(y, centroid->y))
                    r |= (1 << 3 | 1 << 4 | 1 << 7 | 1 << 8);
                
                /* otherwise */
                else if(FPgt(x, centroid->x) && FPgt(y, centroid->y))
                    r |= (1 << 1 | 1 << 5);
                else if(FPgt(x, centroid->x) && FPlt(y, centroid->y))
                    r |= (1 << 2 | 1 << 6);
                else if(FPlt(x, centroid->x) && FPgt(y, centroid->y))
                    r |= (1 << 4 | 1 << 8);
                else if(FPlt(x, centroid->x) && FPlt(y, centroid->y))
                    r |= (1 << 3 | 1 << 7);
            }

            which &= r;
            break;
        }
        default:
            elog(ERROR, "unrecognized strategy number: %d", strategy);
            which = 0;
            break;
    }
    
    return which;
}

/*
 *  Inner consistency for Line3D
 */
static int
spgist_octree_inner_consistent_line3D(Point3D *centroid, Line3D *query, StrategyNumber strategy)
{
    int			which;
    
    /* "which" is a bitmask of octants that satisfy all constraints */
    which = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8);
    
    switch (strategy)
    {
        case RTLeftStrategyNumber:
            if(!line3D_vert(query))
                which = 0;
            else if (right_point3D_line3D_internal(centroid, query))
                which &= (1 << 3) | (1 << 4) | (1 << 7) | (1 << 8);
            break;
        case RTRightStrategyNumber:
            if(!line3D_vert(query))
                which = 0;
            else if (left_point3D_line3D_internal(centroid, query))
                which &= (1 << 1) | (1 << 2) | (1 << 5) | (1 << 6);
            break;
        case RTBelowStrategyNumber:
            if(!line3D_hori(query))
                which = 0;
            else if (above_point3D_line3D_internal(centroid, query))
                which &= (1 << 2) | (1 << 3) | (1 << 6) | (1 << 7);
            break;
        case RTAboveStrategyNumber:
            if(!line3D_hori(query))
                which = 0;
            else if (below_point3D_line3D_internal(centroid, query))
                which &= (1 << 1) | (1 << 4) | (1 << 5) | (1 << 8);
            break;
        case RTFrontStrategyNumber:
            if(!line3D_perp(query))
                which = 0;
            else if (back_point3D_line3D_internal(centroid, query))
                which &= (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4);
            break;
        case RTBackStrategyNumber:
            if(!line3D_perp(query))
                which = 0;
            else if (front_point3D_line3D_internal(centroid, query))
                which &= (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8);
            break;
        case RTContainedByStrategyNumber:
        {
            /* if centroid is contained in line, return the octants of p[0](p[1]) and its symmetry */
            if(contain_line3D_point3D_internal(query, centroid))
            {
                Point3D p = query->p[0];
                if(point3D_same_internal(centroid, &query->p[0]))
                    p = query->p[1];
                
                /* q is symmetry of p around centroid */
                Point3D q;
                q.x = 2 * centroid->x - p.x;
                q.y = 2 * centroid->y - p.y;
                q.z = 2 * centroid->z - p.z;
                
                which &= (1 << getOctant(centroid, &p) | 1 << getOctant(centroid, &q) | 1 << 1);
                break;
            }
            
            double      x, y, z;
            double      x0 = query->p[0].x, x1 = query->p[1].x;
            double      y0 = query->p[0].y, y1 = query->p[1].y;
            double      z0 = query->p[0].z, z1 = query->p[1].z;
            int         r = 0;
            
            if(!line3D_vert(query))
            {
                /* the crossing point with surface x = centroid->x */
                x = centroid->x;
                y = y0 + (y1 - y0) * (x - x0) / (x1 - x0);
                z = z0 + (z1 - z0) * (x - x0) / (x1 - x0);
                
                /* if the acrossing point is on the axis */
                if(FPeq(y, centroid->y) && FPgt(z, centroid->z))
                    r |= (1 << 5 | 1 << 6 | 1 << 7 | 1 << 8);
                else if(FPeq(y, centroid->y) && FPlt(z, centroid->z))
                    r |= (1 << 1 | 1 << 2 | 1 << 3 | 1 << 4);
                else if(FPgt(y, centroid->y) && FPeq(z, centroid->z))
                    r |= (1 << 1 | 1 << 4 | 1 << 5 | 1 << 8);
                else if(FPlt(y, centroid->y) && FPeq(z, centroid->z))
                    r |= (1 << 2 | 1 << 3 | 1 << 6 | 1 << 7);
                
                /* otherwise */
                else if(FPgt(y, centroid->y) && FPgt(z, centroid->z))
                    r |= (1 << 5 | 1 << 8);
                else if(FPgt(y, centroid->y) && FPlt(z, centroid->z))
                    r |= (1 << 1 | 1 << 4);
                else if(FPlt(y, centroid->y) && FPgt(z, centroid->z))
                    r |= (1 << 6 | 1 << 7);
                else if(FPlt(y, centroid->y) && FPlt(z, centroid->z))
                    r |= (1 << 2 | 1 << 3);
            }
            
            if(!line3D_hori(query))
            {
                /* the crossing point with surface y = centroid->y */
                y = centroid->y;
                x = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
                z = z0 + (z1 - z0) * (y - y0) / (y1 - y0);
                
                /* if the crossing point is on the axis */
                if(FPeq(x, centroid->x) && FPgt(z, centroid->z))
                    r |= (1 << 5 | 1 << 6 | 1 << 7 | 1 << 8);
                else if(FPeq(x, centroid->x) && FPlt(z, centroid->z))
                    r |= (1 << 1 | 1 << 2 | 1 << 3 | 1 << 4);
                else if(FPgt(x, centroid->x) && FPeq(z, centroid->z))
                    r |= (1 << 1 | 1 << 2 | 1 << 5 | 1 << 6);
                else if(FPlt(x, centroid->x) && FPeq(z, centroid->z))
                    r |= (1 << 3 | 1 << 4 | 1 << 7 | 1 << 8);
                
                /* otherwise */
                else if(FPgt(x, centroid->x) && FPgt(z, centroid->z))
                    r |= (1 << 5 | 1 << 6);
                else if(FPgt(x, centroid->x) && FPlt(z, centroid->z))
                    r |= (1 << 1 | 1 << 2);
                else if(FPlt(x, centroid->x) && FPgt(z, centroid->z))
                    r |= (1 << 7 | 1 << 8);
                else if(FPlt(x, centroid->x) && FPlt(z, centroid->z))
                    r |= (1 << 3 | 1 << 4);
            }
            
            if(!line3D_perp(query))
            {
                /* the crossing point with surface z = centroid->z */
                z = centroid->z;
                x = x0 + (x1 - x0) * (z - z0) / (z1 - z0);
                y = y0 + (y1 - y0) * (z - z0) / (z1 - z0);
                
                /* if the crossing point is on the axis */
                if(FPeq(x, centroid->x) && FPgt(y, centroid->y))
                    r |= (1 << 1 | 1 << 4 | 1 << 5 | 1 << 8);
                else if(FPeq(x, centroid->x) && FPlt(y, centroid->y))
                    r |= (1 << 2 | 1 << 3 | 1 << 6 | 1 << 7);
                else if(FPgt(x, centroid->x) && FPeq(y, centroid->y))
                    r |= (1 << 1 | 1 << 2 | 1 << 5 | 1 << 6);
                else if(FPlt(x, centroid->x) && FPeq(y, centroid->y))
                    r |= (1 << 3 | 1 << 4 | 1 << 7 | 1 << 8);
                
                /* otherwise */
                else if(FPgt(x, centroid->x) && FPgt(y, centroid->y))
                    r |= (1 << 1 | 1 << 5);
                else if(FPgt(x, centroid->x) && FPlt(y, centroid->y))
                    r |= (1 << 2 | 1 << 6);
                else if(FPlt(x, centroid->x) && FPgt(y, centroid->y))
                    r |= (1 << 4 | 1 << 8);
                else if(FPlt(x, centroid->x) && FPlt(y, centroid->y))
                    r |= (1 << 3 | 1 << 7);
            }
            
            which &= r;
            break;
        }
        default:
            elog(ERROR, "unrecognized strategy number: %d", strategy);
            which = 0;
            break;
    }
    
    return which;
}

/*
 *  Inner consistency for Box3D
 */
static int
spgist_octree_inner_consistent_box3D(Point3D *centroid, Box3D *query, StrategyNumber strategy)
{
    int			which;
    
    /* "which" is a bitmask of octants that satisfy all constraints */
    which = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8);
    
    switch (strategy)
    {
        case RTLeftStrategyNumber:
            if (right_point3D_box3D_internal(centroid, query))
                which &= (1 << 3) | (1 << 4) | (1 << 7) | (1 << 8);
            break;
        case RTRightStrategyNumber:
            if (left_point3D_box3D_internal(centroid, query))
                which &= (1 << 1) | (1 << 2) | (1 << 5) | (1 << 6);
            break;
        case RTBelowStrategyNumber:
            if (above_point3D_box3D_internal(centroid, query))
                which &= (1 << 2) | (1 << 3) | (1 << 6) | (1 << 7);
            break;
        case RTAboveStrategyNumber:
            if (below_point3D_box3D_internal(centroid, query))
                which &= (1 << 1) | (1 << 4) | (1 << 5) | (1 << 8);
            break;
        case RTFrontStrategyNumber:
            if (back_point3D_box3D_internal(centroid, query))
                which &= (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4);
            break;
        case RTBackStrategyNumber:
            if (front_point3D_box3D_internal(centroid, query))
                which &= (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8);
            break;
        case RTContainedByStrategyNumber:
            if (contained_point3D_box3D_internal(centroid, query))
            {
                /* centroid is in box, so all octants are OK */
            }
            else
            {
                /* identify octant(s) containing all corners of box */
                Point3D		p;
                int			r = 0;
                
                p = query->low;
                r |= 1 << getOctant(centroid, &p);
                p.y = query->high.y;
                r |= 1 << getOctant(centroid, &p);
                p.z = query->high.z;
                r |= 1 << getOctant(centroid, &p);
                p.y = query->low.y;
                r |= 1 << getOctant(centroid, &p);
                
                p = query->high;
                r |= 1 << getOctant(centroid, &p);
                p.y = query->low.y;
                r |= 1 << getOctant(centroid, &p);
                p.z = query->low.z;
                r |= 1 << getOctant(centroid, &p);
                p.y = query->high.y;
                r |= 1 << getOctant(centroid, &p);
                
                which &= r;
            }

            break;
        default:
            elog(ERROR, "unrecognized strategy number: %d", strategy);
            which = 0;
            break;
    }
    
    return which;
}

/*
 *  Inner consistency for Path3D
 */
static int
spgist_octree_inner_consistent_path3D(Point3D *centroid, Path3D *query, StrategyNumber strategy)
{
    int			which;
    
    /* "which" is a bitmask of octants that satisfy all constraints */
    which = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8);
    
    switch (strategy)
    {
        case RTLeftStrategyNumber:
            if (right_point3D_path3D_internal(centroid, query))
                which &= (1 << 3) | (1 << 4) | (1 << 7) | (1 << 8);
            break;
        case RTRightStrategyNumber:
            if (left_point3D_path3D_internal(centroid, query))
                which &= (1 << 1) | (1 << 2) | (1 << 5) | (1 << 6);
            break;
        case RTBelowStrategyNumber:
            if (above_point3D_path3D_internal(centroid, query))
                which &= (1 << 2) | (1 << 3) | (1 << 6) | (1 << 7);
            break;
        case RTAboveStrategyNumber:
            if (below_point3D_path3D_internal(centroid, query))
                which &= (1 << 1) | (1 << 4) | (1 << 5) | (1 << 8);
            break;
        case RTFrontStrategyNumber:
            if (back_point3D_path3D_internal(centroid, query))
                which &= (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4);
            break;
        case RTBackStrategyNumber:
            if (front_point3D_path3D_internal(centroid, query))
                which &= (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8);
            break;
        case RTContainedByStrategyNumber:
        {
            Box3D *boxquery = &query->boundbox;
            if (contained_point3D_box3D_internal(centroid, boxquery))
            {
                /* centroid is in boundbox, so all octants are OK */
            }
            else
            {
                /* identify octant(s) containing all corners of boundbox */
                Point3D		p;
                int			r = 0;
                
                p = boxquery->low;
                r |= 1 << getOctant(centroid, &p);
                p.y = boxquery->high.y;
                r |= 1 << getOctant(centroid, &p);
                p.z = boxquery->high.z;
                r |= 1 << getOctant(centroid, &p);
                p.y = boxquery->low.y;
                r |= 1 << getOctant(centroid, &p);
                
                p = boxquery->high;
                r |= 1 << getOctant(centroid, &p);
                p.y = boxquery->low.y;
                r |= 1 << getOctant(centroid, &p);
                p.z = boxquery->low.z;
                r |= 1 << getOctant(centroid, &p);
                p.y = boxquery->high.y;
                r |= 1 << getOctant(centroid, &p);
                
                which &= r;
            }
            
            break;
        }
        default:
            elog(ERROR, "unrecognized strategy number: %d", strategy);
            which = 0;
            break;
    }
    
    return which;
}

/*
 *  Inner consistency for Polygon3D
 */
static int
spgist_octree_inner_consistent_poly3D(Point3D *centroid, Polygon3D *query, StrategyNumber strategy)
{
    int			which;
    
    /* "which" is a bitmask of octants that satisfy all constraints */
    which = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8);
    
    switch (strategy)
    {
        case RTLeftStrategyNumber:
            if (right_point3D_poly3D_internal(centroid, query))
                which &= (1 << 3) | (1 << 4) | (1 << 7) | (1 << 8);
            break;
        case RTRightStrategyNumber:
            if (left_point3D_poly3D_internal(centroid, query))
                which &= (1 << 1) | (1 << 2) | (1 << 5) | (1 << 6);
            break;
        case RTBelowStrategyNumber:
            if (above_point3D_poly3D_internal(centroid, query))
                which &= (1 << 2) | (1 << 3) | (1 << 6) | (1 << 7);
            break;
        case RTAboveStrategyNumber:
            if (below_point3D_poly3D_internal(centroid, query))
                which &= (1 << 1) | (1 << 4) | (1 << 5) | (1 << 8);
            break;
        case RTFrontStrategyNumber:
            if (back_point3D_poly3D_internal(centroid, query))
                which &= (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4);
            break;
        case RTBackStrategyNumber:
            if (front_point3D_poly3D_internal(centroid, query))
                which &= (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8);
            break;
        case RTContainedByStrategyNumber:
        {
            Box3D *boxquery = &query->boundbox;
            if (contained_point3D_box3D_internal(centroid, boxquery))
            {
                /* centroid is in boundbox, so all octants are OK */
            }
            else
            {
                /* identify octant(s) containing all corners of boundbox */
                Point3D		p;
                int			r = 0;
                
                p = boxquery->low;
                r |= 1 << getOctant(centroid, &p);
                p.y = boxquery->high.y;
                r |= 1 << getOctant(centroid, &p);
                p.z = boxquery->high.z;
                r |= 1 << getOctant(centroid, &p);
                p.y = boxquery->low.y;
                r |= 1 << getOctant(centroid, &p);
                
                p = boxquery->high;
                r |= 1 << getOctant(centroid, &p);
                p.y = boxquery->low.y;
                r |= 1 << getOctant(centroid, &p);
                p.z = boxquery->low.z;
                r |= 1 << getOctant(centroid, &p);
                p.y = boxquery->high.y;
                r |= 1 << getOctant(centroid, &p);
                
                which &= r;
            }
            
            break;
        }
        default:
            elog(ERROR, "unrecognized strategy number: %d", strategy);
            which = 0;
            break;
    }
    
    return which;
}

/*
 *  Inner consistency for Sphere
 */
static int
spgist_octree_inner_consistent_sphere(Point3D *centroid, Sphere *query, StrategyNumber strategy)
{
    int			which;
    
    /* "which" is a bitmask of octants that satisfy all constraints */
    which = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8);
    
    switch (strategy)
    {
        case RTLeftStrategyNumber:
            if (right_point3D_sphere_internal(centroid, query))
                which &= (1 << 3) | (1 << 4) | (1 << 7) | (1 << 8);
            break;
        case RTRightStrategyNumber:
            if (left_point3D_sphere_internal(centroid, query))
                which &= (1 << 1) | (1 << 2) | (1 << 5) | (1 << 6);
            break;
        case RTBelowStrategyNumber:
            if (above_point3D_sphere_internal(centroid, query))
                which &= (1 << 2) | (1 << 3) | (1 << 6) | (1 << 7);
            break;
        case RTAboveStrategyNumber:
            if (below_point3D_sphere_internal(centroid, query))
                which &= (1 << 1) | (1 << 4) | (1 << 5) | (1 << 8);
            break;
        case RTFrontStrategyNumber:
            if (back_point3D_sphere_internal(centroid, query))
                which &= (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4);
            break;
        case RTBackStrategyNumber:
            if (front_point3D_sphere_internal(centroid, query))
                which &= (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8);
            break;
        case RTContainedByStrategyNumber:
            if (contained_point3D_sphere_internal(centroid, query))
            {
                /* centroid is in sphere, so all octants are OK */
            }
            else
            {
                /* identify octant(s) containing all extremums of sphere */
                Point3D		p;
                int			r = 0;
                
                p = query->center;
                p.x = query->center.x + query->radius;
                r |= 1 << getOctant(centroid, &p);
                p.x = query->center.x - query->radius;
                r |= 1 << getOctant(centroid, &p);
                
                p = query->center;
                p.y = query->center.y + query->radius;
                r |= 1 << getOctant(centroid, &p);
                p.y = query->center.y - query->radius;
                r |= 1 << getOctant(centroid, &p);
                
                p = query->center;
                p.z = query->center.z + query->radius;
                r |= 1 << getOctant(centroid, &p);
                p.z = query->center.z - query->radius;
                r |= 1 << getOctant(centroid, &p);
                
                which &= r;
            }

            break;
        default:
            elog(ERROR, "unrecognized strategy number: %d", strategy);
            which = 0;
            break;
    }
    
    return which;
}

/*
 *  Leaf consistency for Point3D
 */
static bool
spgist_octree_leaf_consistent_point3D(Point3D *key, Point3D *query, StrategyNumber strategy)
{
    bool		retval;
    
    switch (strategy)
    {
        case RTLeftStrategyNumber:
            retval = left_point3D_point3D_internal(key, query);
            break;
        case RTRightStrategyNumber:
            retval = right_point3D_point3D_internal(key, query);
            break;
        case RTBelowStrategyNumber:
            retval = below_point3D_point3D_internal(key, query);
            break;
        case RTAboveStrategyNumber:
            retval = above_point3D_point3D_internal(key, query);
            break;
        case RTFrontStrategyNumber:
            retval = front_point3D_point3D_internal(key, query);
            break;
        case RTBackStrategyNumber:
            retval = back_point3D_point3D_internal(key, query);
            break;
        case RTSameStrategyNumber:
            retval = point3D_same_internal(key, query);
            break;
        default:
            elog(ERROR, "unrecognized strategy number: %d", strategy);
            retval = false;
            break;
    }
    
    return retval;
}

/*
 *  Leaf consistency for Lseg3D
 */
static bool
spgist_octree_leaf_consistent_lseg3D(Point3D *key, Lseg3D *query, StrategyNumber strategy)
{
    bool		retval;
    
    switch (strategy)
    {
        case RTLeftStrategyNumber:
            retval = left_point3D_lseg3D_internal(key, query);
            break;
        case RTRightStrategyNumber:
            retval = right_point3D_lseg3D_internal(key, query);
            break;
        case RTBelowStrategyNumber:
            retval = below_point3D_lseg3D_internal(key, query);
            break;
        case RTAboveStrategyNumber:
            retval = above_point3D_lseg3D_internal(key, query);
            break;
        case RTFrontStrategyNumber:
            retval = front_point3D_lseg3D_internal(key, query);
            break;
        case RTBackStrategyNumber:
            retval = back_point3D_lseg3D_internal(key, query);
            break;
        case RTContainedByStrategyNumber:
            retval = contained_point3D_lseg3D_internal(key, query);
            break;
        default:
            elog(ERROR, "unrecognized strategy number: %d", strategy);
            retval = false;
            break;
    }
    
    return retval;
}

/*
 *  Leaf consistency for Line3D
 */
static bool
spgist_octree_leaf_consistent_line3D(Point3D *key, Line3D *query, StrategyNumber strategy)
{
    bool		retval;
    
    switch (strategy)
    {
        case RTLeftStrategyNumber:
            retval = left_point3D_line3D_internal(key, query);
            break;
        case RTRightStrategyNumber:
            retval = right_point3D_line3D_internal(key, query);
            break;
        case RTBelowStrategyNumber:
            retval = below_point3D_line3D_internal(key, query);
            break;
        case RTAboveStrategyNumber:
            retval = above_point3D_line3D_internal(key, query);
            break;
        case RTFrontStrategyNumber:
            retval = front_point3D_line3D_internal(key, query);
            break;
        case RTBackStrategyNumber:
            retval = back_point3D_line3D_internal(key, query);
            break;
        case RTContainedByStrategyNumber:
            retval = contained_point3D_line3D_internal(key, query);
            break;
        default:
            elog(ERROR, "unrecognized strategy number: %d", strategy);
            retval = false;
            break;
    }
    
    return retval;
}

/*
 *  Leaf consistency for Box3D
 */
static bool
spgist_octree_leaf_consistent_box3D(Point3D *key, Box3D *query, StrategyNumber strategy)
{
    bool		retval;
    
    switch (strategy)
    {
        case RTLeftStrategyNumber:
            retval = left_point3D_box3D_internal(key, query);
            break;
        case RTRightStrategyNumber:
            retval = right_point3D_box3D_internal(key, query);
            break;
        case RTBelowStrategyNumber:
            retval = below_point3D_box3D_internal(key, query);
            break;
        case RTAboveStrategyNumber:
            retval = above_point3D_box3D_internal(key, query);
            break;
        case RTFrontStrategyNumber:
            retval = front_point3D_box3D_internal(key, query);
            break;
        case RTBackStrategyNumber:
            retval = back_point3D_box3D_internal(key, query);
            break;
        case RTContainedByStrategyNumber:
            retval = contained_point3D_box3D_internal(key, query);
            break;
        default:
            elog(ERROR, "unrecognized strategy number: %d", strategy);
            retval = false;
            break;
    }
    
    return retval;
}

/*
 *  Leaf consistency for Path3D
 */
static bool
spgist_octree_leaf_consistent_path3D(Point3D *key, Path3D *query, StrategyNumber strategy)
{
    bool		retval;
    
    switch (strategy)
    {
        case RTLeftStrategyNumber:
            retval = left_point3D_path3D_internal(key, query);
            break;
        case RTRightStrategyNumber:
            retval = right_point3D_path3D_internal(key, query);
            break;
        case RTBelowStrategyNumber:
            retval = below_point3D_path3D_internal(key, query);
            break;
        case RTAboveStrategyNumber:
            retval = above_point3D_path3D_internal(key, query);
            break;
        case RTFrontStrategyNumber:
            retval = front_point3D_path3D_internal(key, query);
            break;
        case RTBackStrategyNumber:
            retval = back_point3D_path3D_internal(key, query);
            break;
        case RTContainedByStrategyNumber:
            retval = contained_point3D_path3D_internal(key, query);
            break;
        default:
            elog(ERROR, "unrecognized strategy number: %d", strategy);
            retval = false;
            break;
    }
    
    return retval;
}

/*
 *  Leaf consistency for Polygon3D
 */
static bool
spgist_octree_leaf_consistent_poly3D(Point3D *key, Polygon3D *query, StrategyNumber strategy)
{
    bool		retval;
    
    switch (strategy)
    {
        case RTLeftStrategyNumber:
            retval = left_point3D_poly3D_internal(key, query);
            break;
        case RTRightStrategyNumber:
            retval = right_point3D_poly3D_internal(key, query);
            break;
        case RTBelowStrategyNumber:
            retval = below_point3D_poly3D_internal(key, query);
            break;
        case RTAboveStrategyNumber:
            retval = above_point3D_poly3D_internal(key, query);
            break;
        case RTFrontStrategyNumber:
            retval = front_point3D_poly3D_internal(key, query);
            break;
        case RTBackStrategyNumber:
            retval = back_point3D_poly3D_internal(key, query);
            break;
        case RTContainedByStrategyNumber:
            retval = contained_point3D_poly3D_internal(key, query);
            break;
        default:
            elog(ERROR, "unrecognized strategy number: %d", strategy);
            retval = false;
            break;
    }
    
    return retval;
}

/*
 *  Leaf consistency for Sphere
 */
static bool
spgist_octree_leaf_consistent_sphere(Point3D *key, Sphere *query, StrategyNumber strategy)
{
    bool		retval;
    
    switch (strategy)
    {
        case RTLeftStrategyNumber:
            retval = left_point3D_sphere_internal(key, query);
            break;
        case RTRightStrategyNumber:
            retval = right_point3D_sphere_internal(key, query);
            break;
        case RTBelowStrategyNumber:
            retval = below_point3D_sphere_internal(key, query);
            break;
        case RTAboveStrategyNumber:
            retval = above_point3D_sphere_internal(key, query);
            break;
        case RTFrontStrategyNumber:
            retval = front_point3D_sphere_internal(key, query);
            break;
        case RTBackStrategyNumber:
            retval = back_point3D_sphere_internal(key, query);
            break;
        case RTContainedByStrategyNumber:
            retval = contained_point3D_sphere_internal(key, query);
            break;
        default:
            elog(ERROR, "unrecognized strategy number: %d", strategy);
            retval = false;
            break;
    }
    
    return retval;
}
/*****************************************************************************/
