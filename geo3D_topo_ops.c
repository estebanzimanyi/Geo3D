/*****************************************************************************
 *
 * geo3D_topoo_ops.c
 * Topological operators on 3D geometries:
 *		Overlap, Contain, Contained, Intersect, Interpoint, Closestpoint, Distance
 *
 * Portions Copyright (c) 1996-2017, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *****************************************************************************/

#include <postgres.h>

#include <math.h>
#include <limits.h>
#include <float.h>
#include <ctype.h>
#include <utils/array.h>

#include <libpq/pqformat.h>
#include <miscadmin.h>
#include <utils/builtins.h>
#include <catalog/namespace.h>
#include "geo3D_decls.h"

/*****************************************************************************
 * Internal routines
 *****************************************************************************/

static void shortest_segm_lseg3D_lseg3D(Lseg3D *lseg1, Lseg3D *lseg2, Point3D *pa, Point3D *pb);
static void shortest_segm_lseg3D_line3D(Lseg3D *lseg, Line3D *line, Point3D *pa, Point3D *pb);
static void shortest_segm_lseg3D_box3D(Lseg3D *lseg, Box3D *box, Point3D *pa, Point3D *pb);
static void shortest_segm_lseg3D_path3D(Lseg3D *lseg, Path3D *path, Point3D *pa, Point3D *pb);
static void shortest_segm_lseg3D_poly3D(Lseg3D *lseg, Polygon3D *poly, Point3D *pa, Point3D *pb);
static void shortest_segm_lseg3D_sphere(Lseg3D *lseg, Sphere *sphere, Point3D *pa, Point3D *pb);

static void shortest_segm_line3D_line3D(Line3D *line1, Line3D *line2, Point3D *pa, Point3D *pb);
static void shortest_segm_line3D_box3D(Line3D *line, Box3D *box, Point3D *pa, Point3D *pb);
static void shortest_segm_line3D_path3D(Line3D *line, Path3D *path, Point3D *pa, Point3D *pb);
static void shortest_segm_line3D_poly3D(Line3D *line, Polygon3D *poly, Point3D *pa, Point3D *pb);
static void shortest_segm_line3D_sphere(Line3D *line, Sphere *sphere, Point3D *pa, Point3D *pb);

static void shortest_segm_box3D_box3D(Box3D *box1, Box3D *box2, Point3D *pa, Point3D *pb);
static void shortest_segm_box3D_path3D(Box3D *box, Path3D *path, Point3D *pa, Point3D *pb);
static void shortest_segm_box3D_poly3D(Box3D *box, Polygon3D *poly, Point3D *pa, Point3D *pb);
static void shortest_segm_box3D_sphere(Box3D *box, Sphere *sphere, Point3D *pa, Point3D *pb);

static void shortest_segm_path3D_path3D(Path3D *path1, Path3D *path2, Point3D *pa, Point3D *pb);
static void shortest_segm_path3D_poly3D(Path3D *path, Polygon3D *poly, Point3D *pa, Point3D *pb);
static void shortest_segm_path3D_sphere(Path3D *path, Sphere *sphere, Point3D *pa, Point3D *pb);

static void shortest_segm_poly3D_poly3D(Polygon3D *poly1, Polygon3D *poly2, Point3D *pa, Point3D *pb);
static void shortest_segm_poly3D_sphere(Polygon3D *poly, Sphere *sphere, Point3D *pa, Point3D *pb);

static void Face(int i0, int i1, int i2, Point3D *point, Point3D direction,
    Point3D PtminusExt, Point3D boxExtent, double *lineParameter);
static void CaseNoZeros(Point3D *pnt, Point3D dir, Point3D boxExtent, double *lineParameter);
static void Case0(int i0, int i1, int i2, Point3D *point, Point3D direction,
    Point3D boxExtent, double *lineParameter);	
static void Case00(int i0, int i1, int i2, Point3D *point, Point3D direction,
    Point3D boxExtent, double *lineParameter);
static void Case000(Point3D *pnt, Point3D boxExtent);
   
static bool touched_lseg2D_inside_poly2D(Point3D *a, Point3D *b, Lseg3D *s, Polygon3D *poly, int start);
static bool lseg2D_inside_poly2D(Point3D *a, Point3D *b, Polygon3D *poly, int start);

static void poly3D_plane(Polygon3D *poly, double *A, double *B, double *C, double *D);
static void poly3D_project2D(Polygon3D  *poly, Point3D *loc0, Point3D *locx, Point3D *locy);

static bool point2D_in_poly2D(Point3D *point, int npts, Point3D *plist);

static bool get_intersection(double faceDist1, double faceDist2, Point3D P1, Point3D P2, Point3D *interpt);
static bool point_in_box(Point3D hit, Point3D low, Point3D high, int Axis);

static bool clip(double denom, double numer, double *t0, double *t1);

/*****************************************************************************
 * Shortest segment routines between two types
 * If the geometries overlap or are parallel (i.e. there are many 
 * shortest segments) the routines return arbitrarily one of them
 *****************************************************************************/
 
/* shortest_segm_lseg3D_TYPE3D */

/* 
 * Return in pa and pb the points defining the shortest segment  
 * connecting the two Lseg3D. 
 *
 * Based on the distance function between two segments in 
 * http://geomalgorithms.com/a07-_distance.html
 */
void
shortest_segm_lseg3D_lseg3D(Lseg3D *lseg1, Lseg3D *lseg2, Point3D *pa, Point3D *pb)
{
	Point3D		u = sub(lseg1->p[1], lseg1->p[0]),
				v = sub(lseg2->p[1], lseg2->p[0]),
				w = sub(lseg1->p[0], lseg2->p[0]);
    double		a = DOT(u,u),         // always >= 0
				b = DOT(u,v),
				c = DOT(v,v),         // always >= 0
				d = DOT(u,w),
    			e = DOT(v,w),
				D = a*c - b*b,        // always >= 0
				sc, sN, sD = D,       // sc = sN / sD, default sD = D >= 0
				tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0

    // compute the line parameters of the two closest points
    if (FPzero(D)) 
	{ 
		// When parallel, force using point P0 on segment lseg1
		// to prevent possible division by 0.0 later
        sN = 0.0;         
        sD = 1.0;         
        tN = e;
        tD = c;
    }
    else 
	{                 
		// get the closest points on the infinite lines
        sN = (b*e - c*d);
        tN = (a*e - b*d);
        if (FPlt(sN, 0.0)) 
		{
			// sc < 0 => the s=0 edge is visible
            sN = 0.0;
            tN = e;
            tD = c;
        }
        else if (FPgt(sN, sD)) 
		{
			// sc > 1  => the s=1 edge is visible
            sN = sD;
            tN = e + b;
            tD = c;
        }
    }

    if (FPlt(tN, 0.0))
	{
		// tc < 0 => the t=0 edge is visible
        tN = 0.0;
        // recompute sc for this edge
        if (FPlt(-d, 0.0))
            sN = 0.0;
        else if (FPgt(-d, a))
            sN = sD;
        else 
		{
            sN = -d;
            sD = a;
        }
    }
    else if (FPgt(tN, tD))
	{      
		// tc > 1  => the t=1 edge is visible
        tN = tD;
        // recompute sc for this edge
        if (FPlt((-d + b), 0.0))
            sN = 0;
        else if (FPgt((-d + b), a))
            sN = sD;
        else 
		{
            sN = (-d +  b);
            sD = a;
        }
    }
    // finally do the division to get sc and tc
    sc = (FPzero(sN) ? 0.0 : sN / sD);
    tc = (FPzero(tN) ? 0.0 : tN / tD);
	
	pa->x = lseg1->p[0].x + sc * u.x;
	pa->y = lseg1->p[0].y + sc * u.y;
	pa->z = lseg1->p[0].z + sc * u.z;
	pb->x = lseg2->p[0].x + tc * v.x;
	pb->y = lseg2->p[0].y + tc * v.y;
	pb->z = lseg2->p[0].z + tc * v.z;
}

/* 
 * Return in pa and pb the points defining the shortest segment  
 * connecting the Lseg3D and the Line3D. 
 *
 * Based on the distance function between two segments in 
 * http://geomalgorithms.com/a07-_distance.html
 */
void
shortest_segm_lseg3D_line3D(Lseg3D *lseg, Line3D *line, Point3D *pa, Point3D *pb)
{
	Point3D		u = sub(lseg->p[1], lseg->p[0]),
				v = sub(line->p[1], line->p[0]),
				w = sub(lseg->p[0], line->p[0]);
    double		a = DOT(u,u),         // always >= 0
				b = DOT(u,v),
				c = DOT(v,v),         // always >= 0
				d = DOT(u,w),
    			e = DOT(v,w),
				D = a*c - b*b,        // always >= 0
				sc, sN, sD = D,       // sc = sN / sD, default sD = D >= 0
				tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0

    // compute the line parameters of the two closest points
    if (FPzero(D)) 
	{ 
		// When parallel, force using point P0 on segment lseg1
		// force using point P0 on segment lseg
		// to prevent possible division by 0.0 later
        sN = 0.0;         
        sD = 1.0;         
        tN = e;
        tD = c;
    }
    else 
	{                 
		// get the closest points on the infinite lines
        sN = (b*e - c*d);
        tN = (a*e - b*d);
        if (FPlt(sN, 0.0)) 
		{
			// sc < 0 => the s=0 edge is visible
            sN = 0.0;
            tN = e;
            tD = c;
        }
        else if (FPgt(sN, sD)) 
		{
			// sc > 1  => the s=1 edge is visible
            sN = sD;
            tN = e + b;
            tD = c;
        }
    }

    // finally do the division to get sc and tc
    sc = (FPzero(sN) ? 0.0 : sN / sD);
    tc = (FPzero(tN) ? 0.0 : tN / tD);

	pa->x = lseg->p[0].x + sc * u.x;
	pa->y = lseg->p[0].y + sc * u.y;
	pa->z = lseg->p[0].z + sc * u.z;
	pb->x = line->p[0].x + tc * v.x;
	pb->y = line->p[0].y + tc * v.y;
	pb->z = line->p[0].z + tc * v.z;
}

/*
 * Return in pa and pb the points defining the shortest segment 
 * connecting the Lseg3D and the Box3D. 
*/

static void 
shortest_segm_lseg3D_box3D(Lseg3D *lseg, Box3D *box, Point3D *pa, Point3D *pb)
{
	Point3D	   *pb1,
			   *pb2;
	Line3D	   *line;

	line = lseg3D_line3D_internal(lseg);
	shortest_segm_line3D_box3D(line, box, pa, pb);
	pfree(line);
	
	if (contain_lseg3D_point3D_internal(lseg, pa))
		return;
	
	pb1 = closestpt_point3D_box3D(&lseg->p[0], box);
	pb2 = closestpt_point3D_box3D(&lseg->p[1], box);
	if (FPgt(dist_point3D_point3D(&lseg->p[0], pb1), dist_point3D_point3D(&lseg->p[1], pb2)))
	{
		*pa = lseg->p[1];
		*pb = *pb2;
	}
	else
	{
		*pa = lseg->p[0];
		*pb = *pb1;		
	}
		
	pfree(pb1);
	pfree(pb2);
    return;
}

/*
 * Return in pa and pb the points defining the shortest segment 
 * connecting the Lseg3D and the Path3D
*/
static void
shortest_segm_lseg3D_path3D(Lseg3D *lseg, Path3D *path, Point3D *pa, Point3D *pb)
{
	Point3D		pa1,
				pb1,
			   *pt;
	double		distance,
				min_distance = DBL_MAX;	
	Lseg3D		lsegpath;

	if (path->npts == 1)
	{
		// The path contains only one point
		pt = closestpt_point3D_lseg3D(&path->p[0], lseg);
		*pa = *pt;
		*pb = path->p[0];
		return;
	}

	/*
	 * the closest point from an lseg to a path is the closest point
	 * from the lseg to any of its constituent segments.
	 */
	for (int i = 0; i < path->npts; i++)
	{
		int			iprev;

		if (i > 0)
			iprev = i - 1;
		else
		{
			if (!path->closed)
				continue;
			iprev = path->npts - 1;		/* include the closure segment */
		}
		lseg3D_construct_pts_stat(&lsegpath, path->p[iprev], path->p[i]);
		shortest_segm_lseg3D_lseg3D(lseg, &lsegpath, &pa1, &pb1);
		distance = dist_point3D_point3D(&pa1, &pb1);
		if (FPlt(distance, min_distance))
		{
			*pa = pa1;
			*pb = pb1;
			min_distance = distance;
		}		
	}
	return;
}

/*
 * Return in pa and pb the points defining the shortest segment  
 * connecting the Lseg3D and the Polygon3D. 
*/

static void 
shortest_segm_lseg3D_poly3D(Lseg3D *lseg, Polygon3D *poly, Point3D *pa, Point3D *pb)
{
	Point3D	   *pb1,
			   *pb2;
	Line3D	   *line;

	line = lseg3D_line3D_internal(lseg);
	shortest_segm_line3D_poly3D(line, poly, pa, pb);
	pfree(line);
	
	if (contain_lseg3D_point3D_internal(lseg, pa))
		return;
	
	pb1 = closestpt_point3D_poly3D(&lseg->p[0], poly);
	pb2 = closestpt_point3D_poly3D(&lseg->p[1], poly);
	if (FPgt(dist_point3D_point3D(&lseg->p[0], pb1), dist_point3D_point3D(&lseg->p[1], pb2)))
	{
		*pa = lseg->p[1];
		*pb = *pb2;
	}
	else
	{
		*pa = lseg->p[0];
		*pb = *pb1;		
	}
		
	pfree(pb1);
	pfree(pb2);
    return;
}

/*
 * Return in pa and pb the points defining the shortest segment
 * connection the Lseg3D and the Sphere. 
*/
static void 
shortest_segm_lseg3D_sphere(Lseg3D *lseg, Sphere *sphere, Point3D *pa, Point3D *pb)
{
	Point3D			v,
				   *pt;
	double 			magn;
	
	pt = closestpt_point3D_lseg3D(&sphere->center, lseg);
	*pa = *pt;
	pfree(pt);
	v = sub(*pa, sphere->center); 
	magn = magnitude(v);
	v = scalar(1/magn, v); 
	v = scalar(sphere->radius, v);
	*pb = add(v, sphere->center);
	return;
}

/*----------------------------------------------------------*/

/* shortest_segm_line3D_TYPE3D */

/*
 * Return in pa and pb the points defining the shortest segment 
 * connecting the two line3D. 
 *
 * Based on http://paulbourke.net/geometry/pointlineplane/
*/

void 
shortest_segm_line3D_line3D(Line3D *line1, Line3D *line2, Point3D *pa, Point3D *pb)
{
	Point3D 	p13,
				p43,
				p21;
	double 		d1343,
				d4321,
				d1321,
				d4343,
				d2121,
				numer,
				denom,
				mua,
				mub;

	p43 = sub(line2->p[1], line2->p[0]);
	p21 = sub(line1->p[1], line1->p[0]);
	p13 = sub(line1->p[0], line2->p[0]);
	d1343 = DOT(p13, p43);
	d4321 = DOT(p43, p21);
	d1321 = DOT(p13, p21);
	d4343 = DOT(p43, p43);
	d2121 = DOT(p21, p21);

	denom = d2121 * d4343 - d4321 * d4321;
	if (FPzero(denom))
	{ 
		// lines are parallel or coincide 
		// force using point P1 on line1 and P2 on line2
        mua = 0;
        mub = 0;
    }
    else 
	{                 
		numer = d1343 * d4321 - d1321 * d4343;
		mua = numer / denom;
		mub = (d1343 + d4321 * mua) / d4343;
	}
	pa->x = line1->p[0].x + mua * p21.x;
	pa->y = line1->p[0].y + mua * p21.y;
	pa->z = line1->p[0].z + mua * p21.z;
	pb->x = line2->p[0].x + mub * p43.x;
	pb->y = line2->p[0].y + mub * p43.y;
	pb->z = line2->p[0].z + mub * p43.z;
}

/*
 * Return in pa and pb the points defining the shortest segment  
 * connecting the Line3D and the Box3D. 
 *
 * Based on https://www.geometrictools.com/GTEngine/Include/Mathematics/GteDistLine3AlignedBox3.h
*/

static void 
shortest_segm_line3D_box3D(Line3D *line, Box3D *box, Point3D *pa, Point3D *pb)
{
    Point3D 		boxCenter, 
					boxExtent,
					lineOrigin,
					lineDirection;
	double			magn,
					lineParameter = 0.0;
	bool 			reflect[3];

    // Get the centered form of the box
    boxCenter = scalar(0.5, add(box->high, box->low));
    boxExtent = scalar(0.5, sub(box->high, box->low));
	
    // Transform the line to the box coordinate system
    lineOrigin = sub(line->p[0], boxCenter);
	lineDirection = sub(line->p[1], line->p[0]);
	magn = magnitude(lineDirection);
	lineDirection = scalar(1/magn, lineDirection);	

    // Apply reflections so that direction vector has nonnegative components.
	if (FPlt(lineDirection.x, 0.0))
	{
		lineOrigin.x = -lineOrigin.x;
		lineDirection.x = -lineDirection.x;
		reflect[0] = true;
	}
	else
		reflect[0] = false;

	if (FPlt(lineDirection.y, 0.0))
	{
		lineOrigin.y = -lineOrigin.y;
		lineDirection.y = -lineDirection.y;
		reflect[1] = true;
	}
	else
		reflect[1] = false;

	if (FPlt(lineDirection.z, 0.0))
	{
		lineOrigin.z = -lineOrigin.z;
		lineDirection.z = -lineDirection.z;
		reflect[2] = true;
	}
	else
		reflect[2] = false;

    if (FPgt(lineDirection.x, 0.0))
    {
        if (FPgt(lineDirection.y, 0.0))
        {
            if (FPgt(lineDirection.z, 0.0))  // (+,+,+)
                CaseNoZeros(&lineOrigin, lineDirection, boxExtent, &lineParameter);
            else  // (+,+,0)
                Case0(0, 1, 2, &lineOrigin, lineDirection, boxExtent, &lineParameter);
        }
        else
        {
            if (FPgt(lineDirection.z, 0.0))  // (+,0,+)
                Case0(0, 2, 1, &lineOrigin, lineDirection, boxExtent, &lineParameter);
            else  // (+,0,0)
                Case00(0, 1, 2, &lineOrigin, lineDirection, boxExtent, &lineParameter);
        }
    }
    else
    {
        if (FPgt(lineDirection.y, 0.0))
        {
            if (FPgt(lineDirection.z, 0.0))  // (0,+,+)
                Case0(1, 2, 0, &lineOrigin, lineDirection, boxExtent, &lineParameter);
            else  // (0,+,0)
                Case00(1, 0, 2, &lineOrigin, lineDirection, boxExtent, &lineParameter);
        }
        else
        {
            if (FPgt(lineDirection.z, 0.0))  // (0,0,+)
                Case00(2, 0, 1, &lineOrigin, lineDirection, boxExtent, &lineParameter);
            else  // (0,0,0)
                Case000(&lineOrigin, boxExtent);
        }
    }

    // Undo the reflections applied previously.
	if (reflect[0])
		lineOrigin.x = -lineOrigin.x;
	if (reflect[1])
		lineOrigin.y = -lineOrigin.y;
	if (reflect[2])
		lineOrigin.z = -lineOrigin.z;

    // Compute the closest point on the line.
    *pa = add(line->p[0], scalar(lineParameter, lineDirection));

    // Compute the closest point on the box.
    *pb = add(boxCenter, lineOrigin);
}

static void 
Face(int i0, int i1, int i2, Point3D *point, Point3D direction,
    Point3D PtminusExt, Point3D boxExtent, double *lineParameter)
{
    double 		pnt[3],
				dir[3],
				PmE[3],
				boxExt[3],
				PpE[3],
				lenSqr, 
				inv, 
				tmp, 
				param, 
				t;
	
	pnt[0] = point->x;
	pnt[1] = point->y;
	pnt[2] = point->z;
	dir[0] = direction.x;
	dir[1] = direction.y;
	dir[2] = direction.z;
	PmE[0] = PtminusExt.x;
	PmE[1] = PtminusExt.y;
	PmE[2] = PtminusExt.z;
	boxExt[0] = boxExtent.x;
	boxExt[1] = boxExtent.y;
	boxExt[2] = boxExtent.z;
	
    PpE[i1] = pnt[i1] + boxExt[i1];
    PpE[i2] = pnt[i2] + boxExt[i2];
    if (FPge(dir[i0] * PpE[i1], dir[i1] * PmE[i0]))
    {
        if (FPge(dir[i0] * PpE[i2], dir[i2] * PmE[i0]))
        {
            // v[i1] >= -e[i1], v[i2] >= -e[i2] (distance = 0)
            pnt[i0] = boxExt[i0];
            inv = 1.0 / dir[i0];
            pnt[i1] -= dir[i1] * PmE[i0] * inv;
            pnt[i2] -= dir[i2] * PmE[i0] * inv;
            *lineParameter = -PmE[i0] * inv;
        }
        else
        {
            // v[i1] >= -e[i1], v[i2] < -e[i2]
            lenSqr = dir[i0] * dir[i0] + dir[i2] * dir[i2];
            tmp = lenSqr * PpE[i1] - dir[i1] * (dir[i0] * PmE[i0] +
                dir[i2] * PpE[i2]);
            if (FPle(tmp, 2.0 * lenSqr * boxExt[i1]))
            {
                t = tmp / lenSqr;
                *lineParameter = param;
                pnt[i0] = boxExt[i0];
                pnt[i1] = t - boxExt[i1];
                pnt[i2] = -boxExt[i2];
            }
            else
            {
                *lineParameter = param;
                pnt[i0] = boxExt[i0];
                pnt[i1] = boxExt[i1];
                pnt[i2] = -boxExt[i2];
            }
        }
    }
    else
    {
        if (FPge(dir[i0] * PpE[i2], dir[i2] * PmE[i0]))
        {
            // v[i1] < -e[i1], v[i2] >= -e[i2]
            lenSqr = dir[i0] * dir[i0] + dir[i1] * dir[i1];
            tmp = lenSqr*PpE[i2] - dir[i2] * (dir[i0] * PmE[i0] +
                dir[i1] * PpE[i1]);
            if (FPle(tmp, 2.0 * lenSqr * boxExt[i2]))
            {
                t = tmp / lenSqr;
                *lineParameter = param;
                pnt[i0] = boxExt[i0];
                pnt[i1] = -boxExt[i1];
                pnt[i2] = t - boxExt[i2];
            }
            else
            {
                *lineParameter = param;
                pnt[i0] = boxExt[i0];
                pnt[i1] = -boxExt[i1];
                pnt[i2] = boxExt[i2];
            }
        }
        else
        {
            // v[i1] < -e[i1], v[i2] < -e[i2]
            lenSqr = dir[i0] * dir[i0] + dir[i2] * dir[i2];
            tmp = lenSqr*PpE[i1] - dir[i1] * (dir[i0] * PmE[i0] +
                dir[i2] * PpE[i2]);
            if (FPge(tmp, 0.0))
            {
                // v[i1]-edge is closest
                if (FPle(tmp, 2.0 * lenSqr * boxExt[i1]))
                {
                    t = tmp / lenSqr;
                    *lineParameter = param;
                    pnt[i0] = boxExt[i0];
                    pnt[i1] = t - boxExt[i1];
                    pnt[i2] = -boxExt[i2];
                }
                else
                {
                    *lineParameter = param;
                    pnt[i0] = boxExt[i0];
                    pnt[i1] = boxExt[i1];
                    pnt[i2] = -boxExt[i2];
                }
                return;
            }

            lenSqr = dir[i0] * dir[i0] + dir[i1] * dir[i1];
            tmp = lenSqr * PpE[i2] - dir[i2] * (dir[i0] * PmE[i0] +
                dir[i1] * PpE[i1]);
            if (FPge(tmp, 0.0))
            {
                // v[i2]-edge is closest
                if (FPle(tmp, 2.0 * lenSqr * boxExt[i2]))
                {
                    t = tmp / lenSqr;
                    *lineParameter = param;
                    pnt[i0] = boxExt[i0];
                    pnt[i1] = -boxExt[i1];
                    pnt[i2] = t - boxExt[i2];
                }
                else
                {
                    *lineParameter = param;
                    pnt[i0] = boxExt[i0];
                    pnt[i1] = -boxExt[i1];
                    pnt[i2] = boxExt[i2];
                }
                return;
            }

            // (v[i1],v[i2])-corner is closest
            *lineParameter = param;
            pnt[i0] = boxExt[i0];
            pnt[i1] = -boxExt[i1];
            pnt[i2] = -boxExt[i2];
        }
    }
	
	point->x = pnt[0];
	point->y = pnt[1];
	point->z = pnt[2];
}

