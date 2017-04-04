/*****************************************************************************
 *
 * geo3D_decls.h - Declarations for various 3D geometries.
 *
 * Portions Copyright (c) 1996-2017, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * NOTE
 *	  These routines do *not* use the float types from adt/.
 *
 *****************************************************************************/

#ifndef GEO3D_DECLS_H
#define GEO3D_DECLS_H

#include <math.h>

#include <fmgr.h>

/*--------------------------------------------------------------------
 * Useful floating point utilities and constants.
 *-------------------------------------------------------------------*/

#define EPSILON					1.0E-06

#ifdef EPSILON
#define FPzero(A)				(fabs(A) <= EPSILON)
#define FPeq(A,B)				(fabs((A) - (B)) <= EPSILON)
#define FPne(A,B)				(fabs((A) - (B)) > EPSILON)
#define FPlt(A,B)				((B) - (A) > EPSILON)
#define FPle(A,B)				((A) - (B) <= EPSILON)
#define FPgt(A,B)				((A) - (B) > EPSILON)
#define FPge(A,B)				((B) - (A) <= EPSILON)
#else
#define FPzero(A)				((A) == 0)
#define FPeq(A,B)				((A) == (B))
#define FPne(A,B)				((A) != (B))
#define FPlt(A,B)				((A) < (B))
#define FPle(A,B)				((A) <= (B))
#define FPgt(A,B)				((A) > (B))
#define FPge(A,B)				((A) >= (B))
#endif

#define HYPOT3D(A, B, C)		pg_hypot3D(A, B, C)
#define HYPOT(A, B)				pg_hypot(A, B)

#define PT3Deq(pt1, pt2)	(FPeq((pt1).x, (pt2).x) && FPeq((pt1).y, (pt2).y) && FPeq((pt1).z, (pt2).z))
#define PT3Dne(pt1, pt2)	(FPne((pt1).x, (pt2).x) || FPne((pt1).y, (pt2).y) || FPne((pt1).z, (pt2).z))
#define PT3DPeq(pt1, pt2)	(FPeq((pt1)->x, (pt2)->x) && FPeq((pt1)->y, (pt2)->y) && FPeq((pt1)->z, (pt2)->z))

#define DOT(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z * (v).z)

/*--------------------------------------------------------------------
 * Additional strategy numbers for GIST and SP-GIST with respect 
 * to those defined in the file stratnum.h
 *-------------------------------------------------------------------*/

#define RTOverFrontStrategyNumber		28		/* for <</& */
#define RTFrontStrategyNumber			29		/* for <</ */
#define RTBackStrategyNumber			30		/* for />> */
#define RTOverBackStrategyNumber		31		/* for &/>> */

#define Geo3DStrategyNumberOffset		40
#define Point3DStrategyNumberGroup		0
#define Lseg3DStrategyNumberGroup		1
#define Line3DStrategyNumberGroup			2
#define Box3DStrategyNumberGroup		3
#define Path3DStrategyNumberGroup		4
#define Polygon3DStrategyNumberGroup	5
#define SphereStrategyNumberGroup		6

/*---------------------------------------------------------------------
 * Point3D: A 3D point specified by the coordinates (x,y,z)
 *-------------------------------------------------------------------*/
typedef struct
{
	double		x,
				y,
				z;
} Point3D;

Oid point3d_oid() ;

/*---------------------------------------------------------------------
 * Lseg3D: A 3D line segment specified by two end points
 *-------------------------------------------------------------------*/
typedef struct
{
	Point3D		p[2];
} Lseg3D;

/*---------------------------------------------------------------------
 * Line3D: A 3D line specified by two of its points <x0,y0,z0> and <x1,y1,z1>
 * If <a,b,c> = <x1-x0,y1-y0,z1-z0>, its equation in the vector form is
 *		<x,y,z> = <x0,y0,z0> + <a,b,c>t
 *	Its equivalent parametric equations are
 *		x = x0 + at
 *		y = y0 + bt
 *		z = z0 + ct
 *	Its equivalent equation point direction form are
 *		(x-x0)/a = (y-y0)/b = (z-z0)/c
 * or if a, b, or c is 0 (e.g., b) then
 *		(x-x0)/a = (z-z0)/c and y = y0
 *-------------------------------------------------------------------*/
typedef struct
{
	Point3D		p[2];
} Line3D;

/*---------------------------------------------------------------------
 * Box3D: A 3D box specified by two corner points, which are
 * sorted to save calculation time later
 *-------------------------------------------------------------------*/
typedef struct
{
	Point3D		high,
				low;			/* corner points */
} Box3D;

/*---------------------------------------------------------------------
 * Path3D: An open or closed 3D polyline specified by vertex points
 * The number of points and the bounding box are kept for speed purposes
 *-------------------------------------------------------------------*/
typedef struct
{
	int32		vl_len_;		/* varlena header (do not touch directly!) */
	int32		npts;
	int32		closed;			/* is this a closed polygon? */
	int32		dummy;			/* padding to make it double align */
	Box3D		boundbox;
	Point3D		p[FLEXIBLE_ARRAY_MEMBER];
} Path3D;

/*---------------------------------------------------------------------
 * Polygon3D: A 3D polygon specified by the vertex points
 * The number of points and the bounding box are ketp for speed purposes
 *-------------------------------------------------------------------*/
typedef struct
{
	int32		vl_len_;		/* varlena header (do not touch directly!) */
	int32		npts;
	Box3D		boundbox;
	Point3D		p[FLEXIBLE_ARRAY_MEMBER];
} Polygon3D;

/*---------------------------------------------------------------------
 * Sphere: A sphere specified by a center point and a radius
 *-------------------------------------------------------------------*/
typedef struct
{
	Point3D		center;
	double		radius;
} Sphere;

/*---------------------------------------------------------------------
 * fmgr interface macros
 *
 * Path3D and Polygon3D are toastable varlena types, the others are just
 * fixed-size pass-by-reference types.
 *-------------------------------------------------------------------*/

#define DatumGetPoint3DP(X)	 ((Point3D *) DatumGetPointer(X))
#define Point3DPGetDatum(X)	 PointerGetDatum(X)
#define PG_GETARG_POINT3D_P(n) DatumGetPoint3DP(PG_GETARG_DATUM(n))
#define PG_RETURN_POINT3D_P(x) return Point3DPGetDatum(x)

#define DatumGetLseg3DP(X)	((Lseg3D *) DatumGetPointer(X))
#define Lseg3DPGetDatum(X)	PointerGetDatum(X)
#define PG_GETARG_LSEG3D_P(n) DatumGetLseg3DP(PG_GETARG_DATUM(n))
#define PG_RETURN_LSEG3D_P(x) return Lseg3DPGetDatum(x)

#define DatumGetPath3DP(X)				((Path3D *) PG_DETOAST_DATUM(X))
#define DatumGetPath3DPCopy(X)			((Path3D *) PG_DETOAST_DATUM_COPY(X))
#define Path3DPGetDatum(X)				PointerGetDatum(X)
#define PG_GETARG_PATH3D_P(n)			DatumGetPath3DP(PG_GETARG_DATUM(n))
#define PG_GETARG_PATH3D_P_COPY(n)		DatumGetPath3DPCopy(PG_GETARG_DATUM(n))
#define PG_RETURN_PATH3D_P(x)			return Path3DPGetDatum(x)

#define DatumGetLine3DP(X)	((Line3D *) DatumGetPointer(X))
#define Line3DPGetDatum(X)	PointerGetDatum(X)
#define PG_GETARG_LINE3D_P(n) DatumGetLine3DP(PG_GETARG_DATUM(n))
#define PG_RETURN_LINE3D_P(x) return Line3DPGetDatum(x)

#define DatumGetBox3DP(X)    ((Box3D *) DatumGetPointer(X))
#define Box3DPGetDatum(X)    PointerGetDatum(X)
#define PG_GETARG_BOX3D_P(n) DatumGetBox3DP(PG_GETARG_DATUM(n))
#define PG_RETURN_BOX3D_P(x) return Box3DPGetDatum(x)

#define DatumGetPolygon3DP(X)			((Polygon3D *) PG_DETOAST_DATUM(X))
#define DatumGetPolygon3DPCopy(X)		((Polygon3D *) PG_DETOAST_DATUM_COPY(X))
#define Polygon3DPGetDatum(X)			PointerGetDatum(X)
#define PG_GETARG_POLYGON3D_P(n)		DatumGetPolygon3DP(PG_GETARG_DATUM(n))
#define PG_GETARG_POLYGON3D_P_COPY(n)	DatumGetPolygon3DPCopy(PG_GETARG_DATUM(n))
#define PG_RETURN_POLYGON3D_P(x)		return Polygon3DPGetDatum(x)

#define DatumGetSphereP(X)	  ((Sphere *) DatumGetPointer(X))
#define SpherePGetDatum(X)	  PointerGetDatum(X)
#define PG_GETARG_SPHERE_P(n) DatumGetSphereP(PG_GETARG_DATUM(n))
#define PG_RETURN_SPHERE_P(x) return SpherePGetDatum(x)

/*---------------------------------------------------------------------
 * Oid lookups
 *-------------------------------------------------------------------*/

Oid point3d_oid() ;

/*****************************************************************************
 * Generic routines
 *****************************************************************************/

extern Point3D add(Point3D pt1, Point3D pt2);
extern Point3D sub(Point3D pt1, Point3D pt2);
extern Point3D scalar(double k, Point3D pt);
extern Point3D cross(Point3D pt1, Point3D pt2);
extern Point3D absolute(Point3D pt);
extern double magnitude(Point3D pt);
extern bool proportional(Point3D pt1, Point3D pt2);
extern double pg_hypot(double x, double y);
extern double pg_hypot3D(double x, double y, double z);

extern bool plist_test3noncollinearpts(Point3D  *plist, int npts);
extern bool plist_find3noncollinearpts(Point3D  *plist, int npts, int *pos1, int *pos2, int *pos3);

/*****************************************************************************
 * Point3D routines
 *****************************************************************************/

extern Datum point3D_in(PG_FUNCTION_ARGS);
extern Datum point3D_out(PG_FUNCTION_ARGS);
extern Datum point3D_recv(PG_FUNCTION_ARGS);
extern Datum point3D_send(PG_FUNCTION_ARGS);

extern Datum point3D_construct(PG_FUNCTION_ARGS);

extern Datum point3D_x(PG_FUNCTION_ARGS);
extern Datum point3D_y(PG_FUNCTION_ARGS);
extern Datum point3D_z(PG_FUNCTION_ARGS);
extern Datum point3D_boundbox(PG_FUNCTION_ARGS);
extern Datum point3D_boundsphere(PG_FUNCTION_ARGS);

extern Box3D *point3D_bbox(Point3D* point);
extern Sphere *point3D_bsphere(Point3D* point);

extern Datum point3D_vertical(PG_FUNCTION_ARGS);
extern Datum point3D_horizontal(PG_FUNCTION_ARGS);
extern Datum point3D_perpendicular(PG_FUNCTION_ARGS);

extern Datum point3D_collinear(PG_FUNCTION_ARGS);
extern int point3D_collinear_internal(Point3D* point1, Point3D* point2, Point3D* point3);

extern Datum point3D_add(PG_FUNCTION_ARGS);
extern Datum point3D_sub(PG_FUNCTION_ARGS);
extern Datum point3D_mul(PG_FUNCTION_ARGS);
extern Datum point3D_div(PG_FUNCTION_ARGS);

extern Datum point3D_same(PG_FUNCTION_ARGS);
extern Datum point3D_cmp(PG_FUNCTION_ARGS);
extern Datum point3D_lt(PG_FUNCTION_ARGS);
extern Datum point3D_le(PG_FUNCTION_ARGS);
extern Datum point3D_eq(PG_FUNCTION_ARGS);
extern Datum point3D_ne(PG_FUNCTION_ARGS);
extern Datum point3D_ge(PG_FUNCTION_ARGS);
extern Datum point3D_gt(PG_FUNCTION_ARGS);

extern bool point3D_same_internal(Point3D* point1, Point3D* point2);

/*****************************************************************************
 * Lseg3D routines
 *****************************************************************************/

extern Datum lseg3D_in(PG_FUNCTION_ARGS);
extern Datum lseg3D_out(PG_FUNCTION_ARGS);
extern Datum lseg3D_recv(PG_FUNCTION_ARGS);
extern Datum lseg3D_send(PG_FUNCTION_ARGS);

extern Datum lseg3D_construct(PG_FUNCTION_ARGS);
extern Datum lseg3D_construct_pts(PG_FUNCTION_ARGS);

extern void lseg3D_construct_pts_stat(Lseg3D *lseg, Point3D pt1, Point3D pt2);

extern Datum lseg3D_points(PG_FUNCTION_ARGS);
extern Datum lseg3D_length(PG_FUNCTION_ARGS);
extern Datum lseg3D_center(PG_FUNCTION_ARGS);
extern Datum lseg3D_boundbox(PG_FUNCTION_ARGS);
extern Datum lseg3D_boundsphere(PG_FUNCTION_ARGS);

extern Box3D *lseg3D_bbox(Lseg3D *lseg);
extern Sphere *lseg3D_bsphere(Lseg3D *lseg);

extern Datum lseg3D_parallel(PG_FUNCTION_ARGS);
extern Datum lseg3D_coincide(PG_FUNCTION_ARGS);
extern Datum lseg3D_skew(PG_FUNCTION_ARGS);
extern Datum lseg3D_orthogonal(PG_FUNCTION_ARGS);
extern Datum lseg3D_vertical(PG_FUNCTION_ARGS);
extern Datum lseg3D_horizontal(PG_FUNCTION_ARGS);
extern Datum lseg3D_perpendicular(PG_FUNCTION_ARGS);

extern bool lseg3D_parallel_internal(Lseg3D *lseg1, Lseg3D *lseg2);

extern Datum lseg3D_line3D(PG_FUNCTION_ARGS);
extern Line3D *lseg3D_line3D_internal(Lseg3D *lseg);

extern Datum lseg3D_same(PG_FUNCTION_ARGS);
extern Datum lseg3D_cmp(PG_FUNCTION_ARGS);
extern Datum lseg3D_lt(PG_FUNCTION_ARGS);
extern Datum lseg3D_le(PG_FUNCTION_ARGS);
extern Datum lseg3D_eq(PG_FUNCTION_ARGS);
extern Datum lseg3D_ne(PG_FUNCTION_ARGS);
extern Datum lseg3D_gt(PG_FUNCTION_ARGS);
extern Datum lseg3D_ge(PG_FUNCTION_ARGS);

extern bool lseg3D_same_internal(Lseg3D *lseg1, Lseg3D *lseg2);

/*****************************************************************************
 * Line3D routines
 *****************************************************************************/

extern Datum line3D_in(PG_FUNCTION_ARGS);
extern Datum line3D_out(PG_FUNCTION_ARGS);
extern Datum line3D_recv(PG_FUNCTION_ARGS);
extern Datum line3D_send(PG_FUNCTION_ARGS);
extern Datum line3D_construct(PG_FUNCTION_ARGS);

extern Datum line3D_points(PG_FUNCTION_ARGS);

extern Datum line3D_same(PG_FUNCTION_ARGS);
extern Datum line3D_cmp(PG_FUNCTION_ARGS);
extern Datum line3D_lt(PG_FUNCTION_ARGS);
extern Datum line3D_le(PG_FUNCTION_ARGS);
extern Datum line3D_eq(PG_FUNCTION_ARGS);
extern Datum line3D_ne(PG_FUNCTION_ARGS);
extern Datum line3D_ge(PG_FUNCTION_ARGS);
extern Datum line3D_gt(PG_FUNCTION_ARGS);

extern bool line3D_same_internal(Line3D	*line1, Line3D *line2);

extern Datum line3D_parallel(PG_FUNCTION_ARGS);
extern Datum line3D_coincide(PG_FUNCTION_ARGS);
extern Datum line3D_orthogonal(PG_FUNCTION_ARGS);
extern Datum line3D_vertical(PG_FUNCTION_ARGS);
extern Datum line3D_horizontal(PG_FUNCTION_ARGS);
extern Datum line3D_perpendicular(PG_FUNCTION_ARGS);

extern bool line3D_coincide_internal(Line3D *line1, Line3D *line2);
extern bool line3D_parallel_internal(Line3D *line1, Line3D *line2);
extern bool line3D_skew_internal(Line3D *line1, Line3D *line2);

extern bool line3D_hori(Line3D *line);
extern bool line3D_vert(Line3D *line);
extern bool line3D_perp(Line3D *line);

/*****************************************************************************
 * Box3D routines
 *****************************************************************************/

extern Datum box3D_in(PG_FUNCTION_ARGS);
extern Datum box3D_out(PG_FUNCTION_ARGS);
extern Datum box3D_recv(PG_FUNCTION_ARGS);
extern Datum box3D_send(PG_FUNCTION_ARGS);
extern Datum box3D_construct(PG_FUNCTION_ARGS);
extern Datum box3D_construct_pts(PG_FUNCTION_ARGS);

extern Datum box3D_high(PG_FUNCTION_ARGS);
extern Datum box3D_low(PG_FUNCTION_ARGS);
extern Datum box3D_points(PG_FUNCTION_ARGS);
extern Datum box3D_diagonal(PG_FUNCTION_ARGS);
extern Datum box3D_volume(PG_FUNCTION_ARGS);
extern Datum box3D_width(PG_FUNCTION_ARGS);
extern Datum box3D_height(PG_FUNCTION_ARGS);
extern Datum box3D_depth(PG_FUNCTION_ARGS);
extern Datum box3D_center(PG_FUNCTION_ARGS);
extern Datum box3D_boundbox(PG_FUNCTION_ARGS);
extern Datum box3D_boundsphere(PG_FUNCTION_ARGS);

extern Point3D *box3D_cn(Box3D *box);
extern double box3D_vol(Box3D *box);
extern Box3D *box3D_bbox(Box3D *box);
extern Sphere *box3D_bsphere(Box3D *box);

extern Datum box3D_same(PG_FUNCTION_ARGS);
extern Datum box3D_cmp(PG_FUNCTION_ARGS);
extern Datum box3D_lt(PG_FUNCTION_ARGS);
extern Datum box3D_le(PG_FUNCTION_ARGS);
extern Datum box3D_eq(PG_FUNCTION_ARGS);
extern Datum box3D_ne(PG_FUNCTION_ARGS);
extern Datum box3D_ge(PG_FUNCTION_ARGS);
extern Datum box3D_gt(PG_FUNCTION_ARGS);

extern bool box3D_same_internal(Box3D *box1, Box3D *box2);

extern Datum box3D_add(PG_FUNCTION_ARGS);
extern Datum box3D_sub(PG_FUNCTION_ARGS);
extern Datum box3D_mul(PG_FUNCTION_ARGS);
extern Datum box3D_div(PG_FUNCTION_ARGS);
extern Datum boxes3D_bound_box(PG_FUNCTION_ARGS);

/*****************************************************************************
 * Path3D routines
 *****************************************************************************/

extern Datum path3D_in(PG_FUNCTION_ARGS);
extern Datum path3D_out(PG_FUNCTION_ARGS);
extern Datum path3D_recv(PG_FUNCTION_ARGS);
extern Datum path3D_send(PG_FUNCTION_ARGS);

extern Path3D *path3D_copy(Path3D *path);

