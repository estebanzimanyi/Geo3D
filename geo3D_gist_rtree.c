/*-------------------------------------------------------------------------
 *
 * geo3D_gist_rtree.c
 *	  Support procedures for GiSTs over 3-D objects (boxes, polygons, spheres,
 *	  points).
 *
 * This gives R-tree behavior, with Guttman's poly-time split algorithm.
 *
 *
 * Portions Copyright (c) 2017, Esteban Zimanyi, Université Libre de Bruxelles
 * Portions Copyright (c) 1996-2016, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * IDENTIFICATION
 *	src/backend/access/gist/geo3D_gist_rtree.c
 *
 *-------------------------------------------------------------------------
 */
#include <postgres.h>

#include <math.h>

#include <access/gist.h>
#include <access/stratnum.h>
#include <utils/builtins.h>
#include "geo3D_decls.h"

static bool gist_point3D_leaf_consistent_point3D(Point3D *key, Point3D *query, StrategyNumber strategy);
static bool gist_point3D_leaf_consistent_lseg3D(Point3D *key, Lseg3D *query, StrategyNumber strategy);
static bool gist_point3D_leaf_consistent_line3D(Point3D *key, Line3D *query, StrategyNumber strategy);
static bool gist_point3D_leaf_consistent_box3D(Point3D *key, Box3D *query, StrategyNumber strategy);
static bool gist_point3D_leaf_consistent_path3D(Point3D *key, Path3D *query, StrategyNumber strategy);
static bool gist_point3D_leaf_consistent_poly3D(Point3D *key, Polygon3D *query, StrategyNumber strategy);
static bool gist_point3D_leaf_consistent_sphere3D(Point3D *key, Sphere *query, StrategyNumber strategy);

static bool gist_box3D_leaf_consistent_point3D(Box3D *key, Point3D *query, StrategyNumber strategy);
static bool gist_box3D_leaf_consistent_lseg3D(Box3D *key, Lseg3D *query, StrategyNumber strategy);
static bool gist_box3D_leaf_consistent_line3D(Box3D *key, Line3D *query, StrategyNumber strategy);
static bool gist_box3D_leaf_consistent_box3D(Box3D *key, Box3D *query, StrategyNumber strategy);
static bool gist_box3D_leaf_consistent_path3D(Box3D *key, Path3D *query, StrategyNumber strategy);
static bool gist_box3D_leaf_consistent_poly3D(Box3D *key, Polygon3D *query, StrategyNumber strategy);
static bool gist_box3D_leaf_consistent_sphere(Box3D *key, Sphere *query, StrategyNumber strategy);

static bool gist_rtree3D_internal_consistent(Box3D *key, Box3D *query, StrategyNumber strategy);
static bool gist_rtree3D_internal_consistent_point3D(Box3D *key, Point3D *query, StrategyNumber strategy);
static bool gist_rtree3D_internal_consistent_lseg3D(Box3D *key, Lseg3D *query, StrategyNumber strategy);
static bool gist_rtree3D_internal_consistent_line3D(Box3D *key, Line3D *query, StrategyNumber strategy);
static bool gist_rtree3D_internal_consistent_path3D(Box3D *key, Path3D *query, StrategyNumber strategy);
static bool gist_rtree3D_internal_consistent_poly3D(Box3D *key, Polygon3D *query, StrategyNumber strategy);
static bool gist_rtree3D_internal_consistent_sphere(Box3D *key, Sphere *query, StrategyNumber strategy);

static double computeDistance3D(bool isLeaf, Box3D *box, Point3D *point);

/* Minimum accepted ratio of split */
#define LIMIT_RATIO 0.3

/* Convenience macros for NaN-aware comparisons */
#define FLOAT8_EQ(a,b)	(float8_cmp_internal(a, b) == 0)
#define FLOAT8_LT(a,b)	(float8_cmp_internal(a, b) < 0)
#define FLOAT8_LE(a,b)	(float8_cmp_internal(a, b) <= 0)
#define FLOAT8_GT(a,b)	(float8_cmp_internal(a, b) > 0)
#define FLOAT8_GE(a,b)	(float8_cmp_internal(a, b) >= 0)
#define FLOAT8_MAX(a,b)  (FLOAT8_GT(a, b) ? (a) : (b))
#define FLOAT8_MIN(a,b)  (FLOAT8_LT(a, b) ? (a) : (b))


/**************************************************
 * Box3D ops
 **************************************************/

/*
 * Calculates union of two boxes, a and b. The result is stored in *n.
 */
static void
rt_box3D_union(Box3D *n, const Box3D *a, const Box3D *b)
{
	n->high.x = FLOAT8_MAX(a->high.x, b->high.x);
	n->high.y = FLOAT8_MAX(a->high.y, b->high.y);
	n->high.z = FLOAT8_MAX(a->high.z, b->high.z);
	n->low.x = FLOAT8_MIN(a->low.x, b->low.x);
	n->low.y = FLOAT8_MIN(a->low.y, b->low.y);
	n->low.z = FLOAT8_MIN(a->low.z, b->low.z);
}

/*
 * Size of a Box3D for penalty-calculation purposes.
 * The result can be +Infinity, but not NaN.
 */
static double
size_Box3D(const Box3D *box)
{
	/*
	 * Check for zero-width cases.  Note that we define the size of a zero-
	 * by-infinity box as zero.  It's important to special-case this somehow,
	 * as naively multiplying infinity by zero will produce NaN.
	 *
	 * The less-than cases should not happen, but if they do, say "zero".
	 */
	if (FLOAT8_LE(box->high.x, box->low.x) ||
		FLOAT8_LE(box->high.y, box->low.y) ||
		FLOAT8_LE(box->high.z, box->low.z))
		return 0.0;

	/*
	 * We treat NaN as larger than +Infinity, so any distance involving a NaN
	 * and a non-NaN is infinite.  Note the previous check eliminated the
	 * possibility that the low fields are NaNs.
	 */
	if (isnan(box->high.x) || isnan(box->high.y) || isnan(box->high.z))
		return get_float8_infinity();
	return (box->high.x - box->low.x) * (box->high.y - box->low.y) * (box->high.z - box->low.z);
}

/*
 * Return amount by which the union of the two boxes is larger than
 * the original Box3D's volume.  The result can be +Infinity, but not NaN.
 */
static double
box3D_penalty(const Box3D *original, const Box3D *new)
{
	Box3D			unionbox;

	rt_box3D_union(&unionbox, original, new);
	return size_Box3D(&unionbox) - size_Box3D(original);
}

/*
 * The GiST consistent method for box3D
 *
 * Should return false if for all data items x below entry,
 * the predicate x op query must be FALSE, where op is the oper
 * corresponding to strategy in the pg_amop table.
 */
Datum
gist_box3D_consistent(PG_FUNCTION_ARGS)
{
	GISTENTRY  *entry = (GISTENTRY *) PG_GETARG_POINTER(0);
	StrategyNumber strategy = (StrategyNumber) PG_GETARG_UINT16(2);
	Box3D 	   *key = DatumGetBox3DP(entry->key);
	StrategyNumber strategyGroup = strategy / Geo3DStrategyNumberOffset;

	/* Oid		subtype = PG_GETARG_OID(3); */
	bool	   *recheck = (bool *) PG_GETARG_POINTER(4);
	bool		result;

	/* All cases served by this function are exact */
	*recheck = false;

	if (key == NULL)
		PG_RETURN_BOOL(FALSE);
				
	/*
	 * if entry is leaf, use gist_box3D_leaf_consistent_x
	 * else use gist_rtree3D_internal_consistent_x
	 * depending on the type x of the query parameter 
	 */
	if (GIST_LEAF(entry))
		switch (strategyGroup)
		{
			case Point3DStrategyNumberGroup:
				{
					Point3D  *point = PG_GETARG_POINT3D_P(1);

					if (point == NULL)
						PG_RETURN_BOOL(FALSE);

					PG_RETURN_BOOL(gist_box3D_leaf_consistent_point3D(key, point, 
							strategy % Geo3DStrategyNumberOffset));
				}
				break;
			case Lseg3DStrategyNumberGroup:
				{
					Lseg3D  *lseg = PG_GETARG_LSEG3D_P(1);

					if (lseg == NULL)
						PG_RETURN_BOOL(FALSE);

					PG_RETURN_BOOL(gist_box3D_leaf_consistent_lseg3D(key, lseg, 
						strategy % Geo3DStrategyNumberOffset));
				}
				break;
			case Line3DStrategyNumberGroup:
				{
					Line3D  *line = PG_GETARG_LINE3D_P(1);

					if (line == NULL)
						PG_RETURN_BOOL(FALSE);

					PG_RETURN_BOOL(gist_box3D_leaf_consistent_line3D(key, line, 
						strategy % Geo3DStrategyNumberOffset));
				}
				break;
			case Box3DStrategyNumberGroup:
				{
					Box3D  *box = PG_GETARG_BOX3D_P(1);

					if (box == NULL)
						PG_RETURN_BOOL(FALSE);

					PG_RETURN_BOOL(gist_box3D_leaf_consistent_box3D(key, box, 
						strategy % Geo3DStrategyNumberOffset));
				}
				break;
			case Path3DStrategyNumberGroup:
				{
					Path3D  *path = PG_GETARG_PATH3D_P(1);

					if (path == NULL)
						PG_RETURN_BOOL(FALSE);

					result = gist_box3D_leaf_consistent_path3D(key, path, 
							strategy % Geo3DStrategyNumberOffset);
					/* Avoid memory leak if supplied poly is toasted */
					PG_FREE_IF_COPY(path, 1);
					PG_RETURN_BOOL(result);
				}
				break;
			case Polygon3DStrategyNumberGroup:
				{
					Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

					if (poly == NULL)
						PG_RETURN_BOOL(FALSE);

					result = gist_box3D_leaf_consistent_poly3D(key, poly, 
							strategy % Geo3DStrategyNumberOffset);
					/* Avoid memory leak if supplied poly is toasted */
					PG_FREE_IF_COPY(poly, 1);
					PG_RETURN_BOOL(result);
				}
				break;
			case SphereStrategyNumberGroup:
				{
					Sphere  *sphere = PG_GETARG_SPHERE_P(1);

					if (sphere == NULL)
						PG_RETURN_BOOL(FALSE);

					PG_RETURN_BOOL(gist_box3D_leaf_consistent_sphere(key, sphere, 
						strategy % Geo3DStrategyNumberOffset));
				}
				break;
			default:
				elog(ERROR, "unrecognized strategy number: %d", strategy);
				PG_RETURN_BOOL(FALSE);
				break;
		}
	else
		switch (strategyGroup)
		{
			case Point3DStrategyNumberGroup:
				{
					Point3D  *point = PG_GETARG_POINT3D_P(1);

					if (point == NULL)
						PG_RETURN_BOOL(FALSE);

					PG_RETURN_BOOL(gist_rtree3D_internal_consistent_point3D(key, point, 
						strategy % Geo3DStrategyNumberOffset));
				}
				break;
			case Lseg3DStrategyNumberGroup:
				{
					Lseg3D  *lseg = PG_GETARG_LSEG3D_P(1);

					if (lseg == NULL)
						PG_RETURN_BOOL(FALSE);

					PG_RETURN_BOOL(gist_rtree3D_internal_consistent_lseg3D(key, lseg, 
						strategy % Geo3DStrategyNumberOffset));
				}
				break;
			case Line3DStrategyNumberGroup:
				{
					Line3D  *line = PG_GETARG_LINE3D_P(1);

					if (line == NULL)
						PG_RETURN_BOOL(FALSE);

					PG_RETURN_BOOL(gist_rtree3D_internal_consistent_line3D(key, line, 
						strategy % Geo3DStrategyNumberOffset));
				}
				break;
			case Box3DStrategyNumberGroup:
				{
					Box3D  *box = PG_GETARG_BOX3D_P(1);

					if (box == NULL)
						PG_RETURN_BOOL(FALSE);

					PG_RETURN_BOOL(gist_rtree3D_internal_consistent(key, box, 
						strategy % Geo3DStrategyNumberOffset));
				}
				break;
			case Path3DStrategyNumberGroup:		
				{
					Path3D  *path = PG_GETARG_PATH3D_P(1);

					if (path == NULL)
						PG_RETURN_BOOL(FALSE);

					result = gist_rtree3D_internal_consistent_path3D(key, path, 
						strategy % Geo3DStrategyNumberOffset);
					/* Avoid memory leak if supplied path is toasted */
					PG_FREE_IF_COPY(path, 1);
					PG_RETURN_BOOL(result);
				}
				break;
			case Polygon3DStrategyNumberGroup:		
				{
					Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

					if (poly == NULL)
						PG_RETURN_BOOL(FALSE);

					result = gist_rtree3D_internal_consistent_poly3D(key, poly, 
						strategy % Geo3DStrategyNumberOffset);
					/* Avoid memory leak if supplied poly is toasted */
					PG_FREE_IF_COPY(poly, 1);
					PG_RETURN_BOOL(result);
				}
				break;
			case SphereStrategyNumberGroup:			
				{
					Sphere  *sphere = PG_GETARG_SPHERE_P(1);

					if (sphere == NULL)
						PG_RETURN_BOOL(FALSE);

					PG_RETURN_BOOL(gist_rtree3D_internal_consistent_sphere(key, sphere, 
						strategy % Geo3DStrategyNumberOffset));
				}
				break;
			default:
				elog(ERROR, "unrecognized strategy number: %d", strategy);
				PG_RETURN_BOOL(FALSE);
				break;
		}
}