static void 
CaseNoZeros(Point3D *pnt, Point3D dir, Point3D boxExtent, double *lineParameter)
{
    Point3D PmE = sub(*pnt, boxExtent);
    double prodDxPy = dir.x * PmE.y;
    double prodDyPx = dir.y * PmE.x;
    double prodDzPx, prodDxPz, prodDzPy, prodDyPz;

    if (FPge(prodDyPx, prodDxPy))
    {
        prodDzPx = dir.z * PmE.x;
        prodDxPz = dir.x * PmE.z;
        if (FPge(prodDzPx, prodDxPz))
            // line intersects x = e0
            Face(0, 1, 2, pnt, dir, PmE, boxExtent, lineParameter);
        else
            // line intersects z = e2
            Face(2, 0, 1, pnt, dir, PmE, boxExtent, lineParameter);
    }
    else
    {
        prodDzPy = dir.z * PmE.y;
        prodDyPz = dir.y * PmE.z;
        if (FPge(prodDzPy, prodDyPz))
            // line intersects y = e1
            Face(1, 2, 0, pnt, dir, PmE, boxExtent, lineParameter);
        else
            // line intersects z = e2
            Face(2, 0, 1, pnt, dir, PmE, boxExtent, lineParameter);
    }
}

static void 
Case0(int i0, int i1, int i2, Point3D *point, Point3D direction,
    Point3D boxExtent, double *lineParameter)
{
	double	pnt[3],
			dir[3],
			boxExt[3];
	
	pnt[0] = point->x;
	pnt[1] = point->y;
	pnt[2] = point->z;
	dir[0] = direction.x;
	dir[1] = direction.y;
	dir[2] = direction.z;
	boxExt[0] = boxExtent.x;
	boxExt[1] = boxExtent.y;
	boxExt[2] = boxExtent.z;
	
    double PmE0 = pnt[i0] - boxExt[i0];
    double PmE1 = pnt[i1] - boxExt[i1];
    double prod0 = dir[i1] * PmE0;
    double prod1 = dir[i0] * PmE1;
    double delta, invLSqr, inv;

    if (FPge(prod0, prod1))
    {
        // line intersects P[i0] = e[i0]
        pnt[i0] = boxExt[i0];

        double PpE1 = pnt[i1] + boxExt[i1];
        delta = prod0 - dir[i0] * PpE1;
        if (FPge(delta, 0.0))
        {
            invLSqr = (1.0) / (dir[i0] * dir[i0] + dir[i1] * dir[i1]);
            pnt[i1] = -boxExt[i1];
            *lineParameter = -(dir[i0] * PmE0 + dir[i1] * PpE1) * invLSqr;
        }
        else
        {
            inv = 1.0 / dir[i0];
            pnt[i1] -= prod0 * inv;
            *lineParameter = -PmE0 * inv;
        }
    }
    else
    {
        // line intersects P[i1] = e[i1]
        pnt[i1] = boxExt[i1];

        double PpE0 = pnt[i0] + boxExt[i0];
        delta = prod1 - dir[i1] * PpE0;
        if (FPge(delta, 0.0))
        {
            invLSqr = (1.0) / (dir[i0] * dir[i0] + dir[i1] * dir[i1]);
            pnt[i0] = -boxExt[i0];
            *lineParameter = -(dir[i0] * PpE0 + dir[i1] * PmE1) * invLSqr;
        }
        else
        {
            inv = 1.0 / dir[i1];
            pnt[i0] -= prod1 * inv;
            *lineParameter = -PmE1 * inv;
        }
    }

    if (FPlt(pnt[i2], -boxExt[i2]))
        pnt[i2] = -boxExt[i2];
    else if (FPgt(pnt[i2], boxExt[i2]))
        pnt[i2] = boxExt[i2];
	
	point->x = pnt[0];
	point->y = pnt[1];
	point->z = pnt[2];
}

static void
Case00(int i0, int i1, int i2, Point3D *point, Point3D direction,
    Point3D boxExtent, double *lineParameter)
{
	double	pnt[3],
			dir[3],
			boxExt[3];
	
	pnt[0] = point->x;
	pnt[1] = point->y;
	pnt[2] = point->z;
	dir[0] = direction.x;
	dir[1] = direction.y;
	dir[2] = direction.z;
	boxExt[0] = boxExtent.x;
	boxExt[1] = boxExtent.y;
	boxExt[2] = boxExtent.z;	

    *lineParameter = (boxExt[i0] - pnt[i0]) / dir[i0];

    pnt[i0] = boxExt[i0];

    if (FPlt(pnt[i1], -boxExt[i1]))
        pnt[i1] = -boxExt[i1];
    else if (FPgt(pnt[i1], boxExt[i1]))
        pnt[i1] = boxExt[i1];

    if (FPlt(pnt[i2], -boxExt[i2]))
        pnt[i2] = -boxExt[i2];
    else if (FPgt(pnt[i2], boxExt[i2]))
        pnt[i2] = boxExt[i2];
	
	point->x = pnt[0];
	point->y = pnt[1];
	point->z = pnt[2];
}

static void 
Case000(Point3D *pnt, Point3D boxExtent)
{
    if (FPlt(pnt->x, -boxExtent.x))
        pnt->x = -boxExtent.x;
    else if (FPgt(pnt->x, boxExtent.x))
        pnt->x = boxExtent.x;

    if (FPlt(pnt->y, -boxExtent.y))
        pnt->y = -boxExtent.y;
    else if (FPgt(pnt->y, boxExtent.y))
        pnt->y = boxExtent.y;

    if (FPlt(pnt->z, -boxExtent.z))
        pnt->z = -boxExtent.z;
    else if (FPgt(pnt->z, boxExtent.z))
        pnt->z = boxExtent.z;
}

/*
 * Return in pa and pb the points defining the shortest segment  
 * connecting the Line3D and the Path3D
*/
static void
shortest_segm_line3D_path3D(Line3D *line, Path3D *path, Point3D *pa, Point3D *pb)
{
	Point3D		pa1,
				pb1,
			   *pt;
	double		distance,
				min_distance = DBL_MAX;	
	Lseg3D		lsegpath;

	if (path->npts == 1)
	{
		// The path contains only one point
		pt = closestpt_point3D_line3D(&path->p[0], line);
		*pa = *pt;
		*pb = path->p[0];
		return;
	}

	/*
	 * The shortest segment between a Line3D and a Path3D is the shortest segment
	 * between the Line3D and any of the constituent segments of the Path3D.
	 */
	for (int i = 0; i < path->npts; i++)
	{
		int			iprev;

		if (i > 0)
			iprev = i - 1;
		else
		{
			if (!path->closed)
				continue;
			iprev = path->npts - 1;		/* include the closure segment */
		}
		lseg3D_construct_pts_stat(&lsegpath, path->p[iprev], path->p[i]);
		shortest_segm_lseg3D_line3D(&lsegpath, line, &pb1, &pa1);
		distance = dist_point3D_point3D(&pa1, &pb1);
		if (FPlt(distance, min_distance))
		{
			*pa = pa1;
			*pb = pb1;
			min_distance = distance;
		}		
	}
	return;
}

/*
 * Return in pa and pb the points defining the shortest segment  
 * connecting the Line3D and the Polygon3D
 *
 * If the line and the plane of the polygon are not coplanar find
 * the point where the line and the plane intersect as defined in 
 * http://www.ambrsoft.com/TrigoCalc/Plan3D/PlaneLineIntersection_.htm
 * If the intersection point is inside the polygon then pa and pb are
 * set to the intersection. Otherwise find the shortest segment
 * between the line and the path of the polygon
 */
static void
shortest_segm_line3D_poly3D(Line3D *line, Polygon3D *poly, Point3D *pa, Point3D *pb)
{
	if (!coplanar_poly3D_line3D_internal(poly, line))
	{
		double		A,
					B,
					C,
					D,
					numerator,
					denominator;
		Point3D		interpt;

		poly3D_plane(poly, &A, &B, &C, &D);
		numerator = ( A * line->p[0].x + B * line->p[0].y + C * line->p[0].z + D );
		denominator = A * (line->p[1].x - line->p[0].x) + B * 
			(line->p[1].y - line->p[0].y) + C * (line->p[1].z - line->p[0].z);
		interpt.x = line->p[0].x - ( (line->p[1].x - line->p[0].x) * numerator / denominator );
		interpt.y = line->p[0].y - ( (line->p[1].y - line->p[0].y) * numerator / denominator );
		interpt.z = line->p[0].z - ( (line->p[1].z - line->p[0].z) * numerator / denominator );
		
		if (contain_poly3D_point3D_internal(poly, &interpt))
		{
			pa->x = pb->x = interpt.x;
			pa->y = pb->y = interpt.y;
			pa->z = pb->z = interpt.z;
			return;			
		}
	}
	
	// Line and polygon are coplanar or intersection point is outside the polygon.
	// Compute the shortest segment between the line and the path of the polygon.
	Path3D	   *path;

	path = poly3D_path3D_internal(poly);
	shortest_segm_line3D_path3D(line, path, pa, pb);
	pfree(path);
	
	return;	
}

/*
 * Return in pa and pb the points defining the shortest segment  
 * connecting the Line3D and the Sphere. 
*/
static void 
shortest_segm_line3D_sphere(Line3D *line, Sphere *sphere, Point3D *pa, Point3D *pb)
{
	Point3D			v,
				   *pt;
	double 			magn;
	
	pt = closestpt_point3D_line3D(&sphere->center, line);
	*pa = *pt;
	pfree(pt);
	v = sub(*pa, sphere->center); 
	magn = magnitude(v);
	v = scalar(1/magn, v); 
	v = scalar(sphere->radius, v);
	*pb = add(v, sphere->center);
	return;
}

/*----------------------------------------------------------*/

/* shortest_segm_box3D_TYPE3D */

/*
 * Return in pa and pb the points defining the shortest segment 
 * connecting the two Box3D.
*/

void 
shortest_segm_box3D_box3D(Box3D *box1, Box3D *box2, Point3D *pa, Point3D *pb)
{
	// X axis
	if (FPge(box1->low.x, box2->high.x))
	{
		pa->x = box1->low.x;
		pb->x = box2->high.x;
	}
	else if (FPge(box2->low.x, box1->high.x))
	{
		pa->x = box1->high.x;
		pb->x = box2->low.x;
	}
	else
	{
		// the boxes overlap in the x axis,
		// choose arbitrarily the minimum of the intersection interval
		pa->x = Max(box1->low.x, box2->low.x);
		pb->x = Max(box1->low.x, box2->low.x);
	}

	// Y axis
	if (FPge(box1->low.y, box2->high.y))
	{
		pa->y = box1->low.y;
		pb->y = box2->high.y;
	}
	else if (FPge(box2->low.y, box1->high.y))
	{
		pa->y = box1->high.y;
		pb->y = box2->low.y;
	}
	else
	{
		// the boxes overlap in the y axis,
		// choose arbitrarily the minimum of the intersection interval
		pa->y = Max(box1->low.y, box2->low.y);
		pb->y = Max(box1->low.y, box2->low.y);
	}

	// Z axis
	if (FPge(box1->low.z, box2->high.z))
	{
		pa->z = box1->low.z;
		pb->z = box2->high.z;
	}
	else if (FPge(box2->low.z, box1->high.z))
	{
		pa->z = box1->high.z;
		pb->z = box2->low.z;
	}
	else
	{
		// the boxes overlap in the z axis,
		// choose arbitrarily the minimum of the intersection interval
		pa->z = Max(box1->low.z, box2->low.z);
		pb->z = Max(box1->low.z, box2->low.z);
	}
}

/*
 * Return in pa and pb the points defining the shortest segment  
 * connecting the Box3D and the Path3D
*/
static void
shortest_segm_box3D_path3D(Box3D *box, Path3D *path, Point3D *pa, Point3D *pb)
{
	Point3D		pa1,
				pb1,
			   *pt;
	double		distance,
				min_distance = DBL_MAX;	
	Lseg3D		lsegpath;

	if (path->npts == 1)
	{
		// The path contains only one point
		pt = closestpt_point3D_box3D(&path->p[0], box);
		*pa = *pt;
		*pb = path->p[0];
		return;
	}

	/*
	 * The shortest segment between a Box3D and a Path3D is the shortest segment
	 * between the Box3D and any of the constituent segments of the Path3D.
	 */
	for (int i = 0; i < path->npts; i++)
	{
		int			iprev;

		if (i > 0)
			iprev = i - 1;
		else
		{
			if (!path->closed)
				continue;
			iprev = path->npts - 1;		/* include the closure segment */
		}
		lseg3D_construct_pts_stat(&lsegpath, path->p[iprev], path->p[i]);
		shortest_segm_lseg3D_box3D(&lsegpath, box, &pb1, &pa1);
		distance = dist_point3D_point3D(&pa1, &pb1);
		if (FPlt(distance, min_distance))
		{
			*pa = pa1;
			*pb = pb1;
			min_distance = distance;
		}		
	}
	return;
}

/* 
 * Return in pa and pb the points defining the shortest segment 
 * connecting the Box3D and the Polygon3D
 * 
 * We must compute the shortest segment between the faces of box and the polygon
 * The faces of a box such that low = (x1,y1,z1) and high = (x2,y2,z2) are
 *		front  = P0:(x1,y1,z1) P1:(x2,y1,z1) P2:(x1,y2,z1) P3:(x2,y2,z1) 
 *		back   = P4:(x1,y1,z2) P5:(x2,y1,z2) P6:(x1,y2,z2) P7:(x2,y2,z2) 
 *		top    = P2:(x1,y2,z1) P3:(x2,y2,z1) P6:(x1,y2,z2) P7:(x2,y2,z2) 
 *		bottom = P0:(x1,y1,z1) P1:(x2,y1,z1) P4:(x1,y1,z2) P5:(x2,y1,z2) 
 *		left   = P0:(x1,y1,z1) P2:(x1,y2,z1) P4:(x1,y1,z2) P6:(x1,y2,z2) 
 *		right  = P1:(x2,y1,z1) P3:(x2,y2,z1) P5:(x2,y1,z2) P7:(x2,y2,z2)
*/

static void
shortest_segm_box3D_poly3D(Box3D *box, Polygon3D *poly, Point3D *pa, Point3D *pb)
{
	if (PT3Deq(box->low, box->high))
	{
		Point3D	   *pt;
		
		/* The Box3D is a Point3D */
		pt = closestpt_point3D_poly3D(&box->high, poly);
		*pa = box->high;
		*pb = *pt;
		return;			
	}
	else if (FPzero(box3D_vol(box)))
	{
		/* The Box3D is a Lseg3D */
		Lseg3D	  lseg;
		
		lseg3D_construct_pts_stat(&lseg, box->low, box->high);
		return shortest_segm_lseg3D_poly3D(&lseg, poly, pa, pb);
	}
	else
	{
		Polygon3D  *face;
		int			base_size,
					size;
		Point3D		pa1,
					pb1,
				   *pt;
		double		distance,
					min_distance = DBL_MAX;	
				
		base_size = sizeof(poly->p[0]) * 4;
		size = offsetof(Polygon3D, p) + base_size;
		face = (Polygon3D *) palloc0(size);	/* zero any holes */
		SET_VARSIZE(face, size);
		face->npts = 4;
		
		// front 
		face->p[0].x = box->low.x;
		face->p[0].y = box->low.y;
		face->p[0].z = box->low.z;
		
		face->p[1].x = box->high.x;
		face->p[1].y = box->low.y;
		face->p[1].z = box->low.z; 
		
		face->p[2].x = box->low.x;
		face->p[2].y = box->high.y;
		face->p[2].z = box->low.z; 
		
		face->p[3].x = box->high.x;
		face->p[3].y = box->high.y;
		face->p[3].z = box->low.z;
		
		poly3D_make_bbox(face);
		shortest_segm_poly3D_poly3D(face, poly, &pa1, &pb1);
		distance = dist_point3D_point3D(&pa1, &pb1);
		if (FPlt(distance, min_distance))
		{
			*pa = pa1;
			*pb = pb1;
			min_distance = distance;
		}	
		
		// back
		face->p[0].x = box->low.x;
		face->p[0].y = box->low.y;
		face->p[0].z = box->high.z; 
		
		face->p[1].x = box->high.x;
		face->p[1].y = box->low.y;
		face->p[1].z = box->high.z; 
		
		face->p[2].x = box->low.x;
		face->p[2].y = box->high.y;
		face->p[2].z = box->high.z; 
		
		face->p[3].x = box->high.x;
		face->p[3].y = box->high.y;
		face->p[3].z = box->high.z;
		
		poly3D_make_bbox(face);
		shortest_segm_poly3D_poly3D(face, poly, &pa1, &pb1);
		distance = dist_point3D_point3D(&pa1, &pb1);
		if (FPlt(distance, min_distance))
		{
			*pa = pa1;
			*pb = pb1;
			min_distance = distance;
		}
		
		// top
		face->p[0].x = box->low.x;
		face->p[0].y = box->high.y;
		face->p[0].z = box->low.z; 
		
		face->p[1].x = box->high.x;
		face->p[1].y = box->high.y;
		face->p[1].z = box->low.z; 
		
		face->p[2].x = box->low.x;
		face->p[2].y = box->high.y;
		face->p[2].z = box->high.z; 
		
		face->p[3].x = box->high.x;
		face->p[3].y = box->high.y;
		face->p[3].z = box->high.z;
		
		poly3D_make_bbox(face);
		shortest_segm_poly3D_poly3D(face, poly, &pa1, &pb1);
		distance = dist_point3D_point3D(&pa1, &pb1);
		if (FPlt(distance, min_distance))
		{
			*pa = pa1;
			*pb = pb1;
			min_distance = distance;
		}
		
		// bottom
		face->p[0].x = box->low.x;
		face->p[0].y = box->low.y;
		face->p[0].z = box->low.z; 
		
		face->p[1].x = box->high.x;
		face->p[1].y = box->low.y;
		face->p[1].z = box->low.z; 
		
		face->p[2].x = box->low.x;
		face->p[2].y = box->low.y;
		face->p[2].z = box->high.z; 
		
		face->p[3].x = box->high.x;
		face->p[3].y = box->low.y;
		face->p[3].z = box->high.z;
		
		poly3D_make_bbox(face);
		shortest_segm_poly3D_poly3D(face, poly, &pa1, &pb1);
		distance = dist_point3D_point3D(&pa1, &pb1);
		if (FPlt(distance, min_distance))
		{
			*pa = pa1;
			*pb = pb1;
			min_distance = distance;
		}
		
		// left
		face->p[0].x = box->low.x;
		face->p[0].y = box->low.y;
		face->p[0].z = box->low.z; 
		
		face->p[1].x = box->low.x;
		face->p[1].y = box->high.y;
		face->p[1].z = box->low.z; 
		
		face->p[2].x = box->low.x;
		face->p[2].y = box->low.y;
		face->p[2].z = box->high.z; 
		
		face->p[3].x = box->low.x;
		face->p[3].y = box->high.y;
		face->p[3].z = box->high.z;
		
		poly3D_make_bbox(face);
		shortest_segm_poly3D_poly3D(face, poly, &pa1, &pb1);
		distance = dist_point3D_point3D(&pa1, &pb1);
		if (FPlt(distance, min_distance))
		{
			*pa = pa1;
			*pb = pb1;
			min_distance = distance;
		}
		
		// right
		face->p[0].x = box->high.x;
		face->p[0].y = box->low.y;
		face->p[0].z = box->low.z; 
		
		face->p[1].x = box->high.x;
		face->p[1].y = box->high.y;
		face->p[1].z = box->low.z; 
		
		face->p[2].x = box->high.x;
		face->p[2].y = box->low.y;
		face->p[2].z = box->high.z;
		
		face->p[3].x = box->high.x;	
		face->p[3].y = box->high.y;
		face->p[3].z = box->high.z;	
		
		poly3D_make_bbox(face);
		shortest_segm_poly3D_poly3D(face, poly, &pa1, &pb1);
		distance = dist_point3D_point3D(&pa1, &pb1);
		if (FPlt(distance, min_distance))
		{
			*pa = pa1;
			*pb = pb1;
			min_distance = distance;
		}		
		
		pfree(face);
		return;		
	}
}

/*
 * Return in pa and pb the points defining the shortest segment  
 * connecting the Box3D and the Sphere. 
*/
static void 
shortest_segm_box3D_sphere(Box3D *box, Sphere *sphere, Point3D *pa, Point3D *pb)
{
	Point3D			v,
				   *pt;
	double 			magn;
	
	pt = closestpt_point3D_box3D(&sphere->center, box);
	*pa = *pt;
	pfree(pt);
	v = sub(*pa, sphere->center); 
	magn = magnitude(v);
	v = scalar(1/magn, v); 
	v = scalar(sphere->radius, v);
	*pb = add(v, sphere->center);
	return;
}

/*----------------------------------------------------------*/

/* shortest_segm_path3D_TYPE3D */

/*
 * Return in pa and pb the points defining the shortest segment 
 * connecting the two Path3D
*/
static void
shortest_segm_path3D_path3D(Path3D *path1, Path3D *path2, Point3D *pa, Point3D *pb)
{
	int			i,
				j;
	Lseg3D		lseg1,
				lseg2;
	Point3D		pa1,
				pb1,
			   *pt;
	double		distance,
				min_distance = DBL_MAX;	
				
	/* One of the two Path3D is composed of a single Point3D */
	if (path1->npts == 1)
	{
		if (path2->npts == 1)
		{
			*pa = path1->p[0];
			*pb = path2->p[0];
		}
		else
		{
			pt = closestpt_point3D_path3D(&path1->p[0], path2);
			*pa = path1->p[0];
			*pb = *pt;
		}
		return;
	}
	else
	{
		if (path2->npts == 1)
		{
			pt = closestpt_point3D_path3D(&path2->p[0], path1);
			*pa = *pt;
			*pb = path2->p[0];
			return;
		}	
	}
				
				
	/* Pairwise check the shortest segment between the Lseg3D composing the two Path3D */
	for (i = 0; i < path1->npts; i++)
	{
		int			iprev;

		if (i > 0)
			iprev = i - 1;
		else
		{
			if (!path1->closed)
				continue;
			iprev = path1->npts - 1;		/* include the closure segment */
		}

		for (j = 0; j < path2->npts; j++)
		{
			int			jprev;

			if (j > 0)
				jprev = j - 1;
			else
			{
				if (!path2->closed)
					continue;
				jprev = path2->npts - 1;	/* include the closure segment */
			}

			lseg3D_construct_pts_stat(&lseg1, path1->p[iprev], path1->p[i]);
			lseg3D_construct_pts_stat(&lseg2, path2->p[jprev], path2->p[j]);
			shortest_segm_lseg3D_lseg3D(&lseg1, &lseg2, &pa1, &pb1);
			distance = dist_point3D_point3D(&pa1, &pb1);
			if (FPlt(distance, min_distance))
			{
				*pa = pa1;
				*pb = pb1;
				min_distance = distance;
			}				
		}
	}

	return;
}

/*
 * Return in pa and pb the points defining the shortest segment 
 * connecting the Path3D and the Polygon3D
*/
static void
shortest_segm_path3D_poly3D(Path3D *path, Polygon3D *poly, Point3D *pa, Point3D *pb)
{
	int			i,
				j;
	Lseg3D		lseg;
	Point3D		pa1,
				pb1,
			   *pt;
	double		distance,
				min_distance = DBL_MAX;	
				
	/* The Path3D is composed of a single Point3D */
	if (path->npts == 1)
	{
		pt = closestpt_point3D_poly3D(&path->p[0], poly);
		*pa = path->p[0];
		*pb = *pt;
		return;			
	}
				
	/* Check the shortest segment between the Lseg3D composing the Path3D and the Polygon3D */
	for (i = 0; i < path->npts; i++)
	{
		int			iprev;

		if (i > 0)
			iprev = i - 1;
		else
		{
			if (!path->closed)
				continue;
			iprev = path->npts - 1;		/* include the closure segment */
		}

		lseg3D_construct_pts_stat(&lseg, path->p[iprev], path->p[i]);
		shortest_segm_lseg3D_poly3D(&lseg, poly, &pa1, &pb1);
		distance = dist_point3D_point3D(&pa1, &pb1);
		if (FPlt(distance, min_distance))
		{
			*pa = pa1;
			*pb = pb1;
			min_distance = distance;
		}				
	}
	return;
}