extern Datum path3D_npoints(PG_FUNCTION_ARGS);
extern Datum path3D_points(PG_FUNCTION_ARGS);
extern Datum path3D_center(PG_FUNCTION_ARGS);
extern Datum path3D_length(PG_FUNCTION_ARGS);
extern Datum path3D_isplanar(PG_FUNCTION_ARGS);
extern Datum path3D_area(PG_FUNCTION_ARGS);
extern Datum path3D_isclosed(PG_FUNCTION_ARGS);
extern Datum path3D_isopen(PG_FUNCTION_ARGS);
extern Datum path3D_boundbox(PG_FUNCTION_ARGS);
extern Datum path3D_boundsphere(PG_FUNCTION_ARGS);

extern Box3D *path3D_bbox(Path3D *path);
extern Sphere *path3D_bsphere(Path3D *path);

extern Datum path3D_close(PG_FUNCTION_ARGS);
extern Datum path3D_open(PG_FUNCTION_ARGS);
extern Datum path3D_poly3D(PG_FUNCTION_ARGS);

extern Datum path3D_same(PG_FUNCTION_ARGS);
extern Datum path3D_cmp(PG_FUNCTION_ARGS);
extern Datum path3D_lt(PG_FUNCTION_ARGS);
extern Datum path3D_le(PG_FUNCTION_ARGS);
extern Datum path3D_eq(PG_FUNCTION_ARGS);
extern Datum path3D_ne(PG_FUNCTION_ARGS);
extern Datum path3D_gt(PG_FUNCTION_ARGS);
extern Datum path3D_ge(PG_FUNCTION_ARGS);

extern bool path3D_same_internal(Path3D *path1, Path3D *path2);

extern Datum path3D_concat(PG_FUNCTION_ARGS);
extern Datum path3D_add(PG_FUNCTION_ARGS);
extern Datum path3D_sub(PG_FUNCTION_ARGS);
extern Datum path3D_mul(PG_FUNCTION_ARGS);
extern Datum path3D_div(PG_FUNCTION_ARGS);

/*****************************************************************************
 * Polygon3D routines
 *****************************************************************************/

extern Datum poly3D_in(PG_FUNCTION_ARGS);
extern Datum poly3D_out(PG_FUNCTION_ARGS);
extern Datum poly3D_recv(PG_FUNCTION_ARGS);
extern Datum poly3D_send(PG_FUNCTION_ARGS);

extern void poly3D_make_bbox(Polygon3D *poly);
extern Polygon3D *poly3D_copy(Polygon3D *poly);

extern Datum poly3D_npoints(PG_FUNCTION_ARGS);
extern Datum poly3D_points(PG_FUNCTION_ARGS);
extern Datum poly3D_center(PG_FUNCTION_ARGS);
extern Datum poly3D_length(PG_FUNCTION_ARGS);
extern Datum poly3D_isplanar(PG_FUNCTION_ARGS);
extern Datum poly3D_area(PG_FUNCTION_ARGS);
extern Datum poly3D_boundbox(PG_FUNCTION_ARGS);
extern Datum path3D_boundsphere(PG_FUNCTION_ARGS);

extern Box3D *poly3D_bbox(Polygon3D *poly);
extern Sphere *poly3D_bsphere(Polygon3D *poly);

extern Datum poly3D_path3D(PG_FUNCTION_ARGS);

extern Path3D *poly3D_path3D_internal(Polygon3D *poly);

extern Datum poly3D_same(PG_FUNCTION_ARGS);
extern Datum poly3D_cmp(PG_FUNCTION_ARGS);
extern Datum poly3D_lt(PG_FUNCTION_ARGS);
extern Datum poly3D_le(PG_FUNCTION_ARGS);
extern Datum poly3D_eq(PG_FUNCTION_ARGS);
extern Datum poly3D_ne(PG_FUNCTION_ARGS);
extern Datum poly3D_gt(PG_FUNCTION_ARGS);
extern Datum poly3D_ge(PG_FUNCTION_ARGS);

extern bool poly3D_same_internal(Polygon3D *poly1, Polygon3D *poly2);

extern Datum poly3D_add(PG_FUNCTION_ARGS);
extern Datum poly3D_sub(PG_FUNCTION_ARGS);
extern Datum poly3D_mul(PG_FUNCTION_ARGS);
extern Datum poly3D_div(PG_FUNCTION_ARGS);

/*****************************************************************************
 * Sphere routines
 *****************************************************************************/

extern Datum sphere_in(PG_FUNCTION_ARGS);
extern Datum sphere_out(PG_FUNCTION_ARGS);
extern Datum sphere_recv(PG_FUNCTION_ARGS);
extern Datum sphere_send(PG_FUNCTION_ARGS);

extern Datum sphere_center(PG_FUNCTION_ARGS);
extern Datum sphere_radius(PG_FUNCTION_ARGS);
extern Datum sphere_diameter(PG_FUNCTION_ARGS);
extern Datum sphere_volume(PG_FUNCTION_ARGS);
extern Datum sphere_volume(PG_FUNCTION_ARGS);
extern Datum sphere_boundbox(PG_FUNCTION_ARGS);

extern Box3D *sphere_bbox(Sphere *sphere);

extern Datum sphere_same(PG_FUNCTION_ARGS);
extern Datum sphere_cmp(PG_FUNCTION_ARGS);
extern Datum sphere_eq(PG_FUNCTION_ARGS);
extern Datum sphere_ne(PG_FUNCTION_ARGS);
extern Datum sphere_lt(PG_FUNCTION_ARGS);
extern Datum sphere_gt(PG_FUNCTION_ARGS);
extern Datum sphere_le(PG_FUNCTION_ARGS);
extern Datum sphere_ge(PG_FUNCTION_ARGS);

extern bool sphere_same_internal(Sphere *sphere1, Sphere *sphere2);

extern Datum sphere_add(PG_FUNCTION_ARGS);
extern Datum sphere_sub(PG_FUNCTION_ARGS);
extern Datum sphere_mul(PG_FUNCTION_ARGS);
extern Datum sphere_div(PG_FUNCTION_ARGS);

extern Datum sphere_construct(PG_FUNCTION_ARGS);

/*****************************************************************************
 * Coplanar routines
 *****************************************************************************/
 
extern Datum coplanar_lseg3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum coplanar_lseg3D_line3D(PG_FUNCTION_ARGS);
extern Datum coplanar_lseg3D_path3D(PG_FUNCTION_ARGS);
extern Datum coplanar_lseg3D_poly3D(PG_FUNCTION_ARGS);

extern bool coplanar_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2);
extern bool coplanar_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line);

extern Datum coplanar_line3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum coplanar_line3D_line3D(PG_FUNCTION_ARGS);
extern Datum coplanar_line3D_path3D(PG_FUNCTION_ARGS);
extern Datum coplanar_line3D_poly3D(PG_FUNCTION_ARGS);

extern bool coplanar_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg);
extern bool coplanar_line3D_line3D_internal(Line3D *line1, Line3D *line2);
extern bool coplanar_line3D_path3D_internal(Line3D *line, Path3D *path);
extern bool coplanar_line3D_poly3D_internal(Line3D *line, Polygon3D *poly);
 
extern Datum coplanar_path3D_point3D(PG_FUNCTION_ARGS);
extern Datum coplanar_path3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum coplanar_path3D_line3D(PG_FUNCTION_ARGS);
extern Datum coplanar_path3D_path3D(PG_FUNCTION_ARGS);
extern Datum coplanar_path3D_poly3D(PG_FUNCTION_ARGS);

extern bool coplanar_path3D_point3D_internal(Path3D *path, Point3D *point);
extern bool coplanar_path3D_lseg3D_internal(Path3D *path, Lseg3D *lseg);
extern bool coplanar_path3D_line3D_internal(Path3D *path, Line3D *line);
extern bool coplanar_path3D_path3D_internal(Path3D *path1, Path3D *path2);
extern bool coplanar_path3D_poly3D_internal(Path3D *path, Polygon3D *poly);

extern Datum coplanar_poly3D_point3D(PG_FUNCTION_ARGS);
extern Datum coplanar_poly3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum coplanar_poly3D_line3D(PG_FUNCTION_ARGS);
extern Datum coplanar_poly3D_path3D(PG_FUNCTION_ARGS);
extern Datum coplanar_poly3D_poly3D(PG_FUNCTION_ARGS);

extern bool coplanar_poly3D_point3D_internal(Polygon3D *poly, Point3D *point);
extern bool coplanar_poly3D_lseg3D_internal(Polygon3D *poly, Lseg3D *lseg);
extern bool coplanar_poly3D_line3D_internal(Polygon3D *poly, Line3D *line);
extern bool coplanar_poly3D_path3D_internal(Polygon3D *poly, Path3D *path);
extern bool coplanar_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2);
 
/*****************************************************************************
 *	Overlap functions
 *****************************************************************************/

extern Datum overlap_lseg3D_lseg3D(PG_FUNCTION_ARGS); 
extern Datum overlap_lseg3D_line3D(PG_FUNCTION_ARGS);

extern bool overlap_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2);
extern bool overlap_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line);

extern Datum overlap_line3D_lseg3D(PG_FUNCTION_ARGS);

extern bool overlap_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg);

extern Datum overlap_box3D_box3D(PG_FUNCTION_ARGS);
extern Datum overlap_box3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overlap_box3D_sphere(PG_FUNCTION_ARGS);

extern bool overlap_box3D_box3D_internal(Box3D *box1, Box3D *box2);
extern bool overlap_box3D_poly3D_internal(Box3D *box, Polygon3D *poly);
extern bool overlap_box3D_sphere_internal(Box3D *box, Sphere *sphere);

extern Datum overlap_poly3D_box3D(PG_FUNCTION_ARGS);
extern Datum overlap_poly3D_poly3D(PG_FUNCTION_ARGS);

extern bool overlap_poly3D_box3D_internal(Polygon3D *poly, Box3D *box);
extern bool overlap_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2);

extern Datum overlap_sphere_box(PG_FUNCTION_ARGS);
extern Datum overlap_sphere_sphere(PG_FUNCTION_ARGS);

extern bool overlap_sphere_box_internal(Sphere *sphere, Box3D *box);
extern bool overlap_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2);
 
/*****************************************************************************
 *	Contain functions
 *****************************************************************************/

extern Datum contain_lseg3D_point3D(PG_FUNCTION_ARGS);
extern Datum contain_lseg3D_lseg3D(PG_FUNCTION_ARGS);

extern bool contain_lseg3D_point3D_internal(Lseg3D *lseg, Point3D *point);
extern bool contain_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2);

extern Datum contain_line3D_point3D(PG_FUNCTION_ARGS);
extern Datum contain_line3D_lseg3D(PG_FUNCTION_ARGS);

extern bool	contain_line3D_point3D_internal(Line3D *line, Point3D *point);
extern bool	contain_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg);

extern Datum contain_box3D_point3D(PG_FUNCTION_ARGS);
extern Datum contain_box3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum contain_box3D_box3D(PG_FUNCTION_ARGS);
extern Datum contain_box3D_path3D(PG_FUNCTION_ARGS);
extern Datum contain_box3D_poly3D(PG_FUNCTION_ARGS);
extern Datum contain_box3D_sphere(PG_FUNCTION_ARGS);

extern bool contain_box3D_point3D_internal(Box3D *box, Point3D *point);
extern bool contain_box3D_lseg3D_internal(Box3D *box, Lseg3D *lseg);
extern bool contain_box3D_box3D_internal(Box3D *box1, Box3D *box2);
extern bool contain_box3D_path3D_internal(Box3D *box, Path3D *path);
extern bool contain_box3D_poly3D_internal(Box3D *box, Polygon3D *poly);
extern bool contain_box3D_sphere_internal(Box3D *box, Sphere *sphere);

extern Datum contain_path3D_point3D(PG_FUNCTION_ARGS);
extern Datum contain_path3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum contain_path3D_path3D(PG_FUNCTION_ARGS);

extern bool contain_path3D_point3D_internal(Path3D *path, Point3D *point);
extern bool contain_path3D_lseg3D_internal(Path3D *path, Lseg3D *lseg);
extern bool contain_path3D_path3D_internal(Path3D *path1, Path3D *path2);

extern Datum contain_poly3D_point3D(PG_FUNCTION_ARGS);
extern Datum contain_poly3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum contain_poly3D_path3D(PG_FUNCTION_ARGS);
extern Datum contain_poly3D_poly3D(PG_FUNCTION_ARGS);

extern bool contain_poly3D_point3D_internal(Polygon3D *poly, Point3D *point);
extern bool contain_poly3D_lseg3D_internal(Polygon3D *poly, Lseg3D *lseg);
extern bool contain_poly3D_path3D_internal(Polygon3D *poly, Path3D *path);
extern bool contain_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2);

extern Datum contain_sphere_point3D(PG_FUNCTION_ARGS);
extern Datum contain_sphere_lseg3D(PG_FUNCTION_ARGS);
extern Datum contain_sphere_box3D(PG_FUNCTION_ARGS);
extern Datum contain_sphere_path3D(PG_FUNCTION_ARGS);
extern Datum contain_sphere_poly3D(PG_FUNCTION_ARGS);
extern Datum contain_sphere_sphere(PG_FUNCTION_ARGS);

extern bool contain_sphere_point3D_internal(Sphere *sphere, Point3D *point);
extern bool contain_sphere_lseg3D_internal(Sphere *sphere, Lseg3D *lseg);
extern bool contain_sphere_box3D_internal(Sphere *sphere, Box3D *box);
extern bool contain_sphere_path3D_internal(Sphere *sphere, Path3D *path);
extern bool contain_sphere_poly3D_internal(Sphere *sphere, Polygon3D *poly);
extern bool contain_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2);

/*****************************************************************************
 *	Contained functions
 *****************************************************************************/

extern Datum contained_point3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum contained_point3D_line3D(PG_FUNCTION_ARGS);
extern Datum contained_point3D_box3D(PG_FUNCTION_ARGS);
extern Datum contained_point3D_path3D(PG_FUNCTION_ARGS);
extern Datum contained_point3D_poly3D(PG_FUNCTION_ARGS);
extern Datum contained_point3D_sphere(PG_FUNCTION_ARGS);

extern bool contained_point3D_lseg3D_internal(Point3D *point, Lseg3D *lseg);
extern bool contained_point3D_line3D_internal(Point3D *point, Line3D *line);
extern bool contained_point3D_box3D_internal(Point3D *point, Box3D *box);
extern bool contained_point3D_path3D_internal(Point3D *point, Path3D *path);
extern bool contained_point3D_poly3D_internal(Point3D *point, Polygon3D *poly);
extern bool contained_point3D_sphere_internal(Point3D *point, Sphere *sphere);

extern Datum contained_lseg3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum contained_lseg3D_line3D(PG_FUNCTION_ARGS);
extern Datum contained_lseg3D_box3D(PG_FUNCTION_ARGS);
extern Datum contained_lseg3D_path3D(PG_FUNCTION_ARGS); 
extern Datum contained_lseg3D_poly3D(PG_FUNCTION_ARGS); 
extern Datum contained_lseg3D_sphere(PG_FUNCTION_ARGS);

extern bool contained_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2);
extern bool	contained_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line);
extern bool contained_lseg3D_box3D_internal(Lseg3D *lseg, Box3D *box);
extern bool contained_lseg3D_path3D_internal(Lseg3D *lseg, Path3D *path);
extern bool contained_lseg3D_poly3D_internal(Lseg3D *lseg, Polygon3D *poly);
extern bool contained_lseg3D_sphere_internal(Lseg3D *lseg, Sphere *sphere);

extern Datum contained_box3D_box3D(PG_FUNCTION_ARGS);
extern Datum contained_box3D_sphere(PG_FUNCTION_ARGS);

extern bool contained_box3D_box3D_internal(Box3D *box1, Box3D *box2);
extern bool contained_box3D_sphere_internal(Box3D *box, Sphere *sphere);

extern Datum contained_path3D_box3D(PG_FUNCTION_ARGS);
extern Datum contained_path3D_path3D(PG_FUNCTION_ARGS);
extern Datum contained_path3D_poly3D(PG_FUNCTION_ARGS);
extern Datum contained_path3D_sphere(PG_FUNCTION_ARGS);

extern bool contained_path3D_box3D_internal(Path3D *path, Box3D *box);
extern bool contained_path3D_path3D_internal(Path3D *path1, Path3D *path2);
extern bool contained_path3D_poly3D_internal(Path3D *path, Polygon3D *poly);
extern bool contained_path3D_sphere_internal(Path3D *path, Sphere *sphere);

extern Datum contained_poly3D_box3D(PG_FUNCTION_ARGS);
extern Datum contained_poly3D_path3D(PG_FUNCTION_ARGS);
extern Datum contained_poly3D_poly3D(PG_FUNCTION_ARGS);
extern Datum contained_poly3D_sphere(PG_FUNCTION_ARGS);

extern bool contained_poly3D_box3D_internal(Polygon3D *poly, Box3D *box);
extern bool contained_poly3D_path3D_internal(Path3D *path, Polygon3D *poly);
extern bool contained_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2);
extern bool contained_poly3D_sphere_internal(Polygon3D *poly, Sphere *sphere);

extern Datum contained_sphere_box3D(PG_FUNCTION_ARGS);
extern Datum contained_sphere_sphere(PG_FUNCTION_ARGS);

extern bool contained_sphere_box3D_internal(Sphere *sphere, Box3D *box);
extern bool contained_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2);

/*****************************************************************************
 *	Relative position functions
 *****************************************************************************/

/* point3D op point3D */

extern Datum left_point3D_point3D(PG_FUNCTION_ARGS);
extern Datum overleft_point3D_point3D(PG_FUNCTION_ARGS);
extern Datum right_point3D_point3D(PG_FUNCTION_ARGS);
extern Datum overright_point3D_point3D(PG_FUNCTION_ARGS);
extern Datum below_point3D_point3D(PG_FUNCTION_ARGS);
extern Datum overbelow_point3D_point3D(PG_FUNCTION_ARGS);
extern Datum above_point3D_point3D(PG_FUNCTION_ARGS);
extern Datum overabove_point3D_point3D(PG_FUNCTION_ARGS);
extern Datum front_point3D_point3D(PG_FUNCTION_ARGS);
extern Datum overfront_point3D_point3D(PG_FUNCTION_ARGS);
extern Datum back_point3D_point3D(PG_FUNCTION_ARGS);
extern Datum overback_point3D_point3D(PG_FUNCTION_ARGS);

extern bool left_point3D_point3D_internal(Point3D *point1, Point3D *point2);
extern bool overleft_point3D_point3D_internal(Point3D *point1, Point3D *point2);
extern bool right_point3D_point3D_internal(Point3D *point1, Point3D *point2);
extern bool overright_point3D_point3D_internal(Point3D *point1, Point3D *point2);
extern bool below_point3D_point3D_internal(Point3D *point1, Point3D *point2);
extern bool overbelow_point3D_point3D_internal(Point3D *point1, Point3D *point2);
extern bool above_point3D_point3D_internal(Point3D *point1, Point3D *point2);
extern bool overabove_point3D_point3D_internal(Point3D *point1, Point3D *point2);
extern bool front_point3D_point3D_internal(Point3D *point1, Point3D *point2);
extern bool overfront_point3D_point3D_internal(Point3D *point1, Point3D *point2);
extern bool back_point3D_point3D_internal(Point3D *point1, Point3D *point2);
extern bool overback_point3D_point3D_internal(Point3D *point1, Point3D *point2);

/* point3D op lseg3D */