PG_FUNCTION_INFO_V1(gist_box3D_consistent);

/*
 * Increase Box3D b to include addon.
 */
static void
adjustBox3D(Box3D *b, const Box3D *addon)
{
	if (FLOAT8_LT(b->high.x, addon->high.x))
		b->high.x = addon->high.x;
	if (FLOAT8_GT(b->low.x, addon->low.x))
		b->low.x = addon->low.x;
	if (FLOAT8_LT(b->high.y, addon->high.y))
		b->high.y = addon->high.y;
	if (FLOAT8_GT(b->low.y, addon->low.y))
		b->low.y = addon->low.y;
	if (FLOAT8_LT(b->high.z, addon->high.z))
		b->high.z = addon->high.z;
	if (FLOAT8_GT(b->low.z, addon->low.z))
		b->low.z = addon->low.z;
}

/*
 * The GiST Union method for boxes
 *
 * returns the minimal bounding box that encloses all the entries in entryvec
 */
Datum
gist_box3D_union(PG_FUNCTION_ARGS)
{
	GistEntryVector *entryvec = (GistEntryVector *) PG_GETARG_POINTER(0);
	int			numranges,
				i;
	Box3D	   *cur,
			   *pageunion;

	numranges = entryvec->n;
	pageunion = (Box3D *) palloc(sizeof(Box3D));
	cur = DatumGetBox3DP(entryvec->vector[0].key);
	memcpy((void *) pageunion, (void *) cur, sizeof(Box3D));

	for (i = 1; i < numranges; i++)
	{
		cur = DatumGetBox3DP(entryvec->vector[i].key);
		adjustBox3D(pageunion, cur);
	}

	PG_RETURN_POINTER(pageunion);
}

PG_FUNCTION_INFO_V1(gist_box3D_union);

/*
 * GiST compress methods for boxes
 *
 * do not do anything.
 */
Datum
gist_box3D_compress(PG_FUNCTION_ARGS)
{
	PG_RETURN_POINTER(PG_GETARG_POINTER(0));
}

PG_FUNCTION_INFO_V1(gist_box3D_compress);

/*
 * GiST decompress method for boxes (also used for points, polygons
 * and spheres)
 *
 * do not do anything --- we just use the stored box as is.
 */
Datum
gist_box3D_decompress(PG_FUNCTION_ARGS)
{
	PG_RETURN_POINTER(PG_GETARG_POINTER(0));
}

PG_FUNCTION_INFO_V1(gist_box3D_decompress);

/*
 * GiST Fetch method for boxes
 * do not do anything --- we just return the stored box as is.
 */
Datum
gist_box3D_fetch(PG_FUNCTION_ARGS)
{
	PG_RETURN_POINTER(PG_GETARG_POINTER(0));
}

PG_FUNCTION_INFO_V1(gist_box3D_fetch);

/*
 * The GiST Penalty method for boxes (also used for points)
 *
 * As in the R-tree paper, we use change in area as our penalty metric
 */