/*
 * Return in pa and pb the points defining the shortest segment 
 * connecting the Path3D and the Sphere. 
*/
static void 
shortest_segm_path3D_sphere(Path3D *path, Sphere *sphere, Point3D *pa, Point3D *pb)
{
	Point3D			v,
				   *pt;
	double 			magn;
	
	pt = closestpt_point3D_path3D(&sphere->center, path);
	*pa = *pt;
	pfree(pt);
	v = sub(*pa, sphere->center); 
	magn = magnitude(v);
	v = scalar(1/magn, v); 
	v = scalar(sphere->radius, v);
	*pb = add(v, sphere->center);
	return;
}

/*----------------------------------------------------------*/

/* shortest_segm_poly3D_TYPE3D */

/*
 * Return in pa and pb the points defining the shortest segment 
 * connecting the Polygon3D and the Polygon3D
*/
static void
shortest_segm_poly3D_poly3D(Polygon3D *poly1, Polygon3D *poly2, Point3D *pa, Point3D *pb)
{
	Point3D		pa1,
				pb1,
			   *pt;
	double		distance,
				min_distance = DBL_MAX;	
	Lseg3D		lsegpoly;

	if (poly1->npts == 1)
	{
		//  poly1 contains only one point
		pt = closestpt_point3D_poly3D(&poly1->p[0], poly2);
		*pa = poly1->p[0];
		*pb = *pt;
		return;
	}
	
	if (poly2->npts == 1)
	{
		// poly2 contains only one point
		pt = closestpt_point3D_poly3D(&poly2->p[0], poly1);
		*pa = *pt;
		*pb = poly2->p[0];
		return;
	}

	// Compare edges of poly1 to the interior of poly2.
	for (int i = 0; i < poly1->npts; i++)
	{
		int			iprev;

		if (i > 0)
			iprev = i - 1;
		else
			iprev = poly1->npts - 1;		/* include the closure segment */
		
		lseg3D_construct_pts_stat(&lsegpoly, poly1->p[iprev], poly1->p[i]);
		shortest_segm_lseg3D_poly3D(&lsegpoly, poly2, &pa1, &pb1);
		distance = dist_point3D_point3D(&pa1, &pb1);
		if (FPlt(distance, min_distance))
		{
			*pa = pa1;
			*pb = pb1;
			min_distance = distance;
		}		
	}

	// Compare edges of poly2 to the interior of poly1.
	for (int i = 0; i < poly2->npts; i++)
	{
		int			iprev;

		if (i > 0)
			iprev = i - 1;
		else
			iprev = poly2->npts - 1;		/* include the closure segment */
		
		lseg3D_construct_pts_stat(&lsegpoly, poly2->p[iprev], poly2->p[i]);
		shortest_segm_lseg3D_poly3D(&lsegpoly, poly1, &pb1, &pa1);
		distance = dist_point3D_point3D(&pa1, &pb1);
		if (FPlt(distance, min_distance))
		{
			*pa = pa1;
			*pb = pb1;
			min_distance = distance;
		}		
	}
	return;
}

/*
 * Return in pa and pb the points defining the shortest segment connecting 
 * connecting the Poly3D and the Sphere. 
*/
static void 
shortest_segm_poly3D_sphere(Polygon3D *poly, Sphere *sphere, Point3D *pa, Point3D *pb)
{
	Point3D			v,
				   *pt;
	double 			magn;
	
	pt = closestpt_point3D_poly3D(&sphere->center, poly);
	*pa = *pt;
	pfree(pt);
	v = sub(*pa, sphere->center); 
	magn = magnitude(v);
	v = scalar(1/magn, v); 
	v = scalar(sphere->radius, v);
	*pb = add(v, sphere->center);
	return;
}

/*----------------------------------------------------------*/

/* shortest_segm_sphere_TYPE3D */

/*
 * Return in pa and pb the points defining the shortest segment connecting 
 * connecting the Poly3D and the Sphere. 
*/
static void 
shortest_segm_sphere_sphere(Sphere *sphere1, Sphere *sphere2, Point3D *pa, Point3D *pb)
{
	Point3D			v,
				   *pt1,
				   *pt2;
	double 			magn;
	
	pt1 = closestpt_point3D_sphere(&sphere2->center, sphere1);
	*pa = *pt1;
	pfree(pt1);
	pt2 = closestpt_point3D_sphere(&sphere1->center, sphere2);
	*pb = *pt2;
	pfree(pt2);
	return;
}

/*****************************************************************************
 * Overlap functions 
 *****************************************************************************/

/* Lseg3D overlap TYPE3D */

/*
 * Do the two Lseg3D overlap?
 */
Datum
overlap_lseg3D_lseg3D(PG_FUNCTION_ARGS)
{
	Lseg3D		   *lseg1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D		   *lseg2 = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(inter_lseg3D_lseg3D(lseg1, lseg2));
}

PG_FUNCTION_INFO_V1(overlap_lseg3D_lseg3D);

bool
overlap_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2)
{
	return inter_lseg3D_lseg3D(lseg1, lseg2);
}

/*
 * Do the Lseg3D and the Line3D overlap?
 */
Datum
overlap_lseg3D_line3D(PG_FUNCTION_ARGS)
{
	Lseg3D		   *lseg = PG_GETARG_LSEG3D_P(0);
	Line3D		   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overlap_lseg3D_line3D_internal(lseg, line));
}

PG_FUNCTION_INFO_V1(overlap_lseg3D_line3D);

bool
overlap_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line)
{
	return (contain_line3D_point3D_internal(line, &lseg->p[0]) &&
		contain_line3D_point3D_internal(line, &lseg->p[1]));
}

/*----------------------------------------------------------*/

/*
 * Do the Line3D and the Lseg3D overlap?
 */
Datum
overlap_line3D_lseg3D(PG_FUNCTION_ARGS)
{
	Line3D		   *line = PG_GETARG_LINE3D_P(0);
	Lseg3D		   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overlap_lseg3D_line3D_internal(lseg, line));
}

PG_FUNCTION_INFO_V1(overlap_line3D_lseg3D);

bool
overlap_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg)
{
	return overlap_lseg3D_line3D_internal(lseg, line);
}

/*----------------------------------------------------------*/

/* Box3D overlap TYPE3D */

/*
 * Do the two Box3D overlap?
 */
Datum
overlap_box3D_box3D(PG_FUNCTION_ARGS)
{
	Box3D		   *box1 = PG_GETARG_BOX3D_P(0);
	Box3D		   *box2 = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(inter_box3D_box3D(box1, box2));
}

PG_FUNCTION_INFO_V1(overlap_box3D_box3D);

bool
overlap_box3D_box3D_internal(Box3D *box1, Box3D *box2)
{
	return inter_box3D_box3D(box1, box2);
}

/* 
 * Do the Box3D and the Polygon3D overlap?
*/
Datum
overlap_box3D_poly3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_BOOL(inter_box3D_poly3D(box, poly));
}
	
PG_FUNCTION_INFO_V1(overlap_box3D_poly3D) ;

bool
overlap_box3D_poly3D_internal(Box3D *box, Polygon3D *poly)
{
	return inter_box3D_poly3D(box, poly);
}

/*
 * Do the Box3D and the Sphere overlap?
 */
Datum
overlap_box3D_sphere(PG_FUNCTION_ARGS)
{
	Box3D		   *box = PG_GETARG_BOX3D_P(0);
	Sphere		   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(inter_box3D_sphere(box, sphere));
}

PG_FUNCTION_INFO_V1(overlap_box3D_sphere);

bool
overlap_box3D_sphere_internal(Box3D *box, Sphere *sphere)
{
	return inter_box3D_sphere(box, sphere);
}

/*----------------------------------------------------------*/

/* poly3D overlap TYPE3D */

/* 
 * Do the Polygon3D and the Box3D overlap?
*/
Datum
overlap_poly3D_box3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(inter_box3D_poly3D(box, poly));
}
	
PG_FUNCTION_INFO_V1(overlap_poly3D_box3D) ;

bool
overlap_poly3D_box3D_internal(Polygon3D *poly, Box3D *box)
{
	return inter_box3D_poly3D(box, poly);
}

/*
 * Do the two Polygon3D overlap?
 */
Datum
overlap_poly3D_poly3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly1 = PG_GETARG_POLYGON3D_P(0);
	Polygon3D  *poly2 = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = inter_poly3D_poly3D(poly1, poly2);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly1, 0);
	PG_FREE_IF_COPY(poly2, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overlap_poly3D_poly3D);

bool
overlap_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2)
{
	return inter_poly3D_poly3D(poly1, poly2);
}

/*----------------------------------------------------------*/

/* sphere overlap TYPE3D */

 /*
 * Do the Sphere and the Box3D overlap?
 */
Datum
overlap_sphere_box3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(inter_box3D_sphere(box, sphere));
}

PG_FUNCTION_INFO_V1(overlap_sphere_box3D);

bool
overlap_sphere_box3D_internal(Sphere *sphere, Box3D *box)
{
	return inter_box3D_sphere(box, sphere);
}

/*
 * Do the two Sphere overlap?
 */
Datum
overlap_sphere_sphere(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere1 = PG_GETARG_SPHERE_P(0);
	Sphere	   *sphere2 = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(inter_sphere_sphere(sphere1, sphere2));
}

PG_FUNCTION_INFO_V1(overlap_sphere_sphere);

bool
overlap_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2)
{
	return inter_sphere_sphere(sphere1, sphere2);
}

/*****************************************************************************
 * Contain functions 
 *****************************************************************************/

/* Lseg3D contain TYPE3D */

/*
 * Does the Lseg3D contain the Point3D?
 */
Datum
contain_lseg3D_point3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(contain_lseg3D_point3D_internal(lseg, point));
}

PG_FUNCTION_INFO_V1(contain_lseg3D_point3D) ;

// Determine collinearity by detecting a triangle inequality.
bool
contain_lseg3D_point3D_internal(Lseg3D *lseg, Point3D *point)
{
	return FPeq(dist_point3D_point3D(point, &lseg->p[0]) + dist_point3D_point3D(point, &lseg->p[1]),
				dist_point3D_point3D(&lseg->p[0], &lseg->p[1]));
}

/*
 * Does the first Lseg3D contain the second Lseg3D?
 */
Datum
contain_lseg3D_lseg3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D	   *lseg2 = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(contain_lseg3D_lseg3D_internal(lseg1, lseg2));
}

PG_FUNCTION_INFO_V1(contain_lseg3D_lseg3D) ;

bool
contain_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2)
{
	return (contain_lseg3D_point3D_internal(lseg1, &lseg2->p[0]) &&
			contain_lseg3D_point3D_internal(lseg1, &lseg2->p[1]));
}

/*----------------------------------------------------------*/

/* line3D contain TYPE3D */

/* 
 * Does the Line3D contain the Point3D?
 */
Datum
contain_line3D_point3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(contain_line3D_point3D_internal(line, point));
}

PG_FUNCTION_INFO_V1(contain_line3D_point3D) ;

/* 
 * Determine whether the point (x1,y1,z1) satisfies the parametric
 * equations of the line, i.e., (x,y,z) = (x0,y0,z0) + <a,b,c>t
 */
 bool
contain_line3D_point3D_internal(Line3D *line, Point3D *point)
{
	double		x1,
				y1,
				z1;
	Point3D		diff;

	diff = sub(line->p[1], line->p[0]);
	if (!FPzero(diff.x))
		x1 = (point->x - line->p[0].x) / diff.x;
	if (!FPzero(diff.y))
		y1 = (point->y - line->p[0].y) / diff.y;
	if (!FPzero(diff.z))
		z1 = (point->z - line->p[0].z) / diff.z;
	
	return (FPeq(x1, y1) && FPeq(y1, z1));
}
 
/* 
 * Does the Line3D contain the Lseg3D?
 */
Datum
contain_line3D_lseg3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(contain_line3D_lseg3D_internal(line, lseg));
}

PG_FUNCTION_INFO_V1(contain_line3D_lseg3D) ;

bool
contain_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg)
{
	return (contain_line3D_point3D_internal(line, &lseg->p[0]) &&
			contain_line3D_point3D_internal(line, &lseg->p[1]));
}

/*----------------------------------------------------------*/

/* Box3D contain TYPE3D */

/*
 * Does the Box3D contain the Point3D?
 */
Datum
contain_box3D_point3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(contain_box3D_point3D_internal(box, point));
}

PG_FUNCTION_INFO_V1(contain_box3D_point3D);

bool
contain_box3D_point3D_internal(Box3D *box, Point3D *point)
{
	return (FPle(point->x, box->high.x) && 
			FPge(point->x, box->low.x) &&
			FPle(point->y, box->high.y) && 
			FPge(point->y, box->low.y) &&
			FPle(point->z, box->high.z) && 
			FPge(point->z, box->low.z));
}

/*
 * Does the Box3D contain the Lseg3D?
 */
Datum
contain_box3D_lseg3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(contain_box3D_lseg3D_internal(box, lseg));
}

PG_FUNCTION_INFO_V1(contain_box3D_lseg3D);

bool
contain_box3D_lseg3D_internal(Box3D *box, Lseg3D *lseg)
{
	PG_RETURN_BOOL(contain_box3D_point3D_internal(box, &lseg->p[0]) &&
					contain_box3D_point3D_internal(box, &lseg->p[1]));
}

/*
 * Does the first Box3D contain the second Box3D?
 */
Datum
contain_box3D_box3D(PG_FUNCTION_ARGS)
{
	Box3D		   *box1 = PG_GETARG_BOX3D_P(0);
	Box3D		   *box2 = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(contain_box3D_box3D_internal(box1, box2));
}

PG_FUNCTION_INFO_V1(contain_box3D_box3D);

bool
contain_box3D_box3D_internal(Box3D *box1, Box3D *box2)
{
	return (FPge(box1->high.x, box2->high.x) &&
			FPle(box1->low.x, box2->low.x) &&
			FPge(box1->high.y, box2->high.y) &&
			FPle(box1->low.y, box2->low.y) &&
			FPge(box1->high.z, box2->high.z) &&
			FPle(box1->low.z, box2->low.z));
}

/*
 * Does the Box3D contain the Path3D?
 */
Datum
contain_box3D_path3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Path3D     *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_BOOL(contain_box3D_path3D_internal(box, path));
}

PG_FUNCTION_INFO_V1(contain_box3D_path3D);

bool
contain_box3D_path3D_internal(Box3D *box, Path3D *path)
{
	int			i;
	bool		result = true;
	
	for (i = 0; i < path->npts; i++)
		result = result && contain_box3D_point3D_internal(box, &path->p[i]) ;	

	return result;
}

/*
 * Does the Box3D contain the Polygon3D?
 */
Datum
contain_box3D_poly3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_BOOL(contain_box3D_poly3D_internal(box, poly));
}

PG_FUNCTION_INFO_V1(contain_box3D_poly3D);

bool
contain_box3D_poly3D_internal(Box3D *box, Polygon3D *poly)
{
	Path3D     *path;
	bool		result;

	path = poly3D_path3D_internal(poly);
	result = contain_box3D_path3D_internal(box, path);
	pfree(path);
	
	PG_RETURN_BOOL(result);
}

/*
 * Does the Box3D contain the Sphere?
 */
Datum
contain_box3D_sphere(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Sphere     *sphere = PG_GETARG_SPHERE_P(1);
	
	PG_RETURN_BOOL(contain_box3D_sphere_internal(box, sphere));
}

PG_FUNCTION_INFO_V1(contain_box3D_sphere);

bool 
contain_box3D_sphere_internal(Box3D *box, Sphere *sphere)
{
	Box3D	   *bbox;

	bbox = sphere_bbox(sphere);
	
	return contain_box3D_box3D_internal(box, bbox);
}

/*----------------------------------------------------------*/

/* path3D contain TYPE3D */
 
/* 
 * Does the Path3D contain the Point3D?
 */
Datum
contain_path3D_point3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(contain_path3D_point3D_internal(path, point));
}

PG_FUNCTION_INFO_V1(contain_path3D_point3D) ;

bool
contain_path3D_point3D_internal(Path3D *path, Point3D *point)
{
	int			i,
				iprev;
	Lseg3D		lseg;
	bool		result;

	result = false;
	
	/*
	 * test whether one of the constituent segments of path contains the point
	 */
	for (i = 0; i < path->npts && result == false; i++)
	{
		if (i > 0)
			iprev = i - 1;
		else
		{
			if (!path->closed)
				continue;
			iprev = path->npts - 1;		/* include the closure segment */
		}
		
		lseg3D_construct_pts_stat(&lseg, path->p[iprev], path->p[i]);
		result = contain_lseg3D_point3D_internal(&lseg, point);
	}

	return result;
}

/*
 * Does the Path3D contain the Lseg3D?
 */
 
Datum
contain_path3D_lseg3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(contain_path3D_lseg3D_internal(path, lseg));
}

PG_FUNCTION_INFO_V1(contain_path3D_lseg3D);

bool
contain_path3D_lseg3D_internal(Path3D *path, Lseg3D *lseg)
{
	int			i,
				iprev;
	Lseg3D		lsegpath;
	bool		result;

	result = false;
	
	/*
	 * path contains lseg if one of the constituent segments of path contains lseg
	 */
	for (i = 0; i < path->npts && result == false; i++)
	{
		if (i > 0)
			iprev = i - 1;
		else
		{
			if (!path->closed)
				continue;
			iprev = path->npts - 1;		/* include the closure segment */
		}
		
		lseg3D_construct_pts_stat(&lsegpath, path->p[iprev], path->p[i]);
		result = contain_lseg3D_lseg3D_internal(&lsegpath, lseg);
	}

	return result;
}

/*
 * Does the first Path3D contain the second Path3D?
 */
Datum
contain_path3D_path3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path1 = PG_GETARG_PATH3D_P(0);
	Path3D	   *path2 = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = contain_path3D_path3D_internal(path1, path2);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path1, 0);
	PG_FREE_IF_COPY(path2, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(contain_path3D_path3D);

bool
contain_path3D_path3D_internal(Path3D *path1, Path3D *path2)
{
	int			i,
				iprev;
	Lseg3D		s;
	bool		result;

	if (path2->npts == 0)
		return FALSE;
	
	result = true;
	
	/*
	 * path1 contains path2 if path1 contains all constituent segments of path2
	 */
	for (i = 0; i < path2->npts && result == true; i++)
	{
		if (i > 0)
			iprev = i - 1;
		else
		{
			if (!path2->closed)
				continue;
			iprev = path2->npts - 1;		/* include the closure segment */
		}
				
		lseg3D_construct_pts_stat(&s, path2->p[iprev], path2->p[i]);
		
		result = contain_path3D_lseg3D_internal(path1, &s);
	}

	return result;
}

/*----------------------------------------------------------*/

/* poly3D contain TYPE3D */

/*
 * Does the Polygon3D contain the Point3D?
 */
 
Datum
contain_poly3D_point3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(contain_poly3D_point3D_internal(poly, point));
}

PG_FUNCTION_INFO_V1(contain_poly3D_point3D);

bool
contain_poly3D_point3D_internal(Polygon3D *poly, Point3D *point)
{
	
	if (!coplanar_poly3D_point3D_internal(poly, point))
		return FALSE;
	else
	{
		Point3D			loc0,
						locx,
						locy,
						projpoint;
		Polygon3D	   *projpoly;
		int				i;
		bool			result;
		
		// Project the polygon and the point to 2D
		poly3D_project2D(poly, &loc0, &locx, &locy);
		projpoly = poly3D_copy(poly);
		for (i = 0; i < projpoly->npts; i++)
		{
			projpoly->p[i].x = DOT(sub(poly->p[i], loc0), locx);
			projpoly->p[i].y = DOT(sub(poly->p[i], loc0), locy);
			projpoly->p[i].z = 0; // not used in the 2D tests
		}
		// N.B. It is not necessary to project the bounding box
		
		projpoint.x = DOT(sub(*point, loc0), locx);
		projpoint.y = DOT(sub(*point, loc0), locy);
		projpoint.z = 0; // not used in the 2D tests 

		// Test inclusion in the projected point and polygon
		result = point2D_in_poly2D(&projpoint, projpoly->npts, projpoly->p);
		pfree(projpoly);
		
		return result;
	}
}

/*
 * Compute the plane equation of a planar polygon
 * A planar poly3D defined with 3 non collinear points (x1,y1,z1), (x2,y2,z2)
 * and (x3,y3,z3) has as equation Ax + By + Cz + D = 0 where <a,b,c> is the 
 * vector normal to the plane and D is obtained by plugging into the above
 * equation any point, e.g., (x1,y1,z1)
 * Precondition: it is supposed that the polygon is planar
 */
 
static void
poly3D_plane(Polygon3D *poly, double *A, double *B, double *C, double *D)
{
	// pos1, pos2, pos3: positions in the array poly->p of three non collinear points
	int			pos1,
				pos2,
				pos3,
				i;
	Point3D		normal;
	double		magn;
			
	plist_find3noncollinearpts(poly->p, poly->npts, &pos1, &pos2, &pos3);
	normal = cross(sub(poly->p[pos2], poly->p[pos1]), sub(poly->p[pos3], poly->p[pos1]));
	
	*A = normal.x;
	*B = normal.y;
	*C = normal.z;
	*D = -(normal.x * poly->p[pos1].x + normal.y * poly->p[pos1].y + normal.z * poly->p[pos1].z);
}

/*
 * poly3D_project2D: project a planar polygon in 2D
 * Precondition: it is supposed that the polygon is planar
 * http://stackoverflow.com/questions/26369618/getting-local-2d-coordinates-of-vertices-of-a-planar-polygon-in-3d-space
 */

static void
poly3D_project2D(Polygon3D *poly, Point3D *loc0, Point3D *locx, Point3D *locy)
{
	// pos1, pos2, pos3: positions in the array  poly->p of three non collinear points
	int			pos1,
				pos2,
				pos3,
				i;
	Point3D		normal;	// vector orthogonal to polygon plane
	double		magn;
			
	plist_find3noncollinearpts(poly->p, poly->npts, &pos1, &pos2, &pos3);
	*loc0 = poly->p[pos1];
	*locx = sub(poly->p[pos2], *loc0);
	normal = cross(*locx, sub(poly->p[pos3], *loc0));
	*locy = cross(normal, *locx);

	// normalize the computed local axis locx and locy
	magn = magnitude(*locx);
	locx->x /= magn;
	locx->y /= magn;
	locx->z /= magn;
	magn = magnitude(*locy);
	locy->x /= magn;
	locy->y /= magn;
	locy->z /= magn;
}

/*
 * Given a Point3D and a Polygon3D projected to 2D (x and y axis) 
 * determine whether the point is in the polygon
 * http://geomalgorithms.com/a03-_inclusion.html
*/
static bool
point2D_in_poly2D(Point3D *point, int npts, Point3D *plist)
{
	int    cn = 0;    // the  crossing number counter
	double vt;

	// loop through all edges of the polygon
	for (int i = 0; i < npts ; i++) 
	{    // edge from V[i]  to V[i+1]
		if ( (FPle(plist[i].y, point->y) && FPgt(plist[i+1].y, point->y)) ||		// an upward crossing
			(FPgt(plist[i].y, point->y) && FPle(plist[i+1].y, point->y)) )			// a downward crossing
		{
			// compute  the actual edge-ray intersect x-coordinate
			vt = (point->y  - plist[i].y) / (plist[i+1].y - plist[i].y);
			if (FPlt(point->x,  plist[i].x + vt * (plist[i+1].x - plist[i].x)))	// P.x < intersect
				++cn;   // a valid crossing of y=P.y right of P.x
		}
	}
	return (cn&1 == 1);    // 0 if even (out), and 1 if  odd (in)	
}	