extern Datum left_point3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overleft_point3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum right_point3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overright_point3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum below_point3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overbelow_point3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum above_point3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overabove_point3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum front_point3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overfront_point3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum back_point3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overback_point3D_lseg3D(PG_FUNCTION_ARGS);

extern bool left_point3D_lseg3D_internal(Point3D *point, Lseg3D *lseg);
extern bool overleft_point3D_lseg3D_internal(Point3D *point, Lseg3D *lseg);
extern bool right_point3D_lseg3D_internal(Point3D *point, Lseg3D *lseg);
extern bool overright_point3D_lseg3D_internal(Point3D *point, Lseg3D *lseg);
extern bool below_point3D_lseg3D_internal(Point3D *point, Lseg3D *lseg);
extern bool overbelow_point3D_lseg3D_internal(Point3D *point, Lseg3D *lseg);
extern bool above_point3D_lseg3D_internal(Point3D *point, Lseg3D *lseg);
extern bool overabove_point3D_lseg3D_internal(Point3D *point, Lseg3D *lseg);
extern bool front_point3D_lseg3D_internal(Point3D *point, Lseg3D *lseg);
extern bool overfront_point3D_lseg3D_internal(Point3D *point, Lseg3D *lseg);
extern bool back_point3D_lseg3D_internal(Point3D *point, Lseg3D *lseg);
extern bool overback_point3D_lseg3D_internal(Point3D *point, Lseg3D *lseg);

/* point3D op line3D */

extern Datum left_point3D_line3D(PG_FUNCTION_ARGS);
extern Datum overleft_point3D_line3D(PG_FUNCTION_ARGS);
extern Datum right_point3D_line3D(PG_FUNCTION_ARGS);
extern Datum overright_point3D_line3D(PG_FUNCTION_ARGS);
extern Datum below_point3D_line3D(PG_FUNCTION_ARGS);
extern Datum overbelow_point3D_line3D(PG_FUNCTION_ARGS);
extern Datum above_point3D_line3D(PG_FUNCTION_ARGS);
extern Datum overabove_point3D_line3D(PG_FUNCTION_ARGS);
extern Datum front_point3D_line3D(PG_FUNCTION_ARGS);
extern Datum overfront_point3D_line3D(PG_FUNCTION_ARGS);
extern Datum back_point3D_line3D(PG_FUNCTION_ARGS);
extern Datum overback_point3D_line3D(PG_FUNCTION_ARGS);

extern bool left_point3D_line3D_internal(Point3D *point, Line3D *line);
extern bool overleft_point3D_line3D_internal(Point3D *point, Line3D *line);
extern bool right_point3D_line3D_internal(Point3D *point, Line3D *line);
extern bool overright_point3D_line3D_internal(Point3D *point, Line3D *line);
extern bool below_point3D_line3D_internal(Point3D *point, Line3D *line);
extern bool overbelow_point3D_line3D_internal(Point3D *point, Line3D *line);
extern bool above_point3D_line3D_internal(Point3D *point, Line3D *line);
extern bool overabove_point3D_line3D_internal(Point3D *point, Line3D *line);
extern bool front_point3D_line3D_internal(Point3D *point, Line3D *line);
extern bool overfront_point3D_line3D_internal(Point3D *point, Line3D *line);
extern bool back_point3D_line3D_internal(Point3D *point, Line3D *line);
extern bool overback_point3D_line3D_internal(Point3D *point, Line3D *line);

/* point3D op box3D */

extern Datum left_point3D_box3D(PG_FUNCTION_ARGS);
extern Datum overleft_point3D_box3D(PG_FUNCTION_ARGS);
extern Datum right_point3D_box3D(PG_FUNCTION_ARGS);
extern Datum overright_point3D_box3D(PG_FUNCTION_ARGS);
extern Datum below_point3D_box3D(PG_FUNCTION_ARGS);
extern Datum overbelow_point3D_box3D(PG_FUNCTION_ARGS);
extern Datum above_point3D_box3D(PG_FUNCTION_ARGS);
extern Datum overabove_point3D_box3D(PG_FUNCTION_ARGS);
extern Datum front_point3D_box3D(PG_FUNCTION_ARGS);
extern Datum overfront_point3D_box3D(PG_FUNCTION_ARGS);
extern Datum back_point3D_box3D(PG_FUNCTION_ARGS);
extern Datum overback_point3D_box3D(PG_FUNCTION_ARGS);

extern bool left_point3D_box3D_internal(Point3D *point, Box3D *box);
extern bool overleft_point3D_box3D_internal(Point3D *point, Box3D *box);
extern bool right_point3D_box3D_internal(Point3D *point, Box3D *box);
extern bool overright_point3D_box3D_internal(Point3D *point, Box3D *box);
extern bool below_point3D_box3D_internal(Point3D *point, Box3D *box);
extern bool overbelow_point3D_box3D_internal(Point3D *point, Box3D *box);
extern bool above_point3D_box3D_internal(Point3D *point, Box3D *box);
extern bool overabove_point3D_box3D_internal(Point3D *point, Box3D *box);
extern bool front_point3D_box3D_internal(Point3D *point, Box3D *box);
extern bool overfront_point3D_box3D_internal(Point3D *point, Box3D *box);
extern bool back_point3D_box3D_internal(Point3D *point, Box3D *box);
extern bool overback_point3D_box3D_internal(Point3D *point, Box3D *box);

/* point3D op path3D */

extern Datum left_point3D_path3D(PG_FUNCTION_ARGS);
extern Datum overleft_point3D_path3D(PG_FUNCTION_ARGS);
extern Datum right_point3D_path3D(PG_FUNCTION_ARGS);
extern Datum overright_point3D_path3D(PG_FUNCTION_ARGS);
extern Datum below_point3D_path3D(PG_FUNCTION_ARGS);
extern Datum overbelow_point3D_path3D(PG_FUNCTION_ARGS);
extern Datum above_point3D_path3D(PG_FUNCTION_ARGS);
extern Datum overabove_point3D_path3D(PG_FUNCTION_ARGS);
extern Datum front_point3D_path3D(PG_FUNCTION_ARGS);
extern Datum overfront_point3D_path3D(PG_FUNCTION_ARGS);
extern Datum back_point3D_path3D(PG_FUNCTION_ARGS);
extern Datum overback_point3D_path3D(PG_FUNCTION_ARGS);

extern bool left_point3D_path3D_internal(Point3D *point, Path3D *path);
extern bool overleft_point3D_path3D_internal(Point3D *point, Path3D *path);
extern bool right_point3D_path3D_internal(Point3D *point, Path3D *path);
extern bool overright_point3D_path3D_internal(Point3D *point, Path3D *path);
extern bool below_point3D_path3D_internal(Point3D *point, Path3D *path);
extern bool overbelow_point3D_path3D_internal(Point3D *point, Path3D *path);
extern bool above_point3D_path3D_internal(Point3D *point, Path3D *path);
extern bool overabove_point3D_path3D_internal(Point3D *point, Path3D *path);
extern bool front_point3D_path3D_internal(Point3D *point, Path3D *path);
extern bool overfront_point3D_path3D_internal(Point3D *point, Path3D *path);
extern bool back_point3D_path3D_internal(Point3D *point, Path3D *path);
extern bool overback_point3D_path3D_internal(Point3D *point, Path3D *path);

/* point3D op poly3D */

extern Datum left_point3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overleft_point3D_poly3D(PG_FUNCTION_ARGS);
extern Datum right_point3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overright_point3D_poly3D(PG_FUNCTION_ARGS);
extern Datum below_point3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overbelow_point3D_poly3D(PG_FUNCTION_ARGS);
extern Datum above_point3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overabove_point3D_poly3D(PG_FUNCTION_ARGS);
extern Datum front_point3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overfront_point3D_poly3D(PG_FUNCTION_ARGS);
extern Datum back_point3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overback_point3D_poly3D(PG_FUNCTION_ARGS);

extern bool left_point3D_poly3D_internal(Point3D *point, Polygon3D *poly);
extern bool overleft_point3D_poly3D_internal(Point3D *point, Polygon3D *poly);
extern bool right_point3D_poly3D_internal(Point3D *point, Polygon3D *poly);
extern bool overright_point3D_poly3D_internal(Point3D *point, Polygon3D *poly);
extern bool below_point3D_poly3D_internal(Point3D *point, Polygon3D *poly);
extern bool overbelow_point3D_poly3D_internal(Point3D *point, Polygon3D *poly);
extern bool above_point3D_poly3D_internal(Point3D *point, Polygon3D *poly);
extern bool overabove_point3D_poly3D_internal(Point3D *point, Polygon3D *poly);
extern bool front_point3D_poly3D_internal(Point3D *point, Polygon3D *poly);
extern bool overfront_point3D_poly3D_internal(Point3D *point, Polygon3D *poly);
extern bool back_point3D_poly3D_internal(Point3D *point, Polygon3D *poly);
extern bool overback_point3D_poly3D_internal(Point3D *point, Polygon3D *poly);

/* point3D op sphere*/

extern Datum left_point3D_sphere(PG_FUNCTION_ARGS);
extern Datum overleft_point3D_sphere(PG_FUNCTION_ARGS);
extern Datum right_point3D_sphere(PG_FUNCTION_ARGS);
extern Datum overright_point3D_sphere(PG_FUNCTION_ARGS);
extern Datum below_point3D_sphere(PG_FUNCTION_ARGS);
extern Datum overbelow_point3D_sphere(PG_FUNCTION_ARGS);
extern Datum above_point3D_sphere(PG_FUNCTION_ARGS);
extern Datum overabove_point3D_sphere(PG_FUNCTION_ARGS);
extern Datum front_point3D_sphere(PG_FUNCTION_ARGS);
extern Datum overfront_point3D_sphere(PG_FUNCTION_ARGS);
extern Datum back_point3D_sphere(PG_FUNCTION_ARGS);
extern Datum overback_point3D_sphere(PG_FUNCTION_ARGS);

extern bool left_point3D_sphere_internal(Point3D *point, Sphere *sphere);
extern bool overleft_point3D_sphere_internal(Point3D *point, Sphere *sphere);
extern bool right_point3D_sphere_internal(Point3D *point, Sphere *sphere);
extern bool overright_point3D_sphere_internal(Point3D *point, Sphere *sphere);
extern bool below_point3D_sphere_internal(Point3D *point, Sphere *sphere);
extern bool overbelow_point3D_sphere_internal(Point3D *point, Sphere *sphere);
extern bool above_point3D_sphere_internal(Point3D *point, Sphere *sphere);
extern bool overabove_point3D_sphere_internal(Point3D *point, Sphere *sphere);
extern bool front_point3D_sphere_internal(Point3D *point, Sphere *sphere);
extern bool overfront_point3D_sphere_internal(Point3D *point, Sphere *sphere);
extern bool back_point3D_sphere_internal(Point3D *point, Sphere *sphere);
extern bool overback_point3D_sphere_internal(Point3D *point, Sphere *sphere);

/*----------------------------------------------------------*/

/* lseg3D op point3D */

extern Datum left_lseg3D_point3D(PG_FUNCTION_ARGS);
extern Datum overleft_lseg3D_point3D(PG_FUNCTION_ARGS);
extern Datum right_lseg3D_point3D(PG_FUNCTION_ARGS);
extern Datum overright_lseg3D_point3D(PG_FUNCTION_ARGS);
extern Datum below_lseg3D_point3D(PG_FUNCTION_ARGS);
extern Datum overbelow_lseg3D_point3D(PG_FUNCTION_ARGS);
extern Datum above_lseg3D_point3D(PG_FUNCTION_ARGS);
extern Datum overabove_lseg3D_point3D(PG_FUNCTION_ARGS);
extern Datum front_lseg3D_point3D(PG_FUNCTION_ARGS);
extern Datum overfront_lseg3D_point3D(PG_FUNCTION_ARGS);
extern Datum back_lseg3D_point3D(PG_FUNCTION_ARGS);
extern Datum overback_lseg3D_point3D(PG_FUNCTION_ARGS);

extern bool left_lseg3D_point3D_internal(Lseg3D *lseg, Point3D *point);
extern bool overleft_lseg3D_point3D_internal(Lseg3D *lseg, Point3D *point);
extern bool right_lseg3D_point3D_internal(Lseg3D *lseg, Point3D *point);
extern bool overright_lseg3D_point3D_internal(Lseg3D *lseg, Point3D *point);
extern bool below_lseg3D_point3D_internal(Lseg3D *lseg, Point3D *point);
extern bool overbelow_lseg3D_point3D_internal(Lseg3D *lseg, Point3D *point);
extern bool above_lseg3D_point3D_internal(Lseg3D *lseg, Point3D *point);
extern bool overabove_lseg3D_point3D_internal(Lseg3D *lseg, Point3D *point);
extern bool front_lseg3D_point3D_internal(Lseg3D *lseg, Point3D *point);
extern bool overfront_lseg3D_point3D_internal(Lseg3D *lseg, Point3D *point);
extern bool back_lseg3D_point3D_internal(Lseg3D *lseg, Point3D *point);
extern bool overback_lseg3D_point3D_internal(Lseg3D *lseg, Point3D *point);

/* lseg3D op lseg3D */

extern Datum left_lseg3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overleft_lseg3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum right_lseg3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overright_lseg3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum below_lseg3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overbelow_lseg3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum above_lseg3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overabove_lseg3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum front_lseg3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overfront_lseg3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum back_lseg3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overback_lseg3D_lseg3D(PG_FUNCTION_ARGS);

extern bool left_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2);
extern bool overleft_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2);
extern bool right_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2);
extern bool overright_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2);
extern bool below_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2);
extern bool overbelow_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2);
extern bool above_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2);
extern bool overabove_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2);
extern bool front_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2);
extern bool overfront_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2);
extern bool back_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2);
extern bool overback_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2);

/* lseg3D op line3D */

extern Datum left_lseg3D_line3D(PG_FUNCTION_ARGS);
extern Datum overleft_lseg3D_line3D(PG_FUNCTION_ARGS);
extern Datum right_lseg3D_line3D(PG_FUNCTION_ARGS);
extern Datum overright_lseg3D_line3D(PG_FUNCTION_ARGS);
extern Datum below_lseg3D_line3D(PG_FUNCTION_ARGS);
extern Datum overbelow_lseg3D_line3D(PG_FUNCTION_ARGS);
extern Datum above_lseg3D_line3D(PG_FUNCTION_ARGS);
extern Datum overabove_lseg3D_line3D(PG_FUNCTION_ARGS);
extern Datum front_lseg3D_line3D(PG_FUNCTION_ARGS);
extern Datum overfront_lseg3D_line3D(PG_FUNCTION_ARGS);
extern Datum back_lseg3D_line3D(PG_FUNCTION_ARGS);
extern Datum overback_lseg3D_line3D(PG_FUNCTION_ARGS);

extern bool left_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line);
extern bool overleft_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line);
extern bool right_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line);
extern bool overright_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line);
extern bool below_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line);
extern bool overbelow_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line);
extern bool above_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line);
extern bool overabove_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line);
extern bool front_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line);
extern bool overfront_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line);
extern bool back_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line);
extern bool overback_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line);

/* lseg3D op box3D */

extern Datum left_lseg3D_box3D(PG_FUNCTION_ARGS);
extern Datum overleft_lseg3D_box3D(PG_FUNCTION_ARGS);
extern Datum right_lseg3D_box3D(PG_FUNCTION_ARGS);
extern Datum overright_lseg3D_box3D(PG_FUNCTION_ARGS);
extern Datum below_lseg3D_box3D(PG_FUNCTION_ARGS);
extern Datum overbelow_lseg3D_box3D(PG_FUNCTION_ARGS);
extern Datum above_lseg3D_box3D(PG_FUNCTION_ARGS);
extern Datum overabove_lseg3D_box3D(PG_FUNCTION_ARGS);
extern Datum front_lseg3D_box3D(PG_FUNCTION_ARGS);
extern Datum overfront_lseg3D_box3D(PG_FUNCTION_ARGS);
extern Datum back_lseg3D_box3D(PG_FUNCTION_ARGS);
extern Datum overback_lseg3D_box3D(PG_FUNCTION_ARGS);

extern bool left_lseg3D_box3D_internal(Lseg3D *lseg, Box3D *box);
extern bool overleft_lseg3D_box3D_internal(Lseg3D *lseg, Box3D *box);
extern bool right_lseg3D_box3D_internal(Lseg3D *lseg, Box3D *box);
extern bool overright_lseg3D_box3D_internal(Lseg3D *lseg, Box3D *box);
extern bool below_lseg3D_box3D_internal(Lseg3D *lseg, Box3D *box);
extern bool overbelow_lseg3D_box3D_internal(Lseg3D *lseg, Box3D *box);
extern bool above_lseg3D_box3D_internal(Lseg3D *lseg, Box3D *box);
extern bool overabove_lseg3D_box3D_internal(Lseg3D *lseg, Box3D *box);
extern bool front_lseg3D_box3D_internal(Lseg3D *lseg, Box3D *box);
extern bool overfront_lseg3D_box3D_internal(Lseg3D *lseg, Box3D *box);
extern bool back_lseg3D_box3D_internal(Lseg3D *lseg, Box3D *box);
extern bool overback_lseg3D_box3D_internal(Lseg3D *lseg, Box3D *box);

/* lseg3D op path3D */

extern Datum left_lseg3D_path3D(PG_FUNCTION_ARGS);
extern Datum overleft_lseg3D_path3D(PG_FUNCTION_ARGS);
extern Datum right_lseg3D_path3D(PG_FUNCTION_ARGS);
extern Datum overright_lseg3D_path3D(PG_FUNCTION_ARGS);
extern Datum below_lseg3D_path3D(PG_FUNCTION_ARGS);
extern Datum overbelow_lseg3D_path3D(PG_FUNCTION_ARGS);
extern Datum above_lseg3D_path3D(PG_FUNCTION_ARGS);
extern Datum overabove_lseg3D_path3D(PG_FUNCTION_ARGS);
extern Datum front_lseg3D_path3D(PG_FUNCTION_ARGS);
extern Datum overfront_lseg3D_path3D(PG_FUNCTION_ARGS);
extern Datum back_lseg3D_path3D(PG_FUNCTION_ARGS);
extern Datum overback_lseg3D_path3D(PG_FUNCTION_ARGS);

extern bool left_lseg3D_path3D_internal(Lseg3D *lseg, Path3D *path);
extern bool overleft_lseg3D_path3D_internal(Lseg3D *lseg, Path3D *path);
extern bool right_lseg3D_path3D_internal(Lseg3D *lseg, Path3D *path);
extern bool overright_lseg3D_path3D_internal(Lseg3D *lseg, Path3D *path);
extern bool below_lseg3D_path3D_internal(Lseg3D *lseg, Path3D *path);
extern bool overbelow_lseg3D_path3D_internal(Lseg3D *lseg, Path3D *path);
extern bool above_lseg3D_path3D_internal(Lseg3D *lseg, Path3D *path);
extern bool overabove_lseg3D_path3D_internal(Lseg3D *lseg, Path3D *path);
extern bool front_lseg3D_path3D_internal(Lseg3D *lseg, Path3D *path);
extern bool overfront_lseg3D_path3D_internal(Lseg3D *lseg, Path3D *path);
extern bool back_lseg3D_path3D_internal(Lseg3D *lseg, Path3D *path);
extern bool overback_lseg3D_path3D_internal(Lseg3D *lseg, Path3D *path);

/* lseg3D op poly3D */