Datum
gist_box3D_penalty(PG_FUNCTION_ARGS)
{
	GISTENTRY  *origentry = (GISTENTRY *) PG_GETARG_POINTER(0);
	GISTENTRY  *newentry = (GISTENTRY *) PG_GETARG_POINTER(1);
	float	   *result = (float *) PG_GETARG_POINTER(2);
	Box3D	   *origbox = DatumGetBox3DP(origentry->key);
	Box3D	   *newbox = DatumGetBox3DP(newentry->key);

	*result = (float) box3D_penalty(origbox, newbox);
	PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(gist_box3D_penalty);

/*
 * Trivial split: half of entries will be placed on one page
 * and another half to another
 */
static void
fallbackSplit(GistEntryVector *entryvec, GIST_SPLITVEC *v)
{
	OffsetNumber i,
				maxoff;
	Box3D	   *unionL = NULL,
			   *unionR = NULL;
	int			nbytes;

	maxoff = entryvec->n - 1;

	nbytes = (maxoff + 2) * sizeof(OffsetNumber);
	v->spl_left = (OffsetNumber *) palloc(nbytes);
	v->spl_right = (OffsetNumber *) palloc(nbytes);
	v->spl_nleft = v->spl_nright = 0;

	for (i = FirstOffsetNumber; i <= maxoff; i = OffsetNumberNext(i))
	{
		Box3D   *cur = DatumGetBox3DP(entryvec->vector[i].key);

		if (i <= (maxoff - FirstOffsetNumber + 1) / 2)
		{
			v->spl_left[v->spl_nleft] = i;
			if (unionL == NULL)
			{
				unionL = (Box3D *) palloc(sizeof(Box3D));
				*unionL = *cur;
			}
			else
				adjustBox3D(unionL, cur);

			v->spl_nleft++;
		}
		else
		{
			v->spl_right[v->spl_nright] = i;
			if (unionR == NULL)
			{
				unionR = (Box3D *) palloc(sizeof(Box3D));
				*unionR = *cur;
			}
			else
				adjustBox3D(unionR, cur);

			v->spl_nright++;
		}
	}

	v->spl_ldatum = Box3DPGetDatum(unionL);
	v->spl_rdatum = Box3DPGetDatum(unionR);
}

/*
 * Represents information about an entry that can be placed to either group
 * without affecting overlap over selected axis ("common entry").
 */
typedef struct
{
	/* Index of entry in the initial array */
	int			index;
	/* Delta between penalties of entry insertion into different groups */
	double		delta;
} CommonEntry;

/*
 * Context for g_box3D_consider_split. Contains information about currently
 * selected split and some general information.
 */
typedef struct
{
	int			entriesCount;	/* total number of entries being split */
	Box3D		boundingBox;	/* minimum bounding box across all entries */

	/* Information about currently selected split follows */

	bool		first;			/* true if no split was selected yet */

	double		leftUpper;		/* upper bound of left interval */
	double		rightLower;		/* lower bound of right interval */

	float4		ratio;
	float4		overlap;
	int			dim;			/* axis of this split */
	double		range;			/* width of general MBR projection to the
								 * selected axis */
} ConsiderSplitContext;

/*
 * Interval represents projection of box to axis.
 */
typedef struct
{
	double		lower,
				upper;
} SplitInterval;

/*
 * Interval comparison function by lower bound of the interval;
 */
static int
interval_cmp_lower(const void *i1, const void *i2)
{
	double		lower1 = ((const SplitInterval *) i1)->lower,
				lower2 = ((const SplitInterval *) i2)->lower;

	return float8_cmp_internal(lower1, lower2);
}

/*
 * Interval comparison function by upper bound of the interval;
 */
static int
interval_cmp_upper(const void *i1, const void *i2)
{
	double		upper1 = ((const SplitInterval *) i1)->upper,
				upper2 = ((const SplitInterval *) i2)->upper;

	return float8_cmp_internal(upper1, upper2);
}

/*
 * Replace negative (or NaN) value with zero.
 */
static inline float
non_negative(float val)
{
	if (FPge(val, 0.0f))
		return val;
	else
		return 0.0f;
}

/*
 * Consider replacement of currently selected split with the better one.
 */
static inline void
g_box3D_consider_split(ConsiderSplitContext *context, int dimNum,
					 double rightLower, int minLeftCount,
					 double leftUpper, int maxLeftCount)
{
	int			leftCount,
				rightCount;
	float4		ratio,
				overlap;
	double		range;

	/*
	 * Calculate entries distribution ratio assuming most uniform distribution
	 * of common entries.
	 */
	if (minLeftCount >= (context->entriesCount + 1) / 2)
	{
		leftCount = minLeftCount;
	}
	else
	{
		if (maxLeftCount <= context->entriesCount / 2)
			leftCount = maxLeftCount;
		else
			leftCount = context->entriesCount / 2;
	}
	rightCount = context->entriesCount - leftCount;

	/*
	 * Ratio of split - quotient between size of lesser group and total
	 * entries count.
	 */
	ratio = ((float4) Min(leftCount, rightCount)) /
		((float4) context->entriesCount);

	if (ratio > LIMIT_RATIO)
	{
		bool		selectthis = false;

		/*
		 * The ratio is acceptable, so compare current split with previously
		 * selected one. Between splits of one dimension we search for minimal
		 * overlap (allowing negative values) and minimal ration (between same
		 * overlaps. We switch dimension if find less overlap (non-negative)
		 * or less range with same overlap.
		 */
		if (dimNum == 0)
			range = context->boundingBox.high.x - context->boundingBox.low.x;
		else if (dimNum == 1)
			range = context->boundingBox.high.y - context->boundingBox.low.y;
		else
			range = context->boundingBox.high.z - context->boundingBox.low.z;

		overlap = (leftUpper - rightLower) / range;

		/* If there is no previous selection, select this */
		if (context->first)
			selectthis = true;
		else if (context->dim == dimNum)
		{
			/*
			 * Within the same dimension, choose the new split if it has a
			 * smaller overlap, or same overlap but better ratio.
			 */
			if (overlap < context->overlap ||
				(overlap == context->overlap && ratio > context->ratio))
				selectthis = true;
		}
		else
		{
			/*
			 * Across dimensions, choose the new split if it has a smaller
			 * *non-negative* overlap, or same *non-negative* overlap but
			 * bigger range. This condition differs from the one described in
			 * the article. On the datasets where leaf MBRs don't overlap
			 * themselves, non-overlapping splits (i.e. splits which have zero
			 * *non-negative* overlap) are frequently possible. In this case
			 * splits tends to be along one dimension, because most distant
			 * non-overlapping splits (i.e. having lowest negative overlap)
			 * appears to be in the same dimension as in the previous split.
			 * Therefore MBRs appear to be very prolonged along another
			 * dimension, which leads to bad search performance. Using range
			 * as the second split criteria makes MBRs more quadratic. Using
			 * *non-negative* overlap instead of overlap as the first split
			 * criteria gives to range criteria a chance to matter, because
			 * non-overlapping splits are equivalent in this criteria.
			 */
			if (non_negative(overlap) < non_negative(context->overlap) ||
				(range > context->range &&
				 non_negative(overlap) <= non_negative(context->overlap)))
				selectthis = true;
		}

		if (selectthis)
		{
			/* save information about selected split */
			context->first = false;
			context->ratio = ratio;
			context->range = range;
			context->overlap = overlap;
			context->rightLower = rightLower;
			context->leftUpper = leftUpper;
			context->dim = dimNum;
		}
	}
}

/*
 * Compare common entries by their deltas.
 * (We assume the deltas can't be NaN.)
 */
static int
common_entry_cmp(const void *i1, const void *i2)
{
	double		delta1 = ((const CommonEntry *) i1)->delta,
				delta2 = ((const CommonEntry *) i2)->delta;

	if (delta1 < delta2)
		return -1;
	else if (delta1 > delta2)
		return 1;
	else
		return 0;
}

/*
 * --------------------------------------------------------------------------
 * Double sorting split algorithm. This is used for both boxes and points.
 *
 * The algorithm finds split of boxes by considering splits along each axis.
 * Each entry is first projected as an interval on the X-axis, and different
 * ways to split the intervals into two groups are considered, trying to
 * minimize the overlap of the groups. Then the same is repeated for the
 * Y-axis, and the overall best split is chosen. The quality of a split is
 * determined by overlap along that axis and some other criteria (see
 * g_box3D_consider_split).
 *
 * After that, all the entries are divided into three groups:
 *
 * 1) Entries which should be placed to the left group
 * 2) Entries which should be placed to the right group
 * 3) "Common entries" which can be placed to any of groups without affecting
 *	  of overlap along selected axis.
 *
 * The common entries are distributed by minimizing penalty.
 *
 * For details see:
 * "A new double sorting-based node splitting algorithm for R-tree", A. Korotkov
 * http://syrcose.ispras.ru/2011/files/SYRCoSE2011_Proceedings.pdf#page=36
 * --------------------------------------------------------------------------
 */
Datum
gist_box3D_picksplit(PG_FUNCTION_ARGS)
{
	GistEntryVector *entryvec = (GistEntryVector *) PG_GETARG_POINTER(0);
	GIST_SPLITVEC *v = (GIST_SPLITVEC *) PG_GETARG_POINTER(1);
	OffsetNumber i,
				maxoff;
	ConsiderSplitContext context;
	Box3D	   *box,
			   *leftBox,
			   *rightBox;
	int			dim,
				commonEntriesCount;
	SplitInterval *intervalsLower,
			   *intervalsUpper;
	CommonEntry *commonEntries;
	int			nentries;

	memset(&context, 0, sizeof(ConsiderSplitContext));

	maxoff = entryvec->n - 1;
	nentries = context.entriesCount = maxoff - FirstOffsetNumber + 1;

	/* Allocate arrays for intervals along axes */
	intervalsLower = (SplitInterval *) palloc(nentries * sizeof(SplitInterval));
	intervalsUpper = (SplitInterval *) palloc(nentries * sizeof(SplitInterval));

	/*
	 * Calculate the overall minimum bounding box over all the entries.
	 */
	for (i = FirstOffsetNumber; i <= maxoff; i = OffsetNumberNext(i))
	{
		box = DatumGetBox3DP(entryvec->vector[i].key);
		if (i == FirstOffsetNumber)
			context.boundingBox = *box;
		else
			adjustBox3D(&context.boundingBox, box);
	}

	/*
	 * Iterate over axes for optimal split searching.
	 */
	context.first = true;		/* nothing selected yet */
	for (dim = 0; dim < 3; dim++)
	{
		double		leftUpper,
					rightLower;
		int			i1,
					i2;

		/* Project each entry as an interval on the selected axis. */
		for (i = FirstOffsetNumber; i <= maxoff; i = OffsetNumberNext(i))
		{
			box = DatumGetBox3DP(entryvec->vector[i].key);
			if (dim == 0)
			{
				intervalsLower[i - FirstOffsetNumber].lower = box->low.x;
				intervalsLower[i - FirstOffsetNumber].upper = box->high.x;
			}
			else if (dim == 1)
			{
				intervalsLower[i - FirstOffsetNumber].lower = box->low.y;
				intervalsLower[i - FirstOffsetNumber].upper = box->high.y;
			}
			else 
			{
				intervalsLower[i - FirstOffsetNumber].lower = box->low.z;
				intervalsLower[i - FirstOffsetNumber].upper = box->high.z;
			}
		}

		/*
		 * Make two arrays of intervals: one sorted by lower bound and another
		 * sorted by upper bound.
		 */
		memcpy(intervalsUpper, intervalsLower,
			   sizeof(SplitInterval) * nentries);
		qsort(intervalsLower, nentries, sizeof(SplitInterval),
			  interval_cmp_lower);
		qsort(intervalsUpper, nentries, sizeof(SplitInterval),
			  interval_cmp_upper);

		/*----
		 * The goal is to form a left and right interval, so that every entry
		 * interval is contained by either left or right interval (or both).
		 *
		 * For example, with the intervals (0,1), (1,3), (2,3), (2,4):
		 *
		 * 0 1 2 3 4
		 * +-+
		 *	 +---+
		 *	   +-+
		 *	   +---+
		 *
		 * The left and right intervals are of the form (0,a) and (b,4).
		 * We first consider splits where b is the lower bound of an entry.
		 * We iterate through all entries, and for each b, calculate the
		 * smallest possible a. Then we consider splits where a is the
		 * upper bound of an entry, and for each a, calculate the greatest
		 * possible b.
		 *
		 * In the above example, the first loop would consider splits:
		 * b=0: (0,1)-(0,4)
		 * b=1: (0,1)-(1,4)
		 * b=2: (0,3)-(2,4)
		 *
		 * And the second loop:
		 * a=1: (0,1)-(1,4)
		 * a=3: (0,3)-(2,4)
		 * a=4: (0,4)-(2,4)
		 */

		/*
		 * Iterate over lower bound of right group, finding smallest possible
		 * upper bound of left group.
		 */
		i1 = 0;
		i2 = 0;
		rightLower = intervalsLower[i1].lower;
		leftUpper = intervalsUpper[i2].lower;
		while (true)
		{
			/*
			 * Find next lower bound of right group.
			 */
			while (i1 < nentries &&
				   FLOAT8_EQ(rightLower, intervalsLower[i1].lower))
			{
				if (FLOAT8_LT(leftUpper, intervalsLower[i1].upper))
					leftUpper = intervalsLower[i1].upper;
				i1++;
			}
			if (i1 >= nentries)
				break;
			rightLower = intervalsLower[i1].lower;

			/*
			 * Find count of intervals which anyway should be placed to the
			 * left group.
			 */
			while (i2 < nentries &&
				   FLOAT8_LE(intervalsUpper[i2].upper, leftUpper))
				i2++;

			/*
			 * Consider found split.
			 */
			g_box3D_consider_split(&context, dim, rightLower, i1, leftUpper, i2);
		}

		/*
		 * Iterate over upper bound of left group finding greatest possible
		 * lower bound of right group.
		 */
		i1 = nentries - 1;
		i2 = nentries - 1;
		rightLower = intervalsLower[i1].upper;
		leftUpper = intervalsUpper[i2].upper;
		while (true)
		{
			/*
			 * Find next upper bound of left group.
			 */
			while (i2 >= 0 && FLOAT8_EQ(leftUpper, intervalsUpper[i2].upper))
			{
				if (FLOAT8_GT(rightLower, intervalsUpper[i2].lower))
					rightLower = intervalsUpper[i2].lower;
				i2--;
			}
			if (i2 < 0)
				break;
			leftUpper = intervalsUpper[i2].upper;

			/*
			 * Find count of intervals which anyway should be placed to the
			 * right group.
			 */
			while (i1 >= 0 && FLOAT8_GE(intervalsLower[i1].lower, rightLower))
				i1--;

			/*
			 * Consider found split.
			 */
			g_box3D_consider_split(&context, dim,
								 rightLower, i1 + 1, leftUpper, i2 + 1);
		}
	}

	/*
	 * If we failed to find any acceptable splits, use trivial split.
	 */
	if (context.first)
	{
		fallbackSplit(entryvec, v);
		PG_RETURN_POINTER(v);
	}

	/*
	 * Ok, we have now selected the split across one axis.
	 *
	 * While considering the splits, we already determined that there will be
	 * enough entries in both groups to reach the desired ratio, but we did
	 * not memorize which entries go to which group. So determine that now.
	 */

	/* Allocate vectors for results */
	v->spl_left = (OffsetNumber *) palloc(nentries * sizeof(OffsetNumber));
	v->spl_right = (OffsetNumber *) palloc(nentries * sizeof(OffsetNumber));
	v->spl_nleft = 0;
	v->spl_nright = 0;

	/* Allocate bounding boxes of left and right groups */
	leftBox = palloc0(sizeof(Box3D));
	rightBox = palloc0(sizeof(Box3D));

	/*
	 * Allocate an array for "common entries" - entries which can be placed to
	 * either group without affecting overlap along selected axis.
	 */
	commonEntriesCount = 0;
	commonEntries = (CommonEntry *) palloc(nentries * sizeof(CommonEntry));

	/* Helper macros to place an entry in the left or right group */
#define PLACE_LEFT(box, off)					\
	do {										\
		if (v->spl_nleft > 0)					\
			adjustBox3D(leftBox, box);			\
		else									\
			*leftBox = *(box);					\
		v->spl_left[v->spl_nleft++] = off;		\
	} while(0)

#define PLACE_RIGHT(box, off)					\
	do {										\
		if (v->spl_nright > 0)					\
			adjustBox3D(rightBox, box);			\
		else									\
			*rightBox = *(box);					\
		v->spl_right[v->spl_nright++] = off;	\
	} while(0)

	/*
	 * Distribute entries which can be distributed unambiguously, and collect
	 * common entries.
	 */
	for (i = FirstOffsetNumber; i <= maxoff; i = OffsetNumberNext(i))
	{
		double		lower,
					upper;

		/*
		 * Get upper and lower bounds along selected axis.
		 */
		box = DatumGetBox3DP(entryvec->vector[i].key);
		if (context.dim == 0)
		{
			lower = box->low.x;
			upper = box->high.x;
		}
		else if (context.dim == 1)
		{
			lower = box->low.y;
			upper = box->high.y;
		}
		else
		{
			lower = box->low.z;
			upper = box->high.z;
		}

		if (FLOAT8_LE(upper, context.leftUpper))
		{
			/* Fits to the left group */
			if (FLOAT8_GE(lower, context.rightLower))
			{
				/* Fits also to the right group, so "common entry" */
				commonEntries[commonEntriesCount++].index = i;
			}
			else
			{
				/* Doesn't fit to the right group, so join to the left group */
				PLACE_LEFT(box, i);
			}
		}
		else
		{
			/*
			 * Each entry should fit on either left or right group. Since this
			 * entry didn't fit on the left group, it better fit in the right
			 * group.
			 */
			Assert(FLOAT8_GE(lower, context.rightLower));

			/* Doesn't fit to the left group, so join to the right group */
			PLACE_RIGHT(box, i);
		}
	}

	/*
	 * Distribute "common entries", if any.
	 */
	if (commonEntriesCount > 0)
	{
		/*
		 * Calculate minimum number of entries that must be placed in both
		 * groups, to reach LIMIT_RATIO.
		 */
		int			m = ceil(LIMIT_RATIO * (double) nentries);

		/*
		 * Calculate delta between penalties of join "common entries" to
		 * different groups.
		 */
		for (i = 0; i < commonEntriesCount; i++)
		{
			box = DatumGetBox3DP(entryvec->vector[commonEntries[i].index].key);
			commonEntries[i].delta = Abs(box3D_penalty(leftBox, box) -
										 box3D_penalty(rightBox, box));
		}

		/*
		 * Sort "common entries" by calculated deltas in order to distribute
		 * the most ambiguous entries first.
		 */
		qsort(commonEntries, commonEntriesCount, sizeof(CommonEntry), common_entry_cmp);

		/*
		 * Distribute "common entries" between groups.
		 */
		for (i = 0; i < commonEntriesCount; i++)
		{
			box = DatumGetBox3DP(entryvec->vector[commonEntries[i].index].key);

			/*
			 * Check if we have to place this entry in either group to achieve
			 * LIMIT_RATIO.
			 */
			if (v->spl_nleft + (commonEntriesCount - i) <= m)
				PLACE_LEFT(box, commonEntries[i].index);
			else if (v->spl_nright + (commonEntriesCount - i) <= m)
				PLACE_RIGHT(box, commonEntries[i].index);
			else
			{
				/* Otherwise select the group by minimal penalty */
				if (box3D_penalty(leftBox, box) < box3D_penalty(rightBox, box))
					PLACE_LEFT(box, commonEntries[i].index);
				else
					PLACE_RIGHT(box, commonEntries[i].index);
			}
		}
	}

	v->spl_ldatum = PointerGetDatum(leftBox);
	v->spl_rdatum = PointerGetDatum(rightBox);
	PG_RETURN_POINTER(v);
}

PG_FUNCTION_INFO_V1(gist_box3D_picksplit);

/*
 * Equality method
 *
 * This is used for boxes, points, spheres, and polygons, all of which store
 * boxes as GiST index entries.
 *
 * Returns true only when boxes are exactly the same.  We can't use fuzzy
 * comparisons here without breaking index consistency; therefore, this isn't
 * equivalent to box3D_same().
 */
Datum
gist_box3D_same(PG_FUNCTION_ARGS)
{
	Box3D	   *b1 = PG_GETARG_BOX3D_P(0);
	Box3D	   *b2 = PG_GETARG_BOX3D_P(1);
	bool	   *result = (bool *) PG_GETARG_POINTER(2);

	if (b1 && b2)
		*result = (FLOAT8_EQ(b1->low.x, b2->low.x) &&
				   FLOAT8_EQ(b1->low.y, b2->low.y) &&
				   FLOAT8_EQ(b1->low.z, b2->low.z) &&
				   FLOAT8_EQ(b1->high.x, b2->high.x) &&
				   FLOAT8_EQ(b1->high.y, b2->high.y) &&
				   FLOAT8_EQ(b1->high.z, b2->high.z));
	else
		*result = (b1 == NULL && b2 == NULL);
	PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(gist_box3D_same);

/*-------------------------------------------------------------------------*/

/*
 * Leaf-level consistency for point3D with point3D: just apply the query operator
 */
static bool
gist_point3D_leaf_consistent_point3D(Point3D *key, Point3D *query, StrategyNumber strategy)
{
	bool		retval;

	switch (strategy)
	{
		case RTLeftStrategyNumber:
			retval = left_point3D_point3D_internal(key, query);
			break;
		case RTOverLeftStrategyNumber:
			retval = overleft_point3D_point3D_internal(key, query);
			break;
		case RTOverRightStrategyNumber:
			retval = overright_point3D_point3D_internal(key, query);
			break;
		case RTRightStrategyNumber:
			retval = right_point3D_point3D_internal(key, query);
			break;
		case RTSameStrategyNumber:
			retval = point3D_same_internal(key, query);
			break;
		case RTOverBelowStrategyNumber:
			retval = overbelow_point3D_point3D_internal(key, query);
			break;
		case RTBelowStrategyNumber:
			retval = below_point3D_point3D_internal(key, query);
			break;
		case RTAboveStrategyNumber:
			retval = above_point3D_point3D_internal(key, query);
			break;
		case RTOverAboveStrategyNumber:
			retval = overabove_point3D_point3D_internal(key, query);
			break;
		case RTOverFrontStrategyNumber:
			retval = overfront_point3D_point3D_internal(key, query);
			break;
		case RTFrontStrategyNumber:
			retval = front_point3D_point3D_internal(key, query);
			break;
		case RTBackStrategyNumber:
			retval = back_point3D_point3D_internal(key, query);
			break;
		case RTOverBackStrategyNumber:
			retval = overback_point3D_point3D_internal(key, query);
			break;
		default:
			elog(ERROR, "unrecognized strategy number: %d", strategy);
			retval = false;		/* keep compiler quiet */
			break;
	}
	return retval;
}

/*
 * Leaf-level consistency for point3D with lseg3D: just apply the query operator
 */
static bool
gist_point3D_leaf_consistent_lseg3D(Point3D *key, Lseg3D *query, StrategyNumber strategy)
{
	bool		retval;

	switch (strategy)
	{
		case RTLeftStrategyNumber:
			retval = left_point3D_lseg3D_internal(key, query);
			break;
		case RTOverLeftStrategyNumber:
			retval = overleft_point3D_lseg3D_internal(key, query);
			break;
		case RTOverRightStrategyNumber:
			retval = overright_point3D_lseg3D_internal(key, query);
			break;
		case RTRightStrategyNumber:
			retval = right_point3D_lseg3D_internal(key, query);
			break;
		case RTContainedByStrategyNumber:
		case RTOldContainedByStrategyNumber:
			retval = contained_point3D_lseg3D_internal(key, query);
			break;
		case RTOverBelowStrategyNumber:
			retval = overbelow_point3D_lseg3D_internal(key, query);
			break;
		case RTBelowStrategyNumber:
			retval = below_point3D_lseg3D_internal(key, query);
			break;
		case RTAboveStrategyNumber:
			retval = above_point3D_lseg3D_internal(key, query);
			break;
		case RTOverAboveStrategyNumber:
			retval = overabove_point3D_lseg3D_internal(key, query);
			break;
		case RTOverFrontStrategyNumber:
			retval = overfront_point3D_lseg3D_internal(key, query);
			break;
		case RTFrontStrategyNumber:
			retval = front_point3D_lseg3D_internal(key, query);
			break;
		case RTBackStrategyNumber:
			retval = back_point3D_lseg3D_internal(key, query);
			break;
		case RTOverBackStrategyNumber:
			retval = overback_point3D_lseg3D_internal(key, query);
			break;
		default:
			elog(ERROR, "unrecognized strategy number: %d", strategy);
			retval = false;		/* keep compiler quiet */
			break;
	}
	return retval;
}

/*
 * Leaf-level consistency for point3D with line3D: just apply the query operator
 */
static bool
gist_point3D_leaf_consistent_line3D(Point3D *key, Line3D *query, StrategyNumber strategy)
{
	bool		retval;

	switch (strategy)
	{
		case RTLeftStrategyNumber:
			retval = left_point3D_line3D_internal(key, query);
			break;
		case RTOverLeftStrategyNumber:
			retval = overleft_point3D_line3D_internal(key, query);
			break;
		case RTOverRightStrategyNumber:
			retval = overright_point3D_line3D_internal(key, query);
			break;
		case RTRightStrategyNumber:
			retval = right_point3D_line3D_internal(key, query);
			break;
		case RTContainedByStrategyNumber:
		case RTOldContainedByStrategyNumber:
			retval = contained_point3D_line3D_internal(key, query);
			break;
		case RTOverBelowStrategyNumber:
			retval = overbelow_point3D_line3D_internal(key, query);
			break;
		case RTBelowStrategyNumber:
			retval = below_point3D_line3D_internal(key, query);
			break;
		case RTAboveStrategyNumber:
			retval = above_point3D_line3D_internal(key, query);
			break;
		case RTOverAboveStrategyNumber:
			retval = overabove_point3D_line3D_internal(key, query);
			break;
		case RTOverFrontStrategyNumber:
			retval = overfront_point3D_line3D_internal(key, query);
			break;
		case RTFrontStrategyNumber:
			retval = front_point3D_line3D_internal(key, query);
			break;
		case RTBackStrategyNumber:
			retval = back_point3D_line3D_internal(key, query);
			break;
		case RTOverBackStrategyNumber:
			retval = overback_point3D_line3D_internal(key, query);
			break;
		default:
			elog(ERROR, "unrecognized strategy number: %d", strategy);
			retval = false;		/* keep compiler quiet */
			break;
	}
	return retval;
}

/*
 * Leaf-level consistency for point3D with box3D: just apply the query operator
 */
static bool
gist_point3D_leaf_consistent_box3D(Point3D *key, Box3D *query, StrategyNumber strategy)
{
	bool		retval;

	switch (strategy)
	{
		case RTLeftStrategyNumber:
			retval = left_point3D_box3D_internal(key, query);
			break;
		case RTOverLeftStrategyNumber:
			retval = overleft_point3D_box3D_internal(key, query);
			break;
		case RTOverRightStrategyNumber:
			retval = overright_point3D_box3D_internal(key, query);
			break;
		case RTRightStrategyNumber:
			retval = right_point3D_box3D_internal(key, query);
			break;
		case RTContainedByStrategyNumber:
		case RTOldContainedByStrategyNumber:
			retval = contained_point3D_box3D_internal(key, query);
			break;
		case RTOverBelowStrategyNumber:
			retval = overbelow_point3D_box3D_internal(key, query);
			break;
		case RTBelowStrategyNumber:
			retval = below_point3D_box3D_internal(key, query);
			break;
		case RTAboveStrategyNumber:
			retval = above_point3D_box3D_internal(key, query);
			break;
		case RTOverAboveStrategyNumber:
			retval = overabove_point3D_box3D_internal(key, query);
			break;
		case RTOverFrontStrategyNumber:
			retval = overfront_point3D_box3D_internal(key, query);
			break;
		case RTFrontStrategyNumber:
			retval = front_point3D_box3D_internal(key, query);
			break;
		case RTBackStrategyNumber:
			retval = back_point3D_box3D_internal(key, query);
			break;
		case RTOverBackStrategyNumber:
			retval = overback_point3D_box3D_internal(key, query);
			break;
		default:
			elog(ERROR, "unrecognized strategy number: %d", strategy);
			retval = false;		/* keep compiler quiet */
			break;
	}
	return retval;
}

/*
 * Leaf-level consistency for point3D with path3D: just apply the query operator
 */
static bool
gist_point3D_leaf_consistent_path3D(Point3D *key, Path3D *query, StrategyNumber strategy)
{
	bool		retval;

	switch (strategy)
	{
		case RTLeftStrategyNumber:
			retval = left_point3D_path3D_internal(key, query);
			break;
		case RTOverLeftStrategyNumber:
			retval = overleft_point3D_path3D_internal(key, query);
			break;
		case RTOverRightStrategyNumber:
			retval = overright_point3D_path3D_internal(key, query);
			break;
		case RTRightStrategyNumber:
			retval = right_point3D_path3D_internal(key, query);
			break;
		case RTContainedByStrategyNumber:
		case RTOldContainedByStrategyNumber:
			retval = contained_point3D_path3D_internal(key, query);
			break;
		case RTOverBelowStrategyNumber:
			retval = overbelow_point3D_path3D_internal(key, query);
			break;
		case RTBelowStrategyNumber:
			retval = below_point3D_path3D_internal(key, query);
			break;
		case RTAboveStrategyNumber:
			retval = above_point3D_path3D_internal(key, query);
			break;
		case RTOverAboveStrategyNumber:
			retval = overabove_point3D_path3D_internal(key, query);
			break;
		case RTOverFrontStrategyNumber:
			retval = overfront_point3D_path3D_internal(key, query);
			break;
		case RTFrontStrategyNumber:
			retval = front_point3D_path3D_internal(key, query);
			break;
		case RTBackStrategyNumber:
			retval = back_point3D_path3D_internal(key, query);
			break;
		case RTOverBackStrategyNumber:
			retval = overback_point3D_path3D_internal(key, query);
			break;
		default:
			elog(ERROR, "unrecognized strategy number: %d", strategy);
			retval = false;		/* keep compiler quiet */
			break;
	}
	return retval;
}

/*
 * Leaf-level consistency for point3D with polygon3D: just apply the query operator
 */
static bool
gist_point3D_leaf_consistent_poly3D(Point3D *key, Polygon3D *query, StrategyNumber strategy)
{
	bool		retval;

	switch (strategy)
	{
		case RTLeftStrategyNumber:
			retval = left_point3D_poly3D_internal(key, query);
			break;
		case RTOverLeftStrategyNumber:
			retval = overleft_point3D_poly3D_internal(key, query);
			break;
		case RTOverRightStrategyNumber:
			retval = overright_point3D_poly3D_internal(key, query);
			break;
		case RTRightStrategyNumber:
			retval = right_point3D_poly3D_internal(key, query);
			break;
		case RTContainedByStrategyNumber:
		case RTOldContainedByStrategyNumber:
			retval = contained_point3D_poly3D_internal(key, query);
			break;
		case RTOverBelowStrategyNumber:
			retval = overbelow_point3D_poly3D_internal(key, query);
			break;
		case RTBelowStrategyNumber:
			retval = below_point3D_poly3D_internal(key, query);
			break;
		case RTAboveStrategyNumber:
			retval = above_point3D_poly3D_internal(key, query);
			break;
		case RTOverAboveStrategyNumber:
			retval = overabove_point3D_poly3D_internal(key, query);
			break;
		case RTOverFrontStrategyNumber:
			retval = overfront_point3D_poly3D_internal(key, query);
			break;
		case RTFrontStrategyNumber:
			retval = front_point3D_poly3D_internal(key, query);
			break;
		case RTBackStrategyNumber:
			retval = back_point3D_poly3D_internal(key, query);
			break;
		case RTOverBackStrategyNumber:
			retval = overback_point3D_poly3D_internal(key, query);
			break;
		default:
			elog(ERROR, "unrecognized strategy number: %d", strategy);
			retval = false;		/* keep compiler quiet */
			break;
	}
	return retval;
}

/*
 * Leaf-level consistency for point3D with sphere: just apply the query operator
 */
static bool
gist_point3D_leaf_consistent_sphere(Point3D *key, Sphere *query, StrategyNumber strategy)
{
	bool		retval;

	switch (strategy)
	{
		case RTLeftStrategyNumber:
			retval = left_point3D_sphere_internal(key, query);
			break;
		case RTOverLeftStrategyNumber:
			retval = overleft_point3D_sphere_internal(key, query);
			break;
		case RTOverRightStrategyNumber:
			retval = overright_point3D_sphere_internal(key, query);
			break;
		case RTRightStrategyNumber:
			retval = right_point3D_sphere_internal(key, query);
			break;
		case RTContainedByStrategyNumber:
		case RTOldContainedByStrategyNumber:
			retval = contained_point3D_sphere_internal(key, query);
			break;
		case RTOverBelowStrategyNumber:
			retval = overbelow_point3D_sphere_internal(key, query);
			break;
		case RTBelowStrategyNumber:
			retval = below_point3D_sphere_internal(key, query);
			break;
		case RTAboveStrategyNumber:
			retval = above_point3D_sphere_internal(key, query);
			break;
		case RTOverAboveStrategyNumber:
			retval = overabove_point3D_sphere_internal(key, query);
			break;
		case RTOverFrontStrategyNumber:
			retval = overfront_point3D_sphere_internal(key, query);
			break;
		case RTFrontStrategyNumber:
			retval = front_point3D_sphere_internal(key, query);
			break;
		case RTBackStrategyNumber:
			retval = back_point3D_sphere_internal(key, query);
			break;
		case RTOverBackStrategyNumber:
			retval = overback_point3D_sphere_internal(key, query);
			break;
		default:
			elog(ERROR, "unrecognized strategy number: %d", strategy);
			retval = false;		/* keep compiler quiet */
			break;
	}
	return retval;
}

/*-------------------------------------------------------------------------*/

/*
 * Leaf-level consistency for box3D with point3D: just apply the query operator
 */
static bool
gist_box3D_leaf_consistent_point3D(Box3D *key, Point3D *query, StrategyNumber strategy)
{
	bool		retval;

	switch (strategy)
	{
		case RTLeftStrategyNumber:
			retval = left_box3D_point3D_internal(key, query);
			break;
		case RTOverLeftStrategyNumber:
			retval = overleft_box3D_point3D_internal(key, query);
			break;
		case RTOverRightStrategyNumber:
			retval = overright_box3D_point3D_internal(key, query);
			break;
		case RTRightStrategyNumber:
			retval = right_box3D_point3D_internal(key, query);
			break;
		case RTContainsStrategyNumber:
		case RTOldContainsStrategyNumber:
			retval = contain_box3D_point3D_internal(key, query);
			break;
		case RTOverBelowStrategyNumber:
			retval = overbelow_box3D_point3D_internal(key, query);
			break;
		case RTBelowStrategyNumber:
			retval = below_box3D_point3D_internal(key, query);
			break;
		case RTAboveStrategyNumber:
			retval = above_box3D_point3D_internal(key, query);
			break;
		case RTOverAboveStrategyNumber:
			retval = overabove_box3D_point3D_internal(key, query);
			break;
		case RTOverFrontStrategyNumber:
			retval = overfront_box3D_point3D_internal(key, query);
			break;
		case RTFrontStrategyNumber:
			retval = front_box3D_point3D_internal(key, query);
			break;
		case RTBackStrategyNumber:
			retval = back_box3D_point3D_internal(key, query);
			break;
		case RTOverBackStrategyNumber:
			retval = overback_box3D_point3D_internal(key, query);
			break;
		default:
			elog(ERROR, "unrecognized strategy number: %d", strategy);
			retval = false;		/* keep compiler quiet */
			break;
	}
	return retval;
}

/*
 * Leaf-level consistency for box3D with lseg3D: just apply the query operator
 */
static bool
gist_box3D_leaf_consistent_lseg3D(Box3D *key, Lseg3D *query, StrategyNumber strategy)
{
	bool		retval;

	switch (strategy)
	{
		case RTLeftStrategyNumber:
			retval = left_box3D_lseg3D_internal(key, query);
			break;
		case RTOverLeftStrategyNumber:
			retval = overleft_box3D_lseg3D_internal(key, query);
			break;
		case RTOverRightStrategyNumber:
			retval = overright_box3D_lseg3D_internal(key, query);
			break;
		case RTRightStrategyNumber:
			retval = right_box3D_lseg3D_internal(key, query);
			break;
		case RTContainsStrategyNumber:
		case RTOldContainsStrategyNumber:
			retval = contain_box3D_lseg3D_internal(key, query);
			break;
		case RTOverBelowStrategyNumber:
			retval = overbelow_box3D_lseg3D_internal(key, query);
			break;
		case RTBelowStrategyNumber:
			retval = below_box3D_lseg3D_internal(key, query);
			break;
		case RTAboveStrategyNumber:
			retval = above_box3D_lseg3D_internal(key, query);
			break;
		case RTOverAboveStrategyNumber:
			retval = overabove_box3D_lseg3D_internal(key, query);
			break;
		case RTOverFrontStrategyNumber:
			retval = overfront_box3D_lseg3D_internal(key, query);
			break;
		case RTFrontStrategyNumber:
			retval = front_box3D_lseg3D_internal(key, query);
			break;
		case RTBackStrategyNumber:
			retval = back_box3D_lseg3D_internal(key, query);
			break;
		case RTOverBackStrategyNumber:
			retval = overback_box3D_lseg3D_internal(key, query);
			break;
		default:
			elog(ERROR, "unrecognized strategy number: %d", strategy);
			retval = false;		/* keep compiler quiet */
			break;
	}
	return retval;
}

/*
 * Leaf-level consistency for box3D with line3D: just apply the query operator
 */
static bool
gist_box3D_leaf_consistent_line3D(Box3D *key, Line3D *query, StrategyNumber strategy)
{
	bool		retval;

	switch (strategy)
	{
		case RTLeftStrategyNumber:
			retval = left_box3D_line3D_internal(key, query);
			break;
		case RTOverLeftStrategyNumber:
			retval = overleft_box3D_line3D_internal(key, query);
			break;
		case RTOverRightStrategyNumber:
			retval = overright_box3D_line3D_internal(key, query);
			break;
		case RTRightStrategyNumber:
			retval = right_box3D_line3D_internal(key, query);
			break;
		case RTOverBelowStrategyNumber:
			retval = overbelow_box3D_line3D_internal(key, query);
			break;
		case RTBelowStrategyNumber:
			retval = below_box3D_line3D_internal(key, query);
			break;
		case RTAboveStrategyNumber:
			retval = above_box3D_line3D_internal(key, query);
			break;
		case RTOverAboveStrategyNumber:
			retval = overabove_box3D_line3D_internal(key, query);
			break;
		case RTOverFrontStrategyNumber:
			retval = overfront_box3D_line3D_internal(key, query);
			break;
		case RTFrontStrategyNumber:
			retval = front_box3D_line3D_internal(key, query);
			break;
		case RTBackStrategyNumber:
			retval = back_box3D_line3D_internal(key, query);
			break;
		case RTOverBackStrategyNumber:
			retval = overback_box3D_line3D_internal(key, query);
			break;
		default:
			elog(ERROR, "unrecognized strategy number: %d", strategy);
			retval = false;		/* keep compiler quiet */
			break;
	}
	return retval;
}

/*
 * Leaf-level consistency for box3D with box3D: just apply the query operator
 */
static bool
gist_box3D_leaf_consistent_box3D(Box3D *key, Box3D *query, StrategyNumber strategy)
{
	bool		retval;

	switch (strategy)
	{
		case RTLeftStrategyNumber:
			retval = left_box3D_box3D_internal(key, query);
			break;
		case RTOverLeftStrategyNumber:
			retval = overleft_box3D_box3D_internal(key, query);
			break;
		case RTOverlapStrategyNumber:
			retval = overlap_box3D_box3D_internal(key, query);
			break;
		case RTOverRightStrategyNumber:
			retval = overright_box3D_box3D_internal(key, query);
			break;
		case RTRightStrategyNumber:
			retval = right_box3D_box3D_internal(key, query);
			break;
		case RTSameStrategyNumber:
			retval = box3D_same_internal(key, query);
			break;
		case RTContainsStrategyNumber:
		case RTOldContainsStrategyNumber:
			retval = contain_box3D_box3D_internal(key, query);
			break;
		case RTContainedByStrategyNumber:
		case RTOldContainedByStrategyNumber:
			retval = contained_box3D_box3D_internal(key, query);
			break;
		case RTOverBelowStrategyNumber:
			retval = overbelow_box3D_box3D_internal(key, query);
			break;
		case RTBelowStrategyNumber:
			retval = below_box3D_box3D_internal(key, query);
			break;
		case RTAboveStrategyNumber:
			retval = above_box3D_box3D_internal(key, query);
			break;
		case RTOverAboveStrategyNumber:
			retval = overabove_box3D_box3D_internal(key, query);
			break;
		case RTOverFrontStrategyNumber:
			retval = overfront_box3D_box3D_internal(key, query);
			break;
		case RTFrontStrategyNumber:
			retval = front_box3D_box3D_internal(key, query);
			break;
		case RTBackStrategyNumber:
			retval = back_box3D_box3D_internal(key, query);
			break;
		case RTOverBackStrategyNumber:
			retval = overback_box3D_box3D_internal(key, query);
			break;
		default:
			elog(ERROR, "unrecognized strategy number: %d", strategy);
			retval = false;		/* keep compiler quiet */
			break;
	}
	return retval;
}

/*
 * Leaf-level consistency for box3D with path3D: just apply the query operator
 */
static bool
gist_box3D_leaf_consistent_path3D(Box3D *key, Path3D *query, StrategyNumber strategy)
{
	bool		retval;

	switch (strategy)
	{
		case RTLeftStrategyNumber:
			retval = left_box3D_path3D_internal(key, query);
			break;
		case RTOverLeftStrategyNumber:
			retval = overleft_box3D_path3D_internal(key, query);
			break;
		case RTOverRightStrategyNumber:
			retval = overright_box3D_path3D_internal(key, query);
			break;
		case RTRightStrategyNumber:
			retval = right_box3D_path3D_internal(key, query);
			break;
		case RTContainsStrategyNumber:
		case RTOldContainsStrategyNumber:
			retval = contain_box3D_path3D_internal(key, query);
			break;
		case RTOverBelowStrategyNumber:
			retval = overbelow_box3D_path3D_internal(key, query);
			break;
		case RTBelowStrategyNumber:
			retval = below_box3D_path3D_internal(key, query);
			break;
		case RTAboveStrategyNumber:
			retval = above_box3D_path3D_internal(key, query);
			break;
		case RTOverAboveStrategyNumber:
			retval = overabove_box3D_path3D_internal(key, query);
			break;
		case RTOverFrontStrategyNumber:
			retval = overfront_box3D_path3D_internal(key, query);
			break;
		case RTFrontStrategyNumber:
			retval = front_box3D_path3D_internal(key, query);
			break;
		case RTBackStrategyNumber:
			retval = back_box3D_path3D_internal(key, query);
			break;
		case RTOverBackStrategyNumber:
			retval = overback_box3D_path3D_internal(key, query);
			break;
		default:
			elog(ERROR, "unrecognized strategy number: %d", strategy);
			retval = false;		/* keep compiler quiet */
			break;
	}
	return retval;
}

/*
 * Leaf-level consistency for box3D with polygon3D: just apply the query operator
 */
static bool
gist_box3D_leaf_consistent_poly3D(Box3D *key, Polygon3D *query, StrategyNumber strategy)
{
	bool		retval;

	switch (strategy)
	{
		case RTLeftStrategyNumber:
			retval = left_box3D_poly3D_internal(key, query);
			break;
		case RTOverLeftStrategyNumber:
			retval = overleft_box3D_poly3D_internal(key, query);
			break;
		case RTOverlapStrategyNumber:
			retval = overlap_box3D_poly3D_internal(key, query);
			break;
		case RTOverRightStrategyNumber:
			retval = overright_box3D_poly3D_internal(key, query);
			break;
		case RTRightStrategyNumber:
			retval = right_box3D_poly3D_internal(key, query);
			break;
		case RTContainsStrategyNumber:
		case RTOldContainsStrategyNumber:
			retval = contain_box3D_poly3D_internal(key, query);
			break;
		case RTOverBelowStrategyNumber:
			retval = overbelow_box3D_poly3D_internal(key, query);
			break;
		case RTBelowStrategyNumber:
			retval = below_box3D_poly3D_internal(key, query);
			break;
		case RTAboveStrategyNumber:
			retval = above_box3D_poly3D_internal(key, query);
			break;
		case RTOverAboveStrategyNumber:
			retval = overabove_box3D_poly3D_internal(key, query);
			break;
		case RTOverFrontStrategyNumber:
			retval = overfront_box3D_poly3D_internal(key, query);
			break;
		case RTFrontStrategyNumber:
			retval = front_box3D_poly3D_internal(key, query);
			break;
		case RTBackStrategyNumber:
			retval = back_box3D_poly3D_internal(key, query);
			break;
		case RTOverBackStrategyNumber:
			retval = overback_box3D_poly3D_internal(key, query);
			break;
		default:
			elog(ERROR, "unrecognized strategy number: %d", strategy);
			retval = false;		/* keep compiler quiet */
			break;
	}
	return retval;
}

/*
 * Leaf-level consistency for boxbox3D with sphere: just apply the query operator
 */
static bool
gist_box3D_leaf_consistent_sphere(Box3D *key, Sphere *query, StrategyNumber strategy)
{
	bool		retval;

	switch (strategy)
	{
		case RTLeftStrategyNumber:
			retval = left_box3D_sphere_internal(key, query);
			break;
		case RTOverLeftStrategyNumber:
			retval = overleft_box3D_sphere_internal(key, query);
			break;
		case RTOverlapStrategyNumber:
			retval = overlap_box3D_sphere_internal(key, query);
			break;
		case RTOverRightStrategyNumber:
			retval = overright_box3D_sphere_internal(key, query);
			break;
		case RTRightStrategyNumber:
			retval = right_box3D_sphere_internal(key, query);
			break;
		case RTContainsStrategyNumber:
		case RTOldContainsStrategyNumber:
			retval = contain_box3D_sphere_internal(key, query);
			break;
		case RTContainedByStrategyNumber:
		case RTOldContainedByStrategyNumber:
			retval = contained_box3D_sphere_internal(key, query);
			break;
		case RTOverBelowStrategyNumber:
			retval = overbelow_box3D_sphere_internal(key, query);
			break;
		case RTBelowStrategyNumber:
			retval = below_box3D_sphere_internal(key, query);
			break;
		case RTAboveStrategyNumber:
			retval = above_box3D_sphere_internal(key, query);
			break;
		case RTOverAboveStrategyNumber:
			retval = overabove_box3D_sphere_internal(key, query);
			break;
		case RTOverFrontStrategyNumber:
			retval = overfront_box3D_sphere_internal(key, query);
			break;
		case RTFrontStrategyNumber:
			retval = front_box3D_sphere_internal(key, query);
			break;
		case RTBackStrategyNumber:
			retval = back_box3D_sphere_internal(key, query);
			break;
		case RTOverBackStrategyNumber:
			retval = overback_box3D_sphere_internal(key, query);
			break;
		default:
			elog(ERROR, "unrecognized strategy number: %d", strategy);
			retval = false;		/* keep compiler quiet */
			break;
	}
	return retval;
}

/*****************************************
 * Common rtree functions 
 *****************************************/

/*
 * The GiST consistent method for types such that their value
 * cannot be reconstructed from the index entries, i.e., 
 * path3D, polygon3D, and sphere. 
 * In this case the predicate is only possibly true since it is
 * tested against the bounding box and thus a recheck is needed.
 *
 * Should return false if for all data items x below entry,
 * the predicate x op query must be FALSE, where op is the oper
 * corresponding to strategy in the pg_amop table.
 */
Datum
gist_type3D_consistent(PG_FUNCTION_ARGS)
{
	GISTENTRY  *entry = (GISTENTRY *) PG_GETARG_POINTER(0);
	StrategyNumber strategy = (StrategyNumber) PG_GETARG_UINT16(2);
	Box3D 	   *key = DatumGetBox3DP(entry->key);
	StrategyNumber strategyGroup = strategy / Geo3DStrategyNumberOffset;

	/* Oid		subtype = PG_GETARG_OID(3); */
	bool	   *recheck = (bool *) PG_GETARG_POINTER(4);
	bool		result;

	/* All cases served by this function are inexact */
	*recheck = true;

	if (key == NULL)
		PG_RETURN_BOOL(FALSE);
				
	/*
	 * Since the operators require recheck anyway, we can just use
	 * gist_rtree3D_internal_consistent even at leaf nodes.  (This works in part
	 * because the index entries are bounding boxes not polygons.)
	 */
	switch (strategyGroup)
	{
		case Point3DStrategyNumberGroup:
			{
				Point3D	   *point = PG_GETARG_POINT3D_P(1);

				if (point == NULL)
					PG_RETURN_BOOL(FALSE);
				
				PG_RETURN_BOOL(gist_rtree3D_internal_consistent_point3D(key, point, 
					strategy % Geo3DStrategyNumberOffset));
			}
			break;
		case Lseg3DStrategyNumberGroup:
			{
				Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

				if (lseg == NULL)
					PG_RETURN_BOOL(FALSE);
				
				PG_RETURN_BOOL(gist_rtree3D_internal_consistent_lseg3D(key, lseg, 
					strategy % Geo3DStrategyNumberOffset));
			}
			break;
		case Line3DStrategyNumberGroup:
			{
				Line3D	   *line = PG_GETARG_LINE3D_P(1);

				if (line == NULL)
					PG_RETURN_BOOL(FALSE);
				
				PG_RETURN_BOOL(gist_rtree3D_internal_consistent_line3D(key, line, 
					strategy % Geo3DStrategyNumberOffset));
			}
			break;
		case Box3DStrategyNumberGroup:
			{
				Box3D	   *box = PG_GETARG_BOX3D_P(1);

				if (box == NULL)
					PG_RETURN_BOOL(FALSE);
				
				PG_RETURN_BOOL(gist_rtree3D_internal_consistent(key, box, 
					strategy % Geo3DStrategyNumberOffset));
			}
			break;
		case Path3DStrategyNumberGroup:
			{
				Path3D	   *path = PG_GETARG_PATH3D_P(1);

				if (path == NULL)
					PG_RETURN_BOOL(FALSE);
				
				result = gist_rtree3D_internal_consistent_path3D(key, path, 
					strategy % Geo3DStrategyNumberOffset);
				/* Avoid memory leak if supplied path is toasted */
				PG_FREE_IF_COPY(path, 1);
				PG_RETURN_BOOL(result);
			}
			break;
		case Polygon3DStrategyNumberGroup:		
			{
				Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

				if (poly == NULL)
					PG_RETURN_BOOL(FALSE);

				result = gist_rtree3D_internal_consistent_poly3D(key, poly, 
					strategy % Geo3DStrategyNumberOffset);
				/* Avoid memory leak if supplied poly is toasted */
				PG_FREE_IF_COPY(poly, 1);
				PG_RETURN_BOOL(result);
			}
			break;
		case SphereStrategyNumberGroup:			
			{
				Sphere  *sphere = PG_GETARG_SPHERE_P(1);

				if (sphere == NULL)
					PG_RETURN_BOOL(FALSE);

				PG_RETURN_BOOL(gist_rtree3D_internal_consistent_sphere(key, sphere, 
					strategy % Geo3DStrategyNumberOffset));
			}
			break;
		default:
			elog(ERROR, "unrecognized strategy number: %d", strategy);
			PG_RETURN_BOOL(FALSE);
			break;
	}
}

PG_FUNCTION_INFO_V1(gist_type3D_consistent);

static bool
gist_rtree3D_internal_consistent_point3D(Box3D *key, Point3D *query, StrategyNumber strategy)
{
	Box3D 	bbox;
	
	bbox.high.x = bbox.low.x = query->x;
	bbox.high.y = bbox.low.y = query->y;
	bbox.high.z = bbox.low.z = query->z;		
	return gist_rtree3D_internal_consistent(key, &bbox, strategy);
}

static bool
gist_rtree3D_internal_consistent_lseg3D(Box3D *key, Lseg3D *query, StrategyNumber strategy)
{
	Box3D 	bbox;
	
	bbox.low.x = Min(query->p[0].x, query->p[1].x);
	bbox.low.y = Min(query->p[0].y, query->p[1].y);
	bbox.low.z = Min(query->p[0].z, query->p[1].z);		
	bbox.high.x = Max(query->p[1].x, query->p[1].x);
	bbox.high.y = Max(query->p[1].y, query->p[1].y);
	bbox.high.z = Max(query->p[1].z, query->p[1].z);		
	return gist_rtree3D_internal_consistent(key, &bbox, strategy);
}

static bool
gist_rtree3D_internal_consistent_line3D(Box3D *key, Line3D *query, StrategyNumber strategy)
{
	bool		retval;

	switch (strategy)
	{
		case RTLeftStrategyNumber:
			retval = !overright_box3D_line3D_internal(key, query);
			break;
		case RTOverLeftStrategyNumber:
			retval = !right_box3D_line3D_internal(key,query);
			break;
		case RTOverRightStrategyNumber:
			retval = !left_box3D_line3D_internal(key, query);
			break;
		case RTRightStrategyNumber:
			retval = !overleft_box3D_line3D_internal(key, query);
			break;
		case RTContainedByStrategyNumber:
		case RTOldContainedByStrategyNumber:
			retval = inter_box3D_line3D(key, query);
			break;
		case RTOverBelowStrategyNumber:
			retval = !above_box3D_line3D_internal(key, query);
			break;
		case RTBelowStrategyNumber:
			retval = !overabove_box3D_line3D_internal(key, query);
			break;
		case RTAboveStrategyNumber:
			retval = !overbelow_box3D_line3D_internal(key, query);
			break;
		case RTOverAboveStrategyNumber:
			retval = !below_box3D_line3D_internal(key, query);
			break;
		case RTOverFrontStrategyNumber:
			retval = !front_box3D_line3D_internal(key, query);
			break;
		case RTFrontStrategyNumber:
			retval = !overfront_box3D_line3D_internal(key, query);
			break;
		case RTBackStrategyNumber:
			retval = !overback_box3D_line3D_internal(key, query);
			break;
		case RTOverBackStrategyNumber:
			retval = !back_box3D_line3D_internal(key, query);
			break;
		default:
			elog(ERROR, "gist_rtree3D_internal_consistent_line3D: unrecognized strategy number: %d", strategy);
			retval = false;		/* keep compiler quiet */
			break;
	}
	return retval;
}

static bool
gist_rtree3D_internal_consistent_path3D(Box3D *key, Path3D *query, StrategyNumber strategy)
{	
	return gist_rtree3D_internal_consistent(key, &query->boundbox, strategy);
}

static bool
gist_rtree3D_internal_consistent_poly3D(Box3D *key, Polygon3D *query, StrategyNumber strategy)
{	
	return gist_rtree3D_internal_consistent(key, &query->boundbox, strategy);
}

static bool
gist_rtree3D_internal_consistent_sphere(Box3D *key, Sphere *query, StrategyNumber strategy)
{
	Box3D 	bbox;
	
	bbox.high.x = query->center.x + query->radius;
	bbox.low.x = query->center.x - query->radius;
	bbox.high.y = query->center.y + query->radius;
	bbox.low.y = query->center.y - query->radius;
	bbox.high.z = query->center.z + query->radius;
	bbox.low.z = query->center.z - query->radius;
	return gist_rtree3D_internal_consistent(key, &bbox, strategy);
}

/*
 * Internal-page consistency for all types excepted Line3D 
 *
 * We can use the same function since these types use bounding boxes both as the
 * internal-page representation and also for the query.
 */
 
static bool
gist_rtree3D_internal_consistent(Box3D *key, Box3D *query, StrategyNumber strategy)
{
	bool		retval;

	switch (strategy)
	{
		case RTLeftStrategyNumber:
			retval = !overright_box3D_box3D_internal(key, query);
			break;
		case RTOverLeftStrategyNumber:
			retval = !right_box3D_box3D_internal(key,query);
			break;
		case RTOverlapStrategyNumber:
			retval = overlap_box3D_box3D_internal(key, query);
			break;
		case RTOverRightStrategyNumber:
			retval = !left_box3D_box3D_internal(key, query);
			break;
		case RTRightStrategyNumber:
			retval = !overleft_box3D_box3D_internal(key, query);
			break;
		case RTSameStrategyNumber:
		case RTContainsStrategyNumber:
		case RTOldContainsStrategyNumber:
			retval = contain_box3D_box3D_internal(key, query);
			break;
		case RTContainedByStrategyNumber:
		case RTOldContainedByStrategyNumber:
			retval = overlap_box3D_box3D_internal(key, query);
			break;
		case RTOverBelowStrategyNumber:
			retval = !above_box3D_box3D_internal(key, query);
			break;
		case RTBelowStrategyNumber:
			retval = !overabove_box3D_box3D_internal(key, query);
			break;
		case RTAboveStrategyNumber:
			retval = !overbelow_box3D_box3D_internal(key, query);
			break;
		case RTOverAboveStrategyNumber:
			retval = !below_box3D_box3D_internal(key, query);
			break;
		case RTOverFrontStrategyNumber:
			retval = !front_box3D_box3D_internal(key, query);
			break;
		case RTFrontStrategyNumber:
			retval = !overfront_box3D_box3D_internal(key, query);
			break;
		case RTBackStrategyNumber:
			retval = !overback_box3D_box3D_internal(key, query);
			break;
		case RTOverBackStrategyNumber:
			retval = !back_box3D_box3D_internal(key, query);
			break;
		default:
			elog(ERROR, "gist_rtree3D_internal_consistent: unrecognized strategy number: %d", strategy);
			retval = false;		/* keep compiler quiet */
			break;
	}
	return retval;
}

/**************************************************
 * Polygon3D ops
 **************************************************/

/*
 * GiST compress for polygons: represent a polygon by its bounding box
 */
Datum
gist_poly3D_compress(PG_FUNCTION_ARGS)
{
	GISTENTRY  *entry = (GISTENTRY *) PG_GETARG_POINTER(0);
	GISTENTRY  *retval;

	if (entry->leafkey)
	{
		Polygon3D  *in = DatumGetPolygon3DP(entry->key);
		Box3D	   *r;

		r = (Box3D *) palloc(sizeof(Box3D));
		memcpy((void *) r, (void *) &(in->boundbox), sizeof(Box3D));

		retval = (GISTENTRY *) palloc(sizeof(GISTENTRY));
		gistentryinit(*retval, PointerGetDatum(r),
					  entry->rel, entry->page,
					  entry->offset, FALSE);
	}
	else
		retval = entry;
	PG_RETURN_POINTER(retval);
}

PG_FUNCTION_INFO_V1(gist_poly3D_compress);

/**************************************************
 * Sphere ops
 **************************************************/

/*
 * GiST compress for spheres: represent a sphere by its bounding box
 */
Datum
gist_sphere_compress(PG_FUNCTION_ARGS)
{
	GISTENTRY  *entry = (GISTENTRY *) PG_GETARG_POINTER(0);
	GISTENTRY  *retval;

	if (entry->leafkey)
	{
		Sphere	   *in = DatumGetSphereP(entry->key);
		Box3D	   *r;

		r = (Box3D *) palloc(sizeof(Box3D));
		r->high.x = in->center.x + in->radius;
		r->low.x = in->center.x - in->radius;
		r->high.y = in->center.y + in->radius;
		r->low.y = in->center.y - in->radius;
		r->high.z = in->center.z + in->radius;
		r->low.z = in->center.z - in->radius;

		retval = (GISTENTRY *) palloc(sizeof(GISTENTRY));
		gistentryinit(*retval, PointerGetDatum(r),
					  entry->rel, entry->page,
					  entry->offset, FALSE);
	}
	else
		retval = entry;
	PG_RETURN_POINTER(retval);
}

PG_FUNCTION_INFO_V1(gist_sphere_compress);

/**************************************************
 * Point3D ops
 **************************************************/

/*
 * GiST compress method for point3D
 */
Datum
gist_point3D_compress(PG_FUNCTION_ARGS)
{
	GISTENTRY  *entry = (GISTENTRY *) PG_GETARG_POINTER(0);

	if (entry->leafkey)			/* Point3D, actually */
	{
		Box3D	   *box = palloc(sizeof(Box3D));
		Point3D	   *point = DatumGetPoint3DP(entry->key);
		GISTENTRY  *retval = palloc(sizeof(GISTENTRY));

		box->high = box->low = *point;

		gistentryinit(*retval, Box3DPGetDatum(box),
					  entry->rel, entry->page, entry->offset, FALSE);

		PG_RETURN_POINTER(retval);
	}

	PG_RETURN_POINTER(entry);
}

PG_FUNCTION_INFO_V1(gist_point3D_compress);

/*
 * GiST consistent method for point3D
 */
Datum
gist_point3D_consistent(PG_FUNCTION_ARGS)
{
	GISTENTRY  *entry = (GISTENTRY *) PG_GETARG_POINTER(0);
	StrategyNumber strategy = (StrategyNumber) PG_GETARG_UINT16(2);
	Box3D 	   *key = DatumGetBox3DP(entry->key);
	StrategyNumber strategyGroup = strategy / Geo3DStrategyNumberOffset;

	/* Oid		subtype = PG_GETARG_OID(3); */
	bool	   *recheck = (bool *) PG_GETARG_POINTER(4);
	bool		result;

	/* All cases served by this function are exact */
	*recheck = false;

	if (key == NULL)
		PG_RETURN_BOOL(FALSE);
				
	/*
	 * if entry is leaf, use gist_box3D_leaf_consistent_x
	 * else use gist_rtree3D_internal_consistent_x
	 * depending on the type x of the query parameter 
	 */
	if (GIST_LEAF(entry))
	{
		Assert(key->high.x == key->low.x
			   && key->high.y == key->low.y
			   && key->high.z == key->low.z);
		switch (strategyGroup)
		{
			case Point3DStrategyNumberGroup:
				{
					Point3D  *point = PG_GETARG_POINT3D_P(1);

					if (point == NULL)
						PG_RETURN_BOOL(FALSE);

					PG_RETURN_BOOL(gist_point3D_leaf_consistent_point3D(&key->high, point, 
						strategy % Geo3DStrategyNumberOffset));
				}
				break;
			case Lseg3DStrategyNumberGroup:
				{
					Lseg3D  *lseg = PG_GETARG_LSEG3D_P(1);

					if (lseg == NULL)
						PG_RETURN_BOOL(FALSE);

					PG_RETURN_BOOL(gist_point3D_leaf_consistent_lseg3D(&key->high, lseg, 
						strategy % Geo3DStrategyNumberOffset));
				}
				break;
			case Line3DStrategyNumberGroup:
				{
					Line3D  *line = PG_GETARG_LINE3D_P(1);

					if (line == NULL)
						PG_RETURN_BOOL(FALSE);

					PG_RETURN_BOOL(gist_point3D_leaf_consistent_line3D(&key->high, line, 
						strategy % Geo3DStrategyNumberOffset));
				}
				break;
			case Box3DStrategyNumberGroup:
				{
					Box3D  *box = PG_GETARG_BOX3D_P(1);

					if (box == NULL)
						PG_RETURN_BOOL(FALSE);

					PG_RETURN_BOOL(gist_point3D_leaf_consistent_box3D(&key->high, box, 
						strategy % Geo3DStrategyNumberOffset));
				}
				break;
			case Path3DStrategyNumberGroup:
				{
					Path3D  *path = PG_GETARG_PATH3D_P(1);

					if (path == NULL)
						PG_RETURN_BOOL(FALSE);

					result = gist_point3D_leaf_consistent_path3D(&key->high, path, 
							strategy % Geo3DStrategyNumberOffset);
					/* Avoid memory leak if supplied path is toasted */
					PG_FREE_IF_COPY(path, 1);
					PG_RETURN_BOOL(result);
				}
				break;
			case Polygon3DStrategyNumberGroup:
				{
					Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

					if (poly == NULL)
						PG_RETURN_BOOL(FALSE);

					result = gist_point3D_leaf_consistent_poly3D(&key->high, poly, 
							strategy % Geo3DStrategyNumberOffset);
					/* Avoid memory leak if supplied poly is toasted */
					PG_FREE_IF_COPY(poly, 1);
					PG_RETURN_BOOL(result);
				}
				break;
			case SphereStrategyNumberGroup:
				{
					Sphere  *sphere = PG_GETARG_SPHERE_P(1);

					if (sphere == NULL)
						PG_RETURN_BOOL(FALSE);

					PG_RETURN_BOOL(gist_point3D_leaf_consistent_sphere(&key->high, sphere, 
						strategy % Geo3DStrategyNumberOffset));
				}
				break;
			default:
				elog(ERROR, "gist_point3D_consistent: unrecognized strategy number: %d", strategy);
				PG_RETURN_BOOL(false);
				break;
		}
	}
	else
		switch (strategyGroup)
		{
			case Point3DStrategyNumberGroup:
				{
					Point3D  *point = PG_GETARG_POINT3D_P(1);

					if (point == NULL)
						PG_RETURN_BOOL(FALSE);

					PG_RETURN_BOOL(gist_rtree3D_internal_consistent_point3D(key, point, 
						strategy % Geo3DStrategyNumberOffset));
				}
				break;
			case Lseg3DStrategyNumberGroup:
				{
					Lseg3D  *lseg = PG_GETARG_LSEG3D_P(1);

					if (lseg == NULL)
						PG_RETURN_BOOL(FALSE);

					PG_RETURN_BOOL(gist_rtree3D_internal_consistent_lseg3D(key, lseg, 
						strategy % Geo3DStrategyNumberOffset));
				}
				break;
			case Line3DStrategyNumberGroup:
				{
					Line3D  *line = PG_GETARG_LINE3D_P(1);

					if (line == NULL)
						PG_RETURN_BOOL(FALSE);

					PG_RETURN_BOOL(gist_rtree3D_internal_consistent_line3D(key, line, 
						strategy % Geo3DStrategyNumberOffset));
				}
				break;
			case Box3DStrategyNumberGroup:
				{
					Box3D  *box = PG_GETARG_BOX3D_P(1);

					if (box == NULL)
						PG_RETURN_BOOL(FALSE);

					PG_RETURN_BOOL(gist_rtree3D_internal_consistent(key, box, 
						strategy % Geo3DStrategyNumberOffset));
				}
				break;
			case Path3DStrategyNumberGroup:	
				{
					Path3D  *path = PG_GETARG_PATH3D_P(1);

					if (path == NULL)
						PG_RETURN_BOOL(FALSE);

					result = gist_rtree3D_internal_consistent_path3D(key, path, 
						strategy % Geo3DStrategyNumberOffset);
					/* Avoid memory leak if supplied path is toasted */
					PG_FREE_IF_COPY(path, 1);
					PG_RETURN_BOOL(result);
				}
				break;
			case Polygon3DStrategyNumberGroup:
				{
					Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

					if (poly == NULL)
						PG_RETURN_BOOL(FALSE);

					result = gist_rtree3D_internal_consistent_poly3D(key, poly, 
						strategy % Geo3DStrategyNumberOffset);
					/* Avoid memory leak if supplied poly is toasted */
					PG_FREE_IF_COPY(poly, 1);
					PG_RETURN_BOOL(result);
				}
				break;
			case SphereStrategyNumberGroup:			
				{
					Sphere  *sphere = PG_GETARG_SPHERE_P(1);

					if (sphere == NULL)
						PG_RETURN_BOOL(FALSE);

					PG_RETURN_BOOL(gist_rtree3D_internal_consistent_sphere(key, sphere, 
						strategy % Geo3DStrategyNumberOffset));
				}
				break;
			default:
				elog(ERROR, "gist_point3D_consistent: unrecognized strategy number: %d", strategy);
				PG_RETURN_BOOL(false);
				break;
		}
}

PG_FUNCTION_INFO_V1(gist_point3D_consistent);

/*
 * GiST fetch method for point3D
 *
 * Get point coordinates from its bounding box coordinates and form new
 * gistentry.
 */
Datum
gist_point3D_fetch(PG_FUNCTION_ARGS)
{
	GISTENTRY  *entry = (GISTENTRY *) PG_GETARG_POINTER(0);
	Box3D	   *in = DatumGetBox3DP(entry->key);
	Point3D	   *r;
	GISTENTRY  *retval;

	retval = palloc(sizeof(GISTENTRY));

	r = (Point3D *) palloc(sizeof(Point3D));
	r->x = in->high.x;
	r->y = in->high.y;
	r->z = in->high.z;
	gistentryinit(*retval, PointerGetDatum(r),
				  entry->rel, entry->page,
				  entry->offset, FALSE);

	PG_RETURN_POINTER(retval);
}

PG_FUNCTION_INFO_V1(gist_point3D_fetch);

/*
 * GiST distance method for point3D
 */
/*
 * GiST distance method for point3D
 */
Datum
gist_point3D_distance(PG_FUNCTION_ARGS)
{
	GISTENTRY  *entry = (GISTENTRY *) PG_GETARG_POINTER(0);
	StrategyNumber strategy = (StrategyNumber) PG_GETARG_UINT16(2);
	Box3D 	   *key = DatumGetBox3DP(entry->key);
	double		result;
	StrategyNumber strategyGroup = strategy / Geo3DStrategyNumberOffset;

	/*
	 * if entry is leaf, use dist_point3D_x else use dist_box3D_x 
	 * depending on the type x of the query parameter 
	 */
	if (GIST_LEAF(entry))
	{
		Assert(key->high.x == key->low.x
			   && key->high.y == key->low.y
			   && key->high.z == key->low.z);
		switch (strategyGroup)
		{
			case Point3DStrategyNumberGroup:
				{
					Point3D  *point = PG_GETARG_POINT3D_P(1);

					if (point == NULL)
						PG_RETURN_FLOAT8(DBL_MAX);

					PG_RETURN_FLOAT8(dist_point3D_point3D(&key->high, point));
				}
				break;
			case Lseg3DStrategyNumberGroup:
				{
					Lseg3D  *lseg = PG_GETARG_LSEG3D_P(1);

					if (lseg == NULL)
						PG_RETURN_FLOAT8(DBL_MAX);

					PG_RETURN_FLOAT8(dist_point3D_lseg3D(&key->high, lseg));
				}
				break;
			case Line3DStrategyNumberGroup:
				{
					Line3D  *line = PG_GETARG_LINE3D_P(1);

					if (line == NULL)
						PG_RETURN_FLOAT8(DBL_MAX);

					PG_RETURN_FLOAT8(dist_point3D_line3D(&key->high, line));
				}
				break;
			case Box3DStrategyNumberGroup:
				{
					Box3D  *box = PG_GETARG_BOX3D_P(1);

					if (box == NULL)
						PG_RETURN_FLOAT8(DBL_MAX);

					PG_RETURN_FLOAT8(dist_point3D_box3D(&key->high, box));
				}
				break;
			case Path3DStrategyNumberGroup:
				{
					Path3D  *path = PG_GETARG_PATH3D_P(1);

					if (path == NULL)
						PG_RETURN_FLOAT8(DBL_MAX);

					result = dist_point3D_path3D(&key->high, path);
					/* Avoid memory leak if supplied path is toasted */
					PG_FREE_IF_COPY(path, 1);
					PG_RETURN_FLOAT8(result);
				}
				break;
			case Polygon3DStrategyNumberGroup:
				{
					Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

					if (poly == NULL)
						PG_RETURN_FLOAT8(DBL_MAX);

					result = dist_point3D_poly3D(&key->high, poly);
					/* Avoid memory leak if supplied poly is toasted */
					PG_FREE_IF_COPY(poly, 1);
					PG_RETURN_FLOAT8(result);
				}
				break;
			case SphereStrategyNumberGroup:
				{
					Sphere  *sphere = PG_GETARG_SPHERE_P(1);

					if (sphere == NULL)
						PG_RETURN_FLOAT8(DBL_MAX);

					PG_RETURN_FLOAT8(dist_point3D_sphere(&key->high, sphere));
				}
				break;
			default:
				elog(ERROR, "gist_point3D_consistent: unrecognized strategy number: %d", strategy);
				PG_RETURN_FLOAT8(DBL_MAX);
				break;
		}
	}
	else
		switch (strategyGroup)
		{
			case Point3DStrategyNumberGroup:
				{
					Point3D  *point = PG_GETARG_POINT3D_P(1);

					if (point == NULL)
						PG_RETURN_FLOAT8(DBL_MAX);

					PG_RETURN_FLOAT8(dist_box3D_point3D(key, point));
				}
				break;
			case Lseg3DStrategyNumberGroup:
				{
					Lseg3D  *lseg = PG_GETARG_LSEG3D_P(1);

					if (lseg == NULL)
						PG_RETURN_FLOAT8(DBL_MAX);

					PG_RETURN_FLOAT8(dist_box3D_lseg3D(key, lseg));
				}
				break;
			case Line3DStrategyNumberGroup:
				{
					Line3D  *line = PG_GETARG_LINE3D_P(1);

					if (line == NULL)
						PG_RETURN_FLOAT8(DBL_MAX);

					PG_RETURN_FLOAT8(dist_box3D_line3D(key, line));
				}
				break;
			case Box3DStrategyNumberGroup:
				{
					Box3D  *box = PG_GETARG_BOX3D_P(1);

					if (box == NULL)
						PG_RETURN_FLOAT8(DBL_MAX);

					PG_RETURN_FLOAT8(dist_box3D_box3D(key, box));
				}
				break;
			case Path3DStrategyNumberGroup:	
				{
					Path3D  *path = PG_GETARG_PATH3D_P(1);

					if (path == NULL)
						PG_RETURN_FLOAT8(DBL_MAX);

					result = dist_box3D_path3D(key, path);
					/* Avoid memory leak if supplied path is toasted */
					PG_FREE_IF_COPY(path, 1);
					PG_RETURN_FLOAT8(result);
				}
				break;
			case Polygon3DStrategyNumberGroup:
				{
					Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

					if (poly == NULL)
						PG_RETURN_FLOAT8(DBL_MAX);

					result = dist_box3D_poly3D(key, poly);
					/* Avoid memory leak if supplied poly is toasted */
					PG_FREE_IF_COPY(poly, 1);
					PG_RETURN_FLOAT8(result);
				}
				break;
			case SphereStrategyNumberGroup:			
				{
					Sphere  *sphere = PG_GETARG_SPHERE_P(1);

					if (sphere == NULL)
						PG_RETURN_FLOAT8(DBL_MAX);

					PG_RETURN_FLOAT8(dist_box3D_sphere(key, sphere));
				}
				break;
			default:
				elog(ERROR, "gist_point3D_distance: unrecognized strategy number: %d", strategy);
				PG_RETURN_FLOAT8(DBL_MAX);
				break;
		}
	
}

PG_FUNCTION_INFO_V1(gist_point3D_distance);

/*
 * The inexact GiST distance method for geometric types that store bounding
 * boxes.
 *
 * Compute lossy distance from point to index entries.  The result is inexact
 * because index entries are bounding boxes, not the exact shapes of the
 * indexed geometric types.  We use distance from point to MBR of index entry.
 * This is a lower bound estimate of distance from point to indexed geometric
 * type.
 */
static double
gist_bbox3D_distance(Box3D *key, Datum query, StrategyNumber strategy)
{
	StrategyNumber strategyGroup = strategy / Geo3DStrategyNumberOffset;

	switch (strategyGroup)
	{
		case Point3DStrategyNumberGroup:
			{
				Point3D  *point = DatumGetPoint3DP(query);

				if (point == NULL)
					return DBL_MAX;

				return dist_box3D_point3D(key, point);
			}
			break;
		case Lseg3DStrategyNumberGroup:
			{
				Lseg3D  *lseg = DatumGetLseg3DP(query);

				if (lseg == NULL)
					return DBL_MAX;

				return dist_box3D_lseg3D(key, lseg);
			}
			break;
		case Line3DStrategyNumberGroup:
			{
				Line3D  *line = DatumGetLine3DP(query);

				if (line == NULL)
					return DBL_MAX;

				return dist_box3D_line3D(key, line);
			}
			break;
		case Box3DStrategyNumberGroup:
			{
				Box3D  *box = DatumGetBox3DP(query);

				if (box == NULL)
					return DBL_MAX;

				return dist_box3D_box3D(key, box);
			}
			break;
		case Path3DStrategyNumberGroup:	
			{
				Path3D  *path = DatumGetPath3DP(query);

				if (path == NULL)
					return DBL_MAX;

				return dist_box3D_path3D(key, path);
			}
			break;
		case Polygon3DStrategyNumberGroup:
			{
				Polygon3D  *poly = DatumGetPolygon3DP(query);

				if (poly == NULL)
					return DBL_MAX;

				return dist_box3D_poly3D(key, poly);
			}
			break;
		case SphereStrategyNumberGroup:			
			{
				Sphere  *sphere = DatumGetSphereP(query);

				if (sphere == NULL)
					return DBL_MAX;

				return dist_box3D_sphere(key, sphere);
			}
			break;
		default:
			elog(ERROR, "gist_point3D_distance: unrecognized strategy group number: %d", strategy);
			return DBL_MAX;
			break;
	}
}

PG_FUNCTION_INFO_V1(gist_sphere_distance);

Datum
gist_poly3D_distance(PG_FUNCTION_ARGS)
{
	GISTENTRY  *entry = (GISTENTRY *) PG_GETARG_POINTER(0);
	Datum		query = PG_GETARG_DATUM(1);
	StrategyNumber strategy = (StrategyNumber) PG_GETARG_UINT16(2);
	StrategyNumber strategyGroup = strategy / Geo3DStrategyNumberOffset;
	Box3D 	   *key = DatumGetBox3DP(entry->key);

	/* Oid subtype = PG_GETARG_OID(3); */
	bool	   *recheck = (bool *) PG_GETARG_POINTER(4);
	double		distance;

	/* Bounding box distance is always inexact. */
	*recheck = true;
	
	distance = gist_bbox3D_distance(key, query, strategy);

	/* Avoid memory leak if supplied path or poly is toasted */
	switch (strategyGroup)
	{
		case Path3DStrategyNumberGroup:	
			{
				Path3D  *path = DatumGetPath3DP(query);

				PG_FREE_IF_COPY(path, 1);
			}
			break;
		case Polygon3DStrategyNumberGroup:
			{
				Polygon3D  *poly = DatumGetPolygon3DP(query);

				PG_FREE_IF_COPY(poly, 1);
			}
			break;
	}

	PG_RETURN_FLOAT8(distance);
}

PG_FUNCTION_INFO_V1(gist_poly3D_distance);

Datum
gist_sphere_distance(PG_FUNCTION_ARGS)
{
	GISTENTRY  *entry = (GISTENTRY *) PG_GETARG_POINTER(0);
	Datum		query = PG_GETARG_DATUM(1);
	StrategyNumber strategy = (StrategyNumber) PG_GETARG_UINT16(2);
	StrategyNumber strategyGroup = strategy / Geo3DStrategyNumberOffset;
	Box3D 	   *key = DatumGetBox3DP(entry->key);

	/* Oid subtype = PG_GETARG_OID(3); */
	bool	   *recheck = (bool *) PG_GETARG_POINTER(4);
	double		distance;

	/* Bounding box distance is always inexact. */
	*recheck = true;
	
	distance = gist_bbox3D_distance(key, query, strategy);

	/* Avoid memory leak if supplied path or poly is toasted */
	switch (strategyGroup)
	{
		case Path3DStrategyNumberGroup:	
			{
				Path3D  *path = DatumGetPath3DP(query);

				PG_FREE_IF_COPY(path, 1);
			}
			break;
		case Polygon3DStrategyNumberGroup:
			{
				Polygon3D  *poly = DatumGetPolygon3DP(query);

				PG_FREE_IF_COPY(poly, 1);
			}
			break;
	}

	PG_RETURN_FLOAT8(distance);
}

/*-------------------------------------------------------------------------*/