/*
 * Does the Polygon3D contain the Lseg3D?
 */

Datum
contain_poly3D_lseg3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(contain_poly3D_lseg3D_internal(poly, lseg));
}

PG_FUNCTION_INFO_V1(contain_poly3D_lseg3D);

bool
contain_poly3D_lseg3D_internal(Polygon3D *poly, Lseg3D *lseg)
{
	if (!coplanar_poly3D_lseg3D_internal(poly, lseg))
		return FALSE;
	else
	{
		Point3D			loc0,
						locx,
						locy;
		Lseg3D		   *projlseg = (Lseg3D *) palloc(sizeof(Lseg3D));
		Polygon3D	   *projpoly;
		int				i;
		bool			result;

		// Project the polygon and the lseg to 2D
		poly3D_project2D(poly, &loc0, &locx, &locy);
		projpoly = poly3D_copy(poly);
		for (i = 0; i < projpoly->npts; i++)
		{
			projpoly->p[i].x = DOT(sub(poly->p[i], loc0), locx);
			projpoly->p[i].y = DOT(sub(poly->p[i], loc0), locy);
			projpoly->p[i].z = 0; // not used in the 2D tests
		}
		// N.B. It is not necessary to project the bounding box
		
		projlseg->p[0].x = DOT(sub(lseg->p[0], loc0), locx);
		projlseg->p[0].y = DOT(sub(lseg->p[0], loc0), locy);
		projlseg->p[0].z = 0; // not used in the 2D tests
		projlseg->p[1].x = DOT(sub(lseg->p[1], loc0), locx);
		projlseg->p[1].y = DOT(sub(lseg->p[1], loc0), locy);
		projlseg->p[1].z = 0; // not used in the 2D tests

		// Test inclusion in the projected lseg and polygon
		result = lseg2D_inside_poly2D(&projlseg->p[0], &projlseg->p[1], projpoly, 0);
		pfree(projpoly);
		pfree(projlseg);
		
		return result;
	}
}

/*
 * Returns true if segment (a,b) is in polygon, option start is used for optimization 
 * The function checks the edges of the Polygon3D starting from start
 * Function adapted from geo_ops.c so that it manipulates 3D geometries
 * 
 */
static bool
lseg2D_inside_poly2D(Point3D *a, Point3D *b, Polygon3D *poly, int start)
{
	Lseg3D		s,
				t;
	int			i;
	bool		res = true,
				intersection = false;

	t.p[0] = *a;
	t.p[1] = *b;
	s.p[0] = poly->p[(start == 0) ? (poly->npts - 1) : (start - 1)];

	for (i = start; i < poly->npts && res; i++)
	{
		Point3D	   *interpt;

		CHECK_FOR_INTERRUPTS();

		s.p[1] = poly->p[i];

		if (contain_lseg3D_point3D_internal(&s, t.p))
		{
			if (contain_lseg3D_point3D_internal(&s, t.p + 1))
				return true;	/* t is contained by s */

			/* y-cross */
			res = touched_lseg2D_inside_poly2D(t.p, t.p + 1, &s, poly, i + 1);
		}
		else if (contain_lseg3D_point3D_internal(&s, t.p + 1))
		{
			/* y-cross */
			res = touched_lseg2D_inside_poly2D(t.p + 1, t.p, &s, poly, i + 1);
		}
		else if ((interpt = interpt_lseg3D_lseg3D(&t, &s)) != NULL)
		{
			/* segments are x-crossing, go to check each subsegment  */
			intersection = true;
			res = lseg2D_inside_poly2D(t.p, interpt, poly, i + 1);
			if (res)
				res = lseg2D_inside_poly2D(t.p + 1, interpt, poly, i + 1);
			pfree(interpt);
		}

		s.p[0] = s.p[1];
	}

	if (res && !intersection)
	{
		Point3D		p;

		/*
		 * if x-intersection wasn't found  then check central point of tested
		 * segment. In opposite case we already check all subsegments
		 */
		p.x = (t.p[0].x + t.p[1].x) / 2.0;
		p.y = (t.p[0].y + t.p[1].y) / 2.0;
		p.z = (t.p[0].z + t.p[1].z) / 2.0;

		res = point2D_in_poly2D(&p, poly->npts, poly->p);
	}

	return res;
}

/* 
 * Tests special kind of segment for in/out of polygon3D.
 * Special kind means:
 *	- point a should be on segment s
 *	- segment (a,b) should not be contained by s
 * Returns true if:
 *	- segment (a,b) is collinear to s and (a,b) is in polygon3D
 *	- segment (a,b) s not collinear to s. Note: that doesn't
 *	  mean that segment is in polygon!
 * Function adapted from geo_ops.c so that it manipulates 3D geometries
 */

static bool
touched_lseg2D_inside_poly2D(Point3D *a, Point3D *b, Lseg3D *s, Polygon3D *poly, int start)
{
	/* point a is on s, b is not */
	Lseg3D		t;

	t.p[0] = *a;
	t.p[1] = *b;

	if (PT3DPeq(a, s->p))
	{
		if (contain_lseg3D_point3D_internal(&t, s->p + 1))
			return lseg2D_inside_poly2D(b, s->p + 1, poly, start);
	}
	else if (PT3DPeq(a, s->p + 1))
	{
		if (contain_lseg3D_point3D_internal(&t, s->p))
			return lseg2D_inside_poly2D(b, s->p, poly, start);
	}
	else if (contain_lseg3D_point3D_internal(&t, s->p))
	{
		return lseg2D_inside_poly2D(b, s->p, poly, start);
	}
	else if (contain_lseg3D_point3D_internal(&t, s->p + 1))
	{
		return lseg2D_inside_poly2D(b, s->p + 1, poly, start);
	}

	return true;				/* may be not true, but that will check later */
}

/*
 * Does the Polygon3D contain the Path3D?
 */
Datum
contain_poly3D_path3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = contain_poly3D_path3D_internal(poly, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(contain_poly3D_path3D);

bool
contain_poly3D_path3D_internal(Polygon3D *poly, Path3D *path)
{
	/*
	 * Quick check to see if both are coplanar and if bounding box is contained
	 */
	if (!coplanar_poly3D_path3D_internal(poly, path) ||
		!contain_box3D_box3D_internal(&poly->boundbox, &path->boundbox))
			return FALSE;
	else
	{
		Point3D		loc0,
					locx,
					locy;
		Polygon3D  *projpoly;
		Path3D	   *projpath;
		bool		result;
		int			i,
					iprev;
		Lseg3D		s;

		// Project the polygon and the path to 2D
		poly3D_project2D(poly, &loc0, &locx, &locy);
		projpoly = poly3D_copy(poly);
		for (i = 0; i < projpoly->npts; i++)
		{
			projpoly->p[i].x = DOT(sub(poly->p[i], loc0), locx);
			projpoly->p[i].y = DOT(sub(poly->p[i], loc0), locy);
			projpoly->p[i].z = 0; // not used in the 2D tests
		}
		// N.B. It is not necessary to project the bounding box
		
		projpath = path3D_copy(path);
		for (i = 0; i < projpath->npts; i++)
		{
			projpath->p[i].x = DOT(sub(path->p[i], loc0), locx);
			projpath->p[i].y = DOT(sub(path->p[i], loc0), locy);
			projpath->p[i].z = 0; // not used in the 2D tests
		}
		// N.B. It is not necessary to project the bounding box

		// Test inclusion in the projected polygon and path
		result = true;
		for (i = 0; i < projpath->npts && result; i++)
		{
			if (i > 0)
				iprev = i - 1;
			else
			{
				if (!projpath->closed)
					continue;
				iprev = projpath->npts - 1;		/* include the closure segment */
			}
			s.p[0] = projpath->p[iprev];
			s.p[1] = projpath->p[i];
			result = lseg2D_inside_poly2D(s.p, s.p + 1, projpoly, 0);
		}

		pfree(projpoly);
		pfree(projpath);
		
		return result;
	}
}

/*
 * Does the first Polygon3D contain the second Polygon3D?
 */
Datum
contain_poly3D_poly3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly1 = PG_GETARG_POLYGON3D_P(0);
	Polygon3D  *poly2 = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = contain_poly3D_poly3D_internal(poly1, poly2);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly1, 0);
	PG_FREE_IF_COPY(poly2, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(contain_poly3D_poly3D);

bool
contain_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2)
{
	/*
	 * Quick check to see if both are coplanar and if bounding box is contained.
	 */
	if (!coplanar_poly3D_poly3D_internal(poly1, poly2) ||
		!contain_box3D_box3D_internal(&poly1->boundbox, &poly2->boundbox))
			return FALSE;
	else
	{
		Point3D		loc0,
					locx,
					locy;
		Polygon3D  *projpoly1,
				   *projpoly2;
		bool		result;
		int			i,
					iprev;
		Lseg3D		s;

		// Project the polygons to 2D
		poly3D_project2D(poly1, &loc0, &locx, &locy);
		projpoly1 = poly3D_copy(poly1);
		for (i = 0; i < projpoly1->npts; i++)
		{
			projpoly1->p[i].x = DOT(sub(poly1->p[i], loc0), locx);
			projpoly1->p[i].y = DOT(sub(poly1->p[i], loc0), locy);
			projpoly1->p[i].z = 0; // not used in the 2D tests
		}
		// N.B. It is not necessary to project the bounding box
		
		projpoly2 = poly3D_copy(poly2);
		for (i = 0; i < projpoly2->npts; i++)
		{
			projpoly2->p[i].x = DOT(sub(poly2->p[i], loc0), locx);
			projpoly2->p[i].y = DOT(sub(poly2->p[i], loc0), locy);
			projpoly2->p[i].z = 0; // not used in the 2D tests
		}
		// N.B. It is not necessary to project the bounding box

		// Test inclusion in the projected polygon and path
		s.p[0] = projpoly2->p[projpoly2->npts - 1];
		result = true;

		for (i = 0; i < projpoly2->npts && result; i++)
		{
			s.p[1] = projpoly2->p[i];
			result = lseg2D_inside_poly2D(s.p, s.p + 1, projpoly1, 0);
			s.p[0] = s.p[1];
		}
		
		pfree(projpoly1);
		pfree(projpoly2);
			
		return result;
	}
}

/*----------------------------------------------------------*/

/* sphere contain TYPE3D */
 
/*
 * Does the Sphere contain the Point3D?
 */
Datum
contain_sphere_point3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(contain_sphere_point3D_internal(sphere, point));
}

PG_FUNCTION_INFO_V1(contain_sphere_point3D);

bool
contain_sphere_point3D_internal(Sphere *sphere, Point3D *point)
{
	double		d;

	d = dist_point3D_point3D(&sphere->center, point);
	return FPle(d, sphere->radius);
}

/*
 * Does the Sphere contain the Lseg3D?
 */
Datum
contain_sphere_lseg3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(contain_sphere_lseg3D_internal(sphere, lseg));
}

PG_FUNCTION_INFO_V1(contain_sphere_lseg3D);

bool
contain_sphere_lseg3D_internal(Sphere *sphere, Lseg3D *lseg)
{
	return (contain_sphere_point3D_internal(sphere, &lseg->p[0]) &&
			contain_sphere_point3D_internal(sphere, &lseg->p[1]));
}

/*
 * Does the Sphere contain the Box3D?
 */
Datum
contain_sphere_box3D(PG_FUNCTION_ARGS)
{
	Sphere     *sphere = PG_GETARG_SPHERE_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	
	PG_RETURN_BOOL(contain_sphere_box3D_internal(sphere, box));
}

PG_FUNCTION_INFO_V1(contain_sphere_box3D);

bool 
contain_sphere_box3D_internal(Sphere *sphere, Box3D *box)
{
	Sphere	   *bsphere;

	bsphere = box3D_bsphere(box);
	
	return contain_sphere_sphere_internal(sphere, bsphere);
}

/*
 * Does the Sphere contain the Path3D?
 */
Datum
contain_sphere_path3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Path3D     *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_BOOL(contain_sphere_path3D_internal(sphere, path));
}

PG_FUNCTION_INFO_V1(contain_sphere_path3D);

bool
contain_sphere_path3D_internal(Sphere *sphere, Path3D *path)
{
	int			i;
	bool		result;
	
	result = true;
	
	for (i = 0; i < path->npts; i++)
		result = result && contain_sphere_point3D_internal(sphere, &path->p[i]) ;	

	return result;
}

/*
 * Does the Sphere contain the Polygon3D?
 */
Datum
contain_sphere_poly3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	
	PG_RETURN_BOOL(contain_sphere_poly3D_internal(sphere, poly));
}

PG_FUNCTION_INFO_V1(contain_sphere_poly3D);

bool
contain_sphere_poly3D_internal(Sphere *sphere, Polygon3D *poly)
{
	Path3D     *path;
	bool		result;

	path = poly3D_path3D_internal(poly);
	result = contain_sphere_path3D_internal(sphere, path);
	pfree(path);
	
	return result;
}

/*
 * Does the first Sphere contain the second Sphere?
 */
Datum
contain_sphere_sphere(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere1 = PG_GETARG_SPHERE_P(0);
	Sphere	   *sphere2 = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(contain_sphere_sphere_internal(sphere1, sphere2));
}

PG_FUNCTION_INFO_V1(contain_sphere_sphere);

bool
contain_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2)
{
	return FPle((dist_point3D_point3D(&sphere1->center, &sphere2->center) + sphere2->radius), sphere1->radius);
}


/*****************************************************************************
 * Contained functions 
 *****************************************************************************/
 
/* Point3D contained TYPE3D */

/*
 * Is the Point3D contained in the Lseg3D?
 */
Datum
contained_point3D_lseg3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(contain_lseg3D_point3D_internal(lseg, point));
}

PG_FUNCTION_INFO_V1(contained_point3D_lseg3D) ;

bool
contained_point3D_lseg3D_internal(Point3D *point, Lseg3D *lseg)
{
	return contain_lseg3D_point3D_internal(lseg, point);
}

/*
 * Is the Point3D contained in the Line3D?
 */
Datum
contained_point3D_line3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(contain_line3D_point3D_internal(line, point));
}

PG_FUNCTION_INFO_V1(contained_point3D_line3D);

bool
contained_point3D_line3D_internal(Point3D *point, Line3D *line)
{
	return contain_line3D_point3D_internal(line, point);
}

/*
 * Is the Point3D contained in the Box3D?
 */
Datum
contained_point3D_box3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(contain_box3D_point3D_internal(box, point));
}

PG_FUNCTION_INFO_V1(contained_point3D_box3D);

bool
contained_point3D_box3D_internal(Point3D *point, Box3D *box)
{
	return contain_box3D_point3D_internal(box, point);
}

/*
 * Is the Point3D contained in the Path3D?
 */
Datum
contained_point3D_path3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_BOOL(contain_path3D_point3D_internal(path, point));
}

PG_FUNCTION_INFO_V1(contained_point3D_path3D) ;

bool
contained_point3D_path3D_internal(Point3D *point, Path3D *path)
{
	return contain_path3D_point3D_internal(path, point);
}

/*
 * Is the Point3D contained in the Polygon3D?
 */
Datum
contained_point3D_poly3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_BOOL(contain_poly3D_point3D_internal(poly, point));
}

PG_FUNCTION_INFO_V1(contained_point3D_poly3D);

bool
contained_point3D_poly3D_internal(Point3D *point, Polygon3D *poly)
{
	return contain_poly3D_point3D_internal(poly, point);
}

/*
 * Is the Point3D contained in the Sphere?
 */
Datum
contained_point3D_sphere(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(contain_sphere_point3D_internal(sphere, point));
}

PG_FUNCTION_INFO_V1(contained_point3D_sphere);

bool
contained_point3D_sphere_internal(Point3D *point, Sphere *sphere)
{
	return contain_sphere_point3D_internal(sphere, point);
}
 
/*----------------------------------------------------------*/

/* Lseg3D contained TYPE3D */

/*
 * Is the first Lseg3D contained in the second Lseg3D?
 */
Datum
contained_lseg3D_lseg3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D	   *lseg2 = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(contain_lseg3D_lseg3D_internal(lseg2, lseg1));
}

PG_FUNCTION_INFO_V1(contained_lseg3D_lseg3D) ;

bool
contained_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2)
{
	return contain_lseg3D_lseg3D_internal(lseg2, lseg1);
}

/* 
 * Is the Lseg3D contained in the Line3D?
 */

Datum
contained_lseg3D_line3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(contain_line3D_lseg3D_internal(line, lseg));
}

PG_FUNCTION_INFO_V1(contained_lseg3D_line3D) ;

bool
contained_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line)
{
	return contain_line3D_lseg3D_internal(line, lseg);
}

/*
 * Is the Lseg3D contained in the Box3D?
 */
Datum
contained_lseg3D_box3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(contain_box3D_lseg3D_internal(box, lseg));
}

PG_FUNCTION_INFO_V1(contained_lseg3D_box3D);

bool
contained_lseg3D_box3D_internal(Lseg3D *lseg, Box3D *box)
{
	return contain_box3D_lseg3D_internal(box, lseg);
}

/*
 * Is the Lseg3D contained in the Path3D?
 */
Datum
contained_lseg3D_path3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_BOOL(contain_path3D_lseg3D_internal(path, lseg));
}

PG_FUNCTION_INFO_V1(contained_lseg3D_path3D);

bool
contained_lseg3D_path3D_internal(Lseg3D *lseg, Path3D *path)
{
	return contain_path3D_lseg3D_internal(path, lseg);
}

/*
 * Is the Lseg3D contained in the Polygon3D?
 */
Datum
contained_lseg3D_poly3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_BOOL(contain_poly3D_lseg3D_internal(poly, lseg));
}

PG_FUNCTION_INFO_V1(contained_lseg3D_poly3D);

bool
contained_lseg3D_poly3D_internal(Lseg3D *lseg, Polygon3D *poly)
{
	return contain_poly3D_lseg3D_internal(poly, lseg);
}

/*
 * Is the Lseg3D contained in the Sphere?
 */
Datum
contained_lseg3D_sphere(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(contain_sphere_lseg3D_internal(sphere, lseg));
}

PG_FUNCTION_INFO_V1(contained_lseg3D_sphere);

bool
contained_lseg3D_sphere_internal(Lseg3D *lseg, Sphere *sphere)
{
	return contain_sphere_lseg3D_internal(sphere, lseg);
}

/*----------------------------------------------------------*/

/* Box3D contained TYPE3D */

/*
 * Is the first Box3D contained in the second Box3D?
 */
Datum
contained_box3D_box3D(PG_FUNCTION_ARGS)
{
	Box3D		   *box1 = PG_GETARG_BOX3D_P(0);
	Box3D		   *box2 = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(contain_box3D_box3D_internal(box2, box1));
}

PG_FUNCTION_INFO_V1(contained_box3D_box3D);

bool
contained_box3D_box3D_internal(Box3D *box1, Box3D *box2)
{
	return contain_box3D_box3D_internal(box2, box1);
}

/*
 * Is the Box3D contained in the Sphere?
 */
Datum
contained_box3D_sphere(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Sphere     *sphere = PG_GETARG_SPHERE_P(1);
	
	PG_RETURN_BOOL(contain_sphere_box3D_internal(sphere, box));
}

PG_FUNCTION_INFO_V1(contained_box3D_sphere);

bool 
contained_box3D_sphere_internal(Box3D *box, Sphere *sphere)
{
	return contain_sphere_box3D_internal(sphere, box);
}

/*----------------------------------------------------------*/

/* path3D contained TYPE3D */

/*
 * Is the Path3D contained in the Box3D?
 */
Datum
contained_path3D_box3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(contain_box3D_path3D_internal(box, path));
}

PG_FUNCTION_INFO_V1(contained_path3D_box3D);

bool
contained_path3D_box3D_internal(Path3D *path, Box3D *box)
{
	return contain_box3D_path3D_internal(box, path);
}

/*
 * Is the first Path3D contained in the second Path3D?
 */
Datum
contained_path3D_path3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path1 = PG_GETARG_PATH3D_P(0);
	Path3D	   *path2 = PG_GETARG_PATH3D_P(1);

	PG_RETURN_BOOL(contain_path3D_path3D_internal(path2, path1));
}

PG_FUNCTION_INFO_V1(contained_path3D_path3D);

bool
contained_path3D_path3D_internal(Path3D *path1, Path3D *path2)
{
	return contain_path3D_path3D_internal(path2, path1);
}

/*
 * Is the Path3D contained in the Polygon3D?
 */
Datum
contained_path3D_poly3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_BOOL(contain_poly3D_path3D_internal(poly, path));
}

PG_FUNCTION_INFO_V1(contained_path3D_poly3D);

bool
contained_path3D_poly3D_internal(Path3D *path, Polygon3D *poly)
{
	return contain_poly3D_path3D_internal(poly, path);
}

/*
 * Is the Path3D contained in the Sphere?
 */
Datum
contained_path3D_sphere(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(contain_sphere_path3D_internal(sphere, path));
}

PG_FUNCTION_INFO_V1(contained_path3D_sphere);

bool
contained_path3D_sphere_internal(Path3D *path, Sphere *sphere)
{
	return contain_sphere_path3D_internal(sphere, path);
}

/*----------------------------------------------------------*/

/* poly3D contained TYPE3D */

/*
 * Is the Polygon3D contained in the Box3D?
 */
Datum
contained_poly3D_box3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(contain_box3D_poly3D_internal(box, poly));
}

PG_FUNCTION_INFO_V1(contained_poly3D_box3D);

bool
contained_poly3D_box3D_internal(Polygon3D *poly, Box3D *box)
{
	return contain_box3D_poly3D_internal(box, poly);
}

/*
 * Is the first Polygon3D contained in the second Polygon3D?
 */
Datum
contained_poly3D_poly3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly1 = PG_GETARG_POLYGON3D_P(0);
	Polygon3D  *poly2 = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_BOOL(contain_poly3D_poly3D_internal(poly2, poly1));
}

PG_FUNCTION_INFO_V1(contained_poly3D_poly3D);

bool
contained_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2)
{
	return contain_poly3D_poly3D_internal(poly2, poly1);
}

/*
 * Is the Polygon3D contained in the Sphere?
 */
Datum
contained_poly3D_sphere(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(contain_sphere_poly3D_internal(sphere, poly));
}

PG_FUNCTION_INFO_V1(contained_poly3D_sphere);

bool
contained_poly3D_sphere_internal(Polygon3D *poly, Sphere *sphere)
{
	return contain_sphere_poly3D_internal(sphere, poly);
}

/*----------------------------------------------------------*/

/* sphere contained TYPE3D */

/*
 * Is the Sphere contained in the Box3D?
 */
Datum
contained_sphere_box3D(PG_FUNCTION_ARGS)
{
	Sphere  *sphere = PG_GETARG_SPHERE_P(0);
	Box3D	*box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(contain_box3D_sphere_internal(box, sphere));
}

PG_FUNCTION_INFO_V1(contained_sphere_box3D);

bool
contained_sphere_box3D_internal(Sphere *sphere, Box3D *box)
{
	return contain_box3D_sphere_internal(box, sphere);
}

/*
 * Is the first Sphere contained in the second Sphere?
 */
Datum
contained_sphere_sphere(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere1 = PG_GETARG_SPHERE_P(0);
	Sphere	   *sphere2 = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(contain_sphere_sphere_internal(sphere2, sphere1));
}

PG_FUNCTION_INFO_V1(contained_sphere_sphere);

bool
contained_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2)
{
	return contain_sphere_sphere_internal(sphere2, sphere1);
}

/*****************************************************************************
 * Intersect functions
 *****************************************************************************/

/* Lseg3D intersect TYPE3D */

/*
 * Do the two Lseg3D intersect?
 */
Datum
intersect_lseg3D_lseg3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D	   *lseg2 = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(inter_lseg3D_lseg3D(lseg1, lseg2));
}

PG_FUNCTION_INFO_V1(intersect_lseg3D_lseg3D);

bool 
inter_lseg3D_lseg3D(Lseg3D *lseg1, Lseg3D *lseg2)
{
	Point3D		pa,
				pb;

	shortest_segm_lseg3D_lseg3D(lseg1, lseg2, &pa, &pb);
	return PT3Deq(pa, pb);
}

/* 
 * Do the Lseg3D and the Line3D intersect?
 */