extern Datum left_lseg3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overleft_lseg3D_poly3D(PG_FUNCTION_ARGS);
extern Datum right_lseg3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overright_lseg3D_poly3D(PG_FUNCTION_ARGS);
extern Datum below_lseg3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overbelow_lseg3D_poly3D(PG_FUNCTION_ARGS);
extern Datum above_lseg3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overabove_lseg3D_poly3D(PG_FUNCTION_ARGS);
extern Datum front_lseg3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overfront_lseg3D_poly3D(PG_FUNCTION_ARGS);
extern Datum back_lseg3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overback_lseg3D_poly3D(PG_FUNCTION_ARGS);

extern bool left_lseg3D_poly3D_internal(Lseg3D *lseg, Polygon3D *poly);
extern bool overleft_lseg3D_poly3D_internal(Lseg3D *lseg, Polygon3D *poly);
extern bool right_lseg3D_poly3D_internal(Lseg3D *lseg, Polygon3D *poly);
extern bool overright_lseg3D_poly3D_internal(Lseg3D *lseg, Polygon3D *poly);
extern bool below_lseg3D_poly3D_internal(Lseg3D *lseg, Polygon3D *poly);
extern bool overbelow_lseg3D_poly3D_internal(Lseg3D *lseg, Polygon3D *poly);
extern bool above_lseg3D_poly3D_internal(Lseg3D *lseg, Polygon3D *poly);
extern bool overabove_lseg3D_poly3D_internal(Lseg3D *lseg, Polygon3D *poly);
extern bool front_lseg3D_poly3D_internal(Lseg3D *lseg, Polygon3D *poly);
extern bool overfront_lseg3D_poly3D_internal(Lseg3D *lseg, Polygon3D *poly);
extern bool back_lseg3D_poly3D_internal(Lseg3D *lseg, Polygon3D *poly);
extern bool overback_lseg3D_poly3D_internal(Lseg3D *lseg, Polygon3D *poly);

/* lseg3D op sphere */

extern Datum left_lseg3D_sphere(PG_FUNCTION_ARGS);
extern Datum overleft_lseg3D_sphere(PG_FUNCTION_ARGS);
extern Datum right_lseg3D_sphere(PG_FUNCTION_ARGS);
extern Datum overright_lseg3D_sphere(PG_FUNCTION_ARGS);
extern Datum below_lseg3D_sphere(PG_FUNCTION_ARGS);
extern Datum overbelow_lseg3D_sphere(PG_FUNCTION_ARGS);
extern Datum above_lseg3D_sphere(PG_FUNCTION_ARGS);
extern Datum overabove_lseg3D_sphere(PG_FUNCTION_ARGS);
extern Datum front_lseg3D_sphere(PG_FUNCTION_ARGS);
extern Datum overfront_lseg3D_sphere(PG_FUNCTION_ARGS);
extern Datum back_lseg3D_sphere(PG_FUNCTION_ARGS);
extern Datum overback_lseg3D_sphere(PG_FUNCTION_ARGS);

extern bool left_lseg3D_sphere_internal(Lseg3D *lseg, Sphere *sphere);
extern bool overleft_lseg3D_sphere_internal(Lseg3D *lseg, Sphere *sphere);
extern bool right_lseg3D_sphere_internal(Lseg3D *lseg, Sphere *sphere);
extern bool overright_lseg3D_sphere_internal(Lseg3D *lseg, Sphere *sphere);
extern bool below_lseg3D_sphere_internal(Lseg3D *lseg, Sphere *sphere);
extern bool overbelow_lseg3D_sphere_internal(Lseg3D *lseg, Sphere *sphere);
extern bool above_lseg3D_sphere_internal(Lseg3D *lseg, Sphere *sphere);
extern bool overabove_lseg3D_sphere_internal(Lseg3D *lseg, Sphere *sphere);
extern bool front_lseg3D_sphere_internal(Lseg3D *lseg, Sphere *sphere);
extern bool overfront_lseg3D_sphere_internal(Lseg3D *lseg, Sphere *sphere);
extern bool back_lseg3D_sphere_internal(Lseg3D *lseg, Sphere *sphere);
extern bool overback_lseg3D_sphere_internal(Lseg3D *lseg, Sphere *sphere);

/*----------------------------------------------------------*/

/* line3D op point3D */

extern Datum left_line3D_point3D(PG_FUNCTION_ARGS);
extern Datum overleft_line3D_point3D(PG_FUNCTION_ARGS);
extern Datum right_line3D_point3D(PG_FUNCTION_ARGS);
extern Datum overright_line3D_point3D(PG_FUNCTION_ARGS);
extern Datum below_line3D_point3D(PG_FUNCTION_ARGS);
extern Datum overbelow_line3D_point3D(PG_FUNCTION_ARGS);
extern Datum above_line3D_point3D(PG_FUNCTION_ARGS);
extern Datum overabove_line3D_point3D(PG_FUNCTION_ARGS);
extern Datum front_line3D_point3D(PG_FUNCTION_ARGS);
extern Datum overfront_line3D_point3D(PG_FUNCTION_ARGS);
extern Datum back_line3D_point3D(PG_FUNCTION_ARGS);
extern Datum overback_line3D_point3D(PG_FUNCTION_ARGS);

extern bool left_line3D_point3D_internal(Line3D *line, Point3D *point);
extern bool overleft_line3D_point3D_internal(Line3D *line, Point3D *point);
extern bool right_line3D_point3D_internal(Line3D *line, Point3D *point);
extern bool overright_line3D_point3D_internal(Line3D *line, Point3D *point);
extern bool below_line3D_point3D_internal(Line3D *line, Point3D *point);
extern bool overbelow_line3D_point3D_internal(Line3D *line, Point3D *point);
extern bool above_line3D_point3D_internal(Line3D *line, Point3D *point);
extern bool overabove_line3D_point3D_internal(Line3D *line, Point3D *point);
extern bool front_line3D_point3D_internal(Line3D *line, Point3D *point);
extern bool overfront_line3D_point3D_internal(Line3D *line, Point3D *point);
extern bool back_line3D_point3D_internal(Line3D *line, Point3D *point);
extern bool overback_line3D_point3D_internal(Line3D *line, Point3D *point);

/* line3D op lseg3D */

extern Datum left_line3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overleft_line3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum right_line3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overright_line3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum below_line3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overbelow_line3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum above_line3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overabove_line3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum front_line3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overfront_line3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum back_line3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overback_line3D_lseg3D(PG_FUNCTION_ARGS);

extern bool left_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg);
extern bool overleft_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg);
extern bool right_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg);
extern bool overright_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg);
extern bool below_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg);
extern bool overbelow_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg);
extern bool above_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg);
extern bool overabove_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg);
extern bool front_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg);
extern bool overfront_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg);
extern bool back_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg);
extern bool overback_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg);

/* line3D op line3D */

extern Datum left_line3D_line3D(PG_FUNCTION_ARGS);
extern Datum overleft_line3D_line3D(PG_FUNCTION_ARGS);
extern Datum right_line3D_line3D(PG_FUNCTION_ARGS);
extern Datum overright_line3D_line3D(PG_FUNCTION_ARGS);
extern Datum below_line3D_line3D(PG_FUNCTION_ARGS);
extern Datum overbelow_line3D_line3D(PG_FUNCTION_ARGS);
extern Datum above_line3D_line3D(PG_FUNCTION_ARGS);
extern Datum overabove_line3D_line3D(PG_FUNCTION_ARGS);
extern Datum front_line3D_line3D(PG_FUNCTION_ARGS);
extern Datum overfront_line3D_line3D(PG_FUNCTION_ARGS);
extern Datum back_line3D_line3D(PG_FUNCTION_ARGS);
extern Datum overback_line3D_line3D(PG_FUNCTION_ARGS);

extern bool left_line3D_line3D_internal(Line3D *line1, Line3D *line2);
extern bool overleft_line3D_line3D_internal(Line3D *line1, Line3D *line2);
extern bool right_line3D_line3D_internal(Line3D *line1, Line3D *line2);
extern bool overright_line3D_line3D_internal(Line3D *line1, Line3D *line2);
extern bool below_line3D_line3D_internal(Line3D *line1, Line3D *line2);
extern bool overbelow_line3D_line3D_internal(Line3D *line1, Line3D *line2);
extern bool above_line3D_line3D_internal(Line3D *line1, Line3D *line2);
extern bool overabove_line3D_line3D_internal(Line3D *line1, Line3D *line2);
extern bool front_line3D_line3D_internal(Line3D *line1, Line3D *line2);
extern bool overfront_line3D_line3D_internal(Line3D *line1, Line3D *line2);
extern bool back_line3D_line3D_internal(Line3D *line1, Line3D *line2);
extern bool overback_line3D_line3D_internal(Line3D *line1, Line3D *line2);

/* line3D op box3D */

extern Datum left_line3D_box3D(PG_FUNCTION_ARGS);
extern Datum overleft_line3D_box3D(PG_FUNCTION_ARGS);
extern Datum right_line3D_box3D(PG_FUNCTION_ARGS);
extern Datum overright_line3D_box3D(PG_FUNCTION_ARGS);
extern Datum below_line3D_box3D(PG_FUNCTION_ARGS);
extern Datum overbelow_line3D_box3D(PG_FUNCTION_ARGS);
extern Datum above_line3D_box3D(PG_FUNCTION_ARGS);
extern Datum overabove_line3D_box3D(PG_FUNCTION_ARGS);
extern Datum front_line3D_box3D(PG_FUNCTION_ARGS);
extern Datum overfront_line3D_box3D(PG_FUNCTION_ARGS);
extern Datum back_line3D_box3D(PG_FUNCTION_ARGS);
extern Datum overback_line3D_box3D(PG_FUNCTION_ARGS);

extern bool left_line3D_box3D_internal(Line3D *line, Box3D *box);
extern bool overleft_line3D_box3D_internal(Line3D *line, Box3D *box);
extern bool right_line3D_box3D_internal(Line3D *line, Box3D *box);
extern bool overright_line3D_box3D_internal(Line3D *line, Box3D *box);
extern bool below_line3D_box3D_internal(Line3D *line, Box3D *box);
extern bool overbelow_line3D_box3D_internal(Line3D *line, Box3D *box);
extern bool above_line3D_box3D_internal(Line3D *line, Box3D *box);
extern bool overabove_line3D_box3D_internal(Line3D *line, Box3D *box);
extern bool front_line3D_box3D_internal(Line3D *line, Box3D *box);
extern bool overfront_line3D_box3D_internal(Line3D *line, Box3D *box);
extern bool back_line3D_box3D_internal(Line3D *line, Box3D *box);
extern bool overback_line3D_box3D_internal(Line3D *line, Box3D *box);

/* line3D op path3D */

extern Datum left_line3D_path3D(PG_FUNCTION_ARGS);
extern Datum overleft_line3D_path3D(PG_FUNCTION_ARGS);
extern Datum right_line3D_path3D(PG_FUNCTION_ARGS);
extern Datum overright_line3D_path3D(PG_FUNCTION_ARGS);
extern Datum below_line3D_path3D(PG_FUNCTION_ARGS);
extern Datum overbelow_line3D_path3D(PG_FUNCTION_ARGS);
extern Datum above_line3D_path3D(PG_FUNCTION_ARGS);
extern Datum overabove_line3D_path3D(PG_FUNCTION_ARGS);
extern Datum front_line3D_path3D(PG_FUNCTION_ARGS);
extern Datum overfront_line3D_path3D(PG_FUNCTION_ARGS);
extern Datum back_line3D_path3D(PG_FUNCTION_ARGS);
extern Datum overback_line3D_path3D(PG_FUNCTION_ARGS);

extern bool left_line3D_path3D_internal(Line3D *line, Path3D *path);
extern bool overleft_line3D_path3D_internal(Line3D *line, Path3D *path);
extern bool right_line3D_path3D_internal(Line3D *line, Path3D *path);
extern bool overright_line3D_path3D_internal(Line3D *line, Path3D *path);
extern bool below_line3D_path3D_internal(Line3D *line, Path3D *path);
extern bool overbelow_line3D_path3D_internal(Line3D *line, Path3D *path);
extern bool above_line3D_path3D_internal(Line3D *line, Path3D *path);
extern bool overabove_line3D_path3D_internal(Line3D *line, Path3D *path);
extern bool front_line3D_path3D_internal(Line3D *line, Path3D *path);
extern bool overfront_line3D_path3D_internal(Line3D *line, Path3D *path);
extern bool back_line3D_path3D_internal(Line3D *line, Path3D *path);
extern bool overback_line3D_path3D_internal(Line3D *line, Path3D *path);

/* line3D op poly3D */

extern Datum left_line3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overleft_line3D_poly3D(PG_FUNCTION_ARGS);
extern Datum right_line3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overright_line3D_poly3D(PG_FUNCTION_ARGS);
extern Datum below_line3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overbelow_line3D_poly3D(PG_FUNCTION_ARGS);
extern Datum above_line3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overabove_line3D_poly3D(PG_FUNCTION_ARGS);
extern Datum front_line3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overfront_line3D_poly3D(PG_FUNCTION_ARGS);
extern Datum back_line3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overback_line3D_poly3D(PG_FUNCTION_ARGS);

extern bool left_line3D_poly3D_internal(Line3D *line, Polygon3D *poly);
extern bool overleft_line3D_poly3D_internal(Line3D *line, Polygon3D *poly);
extern bool right_line3D_poly3D_internal(Line3D *line, Polygon3D *poly);
extern bool overright_line3D_poly3D_internal(Line3D *line, Polygon3D *poly);
extern bool below_line3D_poly3D_internal(Line3D *line, Polygon3D *poly);
extern bool overbelow_line3D_poly3D_internal(Line3D *line, Polygon3D *poly);
extern bool above_line3D_poly3D_internal(Line3D *line, Polygon3D *poly);
extern bool overabove_line3D_poly3D_internal(Line3D *line, Polygon3D *poly);
extern bool front_line3D_poly3D_internal(Line3D *line, Polygon3D *poly);
extern bool overfront_line3D_poly3D_internal(Line3D *line, Polygon3D *poly);
extern bool back_line3D_poly3D_internal(Line3D *line, Polygon3D *poly);
extern bool overback_line3D_poly3D_internal(Line3D *line, Polygon3D *poly);

/* line3D op sphere */

extern Datum left_line3D_sphere(PG_FUNCTION_ARGS);
extern Datum overleft_line3D_sphere(PG_FUNCTION_ARGS);
extern Datum right_line3D_sphere(PG_FUNCTION_ARGS);
extern Datum overright_line3D_sphere(PG_FUNCTION_ARGS);
extern Datum below_line3D_sphere(PG_FUNCTION_ARGS);
extern Datum overbelow_line3D_sphere(PG_FUNCTION_ARGS);
extern Datum above_line3D_sphere(PG_FUNCTION_ARGS);
extern Datum overabove_line3D_sphere(PG_FUNCTION_ARGS);
extern Datum front_line3D_sphere(PG_FUNCTION_ARGS);
extern Datum overfront_line3D_sphere(PG_FUNCTION_ARGS);
extern Datum back_line3D_sphere(PG_FUNCTION_ARGS);
extern Datum overback_line3D_sphere(PG_FUNCTION_ARGS);

extern bool left_line3D_sphere_internal(Line3D *line, Sphere *sphere);
extern bool overleft_line3D_sphere_internal(Line3D *line, Sphere *sphere);
extern bool right_line3D_sphere_internal(Line3D *line, Sphere *sphere);
extern bool overright_line3D_sphere_internal(Line3D *line, Sphere *sphere);
extern bool below_line3D_sphere_internal(Line3D *line, Sphere *sphere);
extern bool overbelow_line3D_sphere_internal(Line3D *line, Sphere *sphere);
extern bool above_line3D_sphere_internal(Line3D *line, Sphere *sphere);
extern bool overabove_line3D_sphere_internal(Line3D *line, Sphere *sphere);
extern bool front_line3D_sphere_internal(Line3D *line, Sphere *sphere);
extern bool overfront_line3D_sphere_internal(Line3D *line, Sphere *sphere);
extern bool back_line3D_sphere_internal(Line3D *line, Sphere *sphere);
extern bool overback_line3D_sphere_internal(Line3D *line, Sphere *sphere);

/*----------------------------------------------------------*/

/* box3D op point3D */

extern Datum left_box3D_point3D(PG_FUNCTION_ARGS);
extern Datum overleft_box3D_point3D(PG_FUNCTION_ARGS);
extern Datum right_box3D_point3D(PG_FUNCTION_ARGS);
extern Datum overright_box3D_point3D(PG_FUNCTION_ARGS);
extern Datum below_box3D_point3D(PG_FUNCTION_ARGS);
extern Datum overbelow_box3D_point3D(PG_FUNCTION_ARGS);
extern Datum above_box3D_point3D(PG_FUNCTION_ARGS);
extern Datum overabove_box3D_point3D(PG_FUNCTION_ARGS);
extern Datum front_box3D_point3D(PG_FUNCTION_ARGS);
extern Datum overfront_box3D_point3D(PG_FUNCTION_ARGS);
extern Datum back_box3D_point3D(PG_FUNCTION_ARGS);
extern Datum overback_box3D_point3D(PG_FUNCTION_ARGS);

extern bool left_box3D_point3D_internal(Box3D *box, Point3D *point);
extern bool overleft_box3D_point3D_internal(Box3D *box, Point3D *point);
extern bool right_box3D_point3D_internal(Box3D *box, Point3D *point);
extern bool overright_box3D_point3D_internal(Box3D *box, Point3D *point);
extern bool below_box3D_point3D_internal(Box3D *box, Point3D *point);
extern bool overbelow_box3D_point3D_internal(Box3D *box, Point3D *point);
extern bool above_box3D_point3D_internal(Box3D *box, Point3D *point);
extern bool overabove_box3D_point3D_internal(Box3D *box, Point3D *point);
extern bool front_box3D_point3D_internal(Box3D *box, Point3D *point);
extern bool overfront_box3D_point3D_internal(Box3D *box, Point3D *point);
extern bool back_box3D_point3D_internal(Box3D *box, Point3D *point);
extern bool overback_box3D_point3D_internal(Box3D *box, Point3D *point);

/* box3D op lseg3D */

extern Datum left_box3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overleft_box3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum right_box3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overright_box3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum below_box3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overbelow_box3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum above_box3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overabove_box3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum front_box3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overfront_box3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum back_box3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overback_box3D_lseg3D(PG_FUNCTION_ARGS);

extern bool left_box3D_lseg3D_internal(Box3D *box, Lseg3D *lseg);
extern bool overleft_box3D_lseg3D_internal(Box3D *box, Lseg3D *lseg);
extern bool right_box3D_lseg3D_internal(Box3D *box, Lseg3D *lseg);
extern bool overright_box3D_lseg3D_internal(Box3D *box, Lseg3D *lseg);
extern bool below_box3D_lseg3D_internal(Box3D *box, Lseg3D *lseg);
extern bool overbelow_box3D_lseg3D_internal(Box3D *box, Lseg3D *lseg);
extern bool above_box3D_lseg3D_internal(Box3D *box, Lseg3D *lseg);
extern bool overabove_box3D_lseg3D_internal(Box3D *box, Lseg3D *lseg);
extern bool front_box3D_lseg3D_internal(Box3D *box, Lseg3D *lseg);
extern bool overfront_box3D_lseg3D_internal(Box3D *box, Lseg3D *lseg);
extern bool back_box3D_lseg3D_internal(Box3D *box, Lseg3D *lseg);
extern bool overback_box3D_lseg3D_internal(Box3D *box, Lseg3D *lseg);