Datum
intersect_lseg3D_line3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(inter_lseg3D_line3D(lseg, line));
}

PG_FUNCTION_INFO_V1(intersect_lseg3D_line3D) ;

bool
inter_lseg3D_line3D(Lseg3D *lseg, Line3D *line)
{
	Point3D		pa,
				pb;

	shortest_segm_lseg3D_line3D(lseg, line, &pa, &pb);
	return PT3Deq(pa, pb);
}

/* 
 * Do the Lseg3D and the Box3D intersect?
 */
Datum
intersect_lseg3D_box3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(inter_lseg3D_box3D(lseg, box));
}

PG_FUNCTION_INFO_V1(intersect_lseg3D_box3D) ;

// Do not use routine shortest_segm_lseg3D_line3D because the code below is faster
bool
inter_lseg3D_box3D(Lseg3D *lseg, Box3D *box)
{
	Point3D		d,
				e,
				c,
				ad;
				
    d = scalar(0.5, sub(lseg->p[1], lseg->p[0]));
    e = scalar(0.5, sub(box->high, box->low));
    c = sub(add(lseg->p[0], d), scalar(0.5, add(box->low, box->high)));
    ad = absolute(d); // Returns same point with all components positive

    if (FPgt(fabs(c.x), e.x + ad.x))
        return false;

    if (FPgt(fabs(c.y), e.y + ad.y))
        return false;

    if (FPgt(fabs(c.z), e.z + ad.z))
        return false;

    if (FPgt(fabs(d.y * c.z - d.z * c.y), e.y * ad.z + e.z * ad.y))
        return false;

    if (FPgt(fabs(d.z * c.x - d.x * c.z), e.z * ad.x + e.x * ad.z))
        return false;

    if (FPgt(fabs(d.x * c.y - d.y * c.x), e.x * ad.y + e.y * ad.x))
        return false;

    return true;
}

/* 
 * Do the Lseg3D and the Path3D intersect?
 */
Datum
intersect_lseg3D_path3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_BOOL(inter_lseg3D_path3D(lseg, path));
}
	
PG_FUNCTION_INFO_V1(intersect_lseg3D_path3D) ;

bool
inter_lseg3D_path3D(Lseg3D *lseg, Path3D *path)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_lseg3D_path3D(lseg, path, &pa, &pb);
	if (PT3Deq(pa, pb))
		return TRUE;
	else
		return FALSE;
}

/* 
 * Do the Lseg3D and the Polygon3D intersect?
 */

Datum
intersect_lseg3D_poly3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_BOOL(inter_lseg3D_poly3D(lseg, poly));
}

PG_FUNCTION_INFO_V1(intersect_lseg3D_poly3D);

bool
inter_lseg3D_poly3D(Lseg3D *lseg, Polygon3D *poly)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_lseg3D_poly3D(lseg, poly, &pa, &pb);
	if (PT3Deq(pa, pb))
		return TRUE;
	else
		return FALSE;
}

/* 
 * Do the Lseg3D and the Sphere intersect?
 */
Datum
intersect_lseg3D_sphere(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(inter_lseg3D_sphere(lseg, sphere));
}

PG_FUNCTION_INFO_V1(intersect_lseg3D_sphere);

bool
inter_lseg3D_sphere(Lseg3D *lseg, Sphere *sphere)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_lseg3D_sphere(lseg, sphere, &pa, &pb);
	if (PT3Deq(pa, pb))
		return TRUE;
	else
		return FALSE;
}

/*---------------------------------------------------------------------*/
 
 /* line3D intersect TYPE3D */

/* 
 * Do the Line3D and the Lseg3D intersect?
*/
Datum
intersect_line3D_lseg3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(inter_lseg3D_line3D(lseg, line));
}

PG_FUNCTION_INFO_V1(intersect_line3D_lseg3D) ;

bool
inter_line3D_lseg3D(Line3D *line, Lseg3D *lseg)
{
	return inter_lseg3D_line3D(lseg, line);
}

/*
 * Do the two Line3D intersect?
 */
Datum
intersect_line3D_line3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line1 = PG_GETARG_LINE3D_P(0);
	Line3D	   *line2 = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(inter_line3D_line3D(line1, line2));
}

PG_FUNCTION_INFO_V1(intersect_line3D_line3D);

bool 
inter_line3D_line3D(Line3D *line1, Line3D *line2)
{
	Point3D		pa,
				pb;

	shortest_segm_line3D_line3D(line1, line2, &pa, &pb);
	return PT3Deq(pa, pb);
}

/* 
 * Do the Line3D and the Box3D intersect?
 */
Datum
intersect_line3D_box3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(inter_line3D_box3D(line, box));
}

PG_FUNCTION_INFO_V1(intersect_line3D_box3D);

// Based on https://www.gamedev.net/topic/338987-aabb---line-segment-intersection-test/
bool
inter_line3D_box3D(Line3D *line, Box3D *box)
{
	Point3D		c,
				d,
				e;
    int			parallel = 0;
	double 		es,
				s,
				invDi,
				t0,
				t1;
				
    c = scalar(0.5, add(box->low, box->high));
    d = sub(c, line->p[0]);
    e = scalar(0.5, sub(box->high, box->low));

	// X Axis
	if (FPzero(fabs(line->p[1].x)))
		parallel |= 1;
	else
	{
		es = FPgt(line->p[1].x, 0.0) ? e.x : -e.x;
		invDi = 1.0 / line->p[1].x;
		t0 = (d.x - es) * invDi;
		t1 = (d.x + es) * invDi;
	}
	
	// Y Axis
	if (FPzero(fabs(line->p[1].y)))
		parallel |= 1 << 1;
	else
	{
		es = FPgt(line->p[1].y, 0.0) ? e.y : -e.y;
		invDi = 1.0 / line->p[1].y;
		s = (d.y - es) * invDi;
		if (FPgt(s, t0))
			t0 = s;
		s = (d.y + es) * invDi;
		if (FPlt(s, t1))
			t1 = s;
		if (FPgt(t0, t1))
			return false;
	}
	
	// Z Axis
	if (FPzero(fabs(line->p[1].z)))
		parallel |= 1 << 2;
	else
	{
		es = FPgt(line->p[1].z, 0.0) ? e.z : -e.z;
		invDi = 1.0 / line->p[1].z;
		s = (d.z - es) * invDi;
		if (FPgt(s, t0))
			t0 = s;
		s = (d.z + es) * invDi;
		if (FPlt(s, t1))
			t1 = s;
		if (FPgt(t0, t1))
			return false;
	}
	
    if (parallel)
	{
		if (parallel & (1 << 0))
			if (FPgt(fabs(d.x - t0 * line->p[1].x), e.x) || FPgt(fabs(d.x - t1 * line->p[1].x), e.x))
				return false;

		if (parallel & (1 << 1))
			if (FPgt(fabs(d.y - t0 * line->p[1].y), e.y) || FPgt(fabs(d.y - t1 * line->p[1].y), e.y))
				return false;

		if (parallel & (1 << 2))
			if (FPgt(fabs(d.z - t0 * line->p[1].z), e.z) || FPgt(fabs(d.z - t1 * line->p[1].z), e.z))
				return false;
	}
    return true;
}

/* 
 * Do the Line3D and the Path3D intersect?
 */
Datum
intersect_line3D_path3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_BOOL(inter_line3D_path3D(line, path));
}
	
PG_FUNCTION_INFO_V1(intersect_line3D_path3D) ;

bool
inter_line3D_path3D(Line3D *line, Path3D *path)
{
	Point3D		pa,
				pb;

	shortest_segm_line3D_path3D(line, path, &pa, &pb);
	if (PT3Deq(pa, pb))
		return TRUE;
	else
		return FALSE;
}

/* 
 * Do the Line3D and the Polygon3D intersect?
 */

Datum
intersect_line3D_poly3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_BOOL(inter_line3D_poly3D(line, poly));
}

PG_FUNCTION_INFO_V1(intersect_line3D_poly3D);

bool
inter_line3D_poly3D(Line3D *line, Polygon3D *poly)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_line3D_poly3D(line, poly, &pa, &pb);
	if (PT3Deq(pa, pb))
		return TRUE;
	else
		return FALSE;
}

/* 
 * Do the Line3D and the Sphere intersect?
 */

Datum
intersect_line3D_sphere(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(inter_line3D_sphere(line, sphere));
}

PG_FUNCTION_INFO_V1(intersect_line3D_sphere);

bool
inter_line3D_sphere(Line3D *line, Sphere *sphere)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_line3D_sphere(line, sphere, &pa, &pb);
	if (PT3Deq(pa, pb))
		return TRUE;
	else
		return FALSE;
}

/*---------------------------------------------------------------------*/
 
/* Box3D intersect TYPE3D */

/* 
 * Do the Box3D and the Lseg3D intersect?
 */
Datum
intersect_box3D_lseg3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(inter_lseg3D_box3D(lseg, box));
}

PG_FUNCTION_INFO_V1(intersect_box3D_lseg3D) ;

bool
inter_box3D_lseg3D(Box3D *box, Lseg3D *lseg)
{
	return inter_lseg3D_box3D(lseg, box);
}

/* 
 * Do the Box3D and the Line3D intersect?
 */
Datum
intersect_box3D_line3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(inter_line3D_box3D(line, box));
}
	
PG_FUNCTION_INFO_V1(intersect_box3D_line3D) ;

bool
inter_box3D_line3D(Box3D *box, Line3D *line)
{
	return inter_line3D_box3D(line, box);
}

/* 
 * Do the two Box3D intersect?
 */
Datum
intersect_box3D_box3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box1 = PG_GETARG_BOX3D_P(0);
	Box3D	   *box2 = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(inter_box3D_box3D(box1, box2));
}
	
PG_FUNCTION_INFO_V1(intersect_box3D_box3D) ;

bool
inter_box3D_box3D(Box3D *box1, Box3D *box2)
{
	return (FPle(box1->low.x, box2->high.x) &&
			FPle(box2->low.x, box1->high.x) &&
			FPle(box1->low.y, box2->high.y) &&
			FPle(box2->low.y, box1->high.y) &&
			FPle(box1->low.z, box2->high.z) &&
			FPle(box2->low.z, box1->high.z));
}

/* 
 * Do the Box3D and the Path3D intersect?
 */
Datum
intersect_box3D_path3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_BOOL(inter_box3D_path3D(box, path));
}
	
PG_FUNCTION_INFO_V1(intersect_box3D_path3D) ;

bool
inter_box3D_path3D(Box3D *box, Path3D *path)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_box3D_path3D(box, path, &pa, &pb);
	if (PT3Deq(pa, pb))
		return TRUE;
	else
		return FALSE;
}

/* 
 * Do the Box3D and the Polygon3D intersect?
 */
Datum
intersect_box3D_poly3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_BOOL(overlap_box3D_poly3D_internal(box, poly));
}
	
PG_FUNCTION_INFO_V1(intersect_box3D_poly3D);

bool
inter_box3D_poly3D(Box3D *box, Polygon3D *poly)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_box3D_poly3D(box, poly, &pa, &pb);
	if (PT3Deq(pa, pb))
		return TRUE;
	else
		return FALSE;
}

/* 
 * Do the Box3D and the Sphere intersect?
 */
Datum
intersect_box3D_sphere(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overlap_box3D_sphere_internal(box, sphere));
}
	
PG_FUNCTION_INFO_V1(intersect_box3D_sphere);

bool
inter_box3D_sphere(Box3D *box, Sphere *sphere)
{
	Point3D		point;
	
	/* Left */
	point.x = sphere->center.x - sphere->radius;
	point.y = sphere->center.y;
	point.z = sphere->center.z;
	if (contained_point3D_box3D_internal(&point, box))
		return TRUE;

	/* Right */
	point.x = sphere->center.x + sphere->radius;
	point.y = sphere->center.y;
	point.z = sphere->center.z;
	if (contained_point3D_box3D_internal(&point, box))
		return TRUE;
	
	/* Below */
	point.x = sphere->center.x;
	point.y = sphere->center.y - sphere->radius;
	point.z = sphere->center.z;
	if (contained_point3D_box3D_internal(&point, box))
		return TRUE;

	/* Above */
	point.x = sphere->center.x;
	point.y = sphere->center.y + sphere->radius;
	point.z = sphere->center.z;
	if (contained_point3D_box3D_internal(&point, box))
		return TRUE;
	
	/* Front */
	point.x = sphere->center.x;
	point.y = sphere->center.y;
	point.z = sphere->center.z - sphere->radius;
	if (contained_point3D_box3D_internal(&point, box))
		return TRUE;

	/* Back */
	point.x = sphere->center.x;
	point.y = sphere->center.y;
	point.z = sphere->center.z + sphere->radius;
	if (contained_point3D_box3D_internal(&point, box))
		return TRUE;
	
	/* No intersection found */
	return FALSE;
}

/*---------------------------------------------------------------------*/
 
/* path3D intersect TYPE3D */

/* 
 * Do the Path3D and the Lseg3D intersect?
 */
Datum
intersect_path3D_lseg3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(inter_lseg3D_path3D(lseg, path));
}
	
PG_FUNCTION_INFO_V1(intersect_path3D_lseg3D) ;

bool
inter_path3D_lseg3D(Path3D *path, Lseg3D *lseg)
{
	return inter_lseg3D_path3D(lseg, path);
}

/* 
 * Do the Path3D and the Line3D intersect?
 */
Datum
intersect_path3D_line3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(inter_line3D_path3D(line, path));
}
	
PG_FUNCTION_INFO_V1(intersect_path3D_line3D) ;

bool
inter_path3D_line3D(Path3D *path, Line3D *line)
{
	return inter_line3D_path3D(line, path);
}

/* 
 * Do the Path3D and the Box3D intersect?
 */
Datum
intersect_path3D_box3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(inter_box3D_path3D(box, path));
}
	
PG_FUNCTION_INFO_V1(intersect_path3D_box3D) ;

bool
inter_path3D_box3D(Path3D *path, Box3D *box)
{
	return inter_box3D_path3D(box, path);
}

/* 
 * Do the two Path3D intersect?
 */
Datum
intersect_path3D_path3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path1 = PG_GETARG_PATH3D_P(0);
	Path3D	   *path2 = PG_GETARG_PATH3D_P(1);

	PG_RETURN_BOOL(inter_path3D_path3D(path1, path2));
}

PG_FUNCTION_INFO_V1(intersect_path3D_path3D) ;

bool
inter_path3D_path3D(Path3D *path1, Path3D *path2)
{
	Point3D		pa,
				pb;

	shortest_segm_path3D_path3D(path1, path2, &pa, &pb);
	if (PT3Deq(pa, pb))
		return TRUE;
	else
		return FALSE;
}

/* 
 * Do the Path3D and the Polygon3D intersect?
 */
Datum
intersect_path3D_poly3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_BOOL(inter_path3D_poly3D(path, poly));
}

PG_FUNCTION_INFO_V1(intersect_path3D_poly3D) ;

bool
inter_path3D_poly3D(Path3D *path, Polygon3D *poly)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_path3D_poly3D(path, poly, &pa, &pb);
	if (PT3Deq(pa, pb))
		return TRUE;
	else
		return FALSE;
}

/* 
 * Do the Path3D and the Sphere intersect?
 */
Datum
intersect_path3D_sphere(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(inter_path3D_sphere(path, sphere));
}
	
PG_FUNCTION_INFO_V1(intersect_path3D_sphere) ;

bool
inter_path3D_sphere(Path3D *path, Sphere *sphere)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_path3D_sphere(path, sphere, &pa, &pb);
	if (PT3Deq(pa, pb))
		return TRUE;
	else
		return FALSE;
}

/*---------------------------------------------------------------------*/

/* poly3D intersect TYPE3D */

/* 
 * Do the Polygon3D and the Lseg3D intersect?
 */
Datum
intersect_poly3D_lseg3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(inter_lseg3D_poly3D(lseg, poly));
}
	
PG_FUNCTION_INFO_V1(intersect_poly3D_lseg3D) ;

bool
inter_poly3D_lseg3D(Polygon3D *poly, Lseg3D *lseg)
{
	return inter_lseg3D_poly3D(lseg, poly);
}

/* 
 * Do the Polygon3D and the Line3D intersect?
 */
Datum
intersect_poly3D_line3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(inter_line3D_poly3D(line, poly));
}
	
PG_FUNCTION_INFO_V1(intersect_poly3D_line3D) ;

bool
inter_poly3D_line3D(Polygon3D *poly, Line3D *line)
{
	return inter_line3D_poly3D(line, poly);
}

/* 
 * Do the Polygon3D and the Box3D intersect?
*/
Datum
intersect_poly3D_box3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overlap_box3D_poly3D_internal(box, poly));
}
	
PG_FUNCTION_INFO_V1(intersect_poly3D_box3D) ;

bool
inter_poly3D_box3D(Polygon3D *poly, Box3D *box)
{
	return overlap_box3D_poly3D_internal(box, poly);
}

/* 
 * Do the Polygon3D and the Path3D intersect?
 */
Datum
intersect_poly3D_path3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_BOOL(inter_path3D_poly3D(path, poly));
}
	
PG_FUNCTION_INFO_V1(intersect_poly3D_path3D) ;

bool
inter_poly3D_path3D(Polygon3D *poly, Path3D *path)
{
	return inter_path3D_poly3D(path, poly);
}

/* 
 * Do the two Polygon3D intersect?
*/
Datum
intersect_poly3D_poly3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly1 = PG_GETARG_POLYGON3D_P(0);
	Polygon3D  *poly2 = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_BOOL(inter_poly3D_poly3D(poly1, poly2));
}
	
PG_FUNCTION_INFO_V1(intersect_poly3D_poly3D) ;

bool
inter_poly3D_poly3D(Polygon3D *poly1, Polygon3D *poly2)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_poly3D_poly3D(poly1, poly2, &pa, &pb);
	if (PT3Deq(pa, pb))
		return TRUE;
	else
		return FALSE;
}

/* 
 * Do the Polygon3D and the Sphere intersect?
 */
Datum
intersect_poly3D_sphere(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(inter_poly3D_sphere(poly, sphere));
}
	
PG_FUNCTION_INFO_V1(intersect_poly3D_sphere) ;

bool
inter_poly3D_sphere(Polygon3D *poly, Sphere *sphere)
{
	if (FPle(dist_point3D_poly3D(&sphere->center, poly), sphere->radius))
		return true;
	else 
		return false;
}

/*---------------------------------------------------------------------*/
 
/* sphere intersect TYPE3D */

/* 
 * Do the Sphere and the Lseg3D intersect?
 */
Datum
intersect_sphere_lseg3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(inter_lseg3D_sphere(lseg, sphere));
}
	
PG_FUNCTION_INFO_V1(intersect_sphere_lseg3D) ;

bool
inter_sphere_lseg3D(Sphere *sphere, Lseg3D *lseg)
{
	return inter_lseg3D_sphere(lseg, sphere);
}

/* 
 * Do the Sphere and the Line3D intersect?
 */
Datum
intersect_sphere_line3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(inter_line3D_sphere(line, sphere));
}
	
PG_FUNCTION_INFO_V1(intersect_sphere_line3D) ;

bool
inter_sphere_line3D(Sphere *sphere, Line3D *line)
{
	return inter_line3D_sphere(line, sphere);
}

/* 
 * Do the Sphere and the Box3D intersect?
 */
Datum
intersect_sphere_box3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overlap_box3D_sphere_internal(box, sphere));
}
	
PG_FUNCTION_INFO_V1(intersect_sphere_box3D) ;

/* 
 * Do the Sphere and the Path3D intersect?
 */
Datum
intersect_sphere_path3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_BOOL(inter_path3D_sphere(path, sphere));
}
	
PG_FUNCTION_INFO_V1(intersect_sphere_path3D) ;

bool
inter_sphere_path3D(Sphere *sphere, Path3D *path)
{
	return inter_path3D_sphere(path, sphere);
}

/* 
 * Do the Sphere and the Polygon3D intersect?
 */
Datum
intersect_sphere_poly3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_BOOL(inter_poly3D_sphere(poly, sphere));
}
	
PG_FUNCTION_INFO_V1(intersect_sphere_poly3D) ;

bool
inter_sphere_poly3D(Sphere *sphere, Polygon3D *poly)
{
	return inter_poly3D_sphere(poly, sphere);
}

/* 
 * Do the two Sphere intersect?
*/
Datum
intersect_sphere_sphere(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere1 = PG_GETARG_SPHERE_P(0);
	Sphere	   *sphere2 = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overlap_sphere_sphere_internal(sphere1, sphere2));
}
	
PG_FUNCTION_INFO_V1(intersect_sphere_sphere) ;

bool
inter_sphere_sphere(Sphere *sphere1, Sphere *sphere2)
{
	return FPle(dist_point3D_point3D(&sphere1->center, &sphere2->center),
						sphere1->radius + sphere2->radius);
}

/*********************************************************************
 * Interpoint functions
 * Intersection point of objects.
 * If there are several intersection points between two objects (e.g. when 
 * two line segments are parallel or overlap) then one of them is returned
 *****************************************************************************/

/* Lseg3D interpoint TYPE3D */

/* 
 * Find the intersection point of the two Lseg3D (if any)
 */
Datum
interpoint_lseg3D_lseg3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D	   *lseg2 = PG_GETARG_LSEG3D_P(1);
	Point3D	   *result;

	result = interpt_lseg3D_lseg3D(lseg1, lseg2);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();

	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(interpoint_lseg3D_lseg3D);

Point3D *
interpt_lseg3D_lseg3D(Lseg3D *lseg1, Lseg3D *lseg2)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_lseg3D_lseg3D(lseg1, lseg2, &pa, &pb);
	if (!PT3Deq(pa, pb))
		return NULL;

	result = (Point3D *) palloc(sizeof(Point3D));
	*result = pa;
	return result;
}

/* 
 * Find the intersection point of the Lseg3D and the Line3D (if any)
 */
Datum
interpoint_lseg3D_line3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	Point3D	   *result;

	result = interpt_lseg3D_line3D(lseg, line);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(interpoint_lseg3D_line3D);

Point3D *
interpt_lseg3D_line3D(Lseg3D *lseg, Line3D *line)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_lseg3D_line3D(lseg, line, &pa, &pb);
	if (!PT3Deq(pa, pb))
		return NULL;

	result = (Point3D *) palloc(sizeof(Point3D));
	*result = pa;
	return result;
}

/* 
 * Find the intersection point of the Lseg3D and the Box3D (if any)
 */

Datum
interpoint_lseg3D_box3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Box3D 	   *box = PG_GETARG_BOX3D_P(1);
	Point3D	   *result;

	result = interpt_lseg3D_box3D(lseg, box);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(interpoint_lseg3D_box3D);

Point3D *
interpt_lseg3D_box3D(Lseg3D *lseg, Box3D *box)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_lseg3D_box3D(lseg, box, &pa, &pb);
	if (!PT3Deq(pa, pb))
		return NULL;

	result = (Point3D *) palloc(sizeof(Point3D));
	*result = pa;
	return result;
}

/* 
 * Find the intersection point of the Lseg3D and the Path3D (if any)
 */
Datum
interpoint_lseg3D_path3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	Point3D	   *result;

	result = interpt_lseg3D_path3D(lseg, path);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(interpoint_lseg3D_path3D);

Point3D *
interpt_lseg3D_path3D(Lseg3D *lseg, Path3D *path)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_lseg3D_path3D(lseg, path, &pa, &pb);
	if (PT3Deq(pa, pb))
		return NULL;

	result = (Point3D *) palloc(sizeof(Point3D));
	*result = pa;
	return result;
}

/* 
 * Find the intersection point of the Lseg3D and the Polygon3D (if any)
 */
Datum
interpoint_lseg3D_poly3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	Point3D	   *result;

	result = interpt_lseg3D_poly3D(lseg, poly);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(interpoint_lseg3D_poly3D);

Point3D *
interpt_lseg3D_poly3D(Lseg3D *lseg, Polygon3D *poly)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_lseg3D_poly3D(lseg, poly, &pa, &pb);
	if (PT3Deq(pa, pb))
		return NULL;

	result = (Point3D *) palloc(sizeof(Point3D));
	*result = pa;
	return result;
}

/* 
 * Find the intersection point of the Lseg3D and the Sphere (if any)
 */