/* box3D op line3D */

extern Datum left_box3D_line3D(PG_FUNCTION_ARGS);
extern Datum overleft_box3D_line3D(PG_FUNCTION_ARGS);
extern Datum right_box3D_line3D(PG_FUNCTION_ARGS);
extern Datum overright_box3D_line3D(PG_FUNCTION_ARGS);
extern Datum below_box3D_line3D(PG_FUNCTION_ARGS);
extern Datum overbelow_box3D_line3D(PG_FUNCTION_ARGS);
extern Datum above_box3D_line3D(PG_FUNCTION_ARGS);
extern Datum overabove_box3D_line3D(PG_FUNCTION_ARGS);
extern Datum front_box3D_line3D(PG_FUNCTION_ARGS);
extern Datum overfront_box3D_line3D(PG_FUNCTION_ARGS);
extern Datum back_box3D_line3D(PG_FUNCTION_ARGS);
extern Datum overback_box3D_line3D(PG_FUNCTION_ARGS);

extern bool left_box3D_line3D_internal(Box3D *box, Line3D *line);
extern bool overleft_box3D_line3D_internal(Box3D *box, Line3D *line);
extern bool right_box3D_line3D_internal(Box3D *box, Line3D *line);
extern bool overright_box3D_line3D_internal(Box3D *box, Line3D *line);
extern bool below_box3D_line3D_internal(Box3D *box, Line3D *line);
extern bool overbelow_box3D_line3D_internal(Box3D *box, Line3D *line);
extern bool above_box3D_line3D_internal(Box3D *box, Line3D *line);
extern bool overabove_box3D_line3D_internal(Box3D *box, Line3D *line);
extern bool front_box3D_line3D_internal(Box3D *box, Line3D *line);
extern bool overfront_box3D_line3D_internal(Box3D *box, Line3D *line);
extern bool back_box3D_line3D_internal(Box3D *box, Line3D *line);
extern bool overback_box3D_line3D_internal(Box3D *box, Line3D *line);

/* box3D op box3D */

extern Datum left_box3D_box3D(PG_FUNCTION_ARGS);
extern Datum overleft_box3D_box3D(PG_FUNCTION_ARGS);
extern Datum right_box3D_box3D(PG_FUNCTION_ARGS);
extern Datum overright_box3D_box3D(PG_FUNCTION_ARGS);
extern Datum below_box3D_box3D(PG_FUNCTION_ARGS);
extern Datum overbelow_box3D_box3D(PG_FUNCTION_ARGS);
extern Datum above_box3D_box3D(PG_FUNCTION_ARGS);
extern Datum overabove_box3D_box3D(PG_FUNCTION_ARGS);
extern Datum front_box3D_box3D(PG_FUNCTION_ARGS);
extern Datum overfront_box3D_box3D(PG_FUNCTION_ARGS);
extern Datum back_box3D_box3D(PG_FUNCTION_ARGS);
extern Datum overback_box3D_box3D(PG_FUNCTION_ARGS);

extern bool left_box3D_box3D_internal(Box3D *box1, Box3D *box2);
extern bool overleft_box3D_box3D_internal(Box3D *box1, Box3D *box2);
extern bool right_box3D_box3D_internal(Box3D *box1, Box3D *box2);
extern bool overright_box3D_box3D_internal(Box3D *box1, Box3D *box2);
extern bool below_box3D_box3D_internal(Box3D *box1, Box3D *box2);
extern bool overbelow_box3D_box3D_internal(Box3D *box1, Box3D *box2);
extern bool above_box3D_box3D_internal(Box3D *box1, Box3D *box2);
extern bool overabove_box3D_box3D_internal(Box3D *box1, Box3D *box2);
extern bool front_box3D_box3D_internal(Box3D *box1, Box3D *box2);
extern bool overfront_box3D_box3D_internal(Box3D *box1, Box3D *box2);
extern bool back_box3D_box3D_internal(Box3D *box1, Box3D *box2);
extern bool overback_box3D_box3D_internal(Box3D *box1, Box3D *box2);

/* box3D op path3D */

extern Datum left_box3D_path3D(PG_FUNCTION_ARGS);
extern Datum overleft_box3D_path3D(PG_FUNCTION_ARGS);
extern Datum right_box3D_path3D(PG_FUNCTION_ARGS);
extern Datum overright_box3D_path3D(PG_FUNCTION_ARGS);
extern Datum below_box3D_path3D(PG_FUNCTION_ARGS);
extern Datum overbelow_box3D_path3D(PG_FUNCTION_ARGS);
extern Datum above_box3D_path3D(PG_FUNCTION_ARGS);
extern Datum overabove_box3D_path3D(PG_FUNCTION_ARGS);
extern Datum front_box3D_path3D(PG_FUNCTION_ARGS);
extern Datum overfront_box3D_path3D(PG_FUNCTION_ARGS);
extern Datum back_box3D_path3D(PG_FUNCTION_ARGS);
extern Datum overback_box3D_path3D(PG_FUNCTION_ARGS);

extern bool left_box3D_path3D_internal(Box3D *box, Path3D *path);
extern bool overleft_box3D_path3D_internal(Box3D *box, Path3D *path);
extern bool right_box3D_path3D_internal(Box3D *box, Path3D *path);
extern bool overright_box3D_path3D_internal(Box3D *box, Path3D *path);
extern bool below_box3D_path3D_internal(Box3D *box, Path3D *path);
extern bool overbelow_box3D_path3D_internal(Box3D *box, Path3D *path);
extern bool above_box3D_path3D_internal(Box3D *box, Path3D *path);
extern bool overabove_box3D_path3D_internal(Box3D *box, Path3D *path);
extern bool front_box3D_path3D_internal(Box3D *box, Path3D *path);
extern bool overfront_box3D_path3D_internal(Box3D *box, Path3D *path);
extern bool back_box3D_path3D_internal(Box3D *box, Path3D *path);
extern bool overback_box3D_path3D_internal(Box3D *box, Path3D *path);

/* box3D op poly3D */

extern Datum left_box3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overleft_box3D_poly3D(PG_FUNCTION_ARGS);
extern Datum right_box3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overright_box3D_poly3D(PG_FUNCTION_ARGS);
extern Datum below_box3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overbelow_box3D_poly3D(PG_FUNCTION_ARGS);
extern Datum above_box3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overabove_box3D_poly3D(PG_FUNCTION_ARGS);
extern Datum front_box3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overfront_box3D_poly3D(PG_FUNCTION_ARGS);
extern Datum back_box3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overback_box3D_poly3D(PG_FUNCTION_ARGS);

extern bool left_box3D_poly3D_internal(Box3D *box, Polygon3D *poly);
extern bool overleft_box3D_poly3D_internal(Box3D *box, Polygon3D *poly);
extern bool right_box3D_poly3D_internal(Box3D *box, Polygon3D *poly);
extern bool overright_box3D_poly3D_internal(Box3D *box, Polygon3D *poly);
extern bool below_box3D_poly3D_internal(Box3D *box, Polygon3D *poly);
extern bool overbelow_box3D_poly3D_internal(Box3D *box, Polygon3D *poly);
extern bool above_box3D_poly3D_internal(Box3D *box, Polygon3D *poly);
extern bool overabove_box3D_poly3D_internal(Box3D *box, Polygon3D *poly);
extern bool front_box3D_poly3D_internal(Box3D *box, Polygon3D *poly);
extern bool overfront_box3D_poly3D_internal(Box3D *box, Polygon3D *poly);
extern bool back_box3D_poly3D_internal(Box3D *box, Polygon3D *poly);
extern bool overback_box3D_poly3D_internal(Box3D *box, Polygon3D *poly);

/* box3D op sphere */

extern Datum left_box3D_sphere(PG_FUNCTION_ARGS);
extern Datum overleft_box3D_sphere(PG_FUNCTION_ARGS);
extern Datum right_box3D_sphere(PG_FUNCTION_ARGS);
extern Datum overright_box3D_sphere(PG_FUNCTION_ARGS);
extern Datum below_box3D_sphere(PG_FUNCTION_ARGS);
extern Datum overbelow_box3D_sphere(PG_FUNCTION_ARGS);
extern Datum above_box3D_sphere(PG_FUNCTION_ARGS);
extern Datum overabove_box3D_sphere(PG_FUNCTION_ARGS);
extern Datum front_box3D_sphere(PG_FUNCTION_ARGS);
extern Datum overfront_box3D_sphere(PG_FUNCTION_ARGS);
extern Datum back_box3D_sphere(PG_FUNCTION_ARGS);
extern Datum overback_box3D_sphere(PG_FUNCTION_ARGS);

extern bool left_box3D_sphere_internal(Box3D *box, Sphere *sphere);
extern bool overleft_box3D_sphere_internal(Box3D *box, Sphere *sphere);
extern bool right_box3D_sphere_internal(Box3D *box, Sphere *sphere);
extern bool overright_box3D_sphere_internal(Box3D *box, Sphere *sphere);
extern bool below_box3D_sphere_internal(Box3D *box, Sphere *sphere);
extern bool overbelow_box3D_sphere_internal(Box3D *box, Sphere *sphere);
extern bool above_box3D_sphere_internal(Box3D *box, Sphere *sphere);
extern bool overabove_box3D_sphere_internal(Box3D *box, Sphere *sphere);
extern bool front_box3D_sphere_internal(Box3D *box, Sphere *sphere);
extern bool overfront_box3D_sphere_internal(Box3D *box, Sphere *sphere);
extern bool back_box3D_sphere_internal(Box3D *box, Sphere *sphere);
extern bool overback_box3D_sphere_internal(Box3D *box, Sphere *sphere);

/*----------------------------------------------------------*/

/* path3D op point3D */

extern Datum left_path3D_point3D(PG_FUNCTION_ARGS);
extern Datum overleft_path3D_point3D(PG_FUNCTION_ARGS);
extern Datum right_path3D_point3D(PG_FUNCTION_ARGS);
extern Datum overright_path3D_point3D(PG_FUNCTION_ARGS);
extern Datum below_path3D_point3D(PG_FUNCTION_ARGS);
extern Datum overbelow_path3D_point3D(PG_FUNCTION_ARGS);
extern Datum above_path3D_point3D(PG_FUNCTION_ARGS);
extern Datum overabove_path3D_point3D(PG_FUNCTION_ARGS);
extern Datum front_path3D_point3D(PG_FUNCTION_ARGS);
extern Datum overfront_path3D_point3D(PG_FUNCTION_ARGS);
extern Datum back_path3D_point3D(PG_FUNCTION_ARGS);
extern Datum overback_path3D_point3D(PG_FUNCTION_ARGS);

extern bool left_path3D_point3D_internal(Path3D *path, Point3D *point);
extern bool overleft_path3D_point3D_internal(Path3D *path, Point3D *point);
extern bool right_path3D_point3D_internal(Path3D *path, Point3D *point);
extern bool overright_path3D_point3D_internal(Path3D *path, Point3D *point);
extern bool below_path3D_point3D_internal(Path3D *path, Point3D *point);
extern bool overbelow_path3D_point3D_internal(Path3D *path, Point3D *point);
extern bool above_path3D_point3D_internal(Path3D *path, Point3D *point);
extern bool overabove_path3D_point3D_internal(Path3D *path, Point3D *point);
extern bool front_path3D_point3D_internal(Path3D *path, Point3D *point);
extern bool overfront_path3D_point3D_internal(Path3D *path, Point3D *point);
extern bool back_path3D_point3D_internal(Path3D *path, Point3D *point);
extern bool overback_path3D_point3D_internal(Path3D *path, Point3D *point);

/* path3D op lseg3D */

extern Datum left_path3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overleft_path3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum right_path3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overright_path3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum below_path3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overbelow_path3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum above_path3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overabove_path3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum front_path3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overfront_path3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum back_path3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overback_path3D_path3D(PG_FUNCTION_ARGS);
extern Datum overback_path3D_lseg3D(PG_FUNCTION_ARGS);

extern bool left_path3D_lseg3D_internal(Path3D *path, Lseg3D *lseg);
extern bool overleft_path3D_lseg3D_internal(Path3D *path, Lseg3D *lseg);
extern bool right_path3D_lseg3D_internal(Path3D *path, Lseg3D *lseg);
extern bool overright_path3D_lseg3D_internal(Path3D *path, Lseg3D *lseg);
extern bool below_path3D_lseg3D_internal(Path3D *path, Lseg3D *lseg);
extern bool overbelow_path3D_lseg3D_internal(Path3D *path, Lseg3D *lseg);
extern bool above_path3D_lseg3D_internal(Path3D *path, Lseg3D *lseg);
extern bool overabove_path3D_lseg3D_internal(Path3D *path, Lseg3D *lseg);
extern bool front_path3D_lseg3D_internal(Path3D *path, Lseg3D *lseg);
extern bool overfront_path3D_lseg3D_internal(Path3D *path, Lseg3D *lseg);
extern bool back_path3D_lseg3D_internal(Path3D *path, Lseg3D *lseg);
extern bool overback_path3D_lseg3D_internal(Path3D *path, Lseg3D *lseg);

/* path3D op line3D */

extern Datum left_path3D_line3D(PG_FUNCTION_ARGS);
extern Datum overleft_path3D_line3D(PG_FUNCTION_ARGS);
extern Datum right_path3D_line3D(PG_FUNCTION_ARGS);
extern Datum overright_path3D_line3D(PG_FUNCTION_ARGS);
extern Datum below_path3D_line3D(PG_FUNCTION_ARGS);
extern Datum overbelow_path3D_line3D(PG_FUNCTION_ARGS);
extern Datum above_path3D_line3D(PG_FUNCTION_ARGS);
extern Datum overabove_path3D_line3D(PG_FUNCTION_ARGS);
extern Datum front_path3D_line3D(PG_FUNCTION_ARGS);
extern Datum overfront_path3D_line3D(PG_FUNCTION_ARGS);
extern Datum back_path3D_line3D(PG_FUNCTION_ARGS);
extern Datum overback_path3D_path3D(PG_FUNCTION_ARGS);
extern Datum overback_path3D_line3D(PG_FUNCTION_ARGS);

extern bool left_path3D_line3D_internal(Path3D *path, Line3D *line);
extern bool overleft_path3D_line3D_internal(Path3D *path, Line3D *line);
extern bool right_path3D_line3D_internal(Path3D *path, Line3D *line);
extern bool overright_path3D_line3D_internal(Path3D *path, Line3D *line);
extern bool below_path3D_line3D_internal(Path3D *path, Line3D *line);
extern bool overbelow_path3D_line3D_internal(Path3D *path, Line3D *line);
extern bool above_path3D_line3D_internal(Path3D *path, Line3D *line);
extern bool overabove_path3D_line3D_internal(Path3D *path, Line3D *line);
extern bool front_path3D_line3D_internal(Path3D *path, Line3D *line);
extern bool overfront_path3D_line3D_internal(Path3D *path, Line3D *line);
extern bool back_path3D_line3D_internal(Path3D *path, Line3D *line);
extern bool overback_path3D_line3D_internal(Path3D *path, Line3D *line);

/* path3D op box3D */

extern Datum left_path3D_box3D(PG_FUNCTION_ARGS);
extern Datum overleft_path3D_box3D(PG_FUNCTION_ARGS);
extern Datum right_path3D_box3D(PG_FUNCTION_ARGS);
extern Datum overright_path3D_box3D(PG_FUNCTION_ARGS);
extern Datum below_path3D_box3D(PG_FUNCTION_ARGS);
extern Datum overbelow_path3D_box3D(PG_FUNCTION_ARGS);
extern Datum above_path3D_box3D(PG_FUNCTION_ARGS);
extern Datum overabove_path3D_box3D(PG_FUNCTION_ARGS);
extern Datum front_path3D_box3D(PG_FUNCTION_ARGS);
extern Datum overfront_path3D_box3D(PG_FUNCTION_ARGS);
extern Datum back_path3D_box3D(PG_FUNCTION_ARGS);
extern Datum overback_path3D_box3D(PG_FUNCTION_ARGS);

extern bool left_path3D_box3D_internal(Path3D *path, Box3D *box);
extern bool overleft_path3D_box3D_internal(Path3D *path, Box3D *box);
extern bool right_path3D_box3D_internal(Path3D *path, Box3D *box);
extern bool overright_path3D_box3D_internal(Path3D *path, Box3D *box);
extern bool below_path3D_box3D_internal(Path3D *path, Box3D *box);
extern bool overbelow_path3D_box3D_internal(Path3D *path, Box3D *box);
extern bool above_path3D_box3D_internal(Path3D *path, Box3D *box);
extern bool overabove_path3D_box3D_internal(Path3D *path, Box3D *box);
extern bool front_path3D_box3D_internal(Path3D *path, Box3D *box);
extern bool overfront_path3D_box3D_internal(Path3D *path, Box3D *box);
extern bool back_path3D_box3D_internal(Path3D *path, Box3D *box);
extern bool overback_path3D_box3D_internal(Path3D *path, Box3D *box);

/* path3D op path3D */

extern Datum left_path3D_path3D(PG_FUNCTION_ARGS);
extern Datum overleft_path3D_path3D(PG_FUNCTION_ARGS);
extern Datum right_path3D_path3D(PG_FUNCTION_ARGS);
extern Datum overright_path3D_path3D(PG_FUNCTION_ARGS);
extern Datum below_path3D_path3D(PG_FUNCTION_ARGS);
extern Datum overbelow_path3D_path3D(PG_FUNCTION_ARGS);
extern Datum above_path3D_path3D(PG_FUNCTION_ARGS);
extern Datum overabove_path3D_path3D(PG_FUNCTION_ARGS);
extern Datum front_path3D_path3D(PG_FUNCTION_ARGS);
extern Datum overfront_path3D_path3D(PG_FUNCTION_ARGS);
extern Datum back_path3D_path3D(PG_FUNCTION_ARGS);
extern Datum overback_path3D_path3D(PG_FUNCTION_ARGS);

extern bool left_path3D_path3D_internal(Path3D *path1, Path3D *path2);
extern bool overleft_path3D_path3D_internal(Path3D *path1, Path3D *path2);
extern bool right_path3D_path3D_internal(Path3D *path1, Path3D *path2);
extern bool overright_path3D_path3D_internal(Path3D *path1, Path3D *path2);
extern bool below_path3D_path3D_internal(Path3D *path1, Path3D *path2);
extern bool overbelow_path3D_path3D_internal(Path3D *path1, Path3D *path2);
extern bool above_path3D_path3D_internal(Path3D *path1, Path3D *path2);
extern bool overabove_path3D_path3D_internal(Path3D *path1, Path3D *path2);
extern bool front_path3D_path3D_internal(Path3D *path1, Path3D *path2);
extern bool overfront_path3D_path3D_internal(Path3D *path1, Path3D *path2);
extern bool back_path3D_path3D_internal(Path3D *path1, Path3D *path2);
extern bool overback_path3D_path3D_internal(Path3D *path1, Path3D *path2);

/* path3D op poly3D */

extern Datum left_path3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overleft_path3D_poly3D(PG_FUNCTION_ARGS);
extern Datum right_path3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overright_path3D_poly3D(PG_FUNCTION_ARGS);
extern Datum below_path3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overbelow_path3D_poly3D(PG_FUNCTION_ARGS);
extern Datum above_path3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overabove_path3D_poly3D(PG_FUNCTION_ARGS);
extern Datum front_path3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overfront_path3D_poly3D(PG_FUNCTION_ARGS);
extern Datum back_path3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overback_path3D_poly3D(PG_FUNCTION_ARGS);

extern bool left_path3D_poly3D_internal(Path3D *path, Polygon3D *poly);
extern bool overleft_path3D_poly3D_internal(Path3D *path, Polygon3D *poly);
extern bool right_path3D_poly3D_internal(Path3D *path, Polygon3D *poly);
extern bool overright_path3D_poly3D_internal(Path3D *path, Polygon3D *poly);
extern bool below_path3D_poly3D_internal(Path3D *path, Polygon3D *poly);
extern bool overbelow_path3D_poly3D_internal(Path3D *path, Polygon3D *poly);
extern bool above_path3D_poly3D_internal(Path3D *path, Polygon3D *poly);
extern bool overabove_path3D_poly3D_internal(Path3D *path, Polygon3D *poly);
extern bool front_path3D_poly3D_internal(Path3D *path, Polygon3D *poly);
extern bool overfront_path3D_poly3D_internal(Path3D *path, Polygon3D *poly);
extern bool back_path3D_poly3D_internal(Path3D *path, Polygon3D *poly);
extern bool overback_path3D_poly3D_internal(Path3D *path, Polygon3D *poly);

/* path3D op sphere */

extern Datum left_path3D_sphere(PG_FUNCTION_ARGS);
extern Datum overleft_path3D_sphere(PG_FUNCTION_ARGS);
extern Datum right_path3D_sphere(PG_FUNCTION_ARGS);
extern Datum overright_path3D_sphere(PG_FUNCTION_ARGS);
extern Datum below_path3D_sphere(PG_FUNCTION_ARGS);
extern Datum overbelow_path3D_sphere(PG_FUNCTION_ARGS);
extern Datum above_path3D_sphere(PG_FUNCTION_ARGS);
extern Datum overabove_path3D_sphere(PG_FUNCTION_ARGS);
extern Datum front_path3D_sphere(PG_FUNCTION_ARGS);
extern Datum overfront_path3D_sphere(PG_FUNCTION_ARGS);
extern Datum back_path3D_sphere(PG_FUNCTION_ARGS);
extern Datum overback_path3D_sphere(PG_FUNCTION_ARGS);

extern bool left_path3D_sphere_internal(Path3D *path, Sphere *sphere);
extern bool overleft_path3D_sphere_internal(Path3D *path, Sphere *sphere);
extern bool right_path3D_sphere_internal(Path3D *path, Sphere *sphere);
extern bool overright_path3D_sphere_internal(Path3D *path, Sphere *sphere);
extern bool below_path3D_sphere_internal(Path3D *path, Sphere *sphere);
extern bool overbelow_path3D_sphere_internal(Path3D *path, Sphere *sphere);
extern bool above_path3D_sphere_internal(Path3D *path, Sphere *sphere);
extern bool overabove_path3D_sphere_internal(Path3D *path, Sphere *sphere);
extern bool front_path3D_sphere_internal(Path3D *path, Sphere *sphere);
extern bool overfront_path3D_sphere_internal(Path3D *path, Sphere *sphere);
extern bool back_path3D_sphere_internal(Path3D *path, Sphere *sphere);
extern bool overback_path3D_sphere_internal(Path3D *path, Sphere *sphere);

/*----------------------------------------------------------*/

/* poly3D op point3D */

extern Datum left_poly3D_point3D(PG_FUNCTION_ARGS);
extern Datum overleft_poly3D_point3D(PG_FUNCTION_ARGS);
extern Datum right_poly3D_point3D(PG_FUNCTION_ARGS);
extern Datum overright_poly3D_point3D(PG_FUNCTION_ARGS);
extern Datum below_poly3D_point3D(PG_FUNCTION_ARGS);
extern Datum overbelow_poly3D_point3D(PG_FUNCTION_ARGS);
extern Datum above_poly3D_point3D(PG_FUNCTION_ARGS);
extern Datum overabove_poly3D_point3D(PG_FUNCTION_ARGS);
extern Datum front_poly3D_point3D(PG_FUNCTION_ARGS);
extern Datum overfront_poly3D_point3D(PG_FUNCTION_ARGS);
extern Datum back_poly3D_point3D(PG_FUNCTION_ARGS);
extern Datum overback_poly3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overback_poly3D_point3D(PG_FUNCTION_ARGS);

extern bool left_poly3D_point3D_internal(Polygon3D *poly, Point3D *point);
extern bool overleft_poly3D_point3D_internal(Polygon3D *poly, Point3D *point);
extern bool right_poly3D_point3D_internal(Polygon3D *poly, Point3D *point);
extern bool overright_poly3D_point3D_internal(Polygon3D *poly, Point3D *point);
extern bool below_poly3D_point3D_internal(Polygon3D *poly, Point3D *point);
extern bool overbelow_poly3D_point3D_internal(Polygon3D *poly, Point3D *point);
extern bool above_poly3D_point3D_internal(Polygon3D *poly, Point3D *point);
extern bool overabove_poly3D_point3D_internal(Polygon3D *poly, Point3D *point);
extern bool front_poly3D_point3D_internal(Polygon3D *poly, Point3D *point);
extern bool overfront_poly3D_point3D_internal(Polygon3D *poly, Point3D *point);
extern bool back_poly3D_point3D_internal(Polygon3D *poly, Point3D *point);
extern bool overback_poly3D_point3D_internal(Polygon3D *poly, Point3D *point);

/* poly3D op lseg3D */

extern Datum left_poly3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overleft_poly3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum right_poly3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overright_poly3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum below_poly3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overbelow_poly3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum above_poly3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overabove_poly3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum front_poly3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overfront_poly3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum back_poly3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum overback_poly3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overback_poly3D_lseg3D(PG_FUNCTION_ARGS);

extern bool left_poly3D_lseg3D_internal(Polygon3D *poly, Lseg3D *lseg);
extern bool overleft_poly3D_lseg3D_internal(Polygon3D *poly, Lseg3D *lseg);
extern bool right_poly3D_lseg3D_internal(Polygon3D *poly, Lseg3D *lseg);
extern bool overright_poly3D_lseg3D_internal(Polygon3D *poly, Lseg3D *lseg);
extern bool below_poly3D_lseg3D_internal(Polygon3D *poly, Lseg3D *lseg);
extern bool overbelow_poly3D_lseg3D_internal(Polygon3D *poly, Lseg3D *lseg);
extern bool above_poly3D_lseg3D_internal(Polygon3D *poly, Lseg3D *lseg);
extern bool overabove_poly3D_lseg3D_internal(Polygon3D *poly, Lseg3D *lseg);
extern bool front_poly3D_lseg3D_internal(Polygon3D *poly, Lseg3D *lseg);
extern bool overfront_poly3D_lseg3D_internal(Polygon3D *poly, Lseg3D *lseg);
extern bool back_poly3D_lseg3D_internal(Polygon3D *poly, Lseg3D *lseg);
extern bool overback_poly3D_lseg3D_internal(Polygon3D *poly, Lseg3D *lseg);

/* poly3D op line3D */

extern Datum left_poly3D_line3D(PG_FUNCTION_ARGS);
extern Datum overleft_poly3D_line3D(PG_FUNCTION_ARGS);
extern Datum right_poly3D_line3D(PG_FUNCTION_ARGS);
extern Datum overright_poly3D_line3D(PG_FUNCTION_ARGS);
extern Datum below_poly3D_line3D(PG_FUNCTION_ARGS);
extern Datum overbelow_poly3D_line3D(PG_FUNCTION_ARGS);
extern Datum above_poly3D_line3D(PG_FUNCTION_ARGS);
extern Datum overabove_poly3D_line3D(PG_FUNCTION_ARGS);
extern Datum front_poly3D_line3D(PG_FUNCTION_ARGS);
extern Datum overfront_poly3D_line3D(PG_FUNCTION_ARGS);
extern Datum back_poly3D_line3D(PG_FUNCTION_ARGS);
extern Datum overback_poly3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overback_poly3D_line3D(PG_FUNCTION_ARGS);

extern bool left_poly3D_line3D_internal(Polygon3D *poly, Line3D *line);
extern bool overleft_poly3D_line3D_internal(Polygon3D *poly, Line3D *line);
extern bool right_poly3D_line3D_internal(Polygon3D *poly, Line3D *line);
extern bool overright_poly3D_line3D_internal(Polygon3D *poly, Line3D *line);
extern bool below_poly3D_line3D_internal(Polygon3D *poly, Line3D *line);
extern bool overbelow_poly3D_line3D_internal(Polygon3D *poly, Line3D *line);
extern bool above_poly3D_line3D_internal(Polygon3D *poly, Line3D *line);
extern bool overabove_poly3D_line3D_internal(Polygon3D *poly, Line3D *line);
extern bool front_poly3D_line3D_internal(Polygon3D *poly, Line3D *line);
extern bool overfront_poly3D_line3D_internal(Polygon3D *poly, Line3D *line);
extern bool back_poly3D_line3D_internal(Polygon3D *poly, Line3D *line);
extern bool overback_poly3D_line3D_internal(Polygon3D *poly, Line3D *line);

/* poly3D op box3D */

extern Datum left_poly3D_box3D(PG_FUNCTION_ARGS);
extern Datum overleft_poly3D_box3D(PG_FUNCTION_ARGS);
extern Datum right_poly3D_box3D(PG_FUNCTION_ARGS);
extern Datum overright_poly3D_box3D(PG_FUNCTION_ARGS);
extern Datum below_poly3D_box3D(PG_FUNCTION_ARGS);
extern Datum overbelow_poly3D_box3D(PG_FUNCTION_ARGS);
extern Datum above_poly3D_box3D(PG_FUNCTION_ARGS);
extern Datum overabove_poly3D_box3D(PG_FUNCTION_ARGS);
extern Datum front_poly3D_box3D(PG_FUNCTION_ARGS);
extern Datum overfront_poly3D_box3D(PG_FUNCTION_ARGS);
extern Datum back_poly3D_box3D(PG_FUNCTION_ARGS);
extern Datum overback_poly3D_box3D(PG_FUNCTION_ARGS);

extern bool left_poly3D_box3D_internal(Polygon3D *poly, Box3D *box);
extern bool overleft_poly3D_box3D_internal(Polygon3D *poly, Box3D *box);
extern bool right_poly3D_box3D_internal(Polygon3D *poly, Box3D *box);
extern bool overright_poly3D_box3D_internal(Polygon3D *poly, Box3D *box);
extern bool below_poly3D_box3D_internal(Polygon3D *poly, Box3D *box);
extern bool overbelow_poly3D_box3D_internal(Polygon3D *poly, Box3D *box);
extern bool above_poly3D_box3D_internal(Polygon3D *poly, Box3D *box);
extern bool overabove_poly3D_box3D_internal(Polygon3D *poly, Box3D *box);
extern bool front_poly3D_box3D_internal(Polygon3D *poly, Box3D *box);
extern bool overfront_poly3D_box3D_internal(Polygon3D *poly, Box3D *box);
extern bool back_poly3D_box3D_internal(Polygon3D *poly, Box3D *box);
extern bool overback_poly3D_box3D_internal(Polygon3D *poly, Box3D *box);

/* poly3D op path3D */

extern Datum left_poly3D_path3D(PG_FUNCTION_ARGS);
extern Datum overleft_poly3D_path3D(PG_FUNCTION_ARGS);
extern Datum right_poly3D_path3D(PG_FUNCTION_ARGS);
extern Datum overright_poly3D_path3D(PG_FUNCTION_ARGS);
extern Datum below_poly3D_path3D(PG_FUNCTION_ARGS);
extern Datum overbelow_poly3D_path3D(PG_FUNCTION_ARGS);
extern Datum above_poly3D_path3D(PG_FUNCTION_ARGS);
extern Datum overabove_poly3D_path3D(PG_FUNCTION_ARGS);
extern Datum front_poly3D_path3D(PG_FUNCTION_ARGS);
extern Datum overfront_poly3D_path3D(PG_FUNCTION_ARGS);
extern Datum back_poly3D_path3D(PG_FUNCTION_ARGS);
extern Datum overback_poly3D_path3D(PG_FUNCTION_ARGS);

extern bool left_poly3D_path3D_internal(Polygon3D *poly, Path3D *path);
extern bool overleft_poly3D_path3D_internal(Polygon3D *poly, Path3D *path);
extern bool right_poly3D_path3D_internal(Polygon3D *poly, Path3D *path);
extern bool overright_poly3D_path3D_internal(Polygon3D *poly, Path3D *path);
extern bool below_poly3D_path3D_internal(Polygon3D *poly, Path3D *path);
extern bool overbelow_poly3D_path3D_internal(Polygon3D *poly, Path3D *path);
extern bool above_poly3D_path3D_internal(Polygon3D *poly, Path3D *path);
extern bool overabove_poly3D_path3D_internal(Polygon3D *poly, Path3D *path);
extern bool front_poly3D_path3D_internal(Polygon3D *poly, Path3D *path);
extern bool overfront_poly3D_path3D_internal(Polygon3D *poly, Path3D *path);
extern bool back_poly3D_path3D_internal(Polygon3D *poly, Path3D *path);
extern bool overback_poly3D_path3D_internal(Polygon3D *poly, Path3D *path);

/* poly3D op poly3D */

extern Datum left_poly3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overleft_poly3D_poly3D(PG_FUNCTION_ARGS);
extern Datum right_poly3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overright_poly3D_poly3D(PG_FUNCTION_ARGS);
extern Datum below_poly3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overbelow_poly3D_poly3D(PG_FUNCTION_ARGS);
extern Datum above_poly3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overabove_poly3D_poly3D(PG_FUNCTION_ARGS);
extern Datum front_poly3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overfront_poly3D_poly3D(PG_FUNCTION_ARGS);
extern Datum back_poly3D_poly3D(PG_FUNCTION_ARGS);
extern Datum overback_poly3D_poly3D(PG_FUNCTION_ARGS);

extern bool left_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2);
extern bool overleft_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2);
extern bool right_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2);
extern bool overright_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2);
extern bool below_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2);
extern bool overbelow_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2);
extern bool above_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2);
extern bool overabove_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2);
extern bool front_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2);
extern bool overfront_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2);
extern bool back_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2);
extern bool overback_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2);

/* poly3D op sphere */

extern Datum left_poly3D_sphere(PG_FUNCTION_ARGS);
extern Datum overleft_poly3D_sphere(PG_FUNCTION_ARGS);
extern Datum right_poly3D_sphere(PG_FUNCTION_ARGS);
extern Datum overright_poly3D_sphere(PG_FUNCTION_ARGS);
extern Datum below_poly3D_sphere(PG_FUNCTION_ARGS);
extern Datum overbelow_poly3D_sphere(PG_FUNCTION_ARGS);
extern Datum above_poly3D_sphere(PG_FUNCTION_ARGS);
extern Datum overabove_poly3D_sphere(PG_FUNCTION_ARGS);
extern Datum front_poly3D_sphere(PG_FUNCTION_ARGS);
extern Datum overfront_poly3D_sphere(PG_FUNCTION_ARGS);
extern Datum back_poly3D_sphere(PG_FUNCTION_ARGS);
extern Datum overback_poly3D_sphere(PG_FUNCTION_ARGS);

extern bool left_poly3D_sphere_internal(Polygon3D *poly, Sphere *sphere);
extern bool overleft_poly3D_sphere_internal(Polygon3D *poly, Sphere *sphere);
extern bool right_poly3D_sphere_internal(Polygon3D *poly, Sphere *sphere);
extern bool overright_poly3D_sphere_internal(Polygon3D *poly, Sphere *sphere);
extern bool below_poly3D_sphere_internal(Polygon3D *poly, Sphere *sphere);
extern bool overbelow_poly3D_sphere_internal(Polygon3D *poly, Sphere *sphere);
extern bool above_poly3D_sphere_internal(Polygon3D *poly, Sphere *sphere);
extern bool overabove_poly3D_sphere_internal(Polygon3D *poly, Sphere *sphere);
extern bool front_poly3D_sphere_internal(Polygon3D *poly, Sphere *sphere);
extern bool overfront_poly3D_sphere_internal(Polygon3D *poly, Sphere *sphere);
extern bool back_poly3D_sphere_internal(Polygon3D *poly, Sphere *sphere);
extern bool overback_poly3D_sphere_internal(Polygon3D *poly, Sphere *sphere);

/*----------------------------------------------------------*/

/* sphere op point3D */

extern Datum left_sphere_point3D(PG_FUNCTION_ARGS);
extern Datum overleft_sphere_point3D(PG_FUNCTION_ARGS);
extern Datum right_sphere_point3D(PG_FUNCTION_ARGS);
extern Datum overright_sphere_point3D(PG_FUNCTION_ARGS);
extern Datum below_sphere_point3D(PG_FUNCTION_ARGS);
extern Datum overbelow_sphere_point3D(PG_FUNCTION_ARGS);
extern Datum above_sphere_point3D(PG_FUNCTION_ARGS);
extern Datum overabove_sphere_point3D(PG_FUNCTION_ARGS);
extern Datum front_sphere_point3D(PG_FUNCTION_ARGS);
extern Datum overfront_sphere_point3D(PG_FUNCTION_ARGS);
extern Datum back_sphere_point3D(PG_FUNCTION_ARGS);
extern Datum overback_sphere_point3D(PG_FUNCTION_ARGS);

extern bool left_sphere_point3D_internal(Sphere *sphere, Point3D *point);
extern bool overleft_sphere_point3D_internal(Sphere *sphere, Point3D *point);
extern bool right_sphere_point3D_internal(Sphere *sphere, Point3D *point);
extern bool overright_sphere_point3D_internal(Sphere *sphere, Point3D *point);
extern bool below_sphere_point3D_internal(Sphere *sphere, Point3D *point);
extern bool overbelow_sphere_point3D_internal(Sphere *sphere, Point3D *point);
extern bool above_sphere_point3D_internal(Sphere *sphere, Point3D *point);
extern bool overabove_sphere_point3D_internal(Sphere *sphere, Point3D *point);
extern bool front_sphere_point3D_internal(Sphere *sphere, Point3D *point);
extern bool overfront_sphere_point3D_internal(Sphere *sphere, Point3D *point);
extern bool back_sphere_point3D_internal(Sphere *sphere, Point3D *point);
extern bool overback_sphere_point3D_internal(Sphere *sphere, Point3D *point);

/* sphere op lseg3D */

extern Datum left_sphere_lseg3D(PG_FUNCTION_ARGS);
extern Datum overleft_sphere_lseg3D(PG_FUNCTION_ARGS);
extern Datum right_sphere_lseg3D(PG_FUNCTION_ARGS);
extern Datum overright_sphere_lseg3D(PG_FUNCTION_ARGS);
extern Datum below_sphere_lseg3D(PG_FUNCTION_ARGS);
extern Datum overbelow_sphere_lseg3D(PG_FUNCTION_ARGS);
extern Datum above_sphere_lseg3D(PG_FUNCTION_ARGS);
extern Datum overabove_sphere_lseg3D(PG_FUNCTION_ARGS);
extern Datum front_sphere_lseg3D(PG_FUNCTION_ARGS);
extern Datum overfront_sphere_lseg3D(PG_FUNCTION_ARGS);
extern Datum back_sphere_lseg3D(PG_FUNCTION_ARGS);
extern Datum overback_sphere_lseg3D(PG_FUNCTION_ARGS);