Datum
interpoint_lseg3D_sphere(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	Point3D	   *result;

	result = interpt_lseg3D_sphere(lseg, sphere);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(interpoint_lseg3D_sphere);

Point3D *
interpt_lseg3D_sphere(Lseg3D *lseg, Sphere *sphere)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_lseg3D_sphere(lseg, sphere, &pa, &pb);
	if (!PT3Deq(pa, pb))
		return NULL;

	result = (Point3D *) palloc(sizeof(Point3D));
	*result = pa;
	return result;
}

/*---------------------------------------------------------------------*/
 
/* line3D interpoint TYPE3D */

/* 
 * Find the intersection point of the Line3D and the Lseg3D (if any)
 */

Datum
interpoint_line3D_lseg3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	Point3D	   *result;

	result = interpt_lseg3D_line3D(lseg, line);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(interpoint_line3D_lseg3D);

Point3D *
interpt_line3D_lseg3D(Line3D *line, Lseg3D *lseg)
{
	return interpt_lseg3D_line3D(lseg, line);
}

/* 
 * Find the intersection point of the two Line3D (if any)
 */
Datum
interpoint_line3D_line3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line1 = PG_GETARG_LINE3D_P(0);
	Line3D	   *line2 = PG_GETARG_LINE3D_P(1);
	Point3D	   *result;

	result = interpt_line3D_line3D(line1, line2);
	if (result == NULL)
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(interpoint_line3D_line3D);

Point3D *
interpt_line3D_line3D(Line3D *line1, Line3D *line2)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_line3D_line3D(line1, line2, &pa, &pb);
	if (PT3Deq(pa, pb))
	{
		result = (Point3D *) palloc(sizeof(Point3D));
		*result = pa;
		return result;
	}
	else
		return NULL;
}

/* 
 * Find the intersection point of the Line3D and the Box3D (if any)
 */
Datum
interpoint_line3D_box3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	Point3D	   *result;

	result = interpt_line3D_box3D(line, box);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(interpoint_line3D_box3D);

// Based on https://www.geometrictools.com/GTEngine/Include/Mathematics/GteIntrLine3AlignedBox3.h
// Do not use the routine shortest_segm_line3D_box3D because the code below is faster 
Point3D *
interpt_line3D_box3D(Line3D *line, Box3D *box)
{
	Point3D 	boxCenter,
				boxExtent,
				lineOrigin,
				lineDirection,
				*result;
	double 		magn,
				t0 = -DBL_MAX,
				t1 = DBL_MAX;	
	
    // Get the centered form of the box
    boxCenter = scalar(0.5, add(box->high, box->low));
    boxExtent = scalar(0.5, sub(box->high, box->low));

    // Transform the line to the box coordinate system
    lineOrigin = sub(line->p[0], boxCenter);
	lineDirection = sub(line->p[1], line->p[0]);
	magn = magnitude(lineDirection);
	lineDirection = scalar(1/magn, lineDirection);
	
    // The line t-values are in the interval (-infinity,+infinity).  
    // Clip the line against all six planes of an aligned box in centered form.  
    if (clip(+lineDirection.x, -lineOrigin.x - boxExtent.x, &t0, &t1) &&
        clip(-lineDirection.x, +lineOrigin.x - boxExtent.x, &t0, &t1) &&
        clip(+lineDirection.y, -lineOrigin.y - boxExtent.y, &t0, &t1) &&
        clip(-lineDirection.y, +lineOrigin.y - boxExtent.y, &t0, &t1) &&
        clip(+lineDirection.z, -lineOrigin.z - boxExtent.z, &t0, &t1) &&
        clip(-lineDirection.z, +lineOrigin.z - boxExtent.z, &t0, &t1))
    {
		result = (Point3D *) palloc(sizeof(Point3D));
		// If (t1 > t0) the line intersect the box in a segment defined by [t0,t1].
		// We arbitrarily return the point corresponding to t0.
		// If (t1 = t0) the line intersect the box in a segment defined by t0.
		// We return the point corresponding to t0.
		*result = add(line->p[0], scalar(t0, lineDirection));
		return result;
    }
	else
		return NULL;
}

static bool
clip(double denom, double numer, double *t0, double *t1)
{
    if (FPgt(denom, 0.0))
    {
        if (FPgt(numer, denom * (*t1)))
            return false;
        if (FPgt(numer, denom * (*t0)))
            *t0 = numer / denom;
        return true;
    }
    else if (FPlt(denom, 0.0))
    {
        if (FPgt(numer, denom * (*t0)))
            return false;
        if (FPgt(numer, denom * (*t1)))
            *t1 = numer / denom;
        return true;
    }
    else
        return FPle(numer, 0.0);
}

/* 
 * Find the intersection point of the Line3D and the Path3D (if any)
 */
Datum
interpoint_line3D_path3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	Point3D	   *result;

	result = interpt_line3D_path3D(line, path);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(interpoint_line3D_path3D);

Point3D *
interpt_line3D_path3D(Line3D *line, Path3D *path)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_line3D_path3D(line, path, &pa, &pb);
	if (!PT3Deq(pa, pb))
		return NULL;

	result = (Point3D *) palloc(sizeof(Point3D));
	*result = pa;
	return result;
}

/* 
 * Find the intersection point of the Line3D and the Polygon3D (if any)
 */
Datum
interpoint_line3D_poly3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	Point3D	   *result;

	result = interpt_line3D_poly3D(line, poly);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(interpoint_line3D_poly3D);

Point3D *
interpt_line3D_poly3D(Line3D *line, Polygon3D *poly)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_line3D_poly3D(line, poly, &pa, &pb);
	if (!PT3Deq(pa, pb))
		return NULL;

	result = (Point3D *) palloc(sizeof(Point3D));
	*result = pa;
	return result;
}

/* 
 * Find the intersection point of the Line3D and the Sphere (if any)
 */
Datum
interpoint_line3D_sphere(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	Point3D	   *result;

	result = interpt_line3D_sphere(line, sphere);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(interpoint_line3D_sphere);

Point3D *
interpt_line3D_sphere(Line3D *line, Sphere *sphere)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_line3D_sphere(line, sphere, &pa, &pb);
	if (!PT3Deq(pa, pb))
		return NULL;

	result = (Point3D *) palloc(sizeof(Point3D));
	*result = pa;
	return result;
}

/*---------------------------------------------------------------------*/
 
/* Box3D interpoint TYPE3D */

/* 
 * Find the intersection point of the Box3D and the Lseg3D (if any)
 */
Datum
interpoint_box3D_lseg3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	Point3D	   *result;

	result = interpt_lseg3D_box3D(lseg, box);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}
	
PG_FUNCTION_INFO_V1(interpoint_box3D_lseg3D) ;

Point3D *
interpt_box3D_lseg3D(Box3D *box, Lseg3D *lseg)
{
	return interpt_lseg3D_box3D(lseg, box);
}

/* 
 * Find the intersection point of the Box3D and the Line3D (if any)
 */
Datum
interpoint_box3D_line3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	Point3D	   *result;

	result = interpt_line3D_box3D(line, box);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}
	
PG_FUNCTION_INFO_V1(interpoint_box3D_line3D) ;

Point3D *
interpt_box3D_line3D(Box3D *box, Line3D *line)
{
	return interpt_line3D_box3D(line, box);
}

/* 
 * Find the intersection point of the two Box3D (if any)
 */
Datum
interpoint_box3D_box3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box1 = PG_GETARG_BOX3D_P(0);
	Box3D	   *box2 = PG_GETARG_BOX3D_P(1);
	Point3D	   *result;

	result = interpt_box3D_box3D(box1, box2);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(interpoint_box3D_box3D) ;

Point3D *
interpt_box3D_box3D(Box3D *box1, Box3D *box2)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_box3D_box3D(box1, box2, &pa, &pb);
	if (PT3Deq(pa, pb))
	{
		result = (Point3D *) palloc(sizeof(Point3D));
		*result = pa;
		return result;
	}
	else
		return NULL;
}

/* 
 * Find the intersection point of the Box3D and the Path3D (if any)
 */
Datum
interpoint_box3D_path3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	Point3D	   *result;

	result = interpt_box3D_path3D(box, path);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(interpoint_box3D_path3D);

Point3D *
interpt_box3D_path3D(Box3D *box, Path3D *path)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_box3D_path3D(box, path, &pa, &pb);
	if (PT3Deq(pa, pb))
	{
		result = (Point3D *) palloc(sizeof(Point3D));
		*result = pa;
		return result;
	}
	else
		return NULL;
}

/* 
 * Find the intersection point of the Box3D and the Polygon3D (if any)
 */
Datum
interpoint_box3D_poly3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	Point3D	   *result;

	result = interpt_box3D_poly3D(box, poly);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}
	
PG_FUNCTION_INFO_V1(interpoint_box3D_poly3D) ;

Point3D *
interpt_box3D_poly3D(Box3D *box, Polygon3D *poly)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_box3D_poly3D(box, poly, &pa, &pb);
	if (PT3Deq(pa, pb))
	{
		result = (Point3D *) palloc(sizeof(Point3D));
		*result = pa;
		return result;
	}
	else
		return NULL;
}

/* 
 * Find the intersection point of the Box3D and the Sphere (if any)
 */
Datum
interpoint_box3D_sphere(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	Point3D	   *result;

	result = interpt_box3D_sphere(box, sphere);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(interpoint_box3D_sphere);

Point3D *
interpt_box3D_sphere(Box3D *box, Sphere *sphere)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_box3D_sphere(box, sphere, &pa, &pb);
	if (!PT3Deq(pa, pb))
		return NULL;

	result = (Point3D *) palloc(sizeof(Point3D));
	*result = pa;
	return result;
}

/*---------------------------------------------------------------------*/
 
/* path3D interpoint TYPE3D */

/* 
 * Find the intersection point of the Path3D and the Lseg3D (if any)
 */
Datum
interpoint_path3D_lseg3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	Point3D	   *result;

	result = interpt_lseg3D_path3D(lseg, path);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}
	
PG_FUNCTION_INFO_V1(interpoint_path3D_lseg3D) ;

Point3D *
interpt_path3D_lseg3D(Path3D *path, Lseg3D *lseg)
{
	return interpt_lseg3D_path3D(lseg, path);
}

/* 
 * Find the intersection point of the Path3D and the Line3D (if any)
 */
Datum
interpoint_path3D_line3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	Point3D	   *result;

	result = interpt_line3D_path3D(line, path);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}
	
PG_FUNCTION_INFO_V1(interpoint_path3D_line3D) ;

Point3D *
interpt_path3D_line3D(Path3D *path, Line3D *line)
{
	return interpt_line3D_path3D(line, path);
}

/* 
 * Find the intersection point of the Path3D and the Box3D (if any)
 */
Datum
interpoint_path3D_box3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	Point3D	   *result;

	result = interpt_box3D_path3D(box, path);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}
	
PG_FUNCTION_INFO_V1(interpoint_path3D_box3D) ;

Point3D *
interpt_path3D_box3D(Path3D *path, Box3D *box)
{
	return interpt_box3D_path3D(box, path);
}

/* 
 * Find the intersection point of the two Path3D (if any)
 */
Datum
interpoint_path3D_path3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path1 = PG_GETARG_PATH3D_P(0);
	Path3D	   *path2 = PG_GETARG_PATH3D_P(1);
	Point3D	   *result;

	result = interpt_path3D_path3D(path1, path2);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(interpoint_path3D_path3D) ;

Point3D *
interpt_path3D_path3D(Path3D *path1, Path3D *path2)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_path3D_path3D(path1, path2, &pa, &pb);
	if (!PT3Deq(pa, pb))
		return NULL;

	result = (Point3D *) palloc(sizeof(Point3D));
	*result = pa;
	return result;
}

/* 
 * Find the intersection point of the Path3D and the Polygon3D (if any)
 */
Datum
interpoint_path3D_poly3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	Point3D	   *result;

	result = interpt_path3D_poly3D(path, poly);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(interpoint_path3D_poly3D) ;

Point3D *
interpt_path3D_poly3D(Path3D *path, Polygon3D *poly)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_path3D_poly3D(path, poly, &pa, &pb);
	if (PT3Deq(pa, pb))
	{
		result = (Point3D *) palloc(sizeof(Point3D));
		*result = pa;
		return result;
	}
	else
		return NULL;
}

/* 
 * Find the intersection point of the Path3D and the Sphere (if any)
 */
Datum
interpoint_path3D_sphere(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	Point3D	   *result;

	result = interpt_path3D_sphere(path, sphere);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}
	
PG_FUNCTION_INFO_V1(interpoint_path3D_sphere) ;

Point3D *
interpt_path3D_sphere(Path3D *path, Sphere *sphere)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_path3D_sphere(path, sphere, &pa, &pb);
	if (!PT3Deq(pa, pb))
		return NULL;

	result = (Point3D *) palloc(sizeof(Point3D));
	*result = pa;
	return result;
}

/*---------------------------------------------------------------------*/
 
/* poly3D interpoint TYPE3D */

/* 
 * Find the intersection point of the Polygon3D and the Lseg3D (if any)
 */
Datum
interpoint_poly3D_lseg3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	Point3D	   *result;

	result = interpt_lseg3D_poly3D(lseg, poly);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}
	
PG_FUNCTION_INFO_V1(interpoint_poly3D_lseg3D) ;

Point3D *
interpt_poly3D_lseg3D(Polygon3D *poly, Lseg3D *lseg)
{
	return interpt_lseg3D_poly3D(lseg, poly);
}

/* 
 * Find the intersection point of the Polygon3D and the Line3D (if any)
 */
Datum
interpoint_poly3D_line3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	Point3D	   *result;

	result = interpt_line3D_poly3D(line, poly);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}
	
PG_FUNCTION_INFO_V1(interpoint_poly3D_line3D) ;

Point3D *
interpt_poly3D_line3D(Polygon3D *poly, Line3D *line)
{
	return interpt_line3D_poly3D(line, poly);
}

/* 
 * Find the intersection point of the Polygon3D and the Box3D (if any)
 */
Datum
interpoint_poly3D_box3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	Point3D	   *result;

	result = interpt_box3D_poly3D(box, poly);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}
	
PG_FUNCTION_INFO_V1(interpoint_poly3D_box3D) ;

Point3D *
interpt_poly3D_box3D(Polygon3D *poly, Box3D *box)
{
	return interpt_box3D_poly3D(box, poly);
}

/* 
 * Find the intersection point of the Polygon3D and the Path3D (if any)
 */
Datum
interpoint_poly3D_path3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	Point3D	   *result;

	result = interpt_path3D_poly3D(path, poly);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}
	
PG_FUNCTION_INFO_V1(interpoint_poly3D_path3D) ;

Point3D *
interpt_poly3D_path3D(Polygon3D *poly, Path3D *path)
{
	return interpt_path3D_poly3D(path, poly);
}

/* 
 * Find the intersection point of the two Polygon3D (if any)
 */
Datum
interpoint_poly3D_poly3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly1 = PG_GETARG_POLYGON3D_P(0);
	Polygon3D  *poly2 = PG_GETARG_POLYGON3D_P(1);
	Point3D	   *result;

	result = interpt_poly3D_poly3D(poly1, poly2);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}
	
PG_FUNCTION_INFO_V1(interpoint_poly3D_poly3D) ;

Point3D *
interpt_poly3D_poly3D(Polygon3D *poly1, Polygon3D *poly2)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_poly3D_poly3D(poly1, poly2, &pa, &pb);
	if (PT3Deq(pa, pb))
	{
		result = (Point3D *) palloc(sizeof(Point3D));
		*result = pa;
		return result;
	}
	else
		return NULL;
}

/* 
 * Find the intersection point of the Polygon3D and the Sphere (if any)
 */
Datum
interpoint_poly3D_sphere(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	Point3D	   *result;

	result = interpt_poly3D_sphere(poly, sphere);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(interpoint_poly3D_sphere);

Point3D *
interpt_poly3D_sphere(Polygon3D *poly, Sphere *sphere)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_poly3D_sphere(poly, sphere, &pa, &pb);
	if (!PT3Deq(pa, pb))
		return NULL;

	result = (Point3D *) palloc(sizeof(Point3D));
	*result = pa;
	return result;
}

/*---------------------------------------------------------------------*/
 
/* sphere interpoint TYPE3D */

/* 
 * Find the intersection point of the Sphere and the Lseg3D (if any)
 */

Datum
interpoint_sphere_lseg3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	Point3D	   *result;

	result = interpt_lseg3D_sphere(lseg, sphere);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(interpoint_sphere_lseg3D);

Point3D *
interpt_sphere_lseg3D(Sphere *sphere, Lseg3D *lseg)
{
	return interpt_lseg3D_sphere(lseg, sphere);
}

/* 
 * Find the intersection point of the Sphere and the Line3D (if any)
 */

Datum
interpoint_sphere_line3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	Point3D	   *result;

	result = interpt_line3D_sphere(line, sphere);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(interpoint_sphere_line3D);

Point3D *
interpt_sphere_line3D(Sphere *sphere, Line3D *line)
{
	return interpt_line3D_sphere(line, sphere);
}

/* 
 * Find the intersection point of the Sphere and the Box3D (if any)
 */
Datum
interpoint_sphere_box3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	Point3D	   *result;

	result = interpt_box3D_sphere(box, sphere);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(interpoint_sphere_box3D);

Point3D *
interpt_sphere_box3D(Sphere *sphere, Box3D *box)
{
	return interpt_box3D_sphere(box, sphere);
}

/* 
 * Find the intersection point of the Sphere and the Path3D (if any)
 */
Datum
interpoint_sphere_path3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	Point3D	   *result;

	result = interpt_path3D_sphere(path, sphere);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(interpoint_sphere_path3D);

Point3D *
interpt_sphere_path3D(Sphere *sphere, Path3D *path)
{
	return interpt_path3D_sphere(path, sphere);
}

/* 
 * Find the intersection point of the Sphere and the Polygon3D (if any)
 */
Datum
interpoint_sphere_poly3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	Point3D	   *result;

	result = interpt_poly3D_sphere(poly, sphere);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(interpoint_sphere_poly3D);

Point3D *
interpt_sphere_poly3D(Sphere *sphere, Polygon3D *poly)
{
	return interpt_poly3D_sphere(poly, sphere);
}

/* 
 * Find the intersection point of the two Sphere (if any)
 */
Datum
interpoint_sphere_sphere(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere1 = PG_GETARG_SPHERE_P(0);
	Sphere	   *sphere2 = PG_GETARG_SPHERE_P(1);
	Point3D	   *result;

	result = interpt_sphere_sphere(sphere1, sphere2);
	if (!PointerIsValid(result))
		PG_RETURN_NULL();
	
	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(interpoint_sphere_sphere);

Point3D *
interpt_sphere_sphere(Sphere *sphere1, Sphere *sphere2)
{
	Point3D		pa,
				pb,
			   *result;

	shortest_segm_sphere_sphere(sphere1, sphere2, &pa, &pb);
	if (!PT3Deq(pa, pb))
		return NULL;

	result = (Point3D *) palloc(sizeof(Point3D));
	*result = pa;
	return result;
}

/*****************************************************************************
 *	Intersection functions
 *****************************************************************************/

/*
 * Returns the overlapping portion of two boxes, or NULL if they do not intersect.
 */
Datum
box3D_intersection(PG_FUNCTION_ARGS)
{
	Box3D		   *box1 = PG_GETARG_BOX3D_P(0);
	Box3D		   *box2 = PG_GETARG_BOX3D_P(1);
	Box3D		   *result;

	if (!overlap_box3D_box3D_internal(box1, box2))
		PG_RETURN_NULL();

	result = (Box3D *) palloc(sizeof(Box3D));

	result->high.x = Min(box1->high.x, box2->high.x);
	result->low.x = Max(box1->low.x, box2->low.x);
	result->high.y = Min(box1->high.y, box2->high.y);
	result->low.y = Max(box1->low.y, box2->low.y);
	result->high.z = Min(box1->high.z, box2->high.z);
	result->low.z = Max(box1->low.z, box2->low.z);

	PG_RETURN_BOX3D_P(result);
}

PG_FUNCTION_INFO_V1(box3D_intersection);

/*****************************************************************************
 *	Distance functions
 *	Minimum distance from one object to another
 *****************************************************************************/

/* Point3D distance TYPE3D */

/*
 * Distance between the two Point3D
 */

Datum
distance_point3D_point3D(PG_FUNCTION_ARGS)
{
	Point3D	   *pt1 = PG_GETARG_POINT3D_P(0);
	Point3D	   *pt2 = PG_GETARG_POINT3D_P(1);

	PG_RETURN_FLOAT8(HYPOT3D(pt1->x - pt2->x, pt1->y - pt2->y, pt1->z - pt2->z));
}

PG_FUNCTION_INFO_V1(distance_point3D_point3D);

double
dist_point3D_point3D(Point3D *pt1, Point3D *pt2)
{
	return HYPOT3D(pt1->x - pt2->x, pt1->y - pt2->y, pt1->z - pt2->z);
}

/*
 * Distance between the Point3D and the Lseg3D
 */
Datum
distance_point3D_lseg3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_FLOAT8(dist_point3D_lseg3D(point, lseg));
}

PG_FUNCTION_INFO_V1(distance_point3D_lseg3D);

// See http://geomalgorithms.com/a02-_lines.html
double
dist_point3D_lseg3D(Point3D *point, Lseg3D *lseg)
{
	Point3D		v,
				w0,
				w1;

	v = sub(lseg->p[1], lseg->p[0]) ;
	w0 = sub(*point, lseg->p[0]) ;

	if (FPle(DOT(w0, v), 0.0))              
		// Point is lagging behind start of the segment, so perpendicular distance is not viable.
		// Use distance to start of segment instead.
		return magnitude(w0) ;          

	w1 = sub(*point, lseg->p[1]) ;

	if (FPge(DOT(w1, v), 0.0))
		// Point is advanced past the end of the segment, so perpendicular distance is not viable.
		// Use distance to end of the segment instead.
		return magnitude(w1);
		
	// Perpendicular distance of point to segment.
	return (magnitude(cross(v, w0))) / magnitude(v);
}

/*
 * Distance between the Point3D and the Line3D
 */
Datum
distance_point3D_line3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_FLOAT8(dist_point3D_line3D(point, line));
}

PG_FUNCTION_INFO_V1(distance_point3D_line3D);

// http://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html
double
dist_point3D_line3D(Point3D *point, Line3D *line)
{
	Point3D		v,
				w;
	
	v = sub(line->p[1], line->p[0]);
	w = sub(line->p[0], *point);
	return (magnitude(cross(w, v)) / magnitude(v));
}

/*
 * Distance between the Point3D and the Box3D
 */
Datum
distance_point3D_box3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_FLOAT8(dist_point3D_box3D(point, box));
}

PG_FUNCTION_INFO_V1(distance_point3D_box3D);

// Inspired from 
// http://stackoverflow.com/questions/5254838/calculating-distance-between-a-point-and-a-rectangular-box-nearest-point
double
distance_aux(double p, double lower, double upper)
{
	if (FPlt(p, lower))
		return lower - p;
	if (FPgt(p, upper))
		return p - upper;
	return Min(p - lower, upper - p);
}

double
dist_point3D_box3D(Point3D *point, Box3D *box) 
{
	if (FPle(point->x, box->high.x) && FPge(point->x, box->low.x) &&
		FPle(point->y, box->high.y) && FPge(point->y, box->low.y) &&
		FPle(point->z, box->high.z) && FPge(point->z, box->low.z))
	{
		// point is inside the box
		return 0.0;			
	}
	else
	{
		double 		dx,
					dy,
					dz;
		
		dx = distance_aux(point->x, box->low.x, box->high.x);
		dy = distance_aux(point->y, box->low.y, box->high.y);
		dz = distance_aux(point->z, box->low.z, box->high.z);
		return sqrt(dx * dx + dy * dy + dz * dz);
	}
}

/*
 * Distance between the Point3D and the Path3D
 */
Datum
distance_point3D_path3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_FLOAT8(dist_point3D_path3D(point, path));
}

PG_FUNCTION_INFO_V1(distance_point3D_path3D);

double
dist_point3D_path3D(Point3D *point, Path3D *path)
{
	Point3D	   *pt;

	pt =  closestpt_point3D_path3D(point, path);
	return dist_point3D_point3D(point, pt);
}