extern bool left_sphere_lseg3D_internal(Sphere *sphere, Lseg3D *lseg);
extern bool overleft_sphere_lseg3D_internal(Sphere *sphere, Lseg3D *lseg);
extern bool right_sphere_lseg3D_internal(Sphere *sphere, Lseg3D *lseg);
extern bool overright_sphere_lseg3D_internal(Sphere *sphere, Lseg3D *lseg);
extern bool below_sphere_lseg3D_internal(Sphere *sphere, Lseg3D *lseg);
extern bool overbelow_sphere_lseg3D_internal(Sphere *sphere, Lseg3D *lseg);
extern bool above_sphere_lseg3D_internal(Sphere *sphere, Lseg3D *lseg);
extern bool overabove_sphere_lseg3D_internal(Sphere *sphere, Lseg3D *lseg);
extern bool front_sphere_lseg3D_internal(Sphere *sphere, Lseg3D *lseg);
extern bool overfront_sphere_lseg3D_internal(Sphere *sphere, Lseg3D *lseg);
extern bool back_sphere_lseg3D_internal(Sphere *sphere, Lseg3D *lseg);
extern bool overback_sphere_lseg3D_internal(Sphere *sphere, Lseg3D *lseg);

/* sphere op line3D */

extern Datum left_sphere_line3D(PG_FUNCTION_ARGS);
extern Datum overleft_sphere_line3D(PG_FUNCTION_ARGS);
extern Datum right_sphere_line3D(PG_FUNCTION_ARGS);
extern Datum overright_sphere_line3D(PG_FUNCTION_ARGS);
extern Datum below_sphere_line3D(PG_FUNCTION_ARGS);
extern Datum overbelow_sphere_line3D(PG_FUNCTION_ARGS);
extern Datum above_sphere_line3D(PG_FUNCTION_ARGS);
extern Datum overabove_sphere_line3D(PG_FUNCTION_ARGS);
extern Datum front_sphere_line3D(PG_FUNCTION_ARGS);
extern Datum overfront_sphere_line3D(PG_FUNCTION_ARGS);
extern Datum back_sphere_line3D(PG_FUNCTION_ARGS);
extern Datum overback_sphere_line3D(PG_FUNCTION_ARGS);

extern bool left_sphere_line3D_internal(Sphere *sphere, Line3D *line);
extern bool overleft_sphere_line3D_internal(Sphere *sphere, Line3D *line);
extern bool right_sphere_line3D_internal(Sphere *sphere, Line3D *line);
extern bool overright_sphere_line3D_internal(Sphere *sphere, Line3D *line);
extern bool below_sphere_line3D_internal(Sphere *sphere, Line3D *line);
extern bool overbelow_sphere_line3D_internal(Sphere *sphere, Line3D *line);
extern bool above_sphere_line3D_internal(Sphere *sphere, Line3D *line);
extern bool overabove_sphere_line3D_internal(Sphere *sphere, Line3D *line);
extern bool front_sphere_line3D_internal(Sphere *sphere, Line3D *line);
extern bool overfront_sphere_line3D_internal(Sphere *sphere, Line3D *line);
extern bool back_sphere_line3D_internal(Sphere *sphere, Line3D *line);
extern bool overback_sphere_line3D_internal(Sphere *sphere, Line3D *line);

/* sphere op box3D */

extern Datum left_sphere_box3D(PG_FUNCTION_ARGS);
extern Datum overleft_sphere_box3D(PG_FUNCTION_ARGS);
extern Datum right_sphere_box3D(PG_FUNCTION_ARGS);
extern Datum overright_sphere_box3D(PG_FUNCTION_ARGS);
extern Datum below_sphere_box3D(PG_FUNCTION_ARGS);
extern Datum overbelow_sphere_box3D(PG_FUNCTION_ARGS);
extern Datum above_sphere_box3D(PG_FUNCTION_ARGS);
extern Datum overabove_sphere_box3D(PG_FUNCTION_ARGS);
extern Datum front_sphere_box3D(PG_FUNCTION_ARGS);
extern Datum overfront_sphere_box3D(PG_FUNCTION_ARGS);
extern Datum back_sphere_box3D(PG_FUNCTION_ARGS);
extern Datum overback_sphere_box3D(PG_FUNCTION_ARGS);

extern bool left_sphere_box3D_internal(Sphere *sphere, Box3D *box);
extern bool overleft_sphere_box3D_internal(Sphere *sphere, Box3D *box);
extern bool right_sphere_box3D_internal(Sphere *sphere, Box3D *box);
extern bool overright_sphere_box3D_internal(Sphere *sphere, Box3D *box);
extern bool below_sphere_box3D_internal(Sphere *sphere, Box3D *box);
extern bool overbelow_sphere_box3D_internal(Sphere *sphere, Box3D *box);
extern bool above_sphere_box3D_internal(Sphere *sphere, Box3D *box);
extern bool overabove_sphere_box3D_internal(Sphere *sphere, Box3D *box);
extern bool front_sphere_box3D_internal(Sphere *sphere, Box3D *box);
extern bool overfront_sphere_box3D_internal(Sphere *sphere, Box3D *box);
extern bool back_sphere_box3D_internal(Sphere *sphere, Box3D *box);
extern bool overback_sphere_box3D_internal(Sphere *sphere, Box3D *box);

/* sphere op path3D */

extern Datum left_sphere_path3D(PG_FUNCTION_ARGS);
extern Datum overleft_sphere_path3D(PG_FUNCTION_ARGS);
extern Datum right_sphere_path3D(PG_FUNCTION_ARGS);
extern Datum overright_sphere_path3D(PG_FUNCTION_ARGS);
extern Datum below_sphere_path3D(PG_FUNCTION_ARGS);
extern Datum overbelow_sphere_path3D(PG_FUNCTION_ARGS);
extern Datum above_sphere_path3D(PG_FUNCTION_ARGS);
extern Datum overabove_sphere_path3D(PG_FUNCTION_ARGS);
extern Datum front_sphere_path3D(PG_FUNCTION_ARGS);
extern Datum overfront_sphere_path3D(PG_FUNCTION_ARGS);
extern Datum back_sphere_path3D(PG_FUNCTION_ARGS);
extern Datum overback_sphere_path3D(PG_FUNCTION_ARGS);

extern bool left_sphere_path3D_internal(Sphere *sphere, Path3D *path);
extern bool overleft_sphere_path3D_internal(Sphere *sphere, Path3D *path);
extern bool right_sphere_path3D_internal(Sphere *sphere, Path3D *path);
extern bool overright_sphere_path3D_internal(Sphere *sphere, Path3D *path);
extern bool below_sphere_path3D_internal(Sphere *sphere, Path3D *path);
extern bool overbelow_sphere_path3D_internal(Sphere *sphere, Path3D *path);
extern bool above_sphere_path3D_internal(Sphere *sphere, Path3D *path);
extern bool overabove_sphere_path3D_internal(Sphere *sphere, Path3D *path);
extern bool front_sphere_path3D_internal(Sphere *sphere, Path3D *path);
extern bool overfront_sphere_path3D_internal(Sphere *sphere, Path3D *path);
extern bool back_sphere_path3D_internal(Sphere *sphere, Path3D *path);
extern bool overback_sphere_path3D_internal(Sphere *sphere, Path3D *path);

/* sphere op poly3D */

extern Datum left_sphere_poly3D(PG_FUNCTION_ARGS);
extern Datum overleft_sphere_poly3D(PG_FUNCTION_ARGS);
extern Datum right_sphere_poly3D(PG_FUNCTION_ARGS);
extern Datum overright_sphere_poly3D(PG_FUNCTION_ARGS);
extern Datum below_sphere_poly3D(PG_FUNCTION_ARGS);
extern Datum overbelow_sphere_poly3D(PG_FUNCTION_ARGS);
extern Datum above_sphere_poly3D(PG_FUNCTION_ARGS);
extern Datum overabove_sphere_poly3D(PG_FUNCTION_ARGS);
extern Datum front_sphere_poly3D(PG_FUNCTION_ARGS);
extern Datum overfront_sphere_poly3D(PG_FUNCTION_ARGS);
extern Datum back_sphere_poly3D(PG_FUNCTION_ARGS);
extern Datum overback_sphere_poly3D(PG_FUNCTION_ARGS);

extern bool left_sphere_poly3D_internal(Sphere *sphere, Polygon3D *poly);
extern bool overleft_sphere_poly3D_internal(Sphere *sphere, Polygon3D *poly);
extern bool right_sphere_poly3D_internal(Sphere *sphere, Polygon3D *poly);
extern bool overright_sphere_poly3D_internal(Sphere *sphere, Polygon3D *poly);
extern bool below_sphere_poly3D_internal(Sphere *sphere, Polygon3D *poly);
extern bool overbelow_sphere_poly3D_internal(Sphere *sphere, Polygon3D *poly);
extern bool above_sphere_poly3D_internal(Sphere *sphere, Polygon3D *poly);
extern bool overabove_sphere_poly3D_internal(Sphere *sphere, Polygon3D *poly);
extern bool front_sphere_poly3D_internal(Sphere *sphere, Polygon3D *poly);
extern bool overfront_sphere_poly3D_internal(Sphere *sphere, Polygon3D *poly);
extern bool back_sphere_poly3D_internal(Sphere *sphere, Polygon3D *poly);
extern bool overback_sphere_poly3D_internal(Sphere *sphere, Polygon3D *poly);

/* sphere op sphere */

extern Datum overleft_sphere_sphere(PG_FUNCTION_ARGS);
extern Datum left_sphere_sphere(PG_FUNCTION_ARGS);
extern Datum right_sphere_sphere(PG_FUNCTION_ARGS);
extern Datum overright_sphere_sphere(PG_FUNCTION_ARGS);
extern Datum front_sphere_sphere(PG_FUNCTION_ARGS);
extern Datum overfront_sphere_sphere(PG_FUNCTION_ARGS);
extern Datum back_sphere_sphere(PG_FUNCTION_ARGS);
extern Datum overback_sphere_sphere(PG_FUNCTION_ARGS);
extern Datum below_sphere_sphere(PG_FUNCTION_ARGS);
extern Datum above_sphere_sphere(PG_FUNCTION_ARGS);
extern Datum overbelow_sphere_sphere(PG_FUNCTION_ARGS);
extern Datum overabove_sphere_sphere(PG_FUNCTION_ARGS);

extern bool overleft_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2);
extern bool left_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2);
extern bool right_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2);
extern bool overright_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2);
extern bool front_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2);
extern bool overfront_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2);
extern bool back_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2);
extern bool overback_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2);
extern bool below_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2);
extern bool above_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2);
extern bool overbelow_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2);
extern bool overabove_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2);

/*****************************************************************************
 *	Distance functions
 *****************************************************************************/

extern Datum distance_point3D_point3D(PG_FUNCTION_ARGS);
extern Datum distance_point3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum distance_point3D_line3D(PG_FUNCTION_ARGS);
extern Datum distance_point3D_box3D(PG_FUNCTION_ARGS);
extern Datum distance_point3D_path3D(PG_FUNCTION_ARGS);
extern Datum distance_point3D_poly3D(PG_FUNCTION_ARGS);
extern Datum distance_point3D_sphere(PG_FUNCTION_ARGS);

extern double dist_point3D_point3D(Point3D *point1, Point3D *point2);
extern double dist_point3D_lseg3D(Point3D *point, Lseg3D *lseg);
extern double dist_point3D_line3D(Point3D *point, Line3D *line);
extern double dist_point3D_box3D(Point3D *point, Box3D *box);
extern double dist_point3D_path3D(Point3D *point, Path3D *path);
extern double dist_point3D_poly3D(Point3D *point, Polygon3D *poly);
extern double dist_point3D_sphere(Point3D *point, Sphere *sphere);

extern Datum distance_lseg3D_point3D(PG_FUNCTION_ARGS);
extern Datum distance_lseg3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum distance_lseg3D_line3D(PG_FUNCTION_ARGS);
extern Datum distance_lseg3D_box3D(PG_FUNCTION_ARGS);
extern Datum distance_lseg3D_path3D(PG_FUNCTION_ARGS);
extern Datum distance_lseg3D_poly3D(PG_FUNCTION_ARGS);
extern Datum distance_lseg3D_sphere(PG_FUNCTION_ARGS);

extern double dist_lseg3D_point3D(Lseg3D *lseg, Point3D *point);
extern double dist_lseg3D_lseg3D(Lseg3D *lseg1, Lseg3D *lseg2);
extern double dist_lseg3D_line3D(Lseg3D *lseg, Line3D *line);
extern double dist_lseg3D_box3D(Lseg3D *lseg, Box3D *box);
extern double dist_lseg3D_path3D(Lseg3D *lseg, Path3D *path);
extern double dist_lseg3D_poly3D(Lseg3D *lseg, Polygon3D *poly);
extern double dist_lseg3D_sphere(Lseg3D *lseg, Sphere *sphere);

extern Datum distance_line3D_point3D(PG_FUNCTION_ARGS);
extern Datum distance_line3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum distance_line3D_line3D(PG_FUNCTION_ARGS);
extern Datum distance_line3D_box3D(PG_FUNCTION_ARGS);
extern Datum distance_line3D_path3D(PG_FUNCTION_ARGS);
extern Datum distance_line3D_poly3D(PG_FUNCTION_ARGS);
extern Datum distance_line3D_sphere(PG_FUNCTION_ARGS);

extern double dist_line3D_point3D(Line3D *line, Point3D *point);
extern double dist_line3D_lseg3D(Line3D *line, Lseg3D *lseg);
extern double dist_line3D_line3D(Line3D *line1, Line3D *line2);
extern double dist_line3D_box3D(Line3D *line, Box3D *box);
extern double dist_line3D_path3D(Line3D *line, Path3D *path);
extern double dist_line3D_poly3D(Line3D *line, Polygon3D *poly);
extern double dist_line3D_sphere(Line3D *line, Sphere *sphere);

extern Datum distance_box3D_point3D(PG_FUNCTION_ARGS);
extern Datum distance_box3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum distance_box3D_line3D(PG_FUNCTION_ARGS);
extern Datum distance_box3D_box3D(PG_FUNCTION_ARGS);
extern Datum distance_box3D_path3D(PG_FUNCTION_ARGS);
extern Datum distance_box3D_poly3D(PG_FUNCTION_ARGS);
extern Datum distance_box3D_sphere(PG_FUNCTION_ARGS);

extern double dist_box3D_point3D(Box3D *box, Point3D *point);
extern double dist_box3D_lseg3D(Box3D *box, Lseg3D *lseg);
extern double dist_box3D_line3D(Box3D *box, Line3D *line);
extern double dist_box3D_box3D(Box3D *box1, Box3D *box2);
extern double dist_box3D_path3D(Box3D *box, Path3D *path);
extern double dist_box3D_poly3D(Box3D *box, Polygon3D *poly);
extern double dist_box3D_sphere(Box3D *box, Sphere *sphere);

extern Datum distance_path3D_point3D(PG_FUNCTION_ARGS);
extern Datum distance_path3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum distance_path3D_line3D(PG_FUNCTION_ARGS);
extern Datum distance_path3D_box3D(PG_FUNCTION_ARGS);
extern Datum distance_path3D_path3D(PG_FUNCTION_ARGS);
extern Datum distance_path3D_poly3D(PG_FUNCTION_ARGS);
extern Datum distance_path3D_sphere(PG_FUNCTION_ARGS);

extern double dist_path3D_point3D(Path3D *path, Point3D *point);
extern double dist_path3D_lseg3D(Path3D *path, Lseg3D *lseg);
extern double dist_path3D_line3D(Path3D *path, Line3D *line);
extern double dist_path3D_box3D(Path3D *path, Box3D *box);
extern double dist_path3D_path3D(Path3D *path1, Path3D *path2);
extern double dist_path3D_poly3D(Path3D *path, Polygon3D *poly);
extern double dist_path3D_sphere(Path3D *path, Sphere *sphere);

extern Datum distance_poly3D_point3D(PG_FUNCTION_ARGS);
extern Datum distance_poly3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum distance_poly3D_line3D(PG_FUNCTION_ARGS);
extern Datum distance_poly3D_box3D(PG_FUNCTION_ARGS);
extern Datum distance_poly3D_path3D(PG_FUNCTION_ARGS);
extern Datum distance_poly3D_poly3D(PG_FUNCTION_ARGS);
extern Datum distance_poly3D_sphere(PG_FUNCTION_ARGS);

extern double dist_poly3D_point3D(Polygon3D *poly, Point3D *point);
extern double dist_poly3D_lseg3D(Polygon3D *poly, Lseg3D *lseg);
extern double dist_poly3D_line3D(Polygon3D *poly, Line3D *line);
extern double dist_poly3D_box3D(Polygon3D *poly, Box3D *box);
extern double dist_poly3D_path3D(Polygon3D *poly, Path3D *path);
extern double dist_poly3D_poly3D(Polygon3D *poly1, Polygon3D *poly2);
extern double dist_poly3D_sphere(Polygon3D *poly, Sphere *sphere);

extern Datum distance_sphere_point3D(PG_FUNCTION_ARGS);
extern Datum distance_sphere_lseg3D(PG_FUNCTION_ARGS);
extern Datum distance_sphere_line3D(PG_FUNCTION_ARGS);
extern Datum distance_sphere_box3D(PG_FUNCTION_ARGS);
extern Datum distance_sphere_path3D(PG_FUNCTION_ARGS);
extern Datum distance_sphere_poly3D(PG_FUNCTION_ARGS);
extern Datum distance_sphere_sphere(PG_FUNCTION_ARGS);

extern double dist_sphere_point3D(Sphere *sphere, Point3D *point);
extern double dist_sphere_lseg3D(Sphere *sphere, Lseg3D *lseg);
extern double dist_sphere_line3D(Sphere *sphere, Line3D *line);
extern double dist_sphere_box3D(Sphere *sphere, Box3D *box);
extern double dist_sphere_path3D(Sphere *sphere, Path3D *path);
extern double dist_sphere_poly3D(Sphere *sphere, Polygon3D *poly2);
extern double dist_sphere_sphere(Sphere *sphere1, Sphere *sphere2);

/*****************************************************************************
 *	Closest point functions
 *****************************************************************************/

extern Datum closestpoint_point3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_point3D_line3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_point3D_box3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_point3D_path3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_point3D_poly3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_point3D_sphere(PG_FUNCTION_ARGS);

extern Point3D *closestpt_point3D_lseg3D(Point3D *point, Lseg3D *lseg);
extern Point3D *closestpt_point3D_line3D(Point3D *point, Line3D *line);
extern Point3D *closestpt_point3D_box3D(Point3D *point, Box3D *box);
extern Point3D *closestpt_point3D_path3D(Point3D *point, Path3D *path);
extern Point3D *closestpt_point3D_poly3D(Point3D *point, Polygon3D *poly);
extern Point3D *closestpt_point3D_sphere(Point3D *point, Sphere *sphere);

extern Datum closestpoint_lseg3D_point3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_lseg3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_lseg3D_line3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_lseg3D_box3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_lseg3D_path3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_lseg3D_poly3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_lseg3D_sphere(PG_FUNCTION_ARGS);

extern Point3D *closestpt_lseg3D_point3D(Lseg3D *lseg, Point3D *point);
extern Point3D *closestpt_lseg3D_lseg3D(Lseg3D *lseg1, Lseg3D *lseg2);
extern Point3D *closestpt_lseg3D_line3D(Lseg3D *lseg, Line3D *line);
extern Point3D *closestpt_lseg3D_box3D(Lseg3D *lseg, Box3D *box);
extern Point3D *closestpt_lseg3D_path3D(Lseg3D *lseg, Path3D *path);
extern Point3D *closestpt_lseg3D_poly3D(Lseg3D *lseg, Polygon3D *poly);
extern Point3D *closestpt_lseg3D_sphere(Lseg3D *lseg, Sphere *sphere);

extern Datum closestpoint_line3D_point3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_line3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_line3D_line3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_line3D_box3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_line3D_path3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_line3D_poly3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_line3D_sphere(PG_FUNCTION_ARGS);

extern Point3D *closestpt_line3D_point3D(Line3D *line, Point3D *point);
extern Point3D *closestpt_line3D_lseg3D(Line3D *line, Lseg3D *lseg);
extern Point3D *closestpt_line3D_line3D(Line3D *line1, Line3D *line2);
extern Point3D *closestpt_line3D_box3D(Line3D *line, Box3D *box);
extern Point3D *closestpt_line3D_path3D(Line3D *line, Path3D *path);
extern Point3D *closestpt_line3D_poly3D(Line3D *line, Polygon3D *poly);
extern Point3D *closestpt_line3D_sphere(Line3D *line, Sphere *sphere);

extern Datum closestpoint_box3D_point3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_box3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_box3D_line3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_box3D_box3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_box3D_path3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_box3D_poly3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_box3D_sphere(PG_FUNCTION_ARGS);

extern Point3D *closestpt_box3D_point3D(Box3D *box, Point3D *point);
extern Point3D *closestpt_box3D_lseg3D(Box3D *box, Lseg3D *lseg);
extern Point3D *closestpt_box3D_line3D(Box3D *box, Line3D *line);
extern Point3D *closestpt_box3D_box3D(Box3D *box1, Box3D *box2);
extern Point3D *closestpt_box3D_path3D(Box3D *box, Path3D *path);
extern Point3D *closestpt_box3D_poly3D(Box3D *box, Polygon3D *poly);
extern Point3D *closestpt_box3D_sphere(Box3D *box, Sphere *sphere);

extern Datum closestpoint_path3D_point3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_path3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_path3D_line3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_path3D_box3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_path3D_path3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_path3D_poly3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_path3D_sphere(PG_FUNCTION_ARGS);

extern Point3D *closestpt_path3D_point3D(Path3D *path, Point3D *point);
extern Point3D *closestpt_path3D_lseg3D(Path3D *path, Lseg3D *lseg);
extern Point3D *closestpt_path3D_line3D(Path3D *path, Line3D *line);
extern Point3D *closestpt_path3D_box3D(Path3D *path, Box3D *box);
extern Point3D *closestpt_path3D_path3D(Path3D *path1, Path3D *path2);
extern Point3D *closestpt_path3D_poly3D(Path3D *path, Polygon3D *poly);
extern Point3D *closestpt_path3D_sphere(Path3D *path, Sphere *sphere);

extern Datum closestpoint_poly3D_point3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_poly3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_poly3D_line3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_poly3D_box3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_poly3D_path3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_poly3D_poly3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_poly3D_sphere(PG_FUNCTION_ARGS);

extern Point3D *closestpt_poly3D_point3D(Polygon3D *poly, Point3D *point);
extern Point3D *closestpt_poly3D_lseg3D(Polygon3D *poly, Lseg3D *lseg);
extern Point3D *closestpt_poly3D_line3D(Polygon3D *poly, Line3D *line);
extern Point3D *closestpt_poly3D_box3D(Polygon3D *poly, Box3D *box);
extern Point3D *closestpt_poly3D_path3D(Polygon3D *poly, Path3D *path);
extern Point3D *closestpt_poly3D_poly3D(Polygon3D *poly1, Polygon3D *poly2);
extern Point3D *closestpt_poly3D_sphere(Polygon3D *poly, Sphere *sphere);

extern Datum closestpoint_sphere_point3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_sphere_lseg3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_sphere_line3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_sphere_box3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_sphere_path3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_sphere_poly3D(PG_FUNCTION_ARGS);
extern Datum closestpoint_sphere_sphere(PG_FUNCTION_ARGS);

extern Point3D *closestpt_sphere_point3D(Sphere *sphere, Point3D *point);
extern Point3D *closestpt_sphere_lseg3D(Sphere *sphere, Lseg3D *lseg);
extern Point3D *closestpt_sphere_line3D(Sphere *sphere, Line3D *line);
extern Point3D *closestpt_sphere_box3D(Sphere *sphere, Box3D *box);
extern Point3D *closestpt_sphere_path3D(Sphere *sphere, Path3D *path);
extern Point3D *closestpt_sphere_poly3D(Sphere *sphere, Polygon3D *poly);
extern Point3D *closestpt_sphere_sphere(Sphere *sphere1, Sphere *sphere2);

/*****************************************************************************
 *	Intersect functions
 *****************************************************************************/

extern Datum intersect_lseg3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum intersect_lseg3D_line3D(PG_FUNCTION_ARGS);
extern Datum intersect_lseg3D_box3D(PG_FUNCTION_ARGS);
extern Datum intersect_lseg3D_path3D(PG_FUNCTION_ARGS);
extern Datum intersect_lseg3D_poly3D(PG_FUNCTION_ARGS);
extern Datum intersect_lseg3D_sphere(PG_FUNCTION_ARGS);

extern bool inter_lseg3D_lseg3D(Lseg3D *lseg1, Lseg3D *lseg2);
extern bool inter_lseg3D_line3D(Lseg3D *lseg, Line3D *line);
extern bool inter_lseg3D_box3D(Lseg3D *lseg, Box3D *box);
extern bool inter_lseg3D_path3D(Lseg3D *lseg, Path3D *path);
extern bool inter_lseg3D_poly3D(Lseg3D *lseg, Polygon3D *poly);
extern bool inter_lseg3D_sphere(Lseg3D *lseg, Sphere *sphere);

extern Datum intersect_line3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum intersect_line3D_line3D(PG_FUNCTION_ARGS);
extern Datum intersect_line3D_box3D(PG_FUNCTION_ARGS);
extern Datum intersect_line3D_path3D(PG_FUNCTION_ARGS);
extern Datum intersect_line3D_poly3D(PG_FUNCTION_ARGS);
extern Datum intersect_line3D_sphere(PG_FUNCTION_ARGS);

extern bool inter_line3D_lseg3D(Line3D *line, Lseg3D *lseg);
extern bool inter_line3D_line3D(Line3D *line1, Line3D *line2);
extern bool inter_line3D_box3D(Line3D *line, Box3D *box);
extern bool inter_line3D_path3D(Line3D *line, Path3D *path);
extern bool inter_line3D_poly3D(Line3D *line, Polygon3D *poly);
extern bool inter_line3D_sphere(Line3D *line, Sphere *sphere);

extern Datum intersect_box3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum intersect_box3D_line3D(PG_FUNCTION_ARGS);
extern Datum intersect_box3D_box3D(PG_FUNCTION_ARGS);
extern Datum intersect_box3D_path3D(PG_FUNCTION_ARGS);
extern Datum intersect_box3D_poly3D(PG_FUNCTION_ARGS);
extern Datum intersect_box3D_sphere(PG_FUNCTION_ARGS);

extern bool inter_box3D_lseg3D(Box3D *box, Lseg3D *lseg);
extern bool inter_box3D_line3D(Box3D *box, Line3D *line);
extern bool inter_box3D_box3D(Box3D *box1, Box3D *box2);
extern bool inter_box3D_path3D(Box3D *box, Path3D *path);
extern bool inter_box3D_poly3D(Box3D *box, Polygon3D *poly);
extern bool inter_box3D_sphere(Box3D *box, Sphere *sphere);

extern Datum intersect_path3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum intersect_path3D_line3D(PG_FUNCTION_ARGS);
extern Datum intersect_path3D_box3D(PG_FUNCTION_ARGS);
extern Datum intersect_path3D_path3D(PG_FUNCTION_ARGS);
extern Datum intersect_path3D_poly3D(PG_FUNCTION_ARGS);
extern Datum intersect_path3D_sphere(PG_FUNCTION_ARGS);

extern bool inter_path3D_lseg3D(Path3D *path, Lseg3D *lseg);
extern bool inter_path3D_line3D(Path3D *path, Line3D *line);
extern bool inter_path3D_box3D(Path3D *path, Box3D *box);
extern bool inter_path3D_path3D(Path3D *path1, Path3D *path2);
extern bool inter_path3D_poly3D(Path3D *path, Polygon3D *poly);
extern bool inter_path3D_sphere(Path3D *path, Sphere *sphere);

extern Datum intersect_poly3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum intersect_poly3D_line3D(PG_FUNCTION_ARGS);
extern Datum intersect_poly3D_box3D(PG_FUNCTION_ARGS);
extern Datum intersect_poly3D_path3D(PG_FUNCTION_ARGS);
extern Datum intersect_poly3D_poly3D(PG_FUNCTION_ARGS);
extern Datum intersect_poly3D_sphere(PG_FUNCTION_ARGS);

extern bool inter_poly3D_lseg3D(Polygon3D *poly, Lseg3D *lseg);
extern bool inter_poly3D_line3D(Polygon3D *poly, Line3D *line);
extern bool inter_poly3D_box3D(Polygon3D *poly, Box3D *box);
extern bool inter_poly3D_path3D(Polygon3D *poly, Path3D *path2);
extern bool inter_poly3D_poly3D(Polygon3D *poly1, Polygon3D *poly2);
extern bool inter_poly3D_sphere(Polygon3D *poly, Sphere *sphere);

extern Datum intersect_sphere_lseg3D(PG_FUNCTION_ARGS);
extern Datum intersect_sphere_line3D(PG_FUNCTION_ARGS);
extern Datum intersect_sphere_box3D(PG_FUNCTION_ARGS);
extern Datum intersect_sphere_path3D(PG_FUNCTION_ARGS);
extern Datum intersect_sphere_poly3D(PG_FUNCTION_ARGS);
extern Datum intersect_sphere_sphere(PG_FUNCTION_ARGS);

extern bool inter_sphere_lseg3D(Sphere *sphere, Lseg3D *lseg);
extern bool inter_sphere_line3D(Sphere *sphere, Line3D *line);
extern bool inter_sphere_box3D(Sphere *sphere, Box3D *box);
extern bool inter_sphere_path3D(Sphere *sphere, Path3D *path2);
extern bool inter_sphere_poly3D(Sphere *sphere, Polygon3D *poly);
extern bool inter_sphere_sphere(Sphere *sphere1, Sphere *sphere2);

/*****************************************************************************
 *	Intersection point functions
 *****************************************************************************/
 
extern Datum interpoint_lseg3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum interpoint_lseg3D_line3D(PG_FUNCTION_ARGS);
extern Datum interpoint_lseg3D_box3D(PG_FUNCTION_ARGS);
extern Datum interpoint_lseg3D_path3D(PG_FUNCTION_ARGS);
extern Datum interpoint_lseg3D_poly3D(PG_FUNCTION_ARGS);
extern Datum interpoint_lseg3D_sphere(PG_FUNCTION_ARGS);

extern Point3D *interpt_lseg3D_lseg3D(Lseg3D *line1, Lseg3D *line2);
extern Point3D *interpt_lseg3D_line3D(Lseg3D *lseg, Line3D *line);
extern Point3D *interpt_lseg3D_box3D(Lseg3D *lseg, Box3D *box);
extern Point3D *interpt_lseg3D_path3D(Lseg3D *lseg, Path3D *path);
extern Point3D *interpt_lseg3D_poly3D(Lseg3D *lseg, Polygon3D *poly);
extern Point3D *interpt_lseg3D_sphere(Lseg3D *lseg, Sphere *sphere);

extern Datum interpoint_line3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum interpoint_line3D_line3D(PG_FUNCTION_ARGS);
extern Datum interpoint_line3D_box3D(PG_FUNCTION_ARGS);
extern Datum interpoint_line3D_path3D(PG_FUNCTION_ARGS);
extern Datum interpoint_line3D_poly3D(PG_FUNCTION_ARGS);
extern Datum interpoint_line3D_sphere(PG_FUNCTION_ARGS);

extern Point3D *interpt_line3D_lseg3D(Line3D *line, Lseg3D *lseg);
extern Point3D *interpt_line3D_line3D(Line3D *line1, Line3D *line2);
extern Point3D *interpt_line3D_box3D(Line3D *line, Box3D *box);
extern Point3D *interpt_line3D_path3D(Line3D *line, Path3D *path);
extern Point3D *interpt_line3D_poly3D(Line3D *line, Polygon3D *poly);
extern Point3D *interpt_line3D_sphere(Line3D *line, Sphere *sphere);

extern Datum interpoint_box3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum interpoint_box3D_line3D(PG_FUNCTION_ARGS);
extern Datum interpoint_box3D_box3D(PG_FUNCTION_ARGS);
extern Datum interpoint_box3D_path3D(PG_FUNCTION_ARGS);
extern Datum interpoint_box3D_poly3D(PG_FUNCTION_ARGS);
extern Datum interpoint_box3D_sphere(PG_FUNCTION_ARGS);

extern Point3D *interpt_box3D_lseg3D(Box3D *box, Lseg3D *lseg);
extern Point3D *interpt_box3D_line3D(Box3D *box, Line3D *line);
extern Point3D *interpt_box3D_box3D(Box3D *box1, Box3D *box2);
extern Point3D *interpt_box3D_path3D(Box3D *box, Path3D *path);
extern Point3D *interpt_box3D_poly3D(Box3D *box, Polygon3D *poly);
extern Point3D *interpt_box3D_sphere(Box3D *box, Sphere *sphere);

extern Datum interpoint_path3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum interpoint_path3D_line3D(PG_FUNCTION_ARGS);
extern Datum interpoint_path3D_box3D(PG_FUNCTION_ARGS);
extern Datum interpoint_path3D_path3D(PG_FUNCTION_ARGS);
extern Datum interpoint_path3D_poly3D(PG_FUNCTION_ARGS);
extern Datum interpoint_path3D_sphere(PG_FUNCTION_ARGS);

extern Point3D *interpt_path3D_lseg3D(Path3D *path, Lseg3D *lseg);
extern Point3D *interpt_path3D_line3D(Path3D *path, Line3D *line);
extern Point3D *interpt_path3D_box3D(Path3D *path, Box3D *box);
extern Point3D *interpt_path3D_path3D(Path3D *path1, Path3D *path2);
extern Point3D *interpt_path3D_poly3D(Path3D *path, Polygon3D *poly);
extern Point3D *interpt_path3D_sphere(Path3D *path, Sphere *sphere);

extern Datum interpoint_poly3D_lseg3D(PG_FUNCTION_ARGS);
extern Datum interpoint_poly3D_line3D(PG_FUNCTION_ARGS);
extern Datum interpoint_poly3D_box3D(PG_FUNCTION_ARGS);
extern Datum interpoint_poly3D_path3D(PG_FUNCTION_ARGS);
extern Datum interpoint_poly3D_poly3D(PG_FUNCTION_ARGS);
extern Datum interpoint_poly3D_sphere(PG_FUNCTION_ARGS);

extern Point3D *interpt_poly3D_lseg3D(Polygon3D *poly, Lseg3D *lseg);
extern Point3D *interpt_poly3D_line3D(Polygon3D *poly, Line3D *line);
extern Point3D *interpt_poly3D_box3D(Polygon3D *poly, Box3D *box);
extern Point3D *interpt_poly3D_path3D(Polygon3D *poly, Path3D *path);
extern Point3D *interpt_poly3D_poly3D(Polygon3D *poly1, Polygon3D *poly2);
extern Point3D *interpt_poly3D_sphere(Polygon3D *poly, Sphere *sphere);

extern Datum interpoint_sphere_lseg3D(PG_FUNCTION_ARGS);
extern Datum interpoint_sphere_line3D(PG_FUNCTION_ARGS);
extern Datum interpoint_sphere_box3D(PG_FUNCTION_ARGS);
extern Datum interpoint_sphere_path3D(PG_FUNCTION_ARGS);
extern Datum interpoint_sphere_poly3D(PG_FUNCTION_ARGS);
extern Datum interpoint_sphere_sphere(PG_FUNCTION_ARGS);

extern Point3D *interpt_sphere_lseg3D(Sphere *sphere, Lseg3D *lseg);
extern Point3D *interpt_sphere_line3D(Sphere *sphere, Line3D *line);
extern Point3D *interpt_sphere_box3D(Sphere *sphere, Box3D *box);
extern Point3D *interpt_sphere_path3D(Sphere *sphere, Path3D *path);
extern Point3D *interpt_sphere_poly3D(Sphere *sphere, Polygon3D *poly);
extern Point3D *interpt_sphere_sphere(Sphere *sphere1, Sphere *sphere2);

/*****************************************************************************
 * Intersection functions 
 *****************************************************************************/

extern Datum box3D_intersection(PG_FUNCTION_ARGS);

/*****************************************************************************
 *	Support routines for the GiST access method (geo3D_gist_rtree.c)
 *****************************************************************************/
 
extern Datum gist_box3D_consistent(PG_FUNCTION_ARGS);
extern Datum gist_box3D_union(PG_FUNCTION_ARGS);
extern Datum gist_box3D_compress(PG_FUNCTION_ARGS);
extern Datum gist_box3D_decompress(PG_FUNCTION_ARGS);
extern Datum gist_box3D_fetch(PG_FUNCTION_ARGS);
extern Datum gist_box3D_penalty(PG_FUNCTION_ARGS);
extern Datum gist_box3D_picksplit(PG_FUNCTION_ARGS);
extern Datum gist_box3D_same(PG_FUNCTION_ARGS);

extern Datum gist_type3D_consistent(PG_FUNCTION_ARGS);

extern Datum gist_point3D_compress(PG_FUNCTION_ARGS);
extern Datum gist_point3D_fetch(PG_FUNCTION_ARGS);
extern Datum gist_point3D_consistent(PG_FUNCTION_ARGS);
extern Datum gist_point3D_distance(PG_FUNCTION_ARGS);

extern Datum gist_poly3D_compress(PG_FUNCTION_ARGS);
extern Datum gist_poly3D_distance(PG_FUNCTION_ARGS);

extern Datum gist_sphere_compress(PG_FUNCTION_ARGS);
extern Datum gist_sphere_consistent(PG_FUNCTION_ARGS);
extern Datum gist_sphere_distance(PG_FUNCTION_ARGS);

/*****************************************************************************
 *	Support routines for the SP-GiST octree access method (geo3D_spgist_octree.c)
 *****************************************************************************/

extern Datum spgist_octree_config(PG_FUNCTION_ARGS);
extern Datum spgist_octree_choose(PG_FUNCTION_ARGS);
extern Datum spgist_octree_picksplit(PG_FUNCTION_ARGS);
extern Datum spgist_octree_inner_consistent(PG_FUNCTION_ARGS);
extern Datum spgist_octree_leaf_consistent(PG_FUNCTION_ARGS);

/*****************************************************************************
 * Selectivity functions (geo3D_selfuncs.c)
 *****************************************************************************/

extern Datum volumesel(PG_FUNCTION_ARGS);
extern Datum volumejoinsel(PG_FUNCTION_ARGS);
extern Datum positionsel3D(PG_FUNCTION_ARGS);
extern Datum positionjoinsel3D(PG_FUNCTION_ARGS);
extern Datum contsel3D(PG_FUNCTION_ARGS);
extern Datum contjoinsel3D(PG_FUNCTION_ARGS);

#endif   /* GEO3D_DECLS_H */

/*****************************************************************************/