/*
 * Distance between the Point3D and the Polygon3D
 */
Datum
distance_point3D_poly3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_FLOAT8(dist_point3D_poly3D(point, poly));
}

PG_FUNCTION_INFO_V1(distance_point3D_poly3D);

double
dist_point3D_poly3D(Point3D *point, Polygon3D *poly)
{
	Point3D	   *pt;

	pt =  closestpt_point3D_poly3D(point, poly);
	return dist_point3D_point3D(point, pt);
}

/*
 * Distance between the Point3D and the Sphere
 */
Datum
distance_point3D_sphere(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_FLOAT8(dist_point3D_sphere(point, sphere));
}

PG_FUNCTION_INFO_V1(distance_point3D_sphere);

double
dist_point3D_sphere(Point3D *point, Sphere *sphere)
{
	double		result;

	result = dist_point3D_point3D(point, &sphere->center) - sphere->radius;
	if (FPlt(result, 0.0))
		result = 0;
	return result;
}

/*----------------------------------------------------------*/

/* Lseg3D distance TYPE3D */

/*
 * Distance between the Lseg3D and the Point3D
 */
Datum
distance_lseg3D_point3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_FLOAT8(dist_point3D_lseg3D(point, lseg));
}

PG_FUNCTION_INFO_V1(distance_lseg3D_point3D);

double
dist_lseg3D_point3D(Lseg3D *lseg, Point3D *point)
{
	return dist_point3D_lseg3D(point, lseg);
}

/* 
 * Distance between the two Lseg3D
 */
Datum
distance_lseg3D_lseg3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D	   *lseg2 = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_FLOAT8(dist_lseg3D_lseg3D(lseg1, lseg2));
}

PG_FUNCTION_INFO_V1(distance_lseg3D_lseg3D);

double
dist_lseg3D_lseg3D(Lseg3D *lseg1, Lseg3D *lseg2)
{
	Point3D		pa,
				pb;
				
	shortest_segm_lseg3D_lseg3D(lseg1, lseg2, &pa, &pb);
	return dist_point3D_point3D(&pa, &pb);
}

/*
 * Distance between the Lseg3D and the Line3D
 */
Datum
distance_lseg3D_line3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_FLOAT8(dist_lseg3D_line3D(lseg, line));
}

PG_FUNCTION_INFO_V1(distance_lseg3D_line3D);

double
dist_lseg3D_line3D(Lseg3D *lseg, Line3D *line)
{
	Point3D		pa,
				pb;
				
	shortest_segm_lseg3D_line3D(lseg, line, &pa, &pb);
	return dist_point3D_point3D(&pa, &pb);
}

/*
 * Distance between the Lseg3D and the Box3D
 */
Datum
distance_lseg3D_box3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_FLOAT8(dist_lseg3D_box3D(lseg, box));
}

PG_FUNCTION_INFO_V1(distance_lseg3D_box3D);

double
dist_lseg3D_box3D(Lseg3D *lseg, Box3D *box) 
{
	Point3D		pa,
				pb;
				
	shortest_segm_lseg3D_box3D(lseg, box, &pa, &pb);
	return dist_point3D_point3D(&pa, &pb);
}

/* 
 * Distance between the Lseg3D and the Path3D
 */
Datum
distance_lseg3D_path3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_FLOAT8(dist_lseg3D_path3D(lseg, path));
}

PG_FUNCTION_INFO_V1(distance_lseg3D_path3D);

double
dist_lseg3D_path3D(Lseg3D *lseg, Path3D *path)
{
	Point3D		pa,
				pb;
				
	shortest_segm_lseg3D_path3D(lseg, path, &pa, &pb);
	return dist_point3D_point3D(&pa, &pb);
}

/* 
 * Distance between the Lseg3D and the Polygon3D
 */
Datum
distance_lseg3D_poly3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_FLOAT8(dist_lseg3D_poly3D(lseg, poly));
}

PG_FUNCTION_INFO_V1(distance_lseg3D_poly3D);

double
dist_lseg3D_poly3D(Lseg3D *lseg, Polygon3D *poly)
{
	Point3D		pa,
				pb;
				
	shortest_segm_lseg3D_poly3D(lseg, poly, &pa, &pb);
	return dist_point3D_point3D(&pa, &pb);
}

/*
 * Distance between the Lseg3D and the Sphere
 */
Datum
distance_lseg3D_sphere(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_FLOAT8(dist_lseg3D_sphere(lseg, sphere));
}

PG_FUNCTION_INFO_V1(distance_lseg3D_sphere);

double
dist_lseg3D_sphere(Lseg3D *lseg, Sphere *sphere)
{
	double		result;

	/* calculate distance to center, and subtract radius */
	result = dist_point3D_lseg3D(&sphere->center, lseg) - sphere->radius;
	if (FPlt(result, 0))
		result = 0;

	return result;
}

/*----------------------------------------------------------*/

/* line3D distance TYPE3D */

/*
 * Distance between the Line3D and the Point3D
 */
Datum
distance_line3D_point3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_FLOAT8(dist_point3D_line3D(point, line));
}

PG_FUNCTION_INFO_V1(distance_line3D_point3D);

double
dist_line3D_point3D(Line3D *line, Point3D *point)
{
	return dist_point3D_line3D(point, line);
}

/*
 * Distance between the Line3D and the Lseg3D
 */
Datum
distance_line3D_lseg3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_FLOAT8(dist_lseg3D_line3D(lseg, line));
}

PG_FUNCTION_INFO_V1(distance_line3D_lseg3D);

double
dist_line3D_lseg3D(Line3D *line, Lseg3D *lseg)
{
	return dist_lseg3D_line3D(lseg, line);
}

/* 
 * Distance between the two Line3D
 */
Datum
distance_line3D_line3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line1 = PG_GETARG_LINE3D_P(0);
	Line3D	   *line2 = PG_GETARG_LINE3D_P(1);

	PG_RETURN_FLOAT8(dist_line3D_line3D(line1, line2));
}

PG_FUNCTION_INFO_V1(distance_line3D_line3D);

double
dist_line3D_line3D(Line3D *line1, Line3D *line2)
{
	Point3D		pa,
				pb;

	shortest_segm_line3D_line3D(line1, line2, &pa, &pb);
	return dist_point3D_point3D(&pa, &pb);
}

/*
 * Distance between the Line3D and the Box3D
 */
Datum
distance_line3D_box3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_FLOAT8(dist_line3D_box3D(line, box));
}

PG_FUNCTION_INFO_V1(distance_line3D_box3D);

double
dist_line3D_box3D(Line3D *line, Box3D *box)
{
	Point3D		pa,
				pb;

	shortest_segm_line3D_box3D(line, box, &pa, &pb);
	return dist_point3D_point3D(&pa, &pb);
}

/* 
 * Distance between the Line3D and the Path3D
 */
Datum
distance_line3D_path3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_FLOAT8(dist_line3D_path3D(line, path));
}

PG_FUNCTION_INFO_V1(distance_line3D_path3D);

double
dist_line3D_path3D(Line3D *line, Path3D *path)
{
	Point3D		pa,
				pb;
				
	shortest_segm_line3D_path3D(line, path, &pa, &pb);
	return dist_point3D_point3D(&pa, &pb);
}

/* 
 * Distance between the Line3D and the Polygon3D
 */
Datum
distance_line3D_poly3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_FLOAT8(dist_line3D_poly3D(line, poly));
}

PG_FUNCTION_INFO_V1(distance_line3D_poly3D);

double
dist_line3D_poly3D(Line3D *line, Polygon3D *poly)
{
	Point3D		pa,
				pb;
				
	shortest_segm_line3D_poly3D(line, poly, &pa, &pb);
	return dist_point3D_point3D(&pa, &pb);
}

/*
 * Distance between the Line3D and the Sphere
 */
Datum
distance_line3D_sphere(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_FLOAT8(dist_line3D_sphere(line, sphere));
}

PG_FUNCTION_INFO_V1(distance_line3D_sphere);

double
dist_line3D_sphere(Line3D *line, Sphere *sphere)
{
	double		result;

	/* calculate distance to center, and subtract radius */
	result = dist_point3D_line3D(&sphere->center, line) - sphere->radius;
	if (FPlt(result, 0))
		result = 0;

	return result;
}

/*----------------------------------------------------------*/

/* Box3D distance TYPE3D */

/*
 * Distance between the Box3D and the Point3D
 */
Datum
distance_box3D_point3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_FLOAT8(dist_point3D_box3D(point, box));
}

PG_FUNCTION_INFO_V1(distance_box3D_point3D);

double
dist_box3D_point3D(Box3D *box, Point3D *point)
{
	return dist_point3D_box3D(point, box);
}

/*
 * Distance between the Box3D and the Lseg3D
 */
Datum
distance_box3D_lseg3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_FLOAT8(dist_lseg3D_box3D(lseg, box));
}

PG_FUNCTION_INFO_V1(distance_box3D_lseg3D);

double
dist_box3D_lseg3D(Box3D *box, Lseg3D *lseg)
{
	return dist_lseg3D_box3D(lseg, box);
}

/*
 * Distance between the Box3D and the Line3D
 */
Datum
distance_box3D_line3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_FLOAT8(dist_line3D_box3D(line, box));
}

PG_FUNCTION_INFO_V1(distance_box3D_line3D);

double
dist_box3D_line3D(Box3D *box, Line3D *line)
{
	return dist_line3D_box3D(line, box);
}

/*
 * Distance between the two Box3D
 */
 
Datum
distance_box3D_box3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box1 = PG_GETARG_BOX3D_P(0);
	Box3D	   *box2 = PG_GETARG_BOX3D_P(1);

	PG_RETURN_FLOAT8(dist_box3D_box3D(box1, box2));
}

PG_FUNCTION_INFO_V1(distance_box3D_box3D);
 
// Inspired from 
// https://www.gamedev.net/topic/568111-any-quick-way-to-calculate-min-distance-between-2-bounding-boxes/
double
dist_box3D_box3D(Box3D *box1, Box3D *box2)
{
	double		sqrDist = 0;
	double		d;
	
	if (overlap_box3D_box3D_internal(box1, box2))
		return 0.0;
	
	// X axis
	if(box1->high.x < box2->low.x)
	{
		d = box1->high.x - box2->low.x;
		sqrDist += d * d;
	}
	else if(box1->low.x > box2->high.x)
	{
		d = box1->low.x - box2->high.x;
		sqrDist += d * d;
	}
	// Y axis
	if(box1->high.y < box2->low.y)
	{
		d = box1->high.y - box2->low.y;
		sqrDist += d * d;
	}
	else if(box1->low.y > box2->high.y)
	{
		d = box1->low.y - box2->high.y;
		sqrDist += d * d;
	}
	// Z axis
	if(box1->high.z < box2->low.z)
	{
		d = box1->high.z - box2->low.z;
		sqrDist += d * d;
	}
	else if(box1->low.z > box2->high.z)
	{
		d = box1->low.z - box2->high.z;
		sqrDist += d * d;
	}

	return sqrt(sqrDist);
}

/* 
 * Distance between the Box3D and the Path3D
 */
Datum
distance_box3D_path3D(PG_FUNCTION_ARGS)
{
	Box3D	   *b = PG_GETARG_BOX3D_P(0);
	Path3D	   *p = PG_GETARG_PATH3D_P(1);
	double		result;

	result = dist_box3D_path3D(b, p);
	if (result == DBL_MAX)
		PG_RETURN_NULL();

	PG_RETURN_FLOAT8(result);
}

PG_FUNCTION_INFO_V1(distance_box3D_path3D);

double
dist_box3D_path3D(Box3D *box, Path3D *path)
{
	Point3D		pa,
				pb;
				
	shortest_segm_box3D_path3D(box, path, &pa, &pb);
	return dist_point3D_point3D(&pa, &pb);
}

/* 
 * Distance between the Box3D and the Polygon3D
 */
Datum
distance_box3D_poly3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_FLOAT8(dist_box3D_poly3D(box, poly));
}

PG_FUNCTION_INFO_V1(distance_box3D_poly3D);

double
dist_box3D_poly3D(Box3D *box, Polygon3D *poly)
{
	Point3D		pa,
				pb;
				
	shortest_segm_box3D_poly3D(box, poly, &pa, &pb);
	return dist_point3D_point3D(&pa, &pb);
}

/*
 * Distance between the Box3D and the Sphere
 */
Datum
distance_box3D_sphere(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_FLOAT8(dist_box3D_sphere(box, sphere));
}

PG_FUNCTION_INFO_V1(distance_box3D_sphere);

double
dist_box3D_sphere(Box3D *box, Sphere *sphere)
{
	double		result;

	/* calculate distance to center, and subtract radius */
	result = dist_point3D_box3D(&sphere->center, box) - sphere->radius;
	if (FPlt(result, 0))
		result = 0;

	return result;
}

/*----------------------------------------------------------*/

/* path3D distance TYPE3D */

/*
 * Distance between the Path3D and the Point3D
 */
Datum
distance_path3D_point3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_FLOAT8(dist_point3D_path3D(point, path));
}

PG_FUNCTION_INFO_V1(distance_path3D_point3D);

double
dist_path3D_point3D(Path3D *path, Point3D *point)
{
	return dist_point3D_path3D(point, path);
}

/*
 * Distance between the Path3D and the Lseg3D
 */
Datum
distance_path3D_lseg3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_FLOAT8(dist_lseg3D_path3D(lseg, path));
}

PG_FUNCTION_INFO_V1(distance_path3D_lseg3D);

double
dist_path3D_lseg3D(Path3D *path, Lseg3D *lseg)
{
	return dist_lseg3D_path3D(lseg, path);
}

/*
 * Distance between the Path3D and the Line3D
 */
Datum
distance_path3D_line3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_FLOAT8(dist_line3D_path3D(line, path));
}

PG_FUNCTION_INFO_V1(distance_path3D_line3D);

double
dist_path3D_line3D(Path3D *path, Line3D *line)
{
	return dist_line3D_path3D(line, path);
}

/*
 * Distance between the Path3D and the Box3D
 */
Datum
distance_path3D_box3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_FLOAT8(dist_box3D_path3D(box, path));
}

PG_FUNCTION_INFO_V1(distance_path3D_box3D);

double
dist_path3D_box3D(Path3D *path, Box3D *box)
{
	return dist_box3D_path3D(box, path);
}

/* 
 * Distance between the two Path3D
 */
Datum
distance_path3D_path3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path1 = PG_GETARG_PATH3D_P(0);
	Path3D	   *path2 = PG_GETARG_PATH3D_P(1);

	PG_RETURN_FLOAT8(dist_path3D_path3D(path1, path2));
}

PG_FUNCTION_INFO_V1(distance_path3D_path3D);

double
dist_path3D_path3D(Path3D *path1, Path3D *path2)
{
	Point3D		pa,
				pb;
				
	shortest_segm_path3D_path3D(path1, path2, &pa, &pb);
	return dist_point3D_point3D(&pa, &pb);}

/* 
 * Distance between the Path3D and the Polygon3D
 */
Datum
distance_path3D_poly3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	
	PG_RETURN_FLOAT8(dist_path3D_poly3D(path, poly));
}

PG_FUNCTION_INFO_V1(distance_path3D_poly3D);

double
dist_path3D_poly3D(Path3D *path, Polygon3D  *poly)
{
	Point3D		pa,
				pb;
				
	shortest_segm_path3D_poly3D(path, poly, &pa, &pb);
	return dist_point3D_point3D(&pa, &pb);
}

/*
 * Distance between the Path3D and the Sphere
 */
Datum
distance_path3D_sphere(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_FLOAT8(dist_path3D_sphere(path, sphere));
}

PG_FUNCTION_INFO_V1(distance_path3D_sphere);

double
dist_path3D_sphere(Path3D *path, Sphere *sphere)
{
	double		result;

	/* calculate distance to center, and subtract radius */
	result = dist_point3D_path3D(&sphere->center, path) - sphere->radius;
	if (FPlt(result, 0))
		result = 0;

	return result;
}

/*----------------------------------------------------------*/

/* poly3D distance TYPE3D */

/*
 * Distance between the Polygon3D and the Point3D
 */
Datum
distance_poly3D_point3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_FLOAT8(dist_point3D_poly3D(point, poly));
}

PG_FUNCTION_INFO_V1(distance_poly3D_point3D);

double
dist_poly3D_point3D(Polygon3D *poly, Point3D *point)
{
	return dist_point3D_poly3D(point, poly);
}

/*
 * Distance between the Polygon3D and the Lseg3D
 */
Datum
distance_poly3D_lseg3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_FLOAT8(dist_lseg3D_poly3D(lseg, poly));
}

PG_FUNCTION_INFO_V1(distance_poly3D_lseg3D);

double
dist_poly3D_lseg3D(Polygon3D *poly, Lseg3D *lseg)
{
	return dist_lseg3D_poly3D(lseg, poly);
}

/* 
 * Distance between the Polygon3D and the Line3D
 */
Datum
distance_poly3D_line3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_FLOAT8(dist_line3D_poly3D(line, poly));
}

PG_FUNCTION_INFO_V1(distance_poly3D_line3D);

double
dist_poly3D_line3D(Polygon3D *poly, Line3D *line)
{
	return dist_line3D_poly3D(line, poly);
}

/*
 * Distance between the Polygon3D and the Box3D
 */
Datum
distance_poly3D_box3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_FLOAT8(dist_box3D_poly3D(box, poly));
}

PG_FUNCTION_INFO_V1(distance_poly3D_box3D);

double
dist_poly3D_box3D(Polygon3D *poly, Box3D *box)
{
	return dist_box3D_poly3D(box, poly);
}

/*
 * Distance between the Polygon3D and the Path3D
 */
Datum
distance_poly3D_path3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_FLOAT8(dist_path3D_poly3D(path, poly));
}

PG_FUNCTION_INFO_V1(distance_poly3D_path3D);

double
dist_poly3D_path3D(Polygon3D *poly, Path3D *path)
{
	return dist_path3D_poly3D(path, poly);
}

/* 
 * Distance between the two Polygon3D 
 */
Datum
distance_poly3D_poly3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly1 = PG_GETARG_POLYGON3D_P(0);
	Polygon3D  *poly2 = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_FLOAT8(dist_poly3D_poly3D(poly1, poly2));
}

PG_FUNCTION_INFO_V1(distance_poly3D_poly3D);

double
dist_poly3D_poly3D(Polygon3D *poly1, Polygon3D *poly2)
{
	Point3D		pa,
				pb;
				
	shortest_segm_poly3D_poly3D(poly1, poly2, &pa, &pb);
	return dist_point3D_point3D(&pa, &pb);
}

/*
 * Distance between the Polygon3D and the Sphere
 */
Datum
distance_poly3D_sphere(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_FLOAT8(dist_poly3D_sphere(poly, sphere));
}

PG_FUNCTION_INFO_V1(distance_poly3D_sphere);

double
dist_poly3D_sphere(Polygon3D *poly, Sphere *sphere)
{
	double		result;

	/* calculate distance to center, and subtract radius */
	result = dist_point3D_poly3D(&sphere->center, poly) - sphere->radius;
	if (FPlt(result, 0))
		result = 0;

	return result;
}

/*----------------------------------------------------------*/

/* sphere distance TYPE3D */

/*
 * Distance between the Sphere and the Point3D
 */
Datum
distance_sphere_point3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_FLOAT8(dist_point3D_sphere(point, sphere));
}

PG_FUNCTION_INFO_V1(distance_sphere_point3D);

double
dist_sphere_point3D(Sphere *sphere, Point3D *point)
{
	return dist_point3D_sphere(point, sphere);
}

/*
 * Distance between the Sphere and the Lseg3D
 */
Datum
distance_sphere_lseg3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_FLOAT8(dist_lseg3D_sphere(lseg, sphere));
}

double
dist_sphere_lseg3D(Sphere *sphere, Lseg3D *lseg)
{
	return dist_lseg3D_sphere(lseg, sphere);
}

PG_FUNCTION_INFO_V1(distance_sphere_lseg3D);

/*
 * Distance between the Sphere and the Line3D
 */
Datum
distance_sphere_line3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_FLOAT8(dist_line3D_sphere(line, sphere));
}

PG_FUNCTION_INFO_V1(distance_sphere_line3D);

double
dist_sphere_line3D(Sphere *sphere, Line3D *line)
{
	return dist_line3D_sphere(line, sphere);
}

/*
 * Distance between the Sphere and the Box3D
 */
Datum
distance_sphere_box3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_FLOAT8(dist_box3D_sphere(box, sphere));
}

PG_FUNCTION_INFO_V1(distance_sphere_box3D);

double
dist_sphere_box3D(Sphere *sphere, Box3D *box)
{
	return dist_box3D_sphere(box, sphere);
}

/*
 * Distance between the Sphere and the Path3D
 */
Datum
distance_sphere_path3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_FLOAT8(dist_path3D_sphere(path, sphere));
}

PG_FUNCTION_INFO_V1(distance_sphere_path3D);

double
dist_sphere_path3D(Sphere *sphere, Path3D *path)
{
	return dist_path3D_sphere(path, sphere);
}

/*
 * Distance between the Sphere and the Polygon3D
 */
Datum
distance_sphere_poly3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_FLOAT8(dist_poly3D_sphere(poly, sphere));
}

PG_FUNCTION_INFO_V1(distance_sphere_poly3D);

double
dist_sphere_poly3D(Sphere *sphere, Polygon3D *poly)
{
	return dist_poly3D_sphere(poly, sphere);
}

/*
 * Distance between two Sphere
 */
Datum
distance_sphere_sphere(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere1 = PG_GETARG_SPHERE_P(0);
	Sphere	   *sphere2 = PG_GETARG_SPHERE_P(1);

	PG_RETURN_FLOAT8(dist_sphere_sphere(sphere1, sphere2));
}

PG_FUNCTION_INFO_V1(distance_sphere_sphere);

double
dist_sphere_sphere(Sphere *sphere1, Sphere *sphere2)
{
	double		result;

	result = dist_point3D_point3D(&sphere1->center, &sphere2->center)
		- (sphere1->radius + sphere2->radius);
	if (FPlt(result, 0))
		result = 0;
	return result;
}

/*****************************************************************************
 * Closestpoint functions
 * Find the point of closest proximity between objects
 * If there are several closest points between two objects (e.g. when  
 * two line segments are parallel or overlap) then one of them is returned
 *****************************************************************************/

/* closestpoint Point3D TYPE3D */

/*	
 * Point of the Lseg3D closest to the Point3D
 */
Datum
closestpoint_point3D_lseg3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	
	PG_RETURN_POINT3D_P(closestpt_point3D_lseg3D(point, lseg));
}

PG_FUNCTION_INFO_V1(closestpoint_point3D_lseg3D) ;

/*	
 *	Find the intersection point of a perpendicular of the line segment through 
 *  the point if exists. Otherwise return the closest end point of the segment
 * 	Based on http://paulbourke.net/geometry/pointlineplane/
 */
Point3D *
closestpt_point3D_lseg3D(Point3D *point, Lseg3D *lseg)
{
    double		magn2,
				k;
	Point3D		u,
			   *result = (Point3D *) palloc(sizeof(Point3D));
			
	u = sub(lseg->p[1], lseg->p[0]);
	magn2 = DOT(u, u); // square of magnitude(u)
 
	k = DOT(sub(*point, lseg->p[0]), u) / magn2;
 
    if (FPlt(k, 0.0d)) 
        // Closest point is to the "left" of the line segment
		*result = lseg->p[0];
    else if (FPgt(k, 1.0d)) 
        // Closest to the "right" of the line segment
		*result = lseg->p[1];
	else
		*result = add(lseg->p[0], scalar(k, u));
	return result;
}

/* 
 * Point of the Line3D closest to the Point3D
 */
Datum
closestpoint_point3D_line3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_point3D_line3D(point, line));
}

PG_FUNCTION_INFO_V1(closestpoint_point3D_line3D) ;

// The intersection point of a perpendicular of the line through the point. 
// Based on http://paulbourke.net/geometry/pointlineplane/
Point3D *
closestpt_point3D_line3D(Point3D *point, Line3D *line)
{
    double		magn2,
				k;
	Point3D		u,
			   *result = (Point3D *) palloc(sizeof(Point3D));

	u = sub(line->p[1], line->p[0]);
	magn2 = DOT(u, u); // square of magnitude(u)
	
    k = DOT(sub(*point, line->p[0]), u) / magn2;
 
	*result = add(line->p[0], scalar(k, u));
	return result;
}

/* 
 * Point of the Box3D closest to the Point3D
 */
Datum
closestpoint_point3D_box3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_point3D_box3D(point, box));
}

PG_FUNCTION_INFO_V1(closestpoint_point3D_box3D);

/* 
 * Inspired from 
 * http://stackoverflow.com/questions/5254838/calculating-distance-between-a-point-and-a-rectangular-box-nearest-point
 */
double
closestpt_aux(double p, double lower, double upper)
{
	if (FPlt(p, lower))
		return lower;
	if (FPgt(p, upper))
		return upper;
	return p;
}

Point3D *
closestpt_point3D_box3D(Point3D *point, Box3D *box) 
{
	Point3D		   *result = (Point3D *) palloc(sizeof(Point3D));	
	
	if (FPle(point->x, box->high.x) && FPge(point->x, box->low.x) &&
		FPle(point->y, box->high.y) && FPge(point->y, box->low.y) &&
		FPle(point->z, box->high.z) && FPge(point->z, box->low.z))
	{
		// point is inside the box
		*result = *point;
		return result;	
	}
	else
	{
		result->x = closestpt_aux(point->x, box->low.x, box->high.x);
		result->y = closestpt_aux(point->y, box->low.y, box->high.y);
		result->z = closestpt_aux(point->z, box->low.z, box->high.z);
		return result;
	}
}

/*
 * Point of the Path3D closest to the Point3D
 */
Datum
closestpoint_point3D_path3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_point3D_path3D(point, path));		
}

PG_FUNCTION_INFO_V1(closestpoint_point3D_path3D);

Point3D *
closestpt_point3D_path3D(Point3D *point, Path3D *path)
{
	Point3D	   *closest,
			   *result = (Point3D *) palloc(sizeof(Point3D));
	double		distance,
				min_distance = DBL_MAX;	/* keep compiler quiet */
	Lseg3D		lseg;

	if (path->npts == 1)
	{
		/* one point in path? then return the point */
		*result = path->p[0];
		return result;
	}

	/*
	 * the closest point from a point to a path is the closest point
	 * from the point to any of its constituent segments.
	 */
	for (int i = 0; i < path->npts; i++)
	{
		int			iprev;

		if (i > 0)
			iprev = i - 1;
		else
		{
			if (!path->closed)
				continue;
			iprev = path->npts - 1;		/* include the closure segment */
		}

		lseg3D_construct_pts_stat(&lseg, path->p[iprev], path->p[i]);
		closest = closestpt_point3D_lseg3D(point, &lseg);
		distance = dist_point3D_point3D(point, closest);
		if (FPlt(distance, min_distance))
		{
			*result = *closest;
			min_distance = distance;
		}
		pfree(closest);
	}
	return result;
}

 /*
 * Point of the Polygon3D closest to the Point3D
 */
Datum
closestpoint_point3D_poly3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_point3D_poly3D(point, poly));
}

PG_FUNCTION_INFO_V1(closestpoint_point3D_poly3D);

/*
 * If the line and the plane of the polygon are not coplanar find
 * the point where the line and the plane intersect as defined in 
 * http://www.ambrsoft.com/TrigoCalc/Plan3D/PlaneLineIntersection_.htm
 * If the intersection point is inside the polygon then the result is
 * the intersection point. Otherwise find the closest point
 * between the line and the path of the polygon
*/
Point3D *
closestpt_point3D_poly3D(Point3D *point, Polygon3D *poly)
{
	Point3D	   *result;
	Path3D	   *path;

	if (contain_poly3D_point3D_internal(poly, point))
		return point;
	
	if (!coplanar_poly3D_point3D_internal(poly, point))
	{
		double		A,
					B,
					C,
					D,
					numerator,
					denominator;
		Point3D		interpoint;

		poly3D_plane(poly, &A, &B, &C, &D);
		numerator = ( A * point->x + B * point->y + C * point->z + D );
		denominator = A * A + B * B + C * C;
		interpoint.x = point->x - ( A * numerator / denominator );
		interpoint.y = point->y - ( B * numerator / denominator );
		interpoint.z = point->z - ( C * numerator / denominator );

		if (contain_poly3D_point3D_internal(poly, &interpoint))
		{
			// The point is perpendicular to the polygon
			result = (Point3D *) palloc(sizeof(Point3D));
			*result = interpoint;
			return result;
		}
	}
	
	// The point and the polygon are coplanar or the point is not perpendicular to the polygon
	// Compute the closest point between the point and the path of the polygon
	path = poly3D_path3D_internal(poly);
	result = closestpt_point3D_path3D(point, path);
	pfree(path);
	
	return result;
}

/* 
 * Point of the Sphere closest to the Point3D
 */
Datum
closestpoint_point3D_sphere(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_POINT3D_P(closestpt_point3D_sphere(point, sphere));
}

PG_FUNCTION_INFO_V1(closestpoint_point3D_sphere);

/* 
 * Inspired from 
 * http://stackoverflow.com/questions/24173889/closest-point-on-a-sphere-to-another-point
 */
Point3D *
closestpt_point3D_sphere(Point3D *point, Sphere *sphere) 
{
	Point3D			v,
				   *result = (Point3D *) palloc(sizeof(Point3D));
	double 			magn;
	
	v = sub(*point, sphere->center); 
	magn = magnitude(v);
	v = scalar(1/magn, v); 
	v = scalar(sphere->radius, v);
	*result = add(v, sphere->center);
	return result;
}
/*----------------------------------------------------------*/

/* closestpoint Lseg3D TYPE3D */

/* 
 * Point of the second Lseg3D closest to the first Lseg3D
 */
Datum
closestpoint_lseg3D_lseg3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D	   *lseg2 = PG_GETARG_LSEG3D_P(1);
	
	PG_RETURN_POINT3D_P(closestpt_lseg3D_lseg3D(lseg1, lseg2));		
}

PG_FUNCTION_INFO_V1(closestpoint_lseg3D_lseg3D) ;

Point3D *
closestpt_lseg3D_lseg3D(Lseg3D *lseg1, Lseg3D *lseg2)
{
	Point3D		pa,
			   *result = (Point3D *) palloc(sizeof(Point3D));
			   
	shortest_segm_lseg3D_lseg3D(lseg1, lseg2, &pa, result);
	return result;			
}

/* 
 * Point of the Line3D closest to the Lseg3D
 */
Datum
closestpoint_lseg3D_line3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_lseg3D_line3D(lseg, line));
}

PG_FUNCTION_INFO_V1(closestpoint_lseg3D_line3D) ;

Point3D *
closestpt_lseg3D_line3D(Lseg3D *lseg, Line3D *line)
{
	Point3D		pa,
			   *result = (Point3D *) palloc(sizeof(Point3D));
			   
	shortest_segm_lseg3D_line3D(lseg, line, &pa, result);
	return result;			
}

/* 
 * Point of the Box3D closest to the Lseg3D
 */
Datum
closestpoint_lseg3D_box3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
 
	PG_RETURN_POINT3D_P(closestpt_lseg3D_box3D(lseg, box));	
}

PG_FUNCTION_INFO_V1(closestpoint_lseg3D_box3D) ;

Point3D *
closestpt_lseg3D_box3D(Lseg3D *lseg, Box3D *box)
{
	Point3D		pa,
			   *result = (Point3D *) palloc(sizeof(Point3D));

	shortest_segm_lseg3D_box3D(lseg, box, &pa, result);
	return result;
}

/*
 * Point of the Path3D closest to the Lseg3D
 */
Datum
closestpoint_lseg3D_path3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	
	PG_RETURN_POINT3D_P(closestpt_lseg3D_path3D(lseg, path));		
}

PG_FUNCTION_INFO_V1(closestpoint_lseg3D_path3D);

Point3D *
closestpt_lseg3D_path3D(Lseg3D *lseg, Path3D *path)
{
	Point3D		pa,
			   *result = (Point3D *) palloc(sizeof(Point3D));

	shortest_segm_lseg3D_path3D(lseg, path, &pa, result);
	return result;
}

/*
 * Point of the Polygon3D closest to the Lseg3D
 */
Datum
closestpoint_lseg3D_poly3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	
	PG_RETURN_POINT3D_P(closestpt_lseg3D_poly3D(lseg, poly));		
}

PG_FUNCTION_INFO_V1(closestpoint_lseg3D_poly3D);

Point3D *
closestpt_lseg3D_poly3D(Lseg3D *lseg, Polygon3D *poly)
{
	Point3D		pa,
			   *result = (Point3D *) palloc(sizeof(Point3D));

	shortest_segm_lseg3D_poly3D(lseg, poly, &pa, result);
	return result;
}

/*
 * Point of the Sphere closest to the Lseg3D
 */
Datum
closestpoint_lseg3D_sphere(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_POINT3D_P(closestpt_lseg3D_sphere(lseg, sphere));
}

PG_FUNCTION_INFO_V1(closestpoint_lseg3D_sphere) ;

Point3D *
closestpt_lseg3D_sphere(Lseg3D *lseg, Sphere *sphere)
{
	Point3D		pa,
			   *result = (Point3D *) palloc(sizeof(Point3D));

	shortest_segm_lseg3D_sphere(lseg, sphere, &pa, result);
	return result;
}

/*----------------------------------------------------------*/

/* closestpoint line3D TYPE3D */

/*
 * Point of the Lseg3D closest to the Line3D
 */
Datum
closestpoint_line3D_lseg3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_lseg3D_line3D(lseg, line));
}

PG_FUNCTION_INFO_V1(closestpoint_line3D_lseg3D) ;

Point3D *
closestpt_line3D_lseg3D(Line3D *line, Lseg3D *lseg)
{
	return closestpt_lseg3D_line3D(lseg, line);
}

/*
 * Point of the second Line3D closest to the first Line3D
 */
Datum
closestpoint_line3D_line3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line1 = PG_GETARG_LINE3D_P(0);
	Line3D	   *line2 = PG_GETARG_LINE3D_P(1);
	Point3D	   *result;

	PG_RETURN_POINT3D_P(closestpt_line3D_line3D(line1, line2));
}

PG_FUNCTION_INFO_V1(closestpoint_line3D_line3D);

Point3D *
closestpt_line3D_line3D(Line3D *line1, Line3D *line2)
{
	Point3D		pa,
			   *result = (Point3D *) palloc(sizeof(Point3D));

	shortest_segm_line3D_line3D(line1, line2, &pa, result);
	return result;
}

/*
 * Point of the Box3D closest to the Line3D
 */
Datum
closestpoint_line3D_box3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_line3D_box3D(line, box));
}

PG_FUNCTION_INFO_V1(closestpoint_line3D_box3D) ;

Point3D *
closestpt_line3D_box3D(Line3D *line, Box3D *box)
{
	Point3D		pa,
			   *result = (Point3D *) palloc(sizeof(Point3D));

	shortest_segm_line3D_box3D(line, box, &pa, result);
	return result;
}

/*
 * Point of the Path3D closest to the Line3D
 */
Datum
closestpoint_line3D_path3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_line3D_path3D(line, path));
}

PG_FUNCTION_INFO_V1(closestpoint_line3D_path3D) ;

Point3D *
closestpt_line3D_path3D(Line3D *line, Path3D *path)
{
	Point3D		pa,
			   *result = (Point3D *) palloc(sizeof(Point3D));

	shortest_segm_line3D_path3D(line, path, &pa, result);
	return result;
}

/*
 * Point of the Polygon3D closest to the Line3D
 */
Datum
closestpoint_line3D_poly3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_line3D_poly3D(line, poly));
}

PG_FUNCTION_INFO_V1(closestpoint_line3D_poly3D) ;

Point3D *
closestpt_line3D_poly3D(Line3D *line, Polygon3D *poly)
{
	Point3D		pa,
			   *result = (Point3D *) palloc(sizeof(Point3D));

	shortest_segm_line3D_poly3D(line, poly, &pa, result);
	return result;
}

/*
 * Point of the Sphere closest to the Line3D
 */
Datum
closestpoint_line3D_sphere(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_POINT3D_P(closestpt_line3D_sphere(line, sphere));
}

PG_FUNCTION_INFO_V1(closestpoint_line3D_sphere) ;

Point3D *
closestpt_line3D_sphere(Line3D *line, Sphere *sphere)
{
	Point3D		pa,
			   *result = (Point3D *) palloc(sizeof(Point3D));

	shortest_segm_line3D_sphere(line, sphere, &pa, result);
	return result;
}

/*----------------------------------------------------------*/

/* closestpoint Box3D TYPE3D */

/*
 * Point of the Lseg3D closest to the Box3D
 */
Datum
closestpoint_box3D_lseg3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_lseg3D_box3D(lseg, box));
}

PG_FUNCTION_INFO_V1(closestpoint_box3D_lseg3D) ;

Point3D *
closestpt_box3D_lseg3D(Box3D *box, Lseg3D *lseg)
{
	return closestpt_lseg3D_box3D(lseg, box);
}

/*
 * Point of the Line3D closest to the Box3D
 */
Datum
closestpoint_box3D_line3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_line3D_box3D(line, box));
}

PG_FUNCTION_INFO_V1(closestpoint_box3D_line3D) ;

Point3D *
closestpt_box3D_line3D(Box3D *box, Line3D *line)
{
	return closestpt_line3D_box3D(line, box);
}

/*
 * Point of the second Box3D closest to the first Box3D
 */
Datum
closestpoint_box3D_box3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box1 = PG_GETARG_BOX3D_P(0);
	Box3D	   *box2 = PG_GETARG_BOX3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_box3D_box3D(box1, box2));
}

PG_FUNCTION_INFO_V1(closestpoint_box3D_box3D) ;

Point3D *
closestpt_box3D_box3D(Box3D *box1, Box3D *box2)
{
	Point3D		pa,
			   *result = (Point3D *) palloc(sizeof(Point3D));

	shortest_segm_box3D_box3D(box1, box2, &pa, result);
	return result;
}

/*
 * Point of the Path3D closest to the Box3D
 */
Datum
closestpoint_box3D_path3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_box3D_path3D(box, path));
}

PG_FUNCTION_INFO_V1(closestpoint_box3D_path3D) ;

Point3D *
closestpt_box3D_path3D(Box3D *box, Path3D *path)
{
	Point3D		pa,
			   *result = (Point3D *) palloc(sizeof(Point3D));

	shortest_segm_box3D_path3D(box, path, &pa, result);
	return result;
}

/*
 * Point of the Polygon3D closest to the Box3D
 */
Datum
closestpoint_box3D_poly3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_box3D_poly3D(box, poly));
}

PG_FUNCTION_INFO_V1(closestpoint_box3D_poly3D) ;

Point3D *
closestpt_box3D_poly3D(Box3D *box, Polygon3D *poly)
{
	Point3D		pa,
			   *result = (Point3D *) palloc(sizeof(Point3D));

	shortest_segm_box3D_poly3D(box, poly, &pa, result);
	return result;
}

/*
 * Point of the Sphere closest to the Box3D
 */
Datum
closestpoint_box3D_sphere(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_POINT3D_P(closestpt_box3D_sphere(box, sphere));
}

PG_FUNCTION_INFO_V1(closestpoint_box3D_sphere) ;

Point3D *
closestpt_box3D_sphere(Box3D *box, Sphere *sphere)
{
	Point3D		pa,
			   *result = (Point3D *) palloc(sizeof(Point3D));

	shortest_segm_box3D_sphere(box, sphere, &pa, result);
	return result;
}

/*----------------------------------------------------------*/

/* closestpoint path3D TYPE3D */

/*
 * Point of the Lseg3D closest to the Path3D
 */
Datum
closestpoint_path3D_lseg3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_lseg3D_path3D(lseg, path));
}

PG_FUNCTION_INFO_V1(closestpoint_path3D_lseg3D) ;

Point3D *
closestpt_path3D_lseg3D(Path3D *path, Lseg3D *lseg)
{
	return closestpt_lseg3D_path3D(lseg, path);
}

/*
 * Point of the Line3D closest to the Path3D
 */
Datum
closestpoint_path3D_line3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_line3D_path3D(line, path));
}

PG_FUNCTION_INFO_V1(closestpoint_path3D_line3D) ;

Point3D *
closestpt_path3D_line3D(Path3D *path, Line3D *line)
{
	return closestpt_line3D_path3D(line, path);
}

/*
 * Point of the Box3D closest to the Path3D
 */
Datum
closestpoint_path3D_box3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_box3D_path3D(box, path));
}

PG_FUNCTION_INFO_V1(closestpoint_path3D_box3D) ;

Point3D *
closestpt_path3D_box3D(Path3D *path, Box3D *box)
{
	return closestpt_box3D_path3D(box, path);
}

/*
 * Point of the Path3D closest to the Path3D
 */
Datum
closestpoint_path3D_path3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path1 = PG_GETARG_PATH3D_P(0);
	Path3D	   *path2 = PG_GETARG_PATH3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_path3D_path3D(path1, path2));
}

PG_FUNCTION_INFO_V1(closestpoint_path3D_path3D) ;

Point3D *
closestpt_path3D_path3D(Path3D *path1, Path3D *path2)
{
	Point3D		pa,
			   *result = (Point3D *) palloc(sizeof(Point3D));

	shortest_segm_path3D_path3D(path1, path2, &pa, result);
	return result;
}

/*
 * Point of the Polygon3D closest to the Path3D
 */
Datum
closestpoint_path3D_poly3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_path3D_poly3D(path, poly));
}

PG_FUNCTION_INFO_V1(closestpoint_path3D_poly3D) ;

Point3D *
closestpt_path3D_poly3D(Path3D *path, Polygon3D *poly)
{
	Point3D		pa,
			   *result = (Point3D *) palloc(sizeof(Point3D));

	shortest_segm_path3D_poly3D(path, poly, &pa, result);
	return result;
}

/*
 * Point of the Sphere closest to the Path3D
 */
Datum
closestpoint_path3D_sphere(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_POINT3D_P(closestpt_path3D_sphere(path, sphere));
}

PG_FUNCTION_INFO_V1(closestpoint_path3D_sphere) ;

Point3D *
closestpt_path3D_sphere(Path3D *path, Sphere *sphere)
{
	Point3D		pa,
			   *result = (Point3D *) palloc(sizeof(Point3D));

	shortest_segm_path3D_sphere(path, sphere, &pa, result);
	return result;
}

/*----------------------------------------------------------*/

/* closestpoint poly3D TYPE3D */

/*
 * Point of the Lseg3D closest to the Polygon3D
 */
Datum
closestpoint_poly3D_lseg3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_lseg3D_poly3D(lseg, poly));
}

PG_FUNCTION_INFO_V1(closestpoint_poly3D_lseg3D) ;

Point3D *
closestpt_poly3D_lseg3D(Polygon3D *poly, Lseg3D *lseg)
{
	return closestpt_lseg3D_poly3D(lseg, poly);
}

/*
 * Point of the Line3D closest to the Polygon3D
 */
Datum
closestpoint_poly3D_line3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_line3D_poly3D(line, poly));
}

PG_FUNCTION_INFO_V1(closestpoint_poly3D_line3D) ;

Point3D *
closestpt_poly3D_line3D(Polygon3D *poly, Line3D *line)
{
	return closestpt_line3D_poly3D(line, poly);
}

/*
 * Point of the Box3D closest to the Polygon3D
 */
Datum
closestpoint_poly3D_box3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_box3D_poly3D(box, poly));
}

PG_FUNCTION_INFO_V1(closestpoint_poly3D_box3D) ;

Point3D *
closestpt_poly3D_box3D(Polygon3D *poly, Box3D *box)
{
	return closestpt_box3D_poly3D(box, poly);
}

/*
 * Point of the Path3D closest to the Polygon3D
 */
Datum
closestpoint_poly3D_path3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_path3D_poly3D(path, poly));
}

PG_FUNCTION_INFO_V1(closestpoint_poly3D_path3D) ;

Point3D *
closestpt_poly3D_path3D(Polygon3D *poly, Path3D *path)
{
	return closestpt_path3D_poly3D(path, poly);
}

/*
 * Point of the second Polygon3D closest to the first Polygon3D
 */
Datum
closestpoint_poly3D_poly3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly1 = PG_GETARG_POLYGON3D_P(0);
	Polygon3D  *poly2 = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_poly3D_poly3D(poly1, poly2));
}

PG_FUNCTION_INFO_V1(closestpoint_poly3D_poly3D) ;

Point3D *
closestpt_poly3D_poly3D(Polygon3D *poly1, Polygon3D *poly2)
{
	Point3D		pa,
			   *result = (Point3D *) palloc(sizeof(Point3D));

	shortest_segm_poly3D_poly3D(poly1, poly2, &pa, result);
	return result;
}

/*
 * Point of the Sphere closest to the Polygon3D
 */
Datum
closestpoint_poly3D_sphere(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_POINT3D_P(closestpt_poly3D_sphere(poly, sphere));
}

PG_FUNCTION_INFO_V1(closestpoint_poly3D_sphere) ;

Point3D *
closestpt_poly3D_sphere(Polygon3D *poly, Sphere *sphere)
{
	Point3D		pa,
			   *result = (Point3D *) palloc(sizeof(Point3D));

	shortest_segm_poly3D_sphere(poly, sphere, &pa, result);
	return result;
}

/*----------------------------------------------------------*/

/* closestpoint sphere TYPE3D */

/*
 * Point of the Lseg3D closest to the Sphere
 */
Datum
closestpoint_sphere_lseg3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_lseg3D_sphere(lseg, sphere));
}

PG_FUNCTION_INFO_V1(closestpoint_sphere_lseg3D) ;

Point3D *
closestpt_sphere_lseg3D(Sphere *sphere, Lseg3D *lseg)
{
	return closestpt_lseg3D_sphere(lseg, sphere);
}

/*
 * Point of the Line3D closest to the Sphere
 */
Datum
closestpoint_sphere_line3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_line3D_sphere(line, sphere));
}

PG_FUNCTION_INFO_V1(closestpoint_sphere_line3D) ;

Point3D *
closestpt_sphere_line3D(Sphere *sphere, Line3D *line)
{
	return closestpt_line3D_sphere(line, sphere);
}

/*
 * Point of the Box3D closest to the Sphere
 */
Datum
closestpoint_sphere_box3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_box3D_sphere(box, sphere));
}

PG_FUNCTION_INFO_V1(closestpoint_sphere_box3D) ;

Point3D *
closestpt_sphere_box3D(Sphere *sphere, Box3D *box)
{
	return closestpt_box3D_sphere(box, sphere);
}

/*
 * Point of the Path3D closest to the Sphere
 */
Datum
closestpoint_sphere_path3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_path3D_sphere(path, sphere));
}

PG_FUNCTION_INFO_V1(closestpoint_sphere_path3D) ;

Point3D *
closestpt_sphere_path3D(Sphere *sphere, Path3D *path)
{
	return closestpt_path3D_sphere(path, sphere);
}

/*
 * Point of the Polygon3D closest to the Sphere
 */
Datum
closestpoint_sphere_poly3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_POINT3D_P(closestpt_poly3D_sphere(poly, sphere));
}

PG_FUNCTION_INFO_V1(closestpoint_sphere_poly3D) ;

Point3D *
closestpt_sphere_poly3D(Sphere *sphere, Polygon3D *poly)
{
	return closestpt_poly3D_sphere(poly, sphere);
}

/*
 * Point of the second Sphere closest to the first Sphere
 */
Datum
closestpoint_sphere_sphere(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere1 = PG_GETARG_SPHERE_P(0);
	Sphere	   *sphere2 = PG_GETARG_SPHERE_P(1);

	PG_RETURN_POINT3D_P(closestpt_sphere_sphere(sphere1, sphere2));
}

PG_FUNCTION_INFO_V1(closestpoint_sphere_sphere) ;

Point3D *
closestpt_sphere_sphere(Sphere *sphere1, Sphere *sphere2)
{
	Point3D		pa,
			   *result = (Point3D *) palloc(sizeof(Point3D));

	shortest_segm_sphere_sphere(sphere1, sphere2, &pa, result);
	return result;
}

/*****************************************************************************/
