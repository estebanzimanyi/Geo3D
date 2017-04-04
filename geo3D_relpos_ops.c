/*****************************************************************************
 *
 * geo3D_relpos_ops.c
 * Relative position operators for 3D geometries:
 *		Left, Overleft, Right, Overright
 *		Below, Overbelow, Above, Overabove
 *		Front, Overfront, back, Overback
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

/*****************************************************************************/

/* point3D OPERATOR point3D */

/*
 * Is the first Point3D strictly to the left of the second Point3D?
 */
Datum
left_point3D_point3D(PG_FUNCTION_ARGS)
{
	Point3D	   *pt1 = PG_GETARG_POINT3D_P(0);
	Point3D	   *pt2 = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(left_point3D_point3D_internal(pt1, pt2));
}

PG_FUNCTION_INFO_V1(left_point3D_point3D);

bool
left_point3D_point3D_internal(Point3D *pt1, Point3D *pt2)
{
	return FPlt(pt1->x, pt2->x);
}

/*
 * Is the first Point3D to the left or equal the second Point3D?
 */
Datum
overleft_point3D_point3D(PG_FUNCTION_ARGS)
{
	Point3D	   *pt1 = PG_GETARG_POINT3D_P(0);
	Point3D	   *pt2 = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overleft_point3D_point3D_internal(pt1, pt2));
}

PG_FUNCTION_INFO_V1(overleft_point3D_point3D);

bool
overleft_point3D_point3D_internal(Point3D *pt1, Point3D *pt2)
{
	return (FPle(pt1->x, pt2->x));
}

/*
 * Is the first Point3D strictly to the right of the second Point3D?
 */
Datum
right_point3D_point3D(PG_FUNCTION_ARGS)
{
	Point3D	   *pt1 = PG_GETARG_POINT3D_P(0);
	Point3D	   *pt2 = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(right_point3D_point3D_internal(pt1, pt2));
}

PG_FUNCTION_INFO_V1(right_point3D_point3D);

bool
right_point3D_point3D_internal(Point3D *pt1, Point3D *pt2)
{
	return (FPgt(pt1->x, pt2->x));
}

/*
 * Is the first Point3D to the right of or equals to the second Point3D?
 */
Datum
overright_point3D_point3D(PG_FUNCTION_ARGS)
{
	Point3D	   *pt1 = PG_GETARG_POINT3D_P(0);
	Point3D	   *pt2 = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overright_point3D_point3D_internal(pt1, pt2));
}

PG_FUNCTION_INFO_V1(overright_point3D_point3D);

bool
overright_point3D_point3D_internal(Point3D *pt1, Point3D *pt2)
{
	return (FPge(pt1->x, pt2->x));
}

/*
 * Is the first Point3D strictly below the second Point3D?
 */
Datum
below_point3D_point3D(PG_FUNCTION_ARGS)
{
	Point3D	   *pt1 = PG_GETARG_POINT3D_P(0);
	Point3D	   *pt2 = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(below_point3D_point3D_internal(pt1, pt2));
}

PG_FUNCTION_INFO_V1(below_point3D_point3D);

bool
below_point3D_point3D_internal(Point3D *pt1, Point3D *pt2)
{
	return FPlt(pt1->y, pt2->y);
}

/*
 * Is the first Point3D below or equal to the second Point3D?
 */
Datum
overbelow_point3D_point3D(PG_FUNCTION_ARGS)
{
	Point3D	   *pt1 = PG_GETARG_POINT3D_P(0);
	Point3D	   *pt2 = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overbelow_point3D_point3D_internal(pt1, pt2));
}

PG_FUNCTION_INFO_V1(overbelow_point3D_point3D);

bool
overbelow_point3D_point3D_internal(Point3D *pt1, Point3D *pt2)
{
	return FPle(pt1->y, pt2->y);
}

/*
 * Is the first Point3D strictly above the second Point3D?
 */
Datum
above_point3D_point3D(PG_FUNCTION_ARGS)
{
	Point3D	   *pt1 = PG_GETARG_POINT3D_P(0);
	Point3D	   *pt2 = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(above_point3D_point3D_internal(pt1, pt2));
}

PG_FUNCTION_INFO_V1(above_point3D_point3D);

bool
above_point3D_point3D_internal(Point3D *pt1, Point3D *pt2)
{
	return FPgt(pt1->y, pt2->y);
}

/*
 * Is the first Point3D above or equal to the second Point3D?
 */
Datum
overabove_point3D_point3D(PG_FUNCTION_ARGS)
{
	Point3D	   *pt1 = PG_GETARG_POINT3D_P(0);
	Point3D	   *pt2 = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overabove_point3D_point3D_internal(pt1, pt2));
}

PG_FUNCTION_INFO_V1(overabove_point3D_point3D);

bool
overabove_point3D_point3D_internal(Point3D *pt1, Point3D *pt2)
{
	return FPge(pt1->y, pt2->y);
}

/*
 * Is the first Point3D strictly in front of the second Point3D?
 */
Datum
front_point3D_point3D(PG_FUNCTION_ARGS)
{
	Point3D	   *pt1 = PG_GETARG_POINT3D_P(0);
	Point3D	   *pt2 = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(front_point3D_point3D_internal(pt1, pt2));
}

PG_FUNCTION_INFO_V1(front_point3D_point3D);

bool
front_point3D_point3D_internal(Point3D *pt1, Point3D *pt2)
{
	return FPlt(pt1->z, pt2->z);
}

/*
 * Is the first Point3D in front of or equal to the second Point3D?
 */
Datum
overfront_point3D_point3D(PG_FUNCTION_ARGS)
{
	Point3D	   *pt1 = PG_GETARG_POINT3D_P(0);
	Point3D	   *pt2 = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overfront_point3D_point3D_internal(pt1, pt2));
}

PG_FUNCTION_INFO_V1(overfront_point3D_point3D);

bool
overfront_point3D_point3D_internal(Point3D *pt1, Point3D *pt2)
{
	return FPle(pt1->z, pt2->z);
}

/*
 * Is the first Point3D strictly at the back of the second Point3D?
 */
Datum
back_point3D_point3D(PG_FUNCTION_ARGS)
{
	Point3D	   *pt1 = PG_GETARG_POINT3D_P(0);
	Point3D	   *pt2 = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(back_point3D_point3D_internal(pt1, pt2));
}

PG_FUNCTION_INFO_V1(back_point3D_point3D);

bool
back_point3D_point3D_internal(Point3D *pt1, Point3D *pt2)
{
	return FPgt(pt1->z, pt2->z);
}

/*
 * Is the first Point3D at the back of or equal to the second Point3D?
 */
Datum
overback_point3D_point3D(PG_FUNCTION_ARGS)
{
	Point3D	   *pt1 = PG_GETARG_POINT3D_P(0);
	Point3D	   *pt2 = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overback_point3D_point3D_internal(pt1, pt2));
}

PG_FUNCTION_INFO_V1(overback_point3D_point3D);

bool
overback_point3D_point3D_internal(Point3D *pt1, Point3D *pt2)
{
	return FPge(pt1->z, pt2->z);
}

/*----------------------------------------------------------*/

/* point3D OPERATOR lseg3D */

/*
 * Is the Point3D strictly to the left of the Lseg3D?
 */
Datum
left_point3D_lseg3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(left_point3D_lseg3D_internal(point, lseg));
}

PG_FUNCTION_INFO_V1(left_point3D_lseg3D);

bool
left_point3D_lseg3D_internal(Point3D *point, Lseg3D *lseg)
{
	return FPlt(point->x, Min(lseg->p[0].x,lseg->p[1].x));
}

/*
 * Is the Point3D to the left of or in the Lseg3D?
 */
Datum
overleft_point3D_lseg3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overleft_point3D_lseg3D_internal(point, lseg));
}

PG_FUNCTION_INFO_V1(overleft_point3D_lseg3D);

bool
overleft_point3D_lseg3D_internal(Point3D *point, Lseg3D *lseg)
{
	return FPle(point->x, Min(lseg->p[0].x,lseg->p[1].x));
}

/*
 * Is the Point3D strictly to the right of the Lseg3D?
 */
Datum
right_point3D_lseg3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(right_point3D_lseg3D_internal(point, lseg));
}

PG_FUNCTION_INFO_V1(right_point3D_lseg3D);

bool
right_point3D_lseg3D_internal(Point3D *point, Lseg3D *lseg)
{
	return FPgt(point->x, Max(lseg->p[0].x,lseg->p[1].x));
}

/*
 * Is the Point3D to the right of or in the Lseg3D?
 */
Datum
overright_point3D_lseg3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overright_point3D_lseg3D_internal(point, lseg));
}

PG_FUNCTION_INFO_V1(overright_point3D_lseg3D);

bool
overright_point3D_lseg3D_internal(Point3D *point, Lseg3D *lseg)
{
	return FPge(point->x, Max(lseg->p[0].x,lseg->p[1].x));
}

/*
 * Is the Point3D strictly below the Lseg3D?
 */
Datum
below_point3D_lseg3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(below_point3D_lseg3D_internal(point, lseg));
}

PG_FUNCTION_INFO_V1(below_point3D_lseg3D);

bool
below_point3D_lseg3D_internal(Point3D *point, Lseg3D *lseg)
{
	return FPlt(point->y, Min(lseg->p[0].y,lseg->p[1].y));
}

/*
 * Is the Point3D below or in the Lseg3D?
 */
Datum
overbelow_point3D_lseg3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overbelow_point3D_lseg3D_internal(point, lseg));
}

PG_FUNCTION_INFO_V1(overbelow_point3D_lseg3D);

bool
overbelow_point3D_lseg3D_internal(Point3D *point, Lseg3D *lseg)
{
	return FPle(point->y, Min(lseg->p[0].y,lseg->p[1].y));
}

/*
 * Is the Point3D strictly above the Lseg3D?
 */
Datum
above_point3D_lseg3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(above_point3D_lseg3D_internal(point, lseg));
}

PG_FUNCTION_INFO_V1(above_point3D_lseg3D);

bool
above_point3D_lseg3D_internal(Point3D *point, Lseg3D *lseg)
{
	return FPgt(point->y, Max(lseg->p[0].y,lseg->p[1].y));
}

/*
 * Is the Point3D above or in the Lseg3D?
 */
Datum
overabove_point3D_lseg3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overabove_point3D_lseg3D_internal(point, lseg));
}

PG_FUNCTION_INFO_V1(overabove_point3D_lseg3D);

bool
overabove_point3D_lseg3D_internal(Point3D *point, Lseg3D *lseg)
{
	return FPge(point->y, Max(lseg->p[0].y,lseg->p[1].y));
}

/*
 * Is the Point3D strictly in front of the Lseg3D?
 */
Datum
front_point3D_lseg3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(front_point3D_lseg3D_internal(point, lseg));
}

PG_FUNCTION_INFO_V1(front_point3D_lseg3D);

bool
front_point3D_lseg3D_internal(Point3D *point, Lseg3D *lseg)
{
	return FPlt(point->z, Min(lseg->p[0].z,lseg->p[1].z));
}

/*
 * Is the Point3D in front of or in the Lseg3D?
 */
Datum
overfront_point3D_lseg3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overfront_point3D_lseg3D_internal(point, lseg));
}

PG_FUNCTION_INFO_V1(overfront_point3D_lseg3D);

bool
overfront_point3D_lseg3D_internal(Point3D *point, Lseg3D *lseg)
{
	return FPle(point->z, Min(lseg->p[0].z,lseg->p[1].z));
}

/*
 * Is the Point3D strictly at the back of the Lseg3D?
 */
Datum
back_point3D_lseg3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(back_point3D_lseg3D_internal(point, lseg));
}

PG_FUNCTION_INFO_V1(back_point3D_lseg3D);

bool
back_point3D_lseg3D_internal(Point3D *point, Lseg3D *lseg)
{
	return FPgt(point->z, Max(lseg->p[0].z,lseg->p[1].z));
}

/*
 * Is the Point3D at the back of or in the Lseg3D?
 */
Datum
overback_point3D_lseg3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overback_point3D_lseg3D_internal(point, lseg));
}

PG_FUNCTION_INFO_V1(overback_point3D_lseg3D);

bool
overback_point3D_lseg3D_internal(Point3D *point, Lseg3D *lseg)
{
	return FPge(point->z, Max(lseg->p[0].z,lseg->p[1].z));
}

/*----------------------------------------------------------*/

/* point3D OPERATOR line3D */

/*
 * Is the Point3D strictly to the left of the Line3D?
 */
Datum
left_point3D_line3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(left_point3D_line3D_internal(point, line));
}

PG_FUNCTION_INFO_V1(left_point3D_line3D);

bool
left_point3D_line3D_internal(Point3D *point, Line3D *line)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPlt(point->x, line->p[0].x);
}

/*
 * Is the Point3D to the left of or in the Line3D?
 */
Datum
overleft_point3D_line3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overleft_point3D_line3D_internal(point, line));
}

PG_FUNCTION_INFO_V1(overleft_point3D_line3D);

bool
overleft_point3D_line3D_internal(Point3D *point, Line3D *line)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return (FPle(point->x, line->p[0].x));
}

/*
 * Is the Point3D strictly to the right of the Line3D?
 */
Datum
right_point3D_line3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(right_point3D_line3D_internal(point, line));
}

PG_FUNCTION_INFO_V1(right_point3D_line3D);

bool
right_point3D_line3D_internal(Point3D *point, Line3D *line)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return (FPgt(point->x, line->p[0].x));
}

/*
 * Is the Point3D to the right of or in the Line3D?
 */
Datum
overright_point3D_line3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overright_point3D_line3D_internal(point, line));
}

PG_FUNCTION_INFO_V1(overright_point3D_line3D);

bool
overright_point3D_line3D_internal(Point3D *point, Line3D *line)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return (FPge(point->x, line->p[0].x));
}

/*
 * Is the Point3D strictly below the Line3D?
 */
Datum
below_point3D_line3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(below_point3D_line3D_internal(point, line));
}

PG_FUNCTION_INFO_V1(below_point3D_line3D);

bool
below_point3D_line3D_internal(Point3D *point, Line3D *line)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPlt(point->y, line->p[0].y);
}

/*
 * Is the Point3D below or in the Line3D?
 */
Datum
overbelow_point3D_line3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overbelow_point3D_line3D_internal(point, line));
}

PG_FUNCTION_INFO_V1(overbelow_point3D_line3D);

bool
overbelow_point3D_line3D_internal(Point3D *point, Line3D *line)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPle(point->y, line->p[0].y);
}

/*
 * Is the Point3D strictly above the Line3D?
 */
Datum
above_point3D_line3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(above_point3D_line3D_internal(point, line));
}

PG_FUNCTION_INFO_V1(above_point3D_line3D);

bool
above_point3D_line3D_internal(Point3D *point, Line3D *line)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPgt(point->y, line->p[0].y);
}

/*
 * Is the Point3D above or in the Line3D?
 */
Datum
overabove_point3D_line3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overabove_point3D_line3D_internal(point, line));
}

PG_FUNCTION_INFO_V1(overabove_point3D_line3D);

bool
overabove_point3D_line3D_internal(Point3D *point, Line3D *line)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPge(point->y, line->p[0].y);
}

/*
 * Is the Point3D strictly in front of the Line3D?
 */
Datum
front_point3D_line3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(front_point3D_line3D_internal(point, line));
}

PG_FUNCTION_INFO_V1(front_point3D_line3D);

bool
front_point3D_line3D_internal(Point3D *point, Line3D *line)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPlt(point->z, line->p[0].z);
}

/*
 * Is the Point3D in front of or in the Line3D?
 */
Datum
overfront_point3D_line3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overfront_point3D_line3D_internal(point, line));
}

PG_FUNCTION_INFO_V1(overfront_point3D_line3D);

bool
overfront_point3D_line3D_internal(Point3D *point, Line3D *line)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPle(point->z, line->p[0].z);
}

/*
 * Is the Point3D strictly at the back of the Line3D?
 */
Datum
back_point3D_line3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(back_point3D_line3D_internal(point, line));
}

PG_FUNCTION_INFO_V1(back_point3D_line3D);

bool
back_point3D_line3D_internal(Point3D *point, Line3D *line)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPgt(point->z, line->p[0].z);
}

/*
 * Is the Point3D at the back of or in the Line3D?
 */
Datum
overback_point3D_line3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overback_point3D_line3D_internal(point, line));
}

PG_FUNCTION_INFO_V1(overback_point3D_line3D);

bool
overback_point3D_line3D_internal(Point3D *point, Line3D *line)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPge(point->z, line->p[0].z);
}

/*----------------------------------------------------------*/

/* point3D OPERATOR box3D */

/*
 * Is the Point3D strictly to the left of the Box3D?
 */
Datum
left_point3D_box3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(left_point3D_box3D_internal(point, box));
}

PG_FUNCTION_INFO_V1(left_point3D_box3D);

bool
left_point3D_box3D_internal(Point3D *point, Box3D *box)
{
	return FPlt(point->x, box->low.x);
}

/*
 * Is the Point3D to the left of or in the Box3D?
 */
Datum
overleft_point3D_box3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overleft_point3D_box3D_internal(point, box));
}

PG_FUNCTION_INFO_V1(overleft_point3D_box3D);

bool
overleft_point3D_box3D_internal(Point3D *point, Box3D *box)
{
	return FPle(point->x, box->high.x);
}

/*
 * Is the Point3D strictly to the right of the Box3D?
 */
Datum
right_point3D_box3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(right_point3D_box3D_internal(point, box));
}

PG_FUNCTION_INFO_V1(right_point3D_box3D);

bool
right_point3D_box3D_internal(Point3D *point, Box3D *box)
{
	return FPgt(point->x, box->high.x);
}

/*
 * Is the Point3D to the right of or in the Box3D?
 */
Datum
overright_point3D_box3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overright_point3D_box3D_internal(point, box));
}

PG_FUNCTION_INFO_V1(overright_point3D_box3D);

bool
overright_point3D_box3D_internal(Point3D *point, Box3D *box)
{
	return FPge(point->x, box->low.x);
}

/*
 * Is the Point3D strictly below the Box3D?
 */
Datum
below_point3D_box3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(below_point3D_box3D_internal(point, box));
}

PG_FUNCTION_INFO_V1(below_point3D_box3D);

bool
below_point3D_box3D_internal(Point3D *point, Box3D *box)
{
	return FPlt(point->y, box->low.y);
}

/*
 * Is the Point3D below or in the Box3D?
 */
Datum
overbelow_point3D_box3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overbelow_point3D_box3D_internal(point, box));
}

PG_FUNCTION_INFO_V1(overbelow_point3D_box3D);

bool
overbelow_point3D_box3D_internal(Point3D *point, Box3D *box)
{
	return FPle(point->y, box->high.y);
}

/*
 * Is the Point3D strictly above the Box3D?
 */
Datum
above_point3D_box3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(above_point3D_box3D_internal(point, box));
}

PG_FUNCTION_INFO_V1(above_point3D_box3D);

bool
above_point3D_box3D_internal(Point3D *point, Box3D *box)
{
	return FPgt(point->y, box->high.y);
}

/*
 * Is the Point3D above or in the Box3D?
 */
Datum
overabove_point3D_box3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overabove_point3D_box3D_internal(point, box));
}

PG_FUNCTION_INFO_V1(overabove_point3D_box3D);

bool
overabove_point3D_box3D_internal(Point3D *point, Box3D *box)
{
	return FPge(point->y, box->low.y);
}

/*
 * Is the Point3D strictly in front of the Box3D?
 */
Datum
front_point3D_box3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(front_point3D_box3D_internal(point, box));
}

PG_FUNCTION_INFO_V1(front_point3D_box3D);

bool
front_point3D_box3D_internal(Point3D *point, Box3D *box)
{
	return FPlt(point->z, box->low.z);
}

/*
 * Is the Point3D in front of or in the Box3D?
 */
Datum
overfront_point3D_box3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overfront_point3D_box3D_internal(point, box));
}

PG_FUNCTION_INFO_V1(overfront_point3D_box3D);

bool
overfront_point3D_box3D_internal(Point3D *point, Box3D *box)
{
	return FPle(point->z, box->high.z);
}

/*
 * Is the Point3D strictly at the back of the Box3D?
 */
Datum
back_point3D_box3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(back_point3D_box3D_internal(point, box));
}

PG_FUNCTION_INFO_V1(back_point3D_box3D);

bool
back_point3D_box3D_internal(Point3D *point, Box3D *box)
{
	return FPgt(point->z, box->high.z);
}

/*
 * Is the Point3D at the back of or in the Box3D?
 */
Datum
overback_point3D_box3D(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(1);
	Box3D	   *box = PG_GETARG_BOX3D_P(0);

	PG_RETURN_BOOL(overback_point3D_box3D_internal(point, box));
}

PG_FUNCTION_INFO_V1(overback_point3D_box3D);

bool
overback_point3D_box3D_internal(Point3D *point, Box3D *box)
{
	return FPge(point->z, box->low.z);
}

/*----------------------------------------------------------*/

/* point3D OPERATOR path3D */

/*
 * Is the Point3D strictly to the left of the Path3D?
 */
Datum
left_point3D_path3D(PG_FUNCTION_ARGS)
{
	Point3D    *point = PG_GETARG_POINT3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = left_point3D_path3D_internal(point, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(left_point3D_path3D);

bool
left_point3D_path3D_internal(Point3D *point, Path3D *path)
{
	return FPlt(point->x, path->boundbox.low.x);
}

/*
 * Is the Point3D to the left of or in the Path3D?
 */
Datum
overleft_point3D_path3D(PG_FUNCTION_ARGS)
{
	Point3D    *point = PG_GETARG_POINT3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overleft_point3D_path3D_internal(point, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overleft_point3D_path3D);

bool
overleft_point3D_path3D_internal(Point3D *point, Path3D *path)
{
	return FPle(point->x, path->boundbox.high.x);
}

/*
 * Is the Point3D strictly to the right of the Path3D?
 */
Datum
right_point3D_path3D(PG_FUNCTION_ARGS)
{
	Point3D    *point = PG_GETARG_POINT3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = right_point3D_path3D_internal(point, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(right_point3D_path3D);

bool
right_point3D_path3D_internal(Point3D *point, Path3D *path)
{
	return FPgt(point->x, path->boundbox.high.x);
}

/*
 * Is the Point3D to the right of or in the Path3D?
 */
Datum
overright_point3D_path3D(PG_FUNCTION_ARGS)
{
	Point3D    *point = PG_GETARG_POINT3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overright_point3D_path3D_internal(point, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overright_point3D_path3D);

bool
overright_point3D_path3D_internal(Point3D *point, Path3D *path)
{
	return FPge(point->x, path->boundbox.low.x);
}

/*
 * Is the Point3D strictly below the Path3D?
 */
Datum
below_point3D_path3D(PG_FUNCTION_ARGS)
{
	Point3D    *point = PG_GETARG_POINT3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = below_point3D_path3D_internal(point, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(below_point3D_path3D);

bool
below_point3D_path3D_internal(Point3D *point, Path3D *path)
{
	return FPlt(point->y, path->boundbox.low.y);
}

/*
 * Is the Point3D below or in the Path3D?
 */
Datum
overbelow_point3D_path3D(PG_FUNCTION_ARGS)
{
	Point3D    *point = PG_GETARG_POINT3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overbelow_point3D_path3D_internal(point, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overbelow_point3D_path3D);

bool
overbelow_point3D_path3D_internal(Point3D *point, Path3D *path)
{
	return FPle(point->y, path->boundbox.high.y);
}

/*
 * Is the Point3D strictly above the Path3D?
 */
Datum
above_point3D_path3D(PG_FUNCTION_ARGS)
{
	Point3D    *point = PG_GETARG_POINT3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = above_point3D_path3D_internal(point, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(above_point3D_path3D);

bool
above_point3D_path3D_internal(Point3D *point, Path3D *path)
{
	return FPgt(point->y, path->boundbox.high.y);
}

/*
 * Is the Point3D above or in the Path3D?
 */
Datum
overabove_point3D_path3D(PG_FUNCTION_ARGS)
{
	Point3D    *point = PG_GETARG_POINT3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overabove_point3D_path3D_internal(point, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overabove_point3D_path3D);

bool
overabove_point3D_path3D_internal(Point3D *point, Path3D *path)
{
	return FPge(point->y, path->boundbox.low.y);
}

/*
 * Is the Point3D strictly in front of the Path3D?
 */
Datum
front_point3D_path3D(PG_FUNCTION_ARGS)
{
	Point3D    *point = PG_GETARG_POINT3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = front_point3D_path3D_internal(point, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(front_point3D_path3D);

bool
front_point3D_path3D_internal(Point3D *point, Path3D *path)
{
	return FPlt(point->z, path->boundbox.low.z);
}

/*
 * Is the Point3D in front of or in the Path3D?
 */
Datum
overfront_point3D_path3D(PG_FUNCTION_ARGS)
{
	Point3D    *point = PG_GETARG_POINT3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overfront_point3D_path3D_internal(point, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overfront_point3D_path3D);

bool
overfront_point3D_path3D_internal(Point3D *point, Path3D *path)
{
	return FPle(point->z, path->boundbox.high.z);
}

/*
 * Is the Point3D strictly at the back of the Path3D?
 */
Datum
back_point3D_path3D(PG_FUNCTION_ARGS)
{
	Point3D    *point = PG_GETARG_POINT3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = back_point3D_path3D_internal(point, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(back_point3D_path3D);

bool
back_point3D_path3D_internal(Point3D *point, Path3D *path)
{
	return FPgt(point->z, path->boundbox.high.z);
}

/*
 * Is the Point3D at the back of or in the Path3D?
 */
Datum
overback_point3D_path3D(PG_FUNCTION_ARGS)
{
	Point3D    *point = PG_GETARG_POINT3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overback_point3D_path3D_internal(point, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overback_point3D_path3D);

bool
overback_point3D_path3D_internal(Point3D *point, Path3D *path)
{
	return FPge(point->z, path->boundbox.low.z);
}

/*----------------------------------------------------------*/

/* point3D OPERATOR polygon3D */

/*
 * Is the Point3D strictly to the left of the Polygon3D?
 */
Datum
left_point3D_poly3D(PG_FUNCTION_ARGS)
{
	Point3D    *point = PG_GETARG_POINT3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = left_point3D_poly3D_internal(point, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(left_point3D_poly3D);

bool
left_point3D_poly3D_internal(Point3D *point, Polygon3D *poly)
{
	return FPlt(point->x, poly->boundbox.low.x);
}

/*
 * Is the Point3D to the left of or in the Polygon3D?
 */
Datum
overleft_point3D_poly3D(PG_FUNCTION_ARGS)
{
	Point3D    *point = PG_GETARG_POINT3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overleft_point3D_poly3D_internal(point, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overleft_point3D_poly3D);

bool
overleft_point3D_poly3D_internal(Point3D *point, Polygon3D *poly)
{
	return FPle(point->x, poly->boundbox.high.x);
}

/*
 * Is the Point3D strictly to the right of the Polygon3D?
 */
Datum
right_point3D_poly3D(PG_FUNCTION_ARGS)
{
	Point3D    *point = PG_GETARG_POINT3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = right_point3D_poly3D_internal(point, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(right_point3D_poly3D);

bool
right_point3D_poly3D_internal(Point3D *point, Polygon3D *poly)
{
	return FPgt(point->x, poly->boundbox.high.x);
}

/*
 * Is the Point3D to the right of or in the Polygon3D?
 */
Datum
overright_point3D_poly3D(PG_FUNCTION_ARGS)
{
	Point3D    *point = PG_GETARG_POINT3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overright_point3D_poly3D_internal(point, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overright_point3D_poly3D);

bool
overright_point3D_poly3D_internal(Point3D *point, Polygon3D *poly)
{
	return FPge(point->x, poly->boundbox.low.x);
}

/*
 * Is the Point3D strictly below the Polygon3D?
 */
Datum
below_point3D_poly3D(PG_FUNCTION_ARGS)
{
	Point3D    *point = PG_GETARG_POINT3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = below_point3D_poly3D_internal(point, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(below_point3D_poly3D);

bool
below_point3D_poly3D_internal(Point3D *point, Polygon3D *poly)
{
	return FPlt(point->y, poly->boundbox.low.y);
}

/*
 * Is the Point3D below or in the Polygon3D?
 */
Datum
overbelow_point3D_poly3D(PG_FUNCTION_ARGS)
{
	Point3D    *point = PG_GETARG_POINT3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overbelow_point3D_poly3D_internal(point, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overbelow_point3D_poly3D);

bool
overbelow_point3D_poly3D_internal(Point3D *point, Polygon3D *poly)
{
	return FPle(point->y, poly->boundbox.high.y);
}

/*
 * Is the Point3D strictly above the Polygon3D?
 */
Datum
above_point3D_poly3D(PG_FUNCTION_ARGS)
{
	Point3D    *point = PG_GETARG_POINT3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = above_point3D_poly3D_internal(point, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(above_point3D_poly3D);

bool
above_point3D_poly3D_internal(Point3D *point, Polygon3D *poly)
{
	return FPgt(point->y, poly->boundbox.high.y);
}

/*
 * Is the Point3D above or in the Polygon3D?
 */
Datum
overabove_point3D_poly3D(PG_FUNCTION_ARGS)
{
	Point3D    *point = PG_GETARG_POINT3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overabove_point3D_poly3D_internal(point, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overabove_point3D_poly3D);

bool
overabove_point3D_poly3D_internal(Point3D *point, Polygon3D *poly)
{
	return FPge(point->y, poly->boundbox.low.y);
}

/*
 * Is the Point3D strictly in front of the Polygon3D?
 */
Datum
front_point3D_poly3D(PG_FUNCTION_ARGS)
{
	Point3D    *point = PG_GETARG_POINT3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = front_point3D_poly3D_internal(point, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(front_point3D_poly3D);

bool
front_point3D_poly3D_internal(Point3D *point, Polygon3D *poly)
{
	return FPlt(point->z, poly->boundbox.low.z);
}

/*
 * Is the Point3D in front of or in the Polygon3D?
 */
Datum
overfront_point3D_poly3D(PG_FUNCTION_ARGS)
{
	Point3D    *point = PG_GETARG_POINT3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overfront_point3D_poly3D_internal(point, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overfront_point3D_poly3D);

bool
overfront_point3D_poly3D_internal(Point3D *point, Polygon3D *poly)
{
	return FPle(point->z, poly->boundbox.high.z);
}

/*
 * Is the Point3D strictly at the back of the Polygon3D?
 */
Datum
back_point3D_poly3D(PG_FUNCTION_ARGS)
{
	Point3D    *point = PG_GETARG_POINT3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = back_point3D_poly3D_internal(point, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(back_point3D_poly3D);

bool
back_point3D_poly3D_internal(Point3D *point, Polygon3D *poly)
{
	return FPgt(point->z, poly->boundbox.high.z);
}

/*
 * Is the Point3D at the back of or in the Polygon3D?
 */
Datum
overback_point3D_poly3D(PG_FUNCTION_ARGS)
{
	Point3D    *point = PG_GETARG_POINT3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overback_point3D_poly3D_internal(point, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overback_point3D_poly3D);

bool
overback_point3D_poly3D_internal(Point3D *point, Polygon3D *poly)
{
	return FPge(point->z, poly->boundbox.low.z);
}

/*----------------------------------------------------------*/

/* point3D OPERATOR sphere */

/*
 * Is the Point3D strictly to the left of the Sphere?
 */
Datum
left_point3D_sphere(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(left_point3D_sphere_internal(point, sphere));
}

PG_FUNCTION_INFO_V1(left_point3D_sphere);

bool
left_point3D_sphere_internal(Point3D *point, Sphere *sphere)
{
	return FPlt(point->x, (sphere->center.x - sphere->radius));
}

/*
 * Is the Point3D to the left of or in the Sphere?
 */
Datum
overleft_point3D_sphere(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overleft_point3D_sphere_internal(point, sphere));
}

PG_FUNCTION_INFO_V1(overleft_point3D_sphere);

bool
overleft_point3D_sphere_internal(Point3D *point, Sphere *sphere)
{
	return FPle(point->x, (sphere->center.x + sphere->radius));
}

/*
 * Is the Point3D strictly to the right of the Sphere?
 */
Datum
right_point3D_sphere(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(right_point3D_sphere_internal(point, sphere));
}

PG_FUNCTION_INFO_V1(right_point3D_sphere);

bool
right_point3D_sphere_internal(Point3D *point, Sphere *sphere)
{
	return FPgt(point->x, (sphere->center.x + sphere->radius));
}

/*
 * Is the Point3D to the right of or in the Sphere?
 */
Datum
overright_point3D_sphere(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overright_point3D_sphere_internal(point, sphere));
}

PG_FUNCTION_INFO_V1(overright_point3D_sphere);

bool
overright_point3D_sphere_internal(Point3D *point, Sphere *sphere)
{
	return FPge(point->x, (sphere->center.x - sphere->radius));
}

/*
 * Is the Point3D strictly below the Sphere?
 */
Datum
below_point3D_sphere(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(below_point3D_sphere_internal(point, sphere));
}

PG_FUNCTION_INFO_V1(below_point3D_sphere);

bool
below_point3D_sphere_internal(Point3D *point, Sphere *sphere)
{
	return FPlt(point->y, (sphere->center.y - sphere->radius));
}

/*
 * Is the Point3D below or in the Sphere?
 */
Datum
overbelow_point3D_sphere(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overbelow_point3D_sphere_internal(point, sphere));
}

PG_FUNCTION_INFO_V1(overbelow_point3D_sphere);

bool
overbelow_point3D_sphere_internal(Point3D *point, Sphere *sphere)
{
	return FPle(point->y, (sphere->center.y + sphere->radius));
}

/*
 * Is the Point3D strictly above the Sphere?
 */
Datum
above_point3D_sphere(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(above_point3D_sphere_internal(point, sphere));
}

PG_FUNCTION_INFO_V1(above_point3D_sphere);

bool
above_point3D_sphere_internal(Point3D *point, Sphere *sphere)
{
	return FPgt(point->y, (sphere->center.y + sphere->radius));
}

/*
 * Is the Point3D above or in the Sphere?
 */
Datum
overabove_point3D_sphere(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overabove_point3D_sphere_internal(point, sphere));
}

PG_FUNCTION_INFO_V1(overabove_point3D_sphere);

bool
overabove_point3D_sphere_internal(Point3D *point, Sphere *sphere)
{
	return FPge(point->y, (sphere->center.y - sphere->radius));
}

/*
 * Is the Point3D strictly in front of the Sphere?
 */
Datum
front_point3D_sphere(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(front_point3D_sphere_internal(point, sphere));
}

PG_FUNCTION_INFO_V1(front_point3D_sphere);

bool
front_point3D_sphere_internal(Point3D *point, Sphere *sphere)
{
	return FPlt(point->z, (sphere->center.z - sphere->radius));
}

/*
 * Is the Point3D in front of or in the Sphere?
 */
Datum
overfront_point3D_sphere(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overfront_point3D_sphere_internal(point, sphere));
}

PG_FUNCTION_INFO_V1(overfront_point3D_sphere);

bool
overfront_point3D_sphere_internal(Point3D *point, Sphere *sphere)
{
	return FPle(point->z, (sphere->center.z + sphere->radius));
}

/*
 * Is the Point3D strictly at the back of the Sphere?
 */
Datum
back_point3D_sphere(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(back_point3D_sphere_internal(point, sphere));
}

PG_FUNCTION_INFO_V1(back_point3D_sphere);

bool
back_point3D_sphere_internal(Point3D *point, Sphere *sphere)
{
	return FPgt(point->z, (sphere->center.z + sphere->radius));
}

/*
 * Is the Point3D at the back of or in the Sphere?
 */
Datum
overback_point3D_sphere(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overback_point3D_sphere_internal(point, sphere));
}

PG_FUNCTION_INFO_V1(overback_point3D_sphere);

bool
overback_point3D_sphere_internal(Point3D *point, Sphere *sphere)
{
	return FPge(point->z, (sphere->center.z - sphere->radius));
}

/*****************************************************************************/

/* lseg3D OPERATOR point3D */

/*
 * Is the Lseg3D strictly to the left of the Point3D?
 */
Datum
left_lseg3D_point3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(left_lseg3D_point3D_internal(lseg, point));
}

PG_FUNCTION_INFO_V1(left_lseg3D_point3D);

bool
left_lseg3D_point3D_internal(Lseg3D *lseg, Point3D *point)
{
	return FPlt(Max(lseg->p[0].x,lseg->p[1].x), point->x);
}

/*
 * Is the Lseg3D to the left of or overlaps the Point3D?
 */
Datum
overleft_lseg3D_point3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overleft_lseg3D_point3D_internal(lseg, point));
}

PG_FUNCTION_INFO_V1(overleft_lseg3D_point3D);

bool
overleft_lseg3D_point3D_internal(Lseg3D *lseg, Point3D *point)
{
	return FPle(Max(lseg->p[0].x,lseg->p[1].x), point->x);
}

/*
 * Is the Lseg3D strictly to the right of the Point3D?
 */
Datum
right_lseg3D_point3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(right_lseg3D_point3D_internal(lseg, point));
}

PG_FUNCTION_INFO_V1(right_lseg3D_point3D);

bool
right_lseg3D_point3D_internal(Lseg3D *lseg, Point3D *point)
{
	return FPgt(Min(lseg->p[0].x,lseg->p[1].x), point->x);
}

/*
 * Is the Lseg3D to the right of or overlaps the Point3D?
 */
Datum
overright_lseg3D_point3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overright_lseg3D_point3D_internal(lseg, point));
}

PG_FUNCTION_INFO_V1(overright_lseg3D_point3D);

bool
overright_lseg3D_point3D_internal(Lseg3D *lseg, Point3D *point)
{
	return FPge(Min(lseg->p[0].x,lseg->p[1].x), point->x);
}

/*
 * Is the Lseg3D strictly below the Point3D?
 */
Datum
below_lseg3D_point3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(below_lseg3D_point3D_internal(lseg, point));
}

PG_FUNCTION_INFO_V1(below_lseg3D_point3D);

bool
below_lseg3D_point3D_internal(Lseg3D *lseg, Point3D *point)
{
	return FPlt(Max(lseg->p[0].y,lseg->p[1].y), point->y);
}

/*
 * Is the Point3D below or in the Sphere?
 */
Datum
overbelow_lseg3D_point3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overbelow_lseg3D_point3D_internal(lseg, point));
}

PG_FUNCTION_INFO_V1(overbelow_lseg3D_point3D);

bool
overbelow_lseg3D_point3D_internal(Lseg3D *lseg, Point3D *point)
{
	return FPle(Max(lseg->p[0].y,lseg->p[1].y), point->y);
}

/*
 * Is the Lseg3D strictly above the Point3D?
 */
Datum
above_lseg3D_point3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(above_lseg3D_point3D_internal(lseg, point));
}

PG_FUNCTION_INFO_V1(above_lseg3D_point3D);

bool
above_lseg3D_point3D_internal(Lseg3D *lseg, Point3D *point)
{
	return FPgt(Min(lseg->p[0].y,lseg->p[1].y), point->y);
}

/*
 * Is the Lseg3D above or overlaps the Point3D?
 */
Datum
overabove_lseg3D_point3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overabove_lseg3D_point3D_internal(lseg, point));
}

PG_FUNCTION_INFO_V1(overabove_lseg3D_point3D);

bool
overabove_lseg3D_point3D_internal(Lseg3D *lseg, Point3D *point)
{
	return FPge(Min(lseg->p[0].y,lseg->p[1].y), point->y);
}

/*
 * Is the Lseg3D strictly in front of the Point3D?
 */
Datum
front_lseg3D_point3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(front_lseg3D_point3D_internal(lseg, point));
}

PG_FUNCTION_INFO_V1(front_lseg3D_point3D);

bool
front_lseg3D_point3D_internal(Lseg3D *lseg, Point3D *point)
{
	return FPlt(Max(lseg->p[0].z,lseg->p[1].z), point->z);
}

/*
 * Is the Lseg3D in front of or overlaps the Point3D?
 */
Datum
overfront_lseg3D_point3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overfront_lseg3D_point3D_internal(lseg, point));
}

PG_FUNCTION_INFO_V1(overfront_lseg3D_point3D);

bool
overfront_lseg3D_point3D_internal(Lseg3D *lseg, Point3D *point)
{
	return FPle(Max(lseg->p[0].z,lseg->p[1].z), point->z);
}

/*
 * Is the Lseg3D strictly at the back of the Point3D?
 */
Datum
back_lseg3D_point3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(back_lseg3D_point3D_internal(lseg, point));
}

PG_FUNCTION_INFO_V1(back_lseg3D_point3D);

bool
back_lseg3D_point3D_internal(Lseg3D *lseg, Point3D *point)
{
	return FPgt(Min(lseg->p[0].z,lseg->p[1].z), point->z);
}

/*
 * Is the Lseg3D at the back of or overlaps the Point3D?
 */
Datum
overback_lseg3D_point3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overback_lseg3D_point3D_internal(lseg, point));
}

PG_FUNCTION_INFO_V1(overback_lseg3D_point3D);

bool
overback_lseg3D_point3D_internal(Lseg3D *lseg, Point3D *point)
{
	return FPge(Min(lseg->p[0].z,lseg->p[1].z), point->z);
}

/*----------------------------------------------------------*/

/* lseg3D OPERATOR lseg3D */

/*
 * Is the first Lseg3D strictly to the left of the second Lseg3D?
 */
Datum
left_lseg3D_lseg3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D	   *lseg2 = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(left_lseg3D_lseg3D_internal(lseg1, lseg2));
}

PG_FUNCTION_INFO_V1(left_lseg3D_lseg3D);

bool
left_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2)
{
	return FPlt(Max(lseg1->p[0].x,lseg1->p[1].x), 
				Min(lseg2->p[0].x,lseg2->p[1].x));
}

/*
 * Is the first Lseg3D to the left of or overlaps the second Lseg3D?
 */
Datum
overleft_lseg3D_lseg3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D	   *lseg2 = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overleft_lseg3D_lseg3D_internal(lseg1, lseg2));
}

PG_FUNCTION_INFO_V1(overleft_lseg3D_lseg3D);

bool
overleft_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2)
{
	return FPle(Max(lseg1->p[0].x,lseg1->p[1].x), 
				Min(lseg2->p[0].x,lseg2->p[1].x));
}

/*
 * Is the first Lseg3D strictly to the right of the second Lseg3D?
 */
Datum
right_lseg3D_lseg3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D	   *lseg2 = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(right_lseg3D_lseg3D_internal(lseg1, lseg2));
}

PG_FUNCTION_INFO_V1(right_lseg3D_lseg3D);

bool
right_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2)
{
	return FPgt(Min(lseg1->p[0].x,lseg1->p[1].x), 
				Max(lseg2->p[0].x,lseg2->p[1].x));
}

/*
 * Is the first Lseg3D to the right of or overlaps the second Lseg3D?
 */
Datum
overright_lseg3D_lseg3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D	   *lseg2 = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overright_lseg3D_lseg3D_internal(lseg1, lseg2));
}

PG_FUNCTION_INFO_V1(overright_lseg3D_lseg3D);

bool
overright_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2)
{
	return FPge(Min(lseg1->p[0].x,lseg1->p[1].x), 
				Max(lseg2->p[0].x,lseg2->p[1].x));
}

/*
 * Is the first Lseg3D strictly below the second Lseg3D?
 */
Datum
below_lseg3D_lseg3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D	   *lseg2 = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(below_lseg3D_lseg3D_internal(lseg1, lseg2));
}

PG_FUNCTION_INFO_V1(below_lseg3D_lseg3D);

bool
below_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2)
{
	return FPlt(Max(lseg1->p[0].y,lseg1->p[1].y), 
				Min(lseg2->p[0].y,lseg2->p[1].y));
}

/*
 * Is the first Lseg3D below or overlaps the second Lseg3D?
 */
Datum
overbelow_lseg3D_lseg3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D	   *lseg2 = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overbelow_lseg3D_lseg3D_internal(lseg1, lseg2));
}

PG_FUNCTION_INFO_V1(overbelow_lseg3D_lseg3D);

bool
overbelow_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2)
{
	return FPle(Max(lseg1->p[0].y,lseg1->p[1].y),
				Min(lseg2->p[0].y,lseg2->p[1].y));
}

/*
 * Is the first Lseg3D strictly above the second Lseg3D?
 */
Datum
above_lseg3D_lseg3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D	   *lseg2 = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(above_lseg3D_lseg3D_internal(lseg1, lseg2));
}

PG_FUNCTION_INFO_V1(above_lseg3D_lseg3D);

bool
above_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2)
{
	return FPgt(Min(lseg1->p[0].y,lseg1->p[1].y), 
				Max(lseg2->p[0].y,lseg2->p[1].y));
}

/*
 * Is the first Lseg3D above or overlaps the second Lseg3D?
 */
Datum
overabove_lseg3D_lseg3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D	   *lseg2 = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overabove_lseg3D_lseg3D_internal(lseg1, lseg2));
}

PG_FUNCTION_INFO_V1(overabove_lseg3D_lseg3D);

bool
overabove_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2)
{
	return FPge(Min(lseg1->p[0].y,lseg1->p[1].y), 
				Max(lseg2->p[0].y,lseg2->p[1].y));
}

/*
 * Is the first Lseg3D strictly in front of the second Lseg3D?
 */
Datum
front_lseg3D_lseg3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D	   *lseg2 = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(front_lseg3D_lseg3D_internal(lseg1, lseg2));
}

PG_FUNCTION_INFO_V1(front_lseg3D_lseg3D);

bool
front_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2)
{
	return FPlt(Max(lseg1->p[0].z,lseg1->p[1].z),
				Min(lseg2->p[0].z,lseg2->p[1].z));
}

/*
 * Is the first Lseg3D in front of or overlaps the second Lseg3D?
 */
Datum
overfront_lseg3D_lseg3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D	   *lseg2 = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overfront_lseg3D_lseg3D_internal(lseg1, lseg2));
}

PG_FUNCTION_INFO_V1(overfront_lseg3D_lseg3D);

bool
overfront_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2)
{
	return FPle(Max(lseg1->p[0].z,lseg1->p[1].z),
				Min(lseg2->p[0].z,lseg2->p[1].z));
}

/*
 * Is the first Lseg3D strictly at the back of the second Lseg3D?
 */
Datum
back_lseg3D_lseg3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D	   *lseg2 = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(back_lseg3D_lseg3D_internal(lseg1, lseg2));
}

PG_FUNCTION_INFO_V1(back_lseg3D_lseg3D);

bool
back_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2)
{
	return FPgt(Min(lseg1->p[0].z,lseg1->p[1].z), 
				Max(lseg2->p[0].z,lseg2->p[1].z));
}

/*
 * Is the first Lseg3D at the back of or overlaps the second Lseg3D?
 */
Datum
overback_lseg3D_lseg3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D	   *lseg2 = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overback_lseg3D_lseg3D_internal(lseg1, lseg2));
}

PG_FUNCTION_INFO_V1(overback_lseg3D_lseg3D);

bool
overback_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2)
{
	return FPge(Min(lseg1->p[0].z,lseg1->p[1].z), 
				Max(lseg2->p[0].z,lseg2->p[1].z));
}

/*----------------------------------------------------------*/

/* lseg3D OPERATOR line3D */

/*
 * Is the Lseg3D strictly to the left of the Line3D?
 */
Datum
left_lseg3D_line3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(left_lseg3D_line3D_internal(lseg, line));
}

PG_FUNCTION_INFO_V1(left_lseg3D_line3D);

bool
left_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPlt(Max(lseg->p[0].x,lseg->p[1].x), line->p[0].x);
}

/*
 * Is the Lseg3D to the left of or overlaps the Line3D?
 */
Datum
overleft_lseg3D_line3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overleft_lseg3D_line3D_internal(lseg, line));
}

PG_FUNCTION_INFO_V1(overleft_lseg3D_line3D);

bool
overleft_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPle(Max(lseg->p[0].x,lseg->p[1].x), line->p[0].x);
}

/*
 * Is the Lseg3D strictly to the right of the Line3D?
 */
Datum
right_lseg3D_line3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(right_lseg3D_line3D_internal(lseg, line));
}

PG_FUNCTION_INFO_V1(right_lseg3D_line3D);

bool
right_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPgt(Min(lseg->p[0].x,lseg->p[1].x), line->p[0].x);
}

/*
 * Is the Lseg3D to the right of or overlaps the Line3D?
 */
Datum
overright_lseg3D_line3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overright_lseg3D_line3D_internal(lseg, line));
}

PG_FUNCTION_INFO_V1(overright_lseg3D_line3D);

bool
overright_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPge(Min(lseg->p[0].x,lseg->p[1].x), line->p[0].x);
}

/*
 * Is the Lseg3D strictly below the Line3D?
 */
Datum
below_lseg3D_line3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(below_lseg3D_line3D_internal(lseg, line));
}

PG_FUNCTION_INFO_V1(below_lseg3D_line3D);

bool
below_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPlt(Max(lseg->p[0].y,lseg->p[1].y), line->p[0].y);
}

/*
 * Is the Lseg3D below or overlaps the Line3D?
 */
Datum
overbelow_lseg3D_line3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overbelow_lseg3D_line3D_internal(lseg, line));
}

PG_FUNCTION_INFO_V1(overbelow_lseg3D_line3D);

bool
overbelow_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPle(Max(lseg->p[0].y,lseg->p[1].y), line->p[0].y);
}

/*
 * Is the Lseg3D strictly above the Line3D?
 */
Datum
above_lseg3D_line3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(above_lseg3D_line3D_internal(lseg, line));
}

PG_FUNCTION_INFO_V1(above_lseg3D_line3D);

bool
above_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPgt(Min(lseg->p[0].y,lseg->p[1].y), line->p[0].y);
}

/*
 * Is the Lseg3D above or overlaps the Line3D?
 */
Datum
overabove_lseg3D_line3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overabove_lseg3D_line3D_internal(lseg, line));
}

PG_FUNCTION_INFO_V1(overabove_lseg3D_line3D);

bool
overabove_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPge(Min(lseg->p[0].y,lseg->p[1].y), line->p[0].y);
}

/*
 * Is the Lseg3D strictly in front of the Line3D?
 */
Datum
front_lseg3D_line3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(front_lseg3D_line3D_internal(lseg, line));
}

PG_FUNCTION_INFO_V1(front_lseg3D_line3D);

bool
front_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPlt(Max(lseg->p[0].z,lseg->p[1].z), line->p[0].z);
}

/*
 * Is the Lseg3D in front of or overlaps the Line3D?
 */
Datum
overfront_lseg3D_line3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overfront_lseg3D_line3D_internal(lseg, line));
}

PG_FUNCTION_INFO_V1(overfront_lseg3D_line3D);

bool
overfront_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPle(Max(lseg->p[0].z,lseg->p[1].z), line->p[0].z);
}

/*
 * Is the Lseg3D strictly at the back of the Line3D?
 */
Datum
back_lseg3D_line3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(back_lseg3D_line3D_internal(lseg, line));
}

PG_FUNCTION_INFO_V1(back_lseg3D_line3D);

bool
back_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPgt(Min(lseg->p[0].z,lseg->p[1].z), line->p[0].z);
}

/*
 * Is the Lseg3D at the back of or overlaps the Line3D?
 */
Datum
overback_lseg3D_line3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overback_lseg3D_line3D_internal(lseg, line));
}

PG_FUNCTION_INFO_V1(overback_lseg3D_line3D);

bool
overback_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPge(Min(lseg->p[0].z,lseg->p[1].z), line->p[0].z);
}

/*----------------------------------------------------------*/

/* lseg3D OPERATOR box3D */

/*
 * Is the Lseg3D strictly to the left of the Box3D?
 */
Datum
left_lseg3D_box3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(left_lseg3D_box3D_internal(lseg, box));
}

PG_FUNCTION_INFO_V1(left_lseg3D_box3D);

bool
left_lseg3D_box3D_internal(Lseg3D *lseg, Box3D *box)
{
	return FPlt(Max(lseg->p[0].x,lseg->p[1].x), box->low.x);
}

/*
 * Is the Lseg3D to the left of or overlaps the Box3D?
 */
Datum
overleft_lseg3D_box3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overleft_lseg3D_box3D_internal(lseg, box));
}

PG_FUNCTION_INFO_V1(overleft_lseg3D_box3D);

bool
overleft_lseg3D_box3D_internal(Lseg3D *lseg, Box3D *box)
{
	return FPle(Min(lseg->p[0].x,lseg->p[1].x), box->high.x);
}

/*
 * Is the Lseg3D strictly to the right of the Box3D?
 */
Datum
right_lseg3D_box3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(right_lseg3D_box3D_internal(lseg, box));
}

PG_FUNCTION_INFO_V1(right_lseg3D_box3D);

bool
right_lseg3D_box3D_internal(Lseg3D *lseg, Box3D *box)
{
	return FPgt(Min(lseg->p[0].x,lseg->p[1].x), box->high.x);
}

/*
 * Is the Lseg3D to the right of or overlaps the Box3D?
 */
Datum
overright_lseg3D_box3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overright_lseg3D_box3D_internal(lseg, box));
}

PG_FUNCTION_INFO_V1(overright_lseg3D_box3D);

bool
overright_lseg3D_box3D_internal(Lseg3D *lseg, Box3D *box)
{
	return FPge(Max(lseg->p[0].x,lseg->p[1].x), box->low.x);
}

/*
 * Is the Lseg3D strictly below the Box3D?
 */
Datum
below_lseg3D_box3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(below_lseg3D_box3D_internal(lseg, box));
}

PG_FUNCTION_INFO_V1(below_lseg3D_box3D);

bool
below_lseg3D_box3D_internal(Lseg3D *lseg, Box3D *box)
{
	return FPlt(Max(lseg->p[0].y,lseg->p[1].y), box->low.y);
}

/*
 * Is the Lseg3D below or overlaps the Box3D?
 */
Datum
overbelow_lseg3D_box3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overbelow_lseg3D_box3D_internal(lseg, box));
}

PG_FUNCTION_INFO_V1(overbelow_lseg3D_box3D);

bool
overbelow_lseg3D_box3D_internal(Lseg3D *lseg, Box3D *box)
{
	return FPle(Min(lseg->p[0].y,lseg->p[1].y), box->high.y);
}

/*
 * Is the Lseg3D strictly above the Box3D?
 */
Datum
above_lseg3D_box3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(above_lseg3D_box3D_internal(lseg, box));
}

PG_FUNCTION_INFO_V1(above_lseg3D_box3D);

bool
above_lseg3D_box3D_internal(Lseg3D *lseg, Box3D *box)
{
	return FPgt(Min(lseg->p[0].y,lseg->p[1].y), box->high.y);
}

/*
 * Is the Lseg3D above or overlaps the Box3D?
 */
Datum
overabove_lseg3D_box3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overabove_lseg3D_box3D_internal(lseg, box));
}

PG_FUNCTION_INFO_V1(overabove_lseg3D_box3D);

bool
overabove_lseg3D_box3D_internal(Lseg3D *lseg, Box3D *box)
{
	return FPge(Max(lseg->p[0].y,lseg->p[1].y), box->low.y);
}

/*
 * Is the Lseg3D strictly in front of the Box3D?
 */
Datum
front_lseg3D_box3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(front_lseg3D_box3D_internal(lseg, box));
}

PG_FUNCTION_INFO_V1(front_lseg3D_box3D);

bool
front_lseg3D_box3D_internal(Lseg3D *lseg, Box3D *box)
{
	return FPlt(Max(lseg->p[0].z,lseg->p[1].z), box->low.z);
}

/*
 * Is the Lseg3D in front of or overlaps the Box3D?
 */
Datum
overfront_lseg3D_box3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overfront_lseg3D_box3D_internal(lseg, box));
}

PG_FUNCTION_INFO_V1(overfront_lseg3D_box3D);

bool
overfront_lseg3D_box3D_internal(Lseg3D *lseg, Box3D *box)
{
	return FPle(Min(lseg->p[0].z,lseg->p[1].z), box->high.z);
}

/*
 * Is the Lseg3D strictly at the back of the Box3D?
 */
Datum
back_lseg3D_box3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(back_lseg3D_box3D_internal(lseg, box));
}

PG_FUNCTION_INFO_V1(back_lseg3D_box3D);

bool
back_lseg3D_box3D_internal(Lseg3D *lseg, Box3D *box)
{
	return FPgt(Min(lseg->p[0].z,lseg->p[1].z), box->high.z);
}

/*
 * Is the Lseg3D at the back of or overlaps the Box3D?
 */
Datum
overback_lseg3D_box3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overback_lseg3D_box3D_internal(lseg, box));
}

PG_FUNCTION_INFO_V1(overback_lseg3D_box3D);

bool
overback_lseg3D_box3D_internal(Lseg3D *lseg, Box3D *box)
{
	return FPge(Max(lseg->p[0].z,lseg->p[1].z), box->low.z);
}

/*----------------------------------------------------------*/

/* lseg3D OPERATOR path3D */

/*
 * Is the Lseg3D strictly to the left of the Path3D?
 */
Datum
left_lseg3D_path3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = left_lseg3D_path3D_internal(lseg, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(left_lseg3D_path3D);

bool
left_lseg3D_path3D_internal(Lseg3D *lseg, Path3D *path)
{
	return FPlt(Max(lseg->p[0].x,lseg->p[1].x), path->boundbox.low.x);
}

/*
 * Is the Lseg3D to the left of or overlaps the Path3D?
 */
Datum
overleft_lseg3D_path3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overleft_lseg3D_path3D_internal(lseg, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overleft_lseg3D_path3D);

bool
overleft_lseg3D_path3D_internal(Lseg3D *lseg, Path3D *path)
{
	return FPle(Min(lseg->p[0].x,lseg->p[1].x), path->boundbox.high.x);
}

/*
 * Is the Lseg3D strictly to the right of the Path3D?
 */
Datum
right_lseg3D_path3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = right_lseg3D_path3D_internal(lseg, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(right_lseg3D_path3D);

bool
right_lseg3D_path3D_internal(Lseg3D *lseg, Path3D *path)
{
	return FPgt(Min(lseg->p[0].x,lseg->p[1].x), path->boundbox.high.x);
}

/*
 * Is the Lseg3D to the right of or overlaps the Path3D?
 */
Datum
overright_lseg3D_path3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overright_lseg3D_path3D_internal(lseg, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overright_lseg3D_path3D);

bool
overright_lseg3D_path3D_internal(Lseg3D *lseg, Path3D *path)
{
	return FPge(Max(lseg->p[0].x,lseg->p[1].x), path->boundbox.low.x);
}

/*
 * Is the Lseg3D strictly below the Path3D?
 */
Datum
below_lseg3D_path3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = below_lseg3D_path3D_internal(lseg, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(below_lseg3D_path3D);

bool
below_lseg3D_path3D_internal(Lseg3D *lseg, Path3D *path)
{
	return FPlt(Max(lseg->p[0].y,lseg->p[1].y), path->boundbox.low.y);
}

/*
 * Is the Lseg3D below or overlaps the Path3D?
 */
Datum
overbelow_lseg3D_path3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overbelow_lseg3D_path3D_internal(lseg, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overbelow_lseg3D_path3D);

bool
overbelow_lseg3D_path3D_internal(Lseg3D *lseg, Path3D *path)
{
	return FPle(Min(lseg->p[0].y,lseg->p[1].y), path->boundbox.high.y);
}

/*
 * Is the Lseg3D strictly above the Path3D?
 */
Datum
above_lseg3D_path3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = above_lseg3D_path3D_internal(lseg, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(above_lseg3D_path3D);

bool
above_lseg3D_path3D_internal(Lseg3D *lseg, Path3D *path)
{
	return FPgt(Min(lseg->p[0].y,lseg->p[1].y), path->boundbox.high.y);
}

/*
 * Is the Lseg3D above or overlaps the Path3D?
 */
Datum
overabove_lseg3D_path3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overabove_lseg3D_path3D_internal(lseg, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overabove_lseg3D_path3D);

bool
overabove_lseg3D_path3D_internal(Lseg3D *lseg, Path3D *path)
{
	return FPge(Max(lseg->p[0].y,lseg->p[1].y), path->boundbox.low.y);
}

/*
 * Is the Lseg3D strictly in front of the Path3D?
 */
Datum
front_lseg3D_path3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = front_lseg3D_path3D_internal(lseg, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(front_lseg3D_path3D);

bool
front_lseg3D_path3D_internal(Lseg3D *lseg, Path3D *path)
{
	return FPlt(Max(lseg->p[0].z,lseg->p[1].z), path->boundbox.low.z);
}

/*
 * Is the Lseg3D in front of or overlaps the Path3D?
 */
Datum
overfront_lseg3D_path3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overfront_lseg3D_path3D_internal(lseg, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overfront_lseg3D_path3D);

bool
overfront_lseg3D_path3D_internal(Lseg3D *lseg, Path3D *path)
{
	return FPle(Min(lseg->p[0].z,lseg->p[1].z), path->boundbox.high.z);
}

/*
 * Is the Lseg3D strictly at the back of the Path3D?
 */
Datum
back_lseg3D_path3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = back_lseg3D_path3D_internal(lseg, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(back_lseg3D_path3D);

bool
back_lseg3D_path3D_internal(Lseg3D *lseg, Path3D *path)
{
	return FPgt(Min(lseg->p[0].z,lseg->p[1].z), path->boundbox.high.z);
}

/*
 * Is the Lseg3D at the back of or overlaps the Path3D?
 */
Datum
overback_lseg3D_path3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overback_lseg3D_path3D_internal(lseg, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overback_lseg3D_path3D);

bool
overback_lseg3D_path3D_internal(Lseg3D *lseg, Path3D *path)
{
	return FPge(Max(lseg->p[0].z,lseg->p[1].z), path->boundbox.low.z);
}

/*----------------------------------------------------------*/

/* lseg3D OPERATOR poly3D */

/*
 * Is the Lseg3D strictly to the left of the Polygon3D?
 */
Datum
left_lseg3D_poly3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = left_lseg3D_poly3D_internal(lseg, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(left_lseg3D_poly3D);

bool
left_lseg3D_poly3D_internal(Lseg3D *lseg, Polygon3D *poly)
{
	return FPlt(Max(lseg->p[0].x,lseg->p[1].x), poly->boundbox.low.x);
}

/*
 * Is the Lseg3D to the left of or overlaps the Polygon3D?
 */
Datum
overleft_lseg3D_poly3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overleft_lseg3D_poly3D_internal(lseg, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overleft_lseg3D_poly3D);

bool
overleft_lseg3D_poly3D_internal(Lseg3D *lseg, Polygon3D *poly)
{
	return FPle(Min(lseg->p[0].x,lseg->p[1].x), poly->boundbox.high.x);
}

/*
 * Is the Lseg3D strictly to the right of the Polygon3D?
 */
Datum
right_lseg3D_poly3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = right_lseg3D_poly3D_internal(lseg, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(right_lseg3D_poly3D);

bool
right_lseg3D_poly3D_internal(Lseg3D *lseg, Polygon3D *poly)
{
	return FPgt(Min(lseg->p[0].x,lseg->p[1].x), poly->boundbox.high.x);
}

/*
 * Is the Lseg3D to the right of or overlaps the Polygon3D?
 */
Datum
overright_lseg3D_poly3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overright_lseg3D_poly3D_internal(lseg, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overright_lseg3D_poly3D);

bool
overright_lseg3D_poly3D_internal(Lseg3D *lseg, Polygon3D *poly)
{
	return FPge(Max(lseg->p[0].x,lseg->p[1].x), poly->boundbox.low.x);
}

/*
 * Is the Lseg3D strictly below the Polygon3D?
 */
Datum
below_lseg3D_poly3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = below_lseg3D_poly3D_internal(lseg, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(below_lseg3D_poly3D);

bool
below_lseg3D_poly3D_internal(Lseg3D *lseg, Polygon3D *poly)
{
	return FPlt(Max(lseg->p[0].y,lseg->p[1].y), poly->boundbox.low.y);
}

/*
 * Is the Lseg3D below or overlaps the Polygon3D?
 */
Datum
overbelow_lseg3D_poly3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overbelow_lseg3D_poly3D_internal(lseg, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overbelow_lseg3D_poly3D);

bool
overbelow_lseg3D_poly3D_internal(Lseg3D *lseg, Polygon3D *poly)
{
	return FPle(Min(lseg->p[0].y,lseg->p[1].y), poly->boundbox.high.y);
}

/*
 * Is the Lseg3D strictly above the Polygon3D?
 */
Datum
above_lseg3D_poly3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = above_lseg3D_poly3D_internal(lseg, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(above_lseg3D_poly3D);

bool
above_lseg3D_poly3D_internal(Lseg3D *lseg, Polygon3D *poly)
{
	return FPgt(Min(lseg->p[0].y,lseg->p[1].y), poly->boundbox.high.y);
}

/*
 * Is the Lseg3D above or overlaps the Polygon3D?
 */
Datum
overabove_lseg3D_poly3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overabove_lseg3D_poly3D_internal(lseg, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overabove_lseg3D_poly3D);

bool
overabove_lseg3D_poly3D_internal(Lseg3D *lseg, Polygon3D *poly)
{
	return FPge(Max(lseg->p[0].y,lseg->p[1].y), poly->boundbox.low.y);
}

/*
 * Is the Lseg3D strictly in front of the Polygon3D?
 */
Datum
front_lseg3D_poly3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = front_lseg3D_poly3D_internal(lseg, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(front_lseg3D_poly3D);

bool
front_lseg3D_poly3D_internal(Lseg3D *lseg, Polygon3D *poly)
{
	return FPlt(Max(lseg->p[0].z,lseg->p[1].z), poly->boundbox.low.z);
}

/*
 * Is the Lseg3D in front of or overlaps the Polygon3D?
 */
Datum
overfront_lseg3D_poly3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overfront_lseg3D_poly3D_internal(lseg, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overfront_lseg3D_poly3D);

bool
overfront_lseg3D_poly3D_internal(Lseg3D *lseg, Polygon3D *poly)
{
	return FPle(Min(lseg->p[0].z,lseg->p[1].z), poly->boundbox.high.z);
}

/*
 * Is the Lseg3D strictly at the back of the Polygon3D?
 */
Datum
back_lseg3D_poly3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = back_lseg3D_poly3D_internal(lseg, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(back_lseg3D_poly3D);

bool
back_lseg3D_poly3D_internal(Lseg3D *lseg, Polygon3D *poly)
{
	return FPgt(Min(lseg->p[0].z,lseg->p[1].z), poly->boundbox.high.z);
}

/*
 * Is the Lseg3D at the back of or overlaps the Polygon3D?
 */
Datum
overback_lseg3D_poly3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overback_lseg3D_poly3D_internal(lseg, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overback_lseg3D_poly3D);

bool
overback_lseg3D_poly3D_internal(Lseg3D *lseg, Polygon3D *poly)
{
	return FPge(Max(lseg->p[0].z,lseg->p[1].z), poly->boundbox.low.z);
}

/*----------------------------------------------------------*/

/* lseg3D OPERATOR sphere */

/*
 * Is the Lseg3D strictly to the left of the Sphere?
 */
Datum
left_lseg3D_sphere(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(left_lseg3D_sphere_internal(lseg, sphere));
}

PG_FUNCTION_INFO_V1(left_lseg3D_sphere);

bool
left_lseg3D_sphere_internal(Lseg3D *lseg, Sphere *sphere)
{
	return FPlt(Max(lseg->p[0].x,lseg->p[1].x), 
				(sphere->center.x - sphere->radius));
}

/*
 * Is the Lseg3D to the left of or overlaps the Sphere?
 */
Datum
overleft_lseg3D_sphere(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overleft_lseg3D_sphere_internal(lseg, sphere));
}

PG_FUNCTION_INFO_V1(overleft_lseg3D_sphere);

bool
overleft_lseg3D_sphere_internal(Lseg3D *lseg, Sphere *sphere)
{
	return FPle(Max(lseg->p[0].x,lseg->p[1].x), 
				(sphere->center.x + sphere->radius));
}

/*
 * Is the Lseg3D strictly to the right of the Sphere?
 */
Datum
right_lseg3D_sphere(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(right_lseg3D_sphere_internal(lseg, sphere));
}

PG_FUNCTION_INFO_V1(right_lseg3D_sphere);

bool
right_lseg3D_sphere_internal(Lseg3D *lseg, Sphere *sphere)
{
	return FPgt(Min(lseg->p[0].x,lseg->p[1].x), 
				(sphere->center.x + sphere->radius));
}

/*
 * Is the Lseg3D to the right of or overlaps the Sphere?
 */
Datum
overright_lseg3D_sphere(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overright_lseg3D_sphere_internal(lseg, sphere));
}

PG_FUNCTION_INFO_V1(overright_lseg3D_sphere);

bool
overright_lseg3D_sphere_internal(Lseg3D *lseg, Sphere *sphere)
{
	return FPge(Min(lseg->p[0].x,lseg->p[1].x), 
				(sphere->center.x - sphere->radius));
}

/*
 * Is the Lseg3D strictly below the Sphere?
 */
Datum
below_lseg3D_sphere(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(below_lseg3D_sphere_internal(lseg, sphere));
}

PG_FUNCTION_INFO_V1(below_lseg3D_sphere);

bool
below_lseg3D_sphere_internal(Lseg3D *lseg, Sphere *sphere)
{
	return FPlt(Max(lseg->p[0].y,lseg->p[1].y), 
				(sphere->center.y - sphere->radius));
}

/*
 * Is the Lseg3D below or overlaps the Sphere?
 */
Datum
overbelow_lseg3D_sphere(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overbelow_lseg3D_sphere_internal(lseg, sphere));
}

PG_FUNCTION_INFO_V1(overbelow_lseg3D_sphere);

bool
overbelow_lseg3D_sphere_internal(Lseg3D *lseg, Sphere *sphere)
{
	return FPle(Max(lseg->p[0].y,lseg->p[1].y), 
				(sphere->center.y + sphere->radius));
}

/*
 * Is the Lseg3D strictly above the Sphere?
 */
Datum
above_lseg3D_sphere(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(above_lseg3D_sphere_internal(lseg, sphere));
}

PG_FUNCTION_INFO_V1(above_lseg3D_sphere);

bool
above_lseg3D_sphere_internal(Lseg3D *lseg, Sphere *sphere)
{
	return FPgt(Min(lseg->p[0].y,lseg->p[1].y), 
				(sphere->center.y + sphere->radius));
}

/*
 * Is the Lseg3D above or overlaps the Sphere?
 */
Datum
overabove_lseg3D_sphere(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overabove_lseg3D_sphere_internal(lseg, sphere));
}

PG_FUNCTION_INFO_V1(overabove_lseg3D_sphere);

bool
overabove_lseg3D_sphere_internal(Lseg3D *lseg, Sphere *sphere)
{
	return FPge(Min(lseg->p[0].y,lseg->p[1].y), 
				(sphere->center.y - sphere->radius));
}

/*
 * Is the Lseg3D strictly in front of the Sphere?
 */
Datum
front_lseg3D_sphere(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(front_lseg3D_sphere_internal(lseg, sphere));
}

PG_FUNCTION_INFO_V1(front_lseg3D_sphere);

bool
front_lseg3D_sphere_internal(Lseg3D *lseg, Sphere *sphere)
{
	return FPlt(Max(lseg->p[0].z,lseg->p[1].z), 
				(sphere->center.z - sphere->radius));
}

/*
 * Is the Lseg3D in front of or overlaps the Sphere?
 */
Datum
overfront_lseg3D_sphere(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overfront_lseg3D_sphere_internal(lseg, sphere));
}

PG_FUNCTION_INFO_V1(overfront_lseg3D_sphere);

bool
overfront_lseg3D_sphere_internal(Lseg3D *lseg, Sphere *sphere)
{
	return FPle(Max(lseg->p[0].z,lseg->p[1].z), 
				(sphere->center.z + sphere->radius));
}

/*
 * Is the Lseg3D strictly at the back of the Sphere?
 */
Datum
back_lseg3D_sphere(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(back_lseg3D_sphere_internal(lseg, sphere));
}

PG_FUNCTION_INFO_V1(back_lseg3D_sphere);

bool
back_lseg3D_sphere_internal(Lseg3D *lseg, Sphere *sphere)
{
	return FPgt(Min(lseg->p[0].z,lseg->p[1].z), 
				(sphere->center.z + sphere->radius));
}

/*
 * Is the Lseg3D at the back of or overlaps the Sphere?
 */
Datum
overback_lseg3D_sphere(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overback_lseg3D_sphere_internal(lseg, sphere));
}

PG_FUNCTION_INFO_V1(overback_lseg3D_sphere);

bool
overback_lseg3D_sphere_internal(Lseg3D *lseg, Sphere *sphere)
{
	return FPge(Min(lseg->p[0].z,lseg->p[1].z), 
				(sphere->center.z - sphere->radius));
}

/*****************************************************************************/

/* line3D OPERATOR point3D */

/*
 * Is the Line3D strictly to the left of the Point3D?
 */
 Datum
left_line3D_point3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(left_line3D_point3D_internal(line, point));
}

PG_FUNCTION_INFO_V1(left_line3D_point3D);

bool
left_line3D_point3D_internal(Line3D *line, Point3D *point)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPlt(line->p[0].x, point->x);
}

/*
 * Is the Line3D to the left of or overlaps the Point3D?
 */
Datum
overleft_line3D_point3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overleft_line3D_point3D_internal(line, point));
}

PG_FUNCTION_INFO_V1(overleft_line3D_point3D);

bool
overleft_line3D_point3D_internal(Line3D *line, Point3D *point)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return (FPle(line->p[0].x, point->x));
}

/*
 * Is the Line3D strictly to the right of the Point3D?
 */
Datum
right_line3D_point3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(right_line3D_point3D_internal(line, point));
}

PG_FUNCTION_INFO_V1(right_line3D_point3D);

bool
right_line3D_point3D_internal(Line3D *line, Point3D *point)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return (FPgt(line->p[0].x, point->x));
}

/*
 * Is the Line3D to the right of or overlaps the Point3D?
 */
Datum
overright_line3D_point3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overright_line3D_point3D_internal(line, point));
}

PG_FUNCTION_INFO_V1(overright_line3D_point3D);

bool
overright_line3D_point3D_internal(Line3D *line, Point3D *point)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return (FPge(line->p[0].x, point->x));
}

/*
 * Is the Line3D strictly below the Point3D?
 */
Datum
below_line3D_point3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(below_line3D_point3D_internal(line, point));
}

PG_FUNCTION_INFO_V1(below_line3D_point3D);

bool
below_line3D_point3D_internal(Line3D *line, Point3D *point)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPlt(line->p[0].y, point->y);
}

/*
 * Is the Line3D below or overlaps the Point3D?
 */
Datum
overbelow_line3D_point3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overbelow_line3D_point3D_internal(line, point));
}

PG_FUNCTION_INFO_V1(overbelow_line3D_point3D);

bool
overbelow_line3D_point3D_internal(Line3D *line, Point3D *point)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPle(line->p[0].y, point->y);
}

/*
 * Is the Line3D strictly above the Point3D?
 */
Datum
above_line3D_point3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(above_line3D_point3D_internal(line, point));
}

PG_FUNCTION_INFO_V1(above_line3D_point3D);

bool
above_line3D_point3D_internal(Line3D *line, Point3D *point)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPgt(line->p[0].y, point->y);
}

/*
 * Is the Line3D above or overlaps the Point3D?
 */
Datum
overabove_line3D_point3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overabove_line3D_point3D_internal(line, point));
}

PG_FUNCTION_INFO_V1(overabove_line3D_point3D);

bool
overabove_line3D_point3D_internal(Line3D *line, Point3D *point)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPge(line->p[0].y, point->y);
}

/*
 * Is the Line3D strictly in front of the Point3D?
 */
Datum
front_line3D_point3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(front_line3D_point3D_internal(line, point));
}

PG_FUNCTION_INFO_V1(front_line3D_point3D);

bool
front_line3D_point3D_internal(Line3D *line, Point3D *point)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPlt(line->p[0].z, point->z);
}

/*
 * Is the Line3D in front of or overlaps the Point3D?
 */
Datum
overfront_line3D_point3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overfront_line3D_point3D_internal(line, point));
}

PG_FUNCTION_INFO_V1(overfront_line3D_point3D);

bool
overfront_line3D_point3D_internal(Line3D *line, Point3D *point)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPle(line->p[0].z, point->z);
}

/*
 * Is the Line3D strictly at the back of the Point3D?
 */
Datum
back_line3D_point3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(back_line3D_point3D_internal(line, point));
}

PG_FUNCTION_INFO_V1(back_line3D_point3D);

bool
back_line3D_point3D_internal(Line3D *line, Point3D *point)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPgt(line->p[0].z, point->z);
}

/*
 * Is the Line3D at the back of or overlaps the Point3D?
 */
Datum
overback_line3D_point3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overback_line3D_point3D_internal(line, point));
}

PG_FUNCTION_INFO_V1(overback_line3D_point3D);

bool
overback_line3D_point3D_internal(Line3D *line, Point3D *point)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPge(line->p[0].z, point->z);
}

/*----------------------------------------------------------*/

/* line3D OPERATOR lseg3D */

/*
 * Is the Line3D strictly to the left of the Lseg3D?
 */
Datum
left_line3D_lseg3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(left_line3D_lseg3D_internal(line, lseg));
}

PG_FUNCTION_INFO_V1(left_line3D_lseg3D);

bool
left_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPlt(line->p[0].x, Min(lseg->p[0].x,lseg->p[1].x));
}

/*
 * Is the Line3D to the left of or overlaps the Lseg3D?
 */
Datum
overleft_line3D_lseg3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overleft_line3D_lseg3D_internal(line, lseg));
}

PG_FUNCTION_INFO_V1(overleft_line3D_lseg3D);

bool
overleft_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPle(line->p[0].x, Min(lseg->p[0].x,lseg->p[1].x));
}

/*
 * Is the Line3D strictly to the right of the Lseg3D?
 */
Datum
right_line3D_lseg3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(right_line3D_lseg3D_internal(line, lseg));
}

PG_FUNCTION_INFO_V1(right_line3D_lseg3D);

bool
right_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPgt(line->p[0].x, Max(lseg->p[0].x,lseg->p[1].x));
}

/*
 * Is the Line3D to the right of or overlaps the Lseg3D?
 */
Datum
overright_line3D_lseg3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overright_line3D_lseg3D_internal(line, lseg));
}

PG_FUNCTION_INFO_V1(overright_line3D_lseg3D);

bool
overright_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPge(line->p[0].x, Max(lseg->p[0].x,lseg->p[1].x));
}

/*
 * Is the Line3D strictly below the Lseg3D?
 */
Datum
below_line3D_lseg3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(below_line3D_lseg3D_internal(line, lseg));
}

PG_FUNCTION_INFO_V1(below_line3D_lseg3D);

bool
below_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPlt(line->p[0].y, Min(lseg->p[0].y,lseg->p[1].y));
}

/*
 * Is the Line3D below or overlaps the Lseg3D?
 */
Datum
overbelow_line3D_lseg3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overbelow_line3D_lseg3D_internal(line, lseg));
}

PG_FUNCTION_INFO_V1(overbelow_line3D_lseg3D);

bool
overbelow_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPle(line->p[0].y, Min(lseg->p[0].y,lseg->p[1].y));
}

/*
 * Is the Line3D strictly above the Lseg3D?
 */
Datum
above_line3D_lseg3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(above_line3D_lseg3D_internal(line, lseg));
}

PG_FUNCTION_INFO_V1(above_line3D_lseg3D);

bool
above_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPgt(line->p[0].y, Max(lseg->p[0].y,lseg->p[1].y));
}

/*
 * Is the Line3D above or overlaps the Lseg3D?
 */
Datum
overabove_line3D_lseg3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overabove_line3D_lseg3D_internal(line, lseg));
}

PG_FUNCTION_INFO_V1(overabove_line3D_lseg3D);

bool
overabove_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPge(line->p[0].y, Max(lseg->p[0].y,lseg->p[1].y));
}

/*
 * Is the Line3D strictly in front of the Lseg3D?
 */
Datum
front_line3D_lseg3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(front_line3D_lseg3D_internal(line, lseg));
}

PG_FUNCTION_INFO_V1(front_line3D_lseg3D);

bool
front_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPlt(line->p[0].z, Min(lseg->p[0].z,lseg->p[1].z));
}

/*
 * Is the Line3D in front of or overlaps the Lseg3D?
 */
Datum
overfront_line3D_lseg3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overfront_line3D_lseg3D_internal(line, lseg));
}

PG_FUNCTION_INFO_V1(overfront_line3D_lseg3D);

bool
overfront_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPle(line->p[0].z, Min(lseg->p[0].z,lseg->p[1].z));
}

/*
 * Is the Line3D strictly at the back of the Lseg3D?
 */
Datum
back_line3D_lseg3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(back_line3D_lseg3D_internal(line, lseg));
}

PG_FUNCTION_INFO_V1(back_line3D_lseg3D);

bool
back_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPgt(line->p[0].z, Max(lseg->p[0].z,lseg->p[1].z));
}

/*
 * Is the Line3D at the back of or overlaps the Lseg3D?
 */
Datum
overback_line3D_lseg3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overback_line3D_lseg3D_internal(line, lseg));
}

PG_FUNCTION_INFO_V1(overback_line3D_lseg3D);

bool
overback_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPge(line->p[0].z, Max(lseg->p[0].z,lseg->p[1].z));
}

/*----------------------------------------------------------*/

/* line3D OPERATOR line3D */

/*
 * Is the first Line3D strictly to the left of the second Line3D?
 */
Datum
left_line3D_line3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line1 = PG_GETARG_LINE3D_P(0);
	Line3D	   *line2 = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(left_line3D_line3D_internal(line1, line2));
}

PG_FUNCTION_INFO_V1(left_line3D_line3D);

bool
left_line3D_line3D_internal(Line3D *line1, Line3D *line2)
{
	if (!line3D_vert(line1) || !line3D_vert(line2))
		return FALSE;
	
	return FPlt(line1->p[0].x, line2->p[0].x);
}

/*
 * Is the first Line3D to the left of or overlaps the second Line3D?
 */
Datum
overleft_line3D_line3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line1 = PG_GETARG_LINE3D_P(0);
	Line3D	   *line2 = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overleft_line3D_line3D_internal(line1, line2));
}

PG_FUNCTION_INFO_V1(overleft_line3D_line3D);

bool
overleft_line3D_line3D_internal(Line3D *line1, Line3D *line2)
{
	if (!line3D_vert(line1) || !line3D_vert(line2))
		return FALSE;
	
	return (FPle(line1->p[0].x, line2->p[0].x));
}

/*
 * Is the first Line3D strictly to the right of the second Line3D?
 */
Datum
right_line3D_line3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line1 = PG_GETARG_LINE3D_P(0);
	Line3D	   *line2 = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(right_line3D_line3D_internal(line1, line2));
}

PG_FUNCTION_INFO_V1(right_line3D_line3D);

bool
right_line3D_line3D_internal(Line3D *line1, Line3D *line2)
{
	if (!line3D_vert(line1) || !line3D_vert(line2))
		return FALSE;
	
	return (FPgt(line1->p[0].x, line2->p[0].x));
}

/*
 * Is the first Line3D to the right of or overlaps the second Line3D?
 */
Datum
overright_line3D_line3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line1 = PG_GETARG_LINE3D_P(0);
	Line3D	   *line2 = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overright_line3D_line3D_internal(line1, line2));
}

PG_FUNCTION_INFO_V1(overright_line3D_line3D);

bool
overright_line3D_line3D_internal(Line3D *line1, Line3D *line2)
{
	if (!line3D_vert(line1) || !line3D_vert(line2))
		return FALSE;
	
	return (FPge(line1->p[0].x, line2->p[0].x));
}

/*
 * Is the first Line3D strictly below the second Line3D?
 */
Datum
below_line3D_line3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line1 = PG_GETARG_LINE3D_P(0);
	Line3D	   *line2 = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(below_line3D_line3D_internal(line1, line2));
}

PG_FUNCTION_INFO_V1(below_line3D_line3D);

bool
below_line3D_line3D_internal(Line3D *line1, Line3D *line2)
{
	if (!line3D_hori(line1) || !line3D_hori(line2))
		return FALSE;
	
	return FPlt(line1->p[0].y, line2->p[0].y);
}

/*
 * Is the first Line3D below or overlaps the second Line3D?
 */
Datum
overbelow_line3D_line3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line1 = PG_GETARG_LINE3D_P(0);
	Line3D	   *line2 = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overbelow_line3D_line3D_internal(line1, line2));
}

PG_FUNCTION_INFO_V1(overbelow_line3D_line3D);

bool
overbelow_line3D_line3D_internal(Line3D *line1, Line3D *line2)
{
	if (!line3D_hori(line1) || !line3D_hori(line2))
		return FALSE;
	
	return FPle(line1->p[0].y, line2->p[0].y);
}

/*
 * Is the first Line3D strictly above the second Line3D?
 */
Datum
above_line3D_line3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line1 = PG_GETARG_LINE3D_P(0);
	Line3D	   *line2 = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(above_line3D_line3D_internal(line1, line2));
}

PG_FUNCTION_INFO_V1(above_line3D_line3D);

bool
above_line3D_line3D_internal(Line3D *line1, Line3D *line2)
{
	if (!line3D_hori(line1) || !line3D_hori(line2))
		return FALSE;
	
	return FPgt(line1->p[0].y, line2->p[0].y);
}

/*
 * Is the first Line3D above or overlaps the second Line3D?
 */
Datum
overabove_line3D_line3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line1 = PG_GETARG_LINE3D_P(0);
	Line3D	   *line2 = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overabove_line3D_line3D_internal(line1, line2));
}

PG_FUNCTION_INFO_V1(overabove_line3D_line3D);

bool
overabove_line3D_line3D_internal(Line3D *line1, Line3D *line2)
{
	if (!line3D_hori(line1) || !line3D_hori(line2))
		return FALSE;
	
	return FPge(line1->p[0].y, line2->p[0].y);
}

/*
 * Is the first Line3D strictly in front of the second Line3D?
 */
Datum
front_line3D_line3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line1 = PG_GETARG_LINE3D_P(0);
	Line3D	   *line2 = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(front_line3D_line3D_internal(line1, line2));
}

PG_FUNCTION_INFO_V1(front_line3D_line3D);

bool
front_line3D_line3D_internal(Line3D *line1, Line3D *line2)
{
	if (!line3D_perp(line1) || !line3D_perp(line2))
		return FALSE;
	
	return FPlt(line1->p[0].z, line2->p[0].z);
}

/*
 * Is the first Line3D in front of or overlaps the second Line3D?
 */
Datum
overfront_line3D_line3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line1 = PG_GETARG_LINE3D_P(0);
	Line3D	   *line2 = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overfront_line3D_line3D_internal(line1, line2));
}

PG_FUNCTION_INFO_V1(overfront_line3D_line3D);

bool
overfront_line3D_line3D_internal(Line3D *line1, Line3D *line2)
{
	if (!line3D_perp(line1) || !line3D_perp(line2))
		return FALSE;
	
	return FPle(line1->p[0].z, line2->p[0].z);
}

/*
 * Is the first Line3D strictly at the back of the second Line3D?
 */
Datum
back_line3D_line3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line1 = PG_GETARG_LINE3D_P(0);
	Line3D	   *line2 = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(back_line3D_line3D_internal(line1, line2));
}

PG_FUNCTION_INFO_V1(back_line3D_line3D);

bool
back_line3D_line3D_internal(Line3D *line1, Line3D *line2)
{
	if (!line3D_perp(line1) || !line3D_perp(line2))
		return FALSE;
	
	return FPgt(line1->p[0].z, line2->p[0].z);
}

/*
 * Is the first Line3D at the back of or overlaps the second Line3D?
 */
Datum
overback_line3D_line3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line1 = PG_GETARG_LINE3D_P(0);
	Line3D	   *line2 = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overback_line3D_line3D_internal(line1, line2));
}

PG_FUNCTION_INFO_V1(overback_line3D_line3D);

bool
overback_line3D_line3D_internal(Line3D *line1, Line3D *line2)
{
	if (!line3D_perp(line1) || !line3D_perp(line2))
		return FALSE;
	
	return FPge(line1->p[0].z, line2->p[0].z);
}

/*----------------------------------------------------------*/

/* line3D OPERATOR box3D */

/*
 * Is the Line3D strictly to the left of the Box3D?
 */
Datum
left_line3D_box3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(left_line3D_box3D_internal(line, box));
}

PG_FUNCTION_INFO_V1(left_line3D_box3D);

bool
left_line3D_box3D_internal(Line3D *line, Box3D *box)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPlt(line->p[0].x, box->low.x);
}

/*
 * Is the Line3D to the left of or overlaps the Box3D?
 */
Datum
overleft_line3D_box3D(PG_FUNCTION_ARGS)
{
	Line3D		   *line = PG_GETARG_LINE3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overleft_line3D_box3D_internal(line, box));
}

PG_FUNCTION_INFO_V1(overleft_line3D_box3D);

bool
overleft_line3D_box3D_internal(Line3D *line, Box3D *box)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPle(line->p[0].x, box->high.x);
}

/*
 * Is the Line3D strictly to the right of the Box3D?
 */
Datum
right_line3D_box3D(PG_FUNCTION_ARGS)
{
	Line3D		   *line = PG_GETARG_LINE3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(right_line3D_box3D_internal(line, box));
}

PG_FUNCTION_INFO_V1(right_line3D_box3D);

bool
right_line3D_box3D_internal(Line3D *line, Box3D *box)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPgt(line->p[0].x, box->high.x);
}

/*
 * Is the Line3D to the right of or overlaps the Box3D?
 */
Datum
overright_line3D_box3D(PG_FUNCTION_ARGS)
{
	Line3D		   *line = PG_GETARG_LINE3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overright_line3D_box3D_internal(line, box));
}

PG_FUNCTION_INFO_V1(overright_line3D_box3D);

bool
overright_line3D_box3D_internal(Line3D *line, Box3D *box)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPge(line->p[0].x, box->low.x);
}

/*
 * Is the Line3D strictly below the Box3D?
 */
Datum
below_line3D_box3D(PG_FUNCTION_ARGS)
{
	Line3D		   *line = PG_GETARG_LINE3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(below_line3D_box3D_internal(line, box));
}

PG_FUNCTION_INFO_V1(below_line3D_box3D);

bool
below_line3D_box3D_internal(Line3D *line, Box3D *box)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPlt(line->p[0].y, box->low.y);
}

/*
 * Is the Line3D below or overlaps the Box3D?
 */
Datum
overbelow_line3D_box3D(PG_FUNCTION_ARGS)
{
	Line3D		   *line = PG_GETARG_LINE3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overbelow_line3D_box3D_internal(line, box));
}

PG_FUNCTION_INFO_V1(overbelow_line3D_box3D);

bool
overbelow_line3D_box3D_internal(Line3D *line, Box3D *box)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPle(line->p[0].y, box->high.y);
}

/*
 * Is the Line3D strictly above the Box3D?
 */
Datum
above_line3D_box3D(PG_FUNCTION_ARGS)
{
	Line3D		   *line = PG_GETARG_LINE3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(above_line3D_box3D_internal(line, box));
}

PG_FUNCTION_INFO_V1(above_line3D_box3D);

bool
above_line3D_box3D_internal(Line3D *line, Box3D *box)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPgt(line->p[0].y, box->high.y);
}

/*
 * Is the Line3D above or overlaps the Box3D?
 */
Datum
overabove_line3D_box3D(PG_FUNCTION_ARGS)
{
	Line3D		   *line = PG_GETARG_LINE3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overabove_line3D_box3D_internal(line, box));
}

PG_FUNCTION_INFO_V1(overabove_line3D_box3D);

bool
overabove_line3D_box3D_internal(Line3D *line, Box3D *box)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPge(line->p[0].y, box->low.y);
}

/*
 * Is the Line3D strictly in front of the Box3D?
 */
Datum
front_line3D_box3D(PG_FUNCTION_ARGS)
{
	Line3D		   *line = PG_GETARG_LINE3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(front_line3D_box3D_internal(line, box));
}

PG_FUNCTION_INFO_V1(front_line3D_box3D);

bool
front_line3D_box3D_internal(Line3D *line, Box3D *box)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPlt(line->p[0].z, box->low.z);
}

/*
 * Is the Line3D in front of or overlaps the Box3D?
 */
Datum
overfront_line3D_box3D(PG_FUNCTION_ARGS)
{
	Line3D		   *line = PG_GETARG_LINE3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overfront_line3D_box3D_internal(line, box));
}

PG_FUNCTION_INFO_V1(overfront_line3D_box3D);

bool
overfront_line3D_box3D_internal(Line3D *line, Box3D *box)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPle(line->p[0].z, box->high.z);
}

/*
 * Is the Line3D strictly at the back of the Box3D?
 */
Datum
back_line3D_box3D(PG_FUNCTION_ARGS)
{
	Line3D		   *line = PG_GETARG_LINE3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(back_line3D_box3D_internal(line, box));
}

PG_FUNCTION_INFO_V1(back_line3D_box3D);

bool
back_line3D_box3D_internal(Line3D *line, Box3D *box)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPgt(line->p[0].z, box->high.z);
}

/*
 * Is the Line3D at the back of or overlaps the Box3D?
 */
Datum
overback_line3D_box3D(PG_FUNCTION_ARGS)
{
	Line3D		   *line = PG_GETARG_LINE3D_P(1);
	Box3D	   *box = PG_GETARG_BOX3D_P(0);

	PG_RETURN_BOOL(overback_line3D_box3D_internal(line, box));
}

PG_FUNCTION_INFO_V1(overback_line3D_box3D);

bool
overback_line3D_box3D_internal(Line3D *line, Box3D *box)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPge(line->p[0].z, box->low.z);
}

/*----------------------------------------------------------*/

/* line3D OPERATOR path3D */

/*
 * Is the Line3D strictly to the left of the Path3D?
 */
Datum
left_line3D_path3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = left_line3D_path3D_internal(line, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(left_line3D_path3D);

bool
left_line3D_path3D_internal(Line3D *line, Path3D *path)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPlt(line->p[0].x, path->boundbox.low.x);
}

/*
 * Is the Line3D to the left of or overlaps the Path3D?
 */
Datum
overleft_line3D_path3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overleft_line3D_path3D_internal(line, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overleft_line3D_path3D);

bool
overleft_line3D_path3D_internal(Line3D *line, Path3D *path)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPle(line->p[0].x, path->boundbox.high.x);
}

/*
 * Is the Line3D strictly to the right of the Path3D?
 */
Datum
right_line3D_path3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = right_line3D_path3D_internal(line, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(right_line3D_path3D);

bool
right_line3D_path3D_internal(Line3D *line, Path3D *path)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPgt(line->p[0].x, path->boundbox.high.x);
}

/*
 * Is the Line3D to the right of or overlaps the Path3D?
 */
Datum
overright_line3D_path3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overright_line3D_path3D_internal(line, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overright_line3D_path3D);

bool
overright_line3D_path3D_internal(Line3D *line, Path3D *path)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPge(line->p[0].x, path->boundbox.low.x);
}

/*
 * Is the Line3D strictly below the Path3D?
 */
Datum
below_line3D_path3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = below_line3D_path3D_internal(line, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(below_line3D_path3D);

bool
below_line3D_path3D_internal(Line3D *line, Path3D *path)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPlt(line->p[0].y, path->boundbox.low.y);
}

/*
 * Is the Line3D below or overlaps the Path3D?
 */
Datum
overbelow_line3D_path3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overbelow_line3D_path3D_internal(line, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overbelow_line3D_path3D);

bool
overbelow_line3D_path3D_internal(Line3D *line, Path3D *path)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPle(line->p[0].y, path->boundbox.high.y);
}

/*
 * Is the Line3D strictly above the Path3D?
 */
Datum
above_line3D_path3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = above_line3D_path3D_internal(line, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(above_line3D_path3D);

bool
above_line3D_path3D_internal(Line3D *line, Path3D *path)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPgt(line->p[0].y, path->boundbox.high.y);
}

/*
 * Is the Line3D above or overlaps the Path3D?
 */
Datum
overabove_line3D_path3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overabove_line3D_path3D_internal(line, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overabove_line3D_path3D);

bool
overabove_line3D_path3D_internal(Line3D *line, Path3D *path)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPge(line->p[0].y, path->boundbox.low.y);
}

/*
 * Is the Line3D strictly in front of the Path3D?
 */
Datum
front_line3D_path3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = front_line3D_path3D_internal(line, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(front_line3D_path3D);

bool
front_line3D_path3D_internal(Line3D *line, Path3D *path)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPlt(line->p[0].z, path->boundbox.low.z);
}

/*
 * Is the Line3D in front of or overlaps the Path3D?
 */
Datum
overfront_line3D_path3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overfront_line3D_path3D_internal(line, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overfront_line3D_path3D);

bool
overfront_line3D_path3D_internal(Line3D *line, Path3D *path)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPle(line->p[0].z, path->boundbox.high.z);
}

/*
 * Is the Line3D strictly at the back of the Path3D?
 */
Datum
back_line3D_path3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = back_line3D_path3D_internal(line, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(back_line3D_path3D);

bool
back_line3D_path3D_internal(Line3D *line, Path3D *path)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPgt(line->p[0].z, path->boundbox.high.z);
}

/*
 * Is the Line3D at the back of or overlaps the Path3D?
 */
Datum
overback_line3D_path3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overback_line3D_path3D_internal(line, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overback_line3D_path3D);

bool
overback_line3D_path3D_internal(Line3D *line, Path3D *path)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPge(line->p[0].z, path->boundbox.low.z);
}

/*----------------------------------------------------------*/

/* line3D OPERATOR polygon3D */

/*
 * Is the Line3D strictly to the left of the Polygon3D?
 */
Datum
left_line3D_poly3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = left_line3D_poly3D_internal(line, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(left_line3D_poly3D);

bool
left_line3D_poly3D_internal(Line3D *line, Polygon3D *poly)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPlt(line->p[0].x, poly->boundbox.low.x);
}

/*
 * Is the Line3D to the left of or overlaps the Polygon3D?
 */
Datum
overleft_line3D_poly3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overleft_line3D_poly3D_internal(line, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overleft_line3D_poly3D);

bool
overleft_line3D_poly3D_internal(Line3D *line, Polygon3D *poly)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPle(line->p[0].x, poly->boundbox.high.x);
}

/*
 * Is the Line3D strictly to the right of the Polygon3D?
 */
Datum
right_line3D_poly3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = right_line3D_poly3D_internal(line, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(right_line3D_poly3D);

bool
right_line3D_poly3D_internal(Line3D *line, Polygon3D *poly)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPgt(line->p[0].x, poly->boundbox.high.x);
}

/*
 * Is the Line3D to the right of or overlaps the Polygon3D?
 */
Datum
overright_line3D_poly3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overright_line3D_poly3D_internal(line, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overright_line3D_poly3D);

bool
overright_line3D_poly3D_internal(Line3D *line, Polygon3D *poly)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPge(line->p[0].x, poly->boundbox.low.x);
}

/*
 * Is the Line3D strictly below the Polygon3D?
 */
Datum
below_line3D_poly3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = below_line3D_poly3D_internal(line, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(below_line3D_poly3D);

bool
below_line3D_poly3D_internal(Line3D *line, Polygon3D *poly)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPlt(line->p[0].y, poly->boundbox.low.y);
}

/*
 * Is the Line3D below or overlaps the Polygon3D?
 */
Datum
overbelow_line3D_poly3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overbelow_line3D_poly3D_internal(line, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overbelow_line3D_poly3D);

bool
overbelow_line3D_poly3D_internal(Line3D *line, Polygon3D *poly)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPle(line->p[0].y, poly->boundbox.high.y);
}

/*
 * Is the Line3D strictly above the Polygon3D?
 */
Datum
above_line3D_poly3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = above_line3D_poly3D_internal(line, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(above_line3D_poly3D);

bool
above_line3D_poly3D_internal(Line3D *line, Polygon3D *poly)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPgt(line->p[0].y, poly->boundbox.high.y);
}

/*
 * Is the Line3D above or overlaps the Polygon3D?
 */
Datum
overabove_line3D_poly3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overabove_line3D_poly3D_internal(line, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overabove_line3D_poly3D);

bool
overabove_line3D_poly3D_internal(Line3D *line, Polygon3D *poly)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPge(line->p[0].y, poly->boundbox.low.y);
}

/*
 * Is the Line3D strictly in front of the Polygon3D?
 */
Datum
front_line3D_poly3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = front_line3D_poly3D_internal(line, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(front_line3D_poly3D);

bool
front_line3D_poly3D_internal(Line3D *line, Polygon3D *poly)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPlt(line->p[0].z, poly->boundbox.low.z);
}

/*
 * Is the Line3D in front of or overlaps the Polygon3D?
 */
Datum
overfront_line3D_poly3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overfront_line3D_poly3D_internal(line, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overfront_line3D_poly3D);

bool
overfront_line3D_poly3D_internal(Line3D *line, Polygon3D *poly)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPle(line->p[0].z, poly->boundbox.high.z);
}

/*
 * Is the Line3D strictly at the back of the Polygon3D?
 */
Datum
back_line3D_poly3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = back_line3D_poly3D_internal(line, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(back_line3D_poly3D);

bool
back_line3D_poly3D_internal(Line3D *line, Polygon3D *poly)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPgt(line->p[0].z, poly->boundbox.high.z);
}

/*
 * Is the Line3D at the back of or overlaps the Polygon3D?
 */
Datum
overback_line3D_poly3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overback_line3D_poly3D_internal(line, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overback_line3D_poly3D);

bool
overback_line3D_poly3D_internal(Line3D *line, Polygon3D *poly)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPge(line->p[0].z, poly->boundbox.low.z);
}

/*----------------------------------------------------------*/

/* line3D OPERATOR sphere */

/*
 * Is the Line3D strictly to the left of the Sphere?
 */
Datum
left_line3D_sphere(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(left_line3D_sphere_internal(line, sphere));
}

PG_FUNCTION_INFO_V1(left_line3D_sphere);

bool
left_line3D_sphere_internal(Line3D *line, Sphere *sphere)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPlt(line->p[0].x, (sphere->center.x - sphere->radius));
}

/*
 * Is the Line3D to the left of or overlaps the Sphere?
 */
Datum
overleft_line3D_sphere(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overleft_line3D_sphere_internal(line, sphere));
}

PG_FUNCTION_INFO_V1(overleft_line3D_sphere);

bool
overleft_line3D_sphere_internal(Line3D *line, Sphere *sphere)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPle(line->p[0].x, (sphere->center.x + sphere->radius));
}

/*
 * Is the Line3D strictly to the right of the Sphere?
 */
Datum
right_line3D_sphere(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(right_line3D_sphere_internal(line, sphere));
}

PG_FUNCTION_INFO_V1(right_line3D_sphere);

bool
right_line3D_sphere_internal(Line3D *line, Sphere *sphere)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPgt(line->p[0].x, (sphere->center.x + sphere->radius));
}

/*
 * Is the Line3D to the right of or overlaps the Sphere?
 */
Datum
overright_line3D_sphere(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overright_line3D_sphere_internal(line, sphere));
}

PG_FUNCTION_INFO_V1(overright_line3D_sphere);

bool
overright_line3D_sphere_internal(Line3D *line, Sphere *sphere)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPge(line->p[0].x, (sphere->center.x - sphere->radius));
}

/*
 * Is the Line3D strictly below the Sphere?
 */
Datum
below_line3D_sphere(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(below_line3D_sphere_internal(line, sphere));
}

PG_FUNCTION_INFO_V1(below_line3D_sphere);

bool
below_line3D_sphere_internal(Line3D *line, Sphere *sphere)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPlt(line->p[0].y, (sphere->center.y - sphere->radius));
}

/*
 * Is the Line3D below or overlaps the Sphere?
 */
Datum
overbelow_line3D_sphere(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overbelow_line3D_sphere_internal(line, sphere));
}

PG_FUNCTION_INFO_V1(overbelow_line3D_sphere);

bool
overbelow_line3D_sphere_internal(Line3D *line, Sphere *sphere)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPle(line->p[0].y, (sphere->center.y + sphere->radius));
}

/*
 * Is the Line3D strictly above the Sphere?
 */
Datum
above_line3D_sphere(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(above_line3D_sphere_internal(line, sphere));
}

PG_FUNCTION_INFO_V1(above_line3D_sphere);

bool
above_line3D_sphere_internal(Line3D *line, Sphere *sphere)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPgt(line->p[0].y, (sphere->center.y + sphere->radius));
}

/*
 * Is the Line3D above or overlaps the Sphere?
 */
Datum
overabove_line3D_sphere(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overabove_line3D_sphere_internal(line, sphere));
}

PG_FUNCTION_INFO_V1(overabove_line3D_sphere);

bool
overabove_line3D_sphere_internal(Line3D *line, Sphere *sphere)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPge(line->p[0].y, (sphere->center.y - sphere->radius));
}

/*
 * Is the Line3D strictly in front of the Sphere?
 */
Datum
front_line3D_sphere(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(front_line3D_sphere_internal(line, sphere));
}

PG_FUNCTION_INFO_V1(front_line3D_sphere);

bool
front_line3D_sphere_internal(Line3D *line, Sphere *sphere)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPlt(line->p[0].z, (sphere->center.z - sphere->radius));
}

/*
 * Is the Line3D in front of or overlaps the Sphere?
 */
Datum
overfront_line3D_sphere(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overfront_line3D_sphere_internal(line, sphere));
}

PG_FUNCTION_INFO_V1(overfront_line3D_sphere);

bool
overfront_line3D_sphere_internal(Line3D *line, Sphere *sphere)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPle(line->p[0].z, (sphere->center.z + sphere->radius));
}

/*
 * Is the Line3D strictly at the back of the Sphere?
 */
Datum
back_line3D_sphere(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(back_line3D_sphere_internal(line, sphere));
}

PG_FUNCTION_INFO_V1(back_line3D_sphere);

bool
back_line3D_sphere_internal(Line3D *line, Sphere *sphere)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPgt(line->p[0].z, (sphere->center.z + sphere->radius));
}

/*
 * Is the Line3D at the back of or overlaps the Sphere?
 */
Datum
overback_line3D_sphere(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overback_line3D_sphere_internal(line, sphere));
}

PG_FUNCTION_INFO_V1(overback_line3D_sphere);

bool
overback_line3D_sphere_internal(Line3D *line, Sphere *sphere)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPge(line->p[0].z, (sphere->center.z - sphere->radius));
}

/*****************************************************************************/

/* box3D OPERATOR point3D */

/*
 * Is the Box3D strictly to the left of the Point3D?
 */
Datum
left_box3D_point3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(left_box3D_point3D_internal(box, point));
}

PG_FUNCTION_INFO_V1(left_box3D_point3D);

bool
left_box3D_point3D_internal(Box3D *box, Point3D *point)
{
	return FPlt(box->high.x, point->x);
}

/*
 * Is the Box3D to the left of or overlaps the Point3D?
 */
Datum
overleft_box3D_point3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overleft_box3D_point3D_internal(box, point));
}

PG_FUNCTION_INFO_V1(overleft_box3D_point3D);

bool
overleft_box3D_point3D_internal(Box3D *box, Point3D *point)
{
	return FPle(box->low.x, point->x);
}

/*
 * Is the Box3D strictly to the right of the Point3D?
 */
Datum
right_box3D_point3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(right_box3D_point3D_internal(box, point));
}

PG_FUNCTION_INFO_V1(right_box3D_point3D);

bool
right_box3D_point3D_internal(Box3D *box, Point3D *point)
{
	return FPgt(box->low.x, point->x);
}

/*
 * Is the Box3D to the right of or overlaps the Point3D?
 */
Datum
overright_box3D_point3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overright_box3D_point3D_internal(box, point));
}

PG_FUNCTION_INFO_V1(overright_box3D_point3D);

bool
overright_box3D_point3D_internal(Box3D *box, Point3D *point)
{
	return FPge(box->high.x, point->x);
}

/*
 * Is the Box3D strictly below the Point3D?
 */
Datum
below_box3D_point3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(below_box3D_point3D_internal(box, point));
}

PG_FUNCTION_INFO_V1(below_box3D_point3D);

bool
below_box3D_point3D_internal(Box3D *box, Point3D *point)
{
	return FPlt(box->high.y, point->y);
}

/*
 * Is the Box3D below or overlaps the Point3D?
 */
Datum
overbelow_box3D_point3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overbelow_box3D_point3D_internal(box, point));
}

PG_FUNCTION_INFO_V1(overbelow_box3D_point3D);

bool
overbelow_box3D_point3D_internal(Box3D *box, Point3D *point)
{
	return FPle(box->low.y, point->y);
}

/*
 * Is the Box3D strictly above the Point3D?
 */
Datum
above_box3D_point3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(above_box3D_point3D_internal(box, point));
}

PG_FUNCTION_INFO_V1(above_box3D_point3D);

bool
above_box3D_point3D_internal(Box3D *box, Point3D *point)
{
	return FPgt(box->low.y, point->y);
}

/*
 * Is the Box3D above or overlaps the Point3D?
 */
Datum
overabove_box3D_point3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overabove_box3D_point3D_internal(box, point));
}

PG_FUNCTION_INFO_V1(overabove_box3D_point3D);

bool
overabove_box3D_point3D_internal(Box3D *box, Point3D *point)
{
	return FPge(box->high.y, point->y);
}

/*
 * Is the Box3D strictly in front of the Point3D?
 */
Datum
front_box3D_point3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(front_box3D_point3D_internal(box, point));
}

PG_FUNCTION_INFO_V1(front_box3D_point3D);

bool
front_box3D_point3D_internal(Box3D *box, Point3D *point)
{
	return FPlt(box->high.z, point->z);
}

/*
 * Is the Box3D in front of or overlaps the Point3D?
 */
Datum
overfront_box3D_point3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overfront_box3D_point3D_internal(box, point));
}

PG_FUNCTION_INFO_V1(overfront_box3D_point3D);

bool
overfront_box3D_point3D_internal(Box3D *box, Point3D *point)
{
	return FPle(box->low.z, point->z);
}

/*
 * Is the Box3D strictly at the back of the Point3D?
 */
Datum
back_box3D_point3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(back_box3D_point3D_internal(box, point));
}

PG_FUNCTION_INFO_V1(back_box3D_point3D);

bool
back_box3D_point3D_internal(Box3D *box, Point3D *point)
{
	return FPgt(box->low.z, point->z);
}

/*
 * Is the Box3D at the back of or overlaps the Point3D?
 */
Datum
overback_box3D_point3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overback_box3D_point3D_internal(box, point));
}

PG_FUNCTION_INFO_V1(overback_box3D_point3D);

bool
overback_box3D_point3D_internal(Box3D *box, Point3D *point)
{
	return FPge(box->high.z, point->z);
}

/*----------------------------------------------------------*/

/* box3D OPERATOR lseg3D */

/*
 * Is the Box3D strictly to the left of the Lseg3D?
 */
Datum
left_box3D_lseg3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(left_box3D_lseg3D_internal(box, lseg));
}

PG_FUNCTION_INFO_V1(left_box3D_lseg3D);

bool
left_box3D_lseg3D_internal(Box3D *box, Lseg3D *lseg)
{
	return FPlt(box->high.x, Min(lseg->p[0].x,lseg->p[1].x));
}

/*
 * Is the Box3D to the left of or overlaps the Lseg3D?
 */
Datum
overleft_box3D_lseg3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overleft_box3D_lseg3D_internal(box, lseg));
}

PG_FUNCTION_INFO_V1(overleft_box3D_lseg3D);

bool
overleft_box3D_lseg3D_internal(Box3D *box, Lseg3D *lseg)
{
	return FPle(box->low.x, Max(lseg->p[0].x,lseg->p[1].x));
}

/*
 * Is the Box3D strictly to the right of the Lseg3D?
 */
Datum
right_box3D_lseg3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(right_box3D_lseg3D_internal(box, lseg));
}

PG_FUNCTION_INFO_V1(right_box3D_lseg3D);

bool
right_box3D_lseg3D_internal(Box3D *box, Lseg3D *lseg)
{
	return FPgt(box->low.x, Max(lseg->p[0].x,lseg->p[1].x));
}

/*
 * Is the Box3D to the right of or overlaps the Lseg3D?
 */
Datum
overright_box3D_lseg3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overright_box3D_lseg3D_internal(box, lseg));
}

PG_FUNCTION_INFO_V1(overright_box3D_lseg3D);

bool
overright_box3D_lseg3D_internal(Box3D *box, Lseg3D *lseg)
{
	return FPge(box->high.x, Min(lseg->p[0].x,lseg->p[1].x));
}

/*
 * Is the Box3D strictly below the Lseg3D?
 */
Datum
below_box3D_lseg3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(below_box3D_lseg3D_internal(box, lseg));
}

PG_FUNCTION_INFO_V1(below_box3D_lseg3D);

bool
below_box3D_lseg3D_internal(Box3D *box, Lseg3D *lseg)
{
	return FPlt(box->high.y, Min(lseg->p[0].y,lseg->p[1].y));
}

/*
 * Is the Box3D below or overlaps the Lseg3D?
 */
Datum
overbelow_box3D_lseg3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overbelow_box3D_lseg3D_internal(box, lseg));
}

PG_FUNCTION_INFO_V1(overbelow_box3D_lseg3D);

bool
overbelow_box3D_lseg3D_internal(Box3D *box, Lseg3D *lseg)
{
	return FPle(box->low.y, Max(lseg->p[0].y,lseg->p[1].y));
}

/*
 * Is the Box3D strictly above the Lseg3D?
 */
Datum
above_box3D_lseg3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(above_box3D_lseg3D_internal(box, lseg));
}

PG_FUNCTION_INFO_V1(above_box3D_lseg3D);

bool
above_box3D_lseg3D_internal(Box3D *box, Lseg3D *lseg)
{
	return FPgt(box->low.y, Max(lseg->p[0].y,lseg->p[1].y));
}

/*
 * Is the Box3D above or overlaps the Lseg3D?
 */
Datum
overabove_box3D_lseg3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overabove_box3D_lseg3D_internal(box, lseg));
}

PG_FUNCTION_INFO_V1(overabove_box3D_lseg3D);

bool
overabove_box3D_lseg3D_internal(Box3D *box, Lseg3D *lseg)
{
	return FPge(box->high.y, Min(lseg->p[0].y,lseg->p[1].y));
}

/*
 * Is the Box3D strictly in front of the Lseg3D?
 */
Datum
front_box3D_lseg3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(front_box3D_lseg3D_internal(box, lseg));
}

PG_FUNCTION_INFO_V1(front_box3D_lseg3D);

bool
front_box3D_lseg3D_internal(Box3D *box, Lseg3D *lseg)
{
	return FPlt(box->high.z, Min(lseg->p[0].z,lseg->p[1].z));
}

/*
 * Is the Box3D in front of or overlaps the Lseg3D?
 */
Datum
overfront_box3D_lseg3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overfront_box3D_lseg3D_internal(box, lseg));
}

PG_FUNCTION_INFO_V1(overfront_box3D_lseg3D);

bool
overfront_box3D_lseg3D_internal(Box3D *box, Lseg3D *lseg)
{
	return FPle(box->low.z, Max(lseg->p[0].z,lseg->p[1].z));
}

/*
 * Is the Box3D strictly at the back of the Lseg3D?
 */
Datum
back_box3D_lseg3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(back_box3D_lseg3D_internal(box, lseg));
}

PG_FUNCTION_INFO_V1(back_box3D_lseg3D);

bool
back_box3D_lseg3D_internal(Box3D *box, Lseg3D *lseg)
{
	return FPgt(box->low.z, Max(lseg->p[0].z,lseg->p[1].z));
}

/*
 * Is the Box3D at the back of or overlaps the Lseg3D?
 */
Datum
overback_box3D_lseg3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overback_box3D_lseg3D_internal(box, lseg));
}

PG_FUNCTION_INFO_V1(overback_box3D_lseg3D);

bool
overback_box3D_lseg3D_internal(Box3D *box, Lseg3D *lseg)
{
	return FPge(box->high.z, Min(lseg->p[0].z,lseg->p[1].z));
}

/*----------------------------------------------------------*/

/* box3D OPERATOR line3D */

/*
 * Is the Box3D strictly to the left of the Line3D?
 */
Datum
left_box3D_line3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(left_box3D_line3D_internal(box, line));
}

PG_FUNCTION_INFO_V1(left_box3D_line3D);

bool
left_box3D_line3D_internal(Box3D *box, Line3D *line)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPlt(box->high.x, line->p[0].x);
}

/*
 * Is the Box3D to the left of or overlaps the Line3D?
 */
Datum
overleft_box3D_line3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overleft_box3D_line3D_internal(box, line));
}

PG_FUNCTION_INFO_V1(overleft_box3D_line3D);

bool
overleft_box3D_line3D_internal(Box3D *box, Line3D *line)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPle(box->low.x, line->p[0].x);
}

/*
 * Is the Box3D strictly to the right of the Line3D?
 */
Datum
right_box3D_line3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(right_box3D_line3D_internal(box, line));
}

PG_FUNCTION_INFO_V1(right_box3D_line3D);

bool
right_box3D_line3D_internal(Box3D *box, Line3D *line)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPgt(box->low.x, line->p[0].x);
}

/*
 * Is the Box3D to the right of or overlaps the Line3D?
 */
Datum
overright_box3D_line3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overright_box3D_line3D_internal(box, line));
}

PG_FUNCTION_INFO_V1(overright_box3D_line3D);

bool
overright_box3D_line3D_internal(Box3D *box, Line3D *line)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPge(box->high.x, line->p[0].x);
}

/*
 * Is the Box3D strictly below the Line3D?
 */
Datum
below_box3D_line3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(below_box3D_line3D_internal(box, line));
}

PG_FUNCTION_INFO_V1(below_box3D_line3D);

bool
below_box3D_line3D_internal(Box3D *box, Line3D *line)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPlt(box->high.y, line->p[0].y);
}

/*
 * Is the Box3D below or overlaps the Line3D?
 */
Datum
overbelow_box3D_line3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overbelow_box3D_line3D_internal(box, line));
}

PG_FUNCTION_INFO_V1(overbelow_box3D_line3D);

bool
overbelow_box3D_line3D_internal(Box3D *box, Line3D *line)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPle(box->low.y, line->p[0].y);
}

/*
 * Is the Box3D strictly above the Line3D?
 */
Datum
above_box3D_line3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(above_box3D_line3D_internal(box, line));
}

PG_FUNCTION_INFO_V1(above_box3D_line3D);

bool
above_box3D_line3D_internal(Box3D *box, Line3D *line)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPgt(box->low.y, line->p[0].y);
}

/*
 * Is the Box3D above or overlaps the Line3D?
 */
Datum
overabove_box3D_line3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overabove_box3D_line3D_internal(box, line));
}

PG_FUNCTION_INFO_V1(overabove_box3D_line3D);

bool
overabove_box3D_line3D_internal(Box3D *box, Line3D *line)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPge(box->high.y, line->p[0].y);
}

/*
 * Is the Box3D strictly in front of the Line3D?
 */
Datum
front_box3D_line3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(front_box3D_line3D_internal(box, line));
}

PG_FUNCTION_INFO_V1(front_box3D_line3D);

bool
front_box3D_line3D_internal(Box3D *box, Line3D *line)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPlt(box->high.z, line->p[0].z);
}

/*
 * Is the Box3D in front of or overlaps the Line3D?
 */
Datum
overfront_box3D_line3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overfront_box3D_line3D_internal(box, line));
}

PG_FUNCTION_INFO_V1(overfront_box3D_line3D);

bool
overfront_box3D_line3D_internal(Box3D *box, Line3D *line)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPle(box->low.z, line->p[0].z);
}

/*
 * Is the Box3D strictly at the back of the Line3D?
 */
Datum
back_box3D_line3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(back_box3D_line3D_internal(box, line));
}

PG_FUNCTION_INFO_V1(back_box3D_line3D);

bool
back_box3D_line3D_internal(Box3D *box, Line3D *line)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPgt(box->low.z, line->p[0].z);
}

/*
 * Is the Box3D at the back of or overlaps the Line3D?
 */
Datum
overback_box3D_line3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overback_box3D_line3D_internal(box, line));
}

PG_FUNCTION_INFO_V1(overback_box3D_line3D);

bool
overback_box3D_line3D_internal(Box3D *box, Line3D *line)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPge(box->high.z, line->p[0].z);
}

/*----------------------------------------------------------*/

/* box3D OPERATOR box3D */

/*
 * Is the first Box3D strictly to the left of the second Box3D?
 */
Datum
left_box3D_box3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box1 = PG_GETARG_BOX3D_P(0);
	Box3D	   *box2 = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(left_box3D_box3D_internal(box1, box2));
}

PG_FUNCTION_INFO_V1(left_box3D_box3D);

bool
left_box3D_box3D_internal(Box3D *box1, Box3D *box2)
{
	return FPlt(box1->high.x, box2->low.x);
}

/*
 * Is the first Box3D to the left of or overlaps the second Box3D?
 */
Datum
overleft_box3D_box3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box1 = PG_GETARG_BOX3D_P(0);
	Box3D	   *box2 = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overleft_box3D_box3D_internal(box1, box2));
}

PG_FUNCTION_INFO_V1(overleft_box3D_box3D);

bool
overleft_box3D_box3D_internal(Box3D *box1, Box3D *box2)
{
	return FPle(box1->low.x, box2->high.x);
}

/*
 * Is the first Box3D strictly to the right of the second Box3D?
 */
Datum
right_box3D_box3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box1 = PG_GETARG_BOX3D_P(0);
	Box3D	   *box2 = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(right_box3D_box3D_internal(box1, box2));
}

PG_FUNCTION_INFO_V1(right_box3D_box3D);

bool
right_box3D_box3D_internal(Box3D *box1, Box3D *box2)
{
	return FPgt(box1->low.x, box2->high.x);
}

/*
 * Is the first Box3D to the right of or overlaps the second Box3D?
 */
Datum
overright_box3D_box3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box1 = PG_GETARG_BOX3D_P(0);
	Box3D	   *box2 = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overright_box3D_box3D_internal(box1, box2));
}

PG_FUNCTION_INFO_V1(overright_box3D_box3D);

bool
overright_box3D_box3D_internal(Box3D *box1, Box3D *box2)
{
	return FPge(box1->high.x, box2->low.x);
}

/*
 * Is the first Box3D strictly below the second Box3D?
 */
Datum
below_box3D_box3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box1 = PG_GETARG_BOX3D_P(0);
	Box3D	   *box2 = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(below_box3D_box3D_internal(box1, box2));
}

PG_FUNCTION_INFO_V1(below_box3D_box3D);

bool
below_box3D_box3D_internal(Box3D *box1, Box3D *box2)
{
	return FPlt(box1->high.y, box2->low.y);
}

/*
 * Is the first Box3D below or overlaps the second Box3D?
 */
Datum
overbelow_box3D_box3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box1 = PG_GETARG_BOX3D_P(0);
	Box3D	   *box2 = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overbelow_box3D_box3D_internal(box1, box2));
}

PG_FUNCTION_INFO_V1(overbelow_box3D_box3D);

bool
overbelow_box3D_box3D_internal(Box3D *box1, Box3D *box2)
{
	return FPle(box1->low.y, box2->high.y);
}

/*
 * Is the first Box3D strictly above the second Box3D?
 */
Datum
above_box3D_box3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box1 = PG_GETARG_BOX3D_P(0);
	Box3D	   *box2 = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(above_box3D_box3D_internal(box1, box2));
}

PG_FUNCTION_INFO_V1(above_box3D_box3D);

bool
above_box3D_box3D_internal(Box3D *box1, Box3D *box2)
{
	return FPgt(box1->low.y, box2->high.y);
}

/*
 * Is the first Box3D above or overlaps the second Box3D?
 */
Datum
overabove_box3D_box3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box1 = PG_GETARG_BOX3D_P(0);
	Box3D	   *box2 = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overabove_box3D_box3D_internal(box1, box2));
}

PG_FUNCTION_INFO_V1(overabove_box3D_box3D);

bool
overabove_box3D_box3D_internal(Box3D *box1, Box3D *box2)
{
	return FPge(box1->high.y, box2->low.y);
}

/*
 * Is the first Box3D strictly in front of the second Box3D?
 */
Datum
front_box3D_box3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box1 = PG_GETARG_BOX3D_P(0);
	Box3D	   *box2 = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(front_box3D_box3D_internal(box1, box2));
}

PG_FUNCTION_INFO_V1(front_box3D_box3D);

bool
front_box3D_box3D_internal(Box3D *box1, Box3D *box2)
{
	return FPlt(box1->high.z, box2->low.z);
}

/*
 * Is the first Box3D in front of or overlaps the second Box3D?
 */
Datum
overfront_box3D_box3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box1 = PG_GETARG_BOX3D_P(0);
	Box3D	   *box2 = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overfront_box3D_box3D_internal(box1, box2));
}

PG_FUNCTION_INFO_V1(overfront_box3D_box3D);

bool
overfront_box3D_box3D_internal(Box3D *box1, Box3D *box2)
{
	return FPle(box1->low.z, box2->high.z);
}

/*
 * Is the first Box3D strictly at the back of the second Box3D?
 */
Datum
back_box3D_box3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box1 = PG_GETARG_BOX3D_P(0);
	Box3D	   *box2 = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(back_box3D_box3D_internal(box1, box2));
}

PG_FUNCTION_INFO_V1(back_box3D_box3D);

bool
back_box3D_box3D_internal(Box3D *box1, Box3D *box2)
{
	return FPgt(box1->low.z, box2->high.z);
}

/*
 * Is the first Box3D at the back of or overlaps the second Box3D?
 */
Datum
overback_box3D_box3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box1 = PG_GETARG_BOX3D_P(0);
	Box3D	   *box2 = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overback_box3D_box3D_internal(box1, box2));
}

PG_FUNCTION_INFO_V1(overback_box3D_box3D);

bool
overback_box3D_box3D_internal(Box3D *box1, Box3D *box2)
{
	return FPge(box1->high.z, box2->low.z);
}

/*----------------------------------------------------------*/

/* box3D OPERATOR path3D */

/*
 * Is the Box3D strictly to the left of the Path3D?
 */
Datum
left_box3D_path3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = left_box3D_path3D_internal(box, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(left_box3D_path3D);

bool
left_box3D_path3D_internal(Box3D *box, Path3D *path)
{
	return FPlt(box->high.x, path->boundbox.low.x);
}

/*
 * Is the Box3D to the left of or overlaps the Path3D?
 */
Datum
overleft_box3D_path3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overleft_box3D_path3D_internal(box, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overleft_box3D_path3D);

bool
overleft_box3D_path3D_internal(Box3D *box, Path3D *path)
{
	return FPle(box->high.x, path->boundbox.high.x);
}

/*
 * Is the Box3D strictly to the right of the Path3D?
 */
Datum
right_box3D_path3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = right_box3D_path3D_internal(box, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(right_box3D_path3D);

bool
right_box3D_path3D_internal(Box3D *box, Path3D *path)
{
	return FPgt(box->low.x, path->boundbox.high.x);
}

/*
 * Is the Box3D to the right of or overlaps the Path3D?
 */
Datum
overright_box3D_path3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overright_box3D_path3D_internal(box, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overright_box3D_path3D);

bool
overright_box3D_path3D_internal(Box3D *box, Path3D *path)
{
	return FPge(box->low.x, path->boundbox.low.x);
}

/*
 * Is the Box3D strictly below the Path3D?
 */
Datum
below_box3D_path3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = below_box3D_path3D_internal(box, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(below_box3D_path3D);

bool
below_box3D_path3D_internal(Box3D *box, Path3D *path)
{
	return FPlt(box->high.y, path->boundbox.low.y);
}

/*
 * Is the Box3D below or overlaps the Path3D?
 */
Datum
overbelow_box3D_path3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overbelow_box3D_path3D_internal(box, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overbelow_box3D_path3D);

bool
overbelow_box3D_path3D_internal(Box3D *box, Path3D *path)
{
	return FPle(box->high.y, path->boundbox.high.y);
}

/*
 * Is the Box3D strictly above the Path3D?
 */
Datum
above_box3D_path3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = above_box3D_path3D_internal(box, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(above_box3D_path3D);

bool
above_box3D_path3D_internal(Box3D *box, Path3D *path)
{
	return FPgt(box->low.y, path->boundbox.high.y);
}

/*
 * Is the Box3D above or overlaps the Path3D?
 */
Datum
overabove_box3D_path3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overabove_box3D_path3D_internal(box, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overabove_box3D_path3D);

bool
overabove_box3D_path3D_internal(Box3D *box, Path3D *path)
{
	return FPge(box->low.y, path->boundbox.low.y);
}

/*
 * Is the Box3D strictly in front of the Path3D?
 */
Datum
front_box3D_path3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = front_box3D_path3D_internal(box, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(front_box3D_path3D);

bool
front_box3D_path3D_internal(Box3D *box, Path3D *path)
{
	return FPlt(box->high.z, path->boundbox.low.z);
}

/*
 * Is the Box3D in front of or overlaps the Path3D?
 */
Datum
overfront_box3D_path3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overfront_box3D_path3D_internal(box, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overfront_box3D_path3D);

bool
overfront_box3D_path3D_internal(Box3D *box, Path3D *path)
{
	return FPle(box->high.z, path->boundbox.high.z);
}

/*
 * Is the Box3D strictly at the back of the Path3D?
 */
Datum
back_box3D_path3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = back_box3D_path3D_internal(box, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(back_box3D_path3D);

bool
back_box3D_path3D_internal(Box3D *box, Path3D *path)
{
	return FPgt(box->low.z, path->boundbox.high.z);
}

/*
 * Is the Box3D at the back of or overlaps the Path3D?
 */
Datum
overback_box3D_path3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overback_box3D_path3D_internal(box, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overback_box3D_path3D);

bool
overback_box3D_path3D_internal(Box3D *box, Path3D *path)
{
	return FPge(box->low.z, path->boundbox.low.z);
}

/*----------------------------------------------------------*/

/* box3D OPERATOR polygon3D */

/*
 * Is the Box3D strictly to the left of the Polygon3D?
 */
Datum
left_box3D_poly3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = left_box3D_poly3D_internal(box, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(left_box3D_poly3D);

bool
left_box3D_poly3D_internal(Box3D *box, Polygon3D *poly)
{
	return FPlt(box->high.x, poly->boundbox.low.x);
}

/*
 * Is the Box3D to the left of or overlaps the Polygon3D?
 */
Datum
overleft_box3D_poly3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overleft_box3D_poly3D_internal(box, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overleft_box3D_poly3D);

bool
overleft_box3D_poly3D_internal(Box3D *box, Polygon3D *poly)
{
	return FPle(box->high.x, poly->boundbox.high.x);
}

/*
 * Is the Box3D strictly to the right of the Polygon3D?
 */
Datum
right_box3D_poly3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = right_box3D_poly3D_internal(box, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(right_box3D_poly3D);

bool
right_box3D_poly3D_internal(Box3D *box, Polygon3D *poly)
{
	return FPgt(box->low.x, poly->boundbox.high.x);
}

/*
 * Is the Box3D to the right of or overlaps the Polygon3D?
 */
Datum
overright_box3D_poly3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overright_box3D_poly3D_internal(box, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overright_box3D_poly3D);

bool
overright_box3D_poly3D_internal(Box3D *box, Polygon3D *poly)
{
	return FPge(box->low.x, poly->boundbox.low.x);
}

/*
 * Is the Box3D strictly below the Polygon3D?
 */
Datum
below_box3D_poly3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = below_box3D_poly3D_internal(box, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(below_box3D_poly3D);

bool
below_box3D_poly3D_internal(Box3D *box, Polygon3D *poly)
{
	return FPlt(box->high.y, poly->boundbox.low.y);
}

/*
 * Is the Box3D below or overlaps the Polygon3D?
 */
Datum
overbelow_box3D_poly3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overbelow_box3D_poly3D_internal(box, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overbelow_box3D_poly3D);

bool
overbelow_box3D_poly3D_internal(Box3D *box, Polygon3D *poly)
{
	return FPle(box->high.y, poly->boundbox.high.y);
}

/*
 * Is the Box3D strictly above the Polygon3D?
 */
Datum
above_box3D_poly3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = above_box3D_poly3D_internal(box, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(above_box3D_poly3D);

bool
above_box3D_poly3D_internal(Box3D *box, Polygon3D *poly)
{
	return FPgt(box->low.y, poly->boundbox.high.y);
}

/*
 * Is the Box3D above or overlaps the Polygon3D?
 */
Datum
overabove_box3D_poly3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overabove_box3D_poly3D_internal(box, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overabove_box3D_poly3D);

bool
overabove_box3D_poly3D_internal(Box3D *box, Polygon3D *poly)
{
	return FPge(box->low.y, poly->boundbox.low.y);
}

/*
 * Is the Box3D strictly in front of the Polygon3D?
 */
Datum
front_box3D_poly3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = front_box3D_poly3D_internal(box, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(front_box3D_poly3D);

bool
front_box3D_poly3D_internal(Box3D *box, Polygon3D *poly)
{
	return FPlt(box->high.z, poly->boundbox.low.z);
}

/*
 * Is the Box3D in front of or overlaps the Polygon3D?
 */
Datum
overfront_box3D_poly3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overfront_box3D_poly3D_internal(box, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overfront_box3D_poly3D);

bool
overfront_box3D_poly3D_internal(Box3D *box, Polygon3D *poly)
{
	return FPle(box->high.z, poly->boundbox.high.z);
}

/*
 * Is the Box3D strictly at the back of the Polygon3D?
 */
Datum
back_box3D_poly3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = back_box3D_poly3D_internal(box, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(back_box3D_poly3D);

bool
back_box3D_poly3D_internal(Box3D *box, Polygon3D *poly)
{
	return FPgt(box->low.z, poly->boundbox.high.z);
}

/*
 * Is the Box3D at the back of or overlaps the Polygon3D?
 */
Datum
overback_box3D_poly3D(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overback_box3D_poly3D_internal(box, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overback_box3D_poly3D);

bool
overback_box3D_poly3D_internal(Box3D *box, Polygon3D *poly)
{
	return FPge(box->low.z, poly->boundbox.low.z);
}

/*----------------------------------------------------------*/

/* box3D OPERATOR sphere */

/*
 * Is the Box3D strictly to the left of the Sphere?
 */
Datum
left_box3D_sphere(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(left_box3D_sphere_internal(box, sphere));
}

PG_FUNCTION_INFO_V1(left_box3D_sphere);

bool
left_box3D_sphere_internal(Box3D *box, Sphere *sphere)
{
	return FPlt(box->high.x, (sphere->center.x - sphere->radius));
}

/*
 * Is the Box3D to the left of or overlaps the Sphere?
 */
Datum
overleft_box3D_sphere(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overleft_box3D_sphere_internal(box, sphere));
}

PG_FUNCTION_INFO_V1(overleft_box3D_sphere);

bool
overleft_box3D_sphere_internal(Box3D *box, Sphere *sphere)
{
	return FPle(box->low.x, (sphere->center.x + sphere->radius));
}

/*
 * Is the Box3D strictly to the right of the Sphere?
 */
Datum
right_box3D_sphere(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(right_box3D_sphere_internal(box, sphere));
}

PG_FUNCTION_INFO_V1(right_box3D_sphere);

bool
right_box3D_sphere_internal(Box3D *box, Sphere *sphere)
{
	return FPgt(box->low.x, (sphere->center.x + sphere->radius));
}

/*
 * Is the Box3D to the right of or overlaps the Sphere?
 */
Datum
overright_box3D_sphere(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overright_box3D_sphere_internal(box, sphere));
}

PG_FUNCTION_INFO_V1(overright_box3D_sphere);

bool
overright_box3D_sphere_internal(Box3D *box, Sphere *sphere)
{
	return FPge(box->high.x, (sphere->center.x - sphere->radius));
}

/*
 * Is the Box3D strictly below the Sphere?
 */
Datum
below_box3D_sphere(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(below_box3D_sphere_internal(box, sphere));
}

PG_FUNCTION_INFO_V1(below_box3D_sphere);

bool
below_box3D_sphere_internal(Box3D *box, Sphere *sphere)
{
	return FPlt(box->high.y, (sphere->center.y - sphere->radius));
}

/*
 * Is the Box3D below or overlaps the Sphere?
 */
Datum
overbelow_box3D_sphere(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overbelow_box3D_sphere_internal(box, sphere));
}

PG_FUNCTION_INFO_V1(overbelow_box3D_sphere);

bool
overbelow_box3D_sphere_internal(Box3D *box, Sphere *sphere)
{
	return FPle(box->low.y, (sphere->center.y + sphere->radius));
}

/*
 * Is the Box3D strictly above the Sphere?
 */
Datum
above_box3D_sphere(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(above_box3D_sphere_internal(box, sphere));
}

PG_FUNCTION_INFO_V1(above_box3D_sphere);

bool
above_box3D_sphere_internal(Box3D *box, Sphere *sphere)
{
	return FPgt(box->low.y, (sphere->center.y + sphere->radius));
}

/*
 * Is the Box3D above or overlaps the Sphere?
 */
Datum
overabove_box3D_sphere(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overabove_box3D_sphere_internal(box, sphere));
}

PG_FUNCTION_INFO_V1(overabove_box3D_sphere);

bool
overabove_box3D_sphere_internal(Box3D *box, Sphere *sphere)
{
	return FPge(box->high.y, (sphere->center.y - sphere->radius));
}

/*
 * Is the Box3D strictly in front of the Sphere?
 */
Datum
front_box3D_sphere(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(front_box3D_sphere_internal(box, sphere));
}

PG_FUNCTION_INFO_V1(front_box3D_sphere);

bool
front_box3D_sphere_internal(Box3D *box, Sphere *sphere)
{
	return FPlt(box->high.z, (sphere->center.z - sphere->radius));
}

/*
 * Is the Box3D in front of or overlaps the Sphere?
 */
Datum
overfront_box3D_sphere(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overfront_box3D_sphere_internal(box, sphere));
}

PG_FUNCTION_INFO_V1(overfront_box3D_sphere);

bool
overfront_box3D_sphere_internal(Box3D *box, Sphere *sphere)
{
	return FPle(box->low.z, (sphere->center.z + sphere->radius));
}

/*
 * Is the Box3D strictly at the back of the Sphere?
 */
Datum
back_box3D_sphere(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(back_box3D_sphere_internal(box, sphere));
}

PG_FUNCTION_INFO_V1(back_box3D_sphere);

bool
back_box3D_sphere_internal(Box3D *box, Sphere *sphere)
{
	return FPgt(box->low.z, (sphere->center.z + sphere->radius));
}

/*
 * Is the Box3D at the back of or overlaps the Sphere?
 */
Datum
overback_box3D_sphere(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overback_box3D_sphere_internal(box, sphere));
}

PG_FUNCTION_INFO_V1(overback_box3D_sphere);

bool
overback_box3D_sphere_internal(Box3D *box, Sphere *sphere)
{
	return FPge(box->high.z, (sphere->center.z - sphere->radius));
}

/*****************************************************************************/

/* path3D OPERATOR point3D */

/*
 * Is the Path3D strictly to the left of the Point3D?
 */
Datum
left_path3D_point3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Point3D    *point = PG_GETARG_POINT3D_P(1);
	bool		result;

	result = left_path3D_point3D_internal(path, point);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(left_path3D_point3D);

bool
left_path3D_point3D_internal(Path3D *path, Point3D *point)
{
	return FPlt(path->boundbox.high.x, point->x);
}

/*
 * Is the Path3D to the left of or overlaps the Point3D?
 */
Datum
overleft_path3D_point3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Point3D    *point = PG_GETARG_POINT3D_P(1);
	bool		result;

	result = overleft_path3D_point3D_internal(path, point);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overleft_path3D_point3D);

bool
overleft_path3D_point3D_internal(Path3D *path, Point3D *point)
{
	return FPle(path->boundbox.high.x, point->x);
}

/*
 * Is the Path3D strictly to the right of the Point3D?
 */
Datum
right_path3D_point3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Point3D    *point = PG_GETARG_POINT3D_P(1);
	bool		result;

	result = right_path3D_point3D_internal(path, point);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(right_path3D_point3D);

bool
right_path3D_point3D_internal(Path3D *path, Point3D *point)
{
	return FPgt(path->boundbox.low.x, point->x);
}

/*
 * Is the Path3D to the right of or overlaps the Point3D?
 */
Datum
overright_path3D_point3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Point3D    *point = PG_GETARG_POINT3D_P(1);
	bool		result;

	result = overright_path3D_point3D_internal(path, point);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overright_path3D_point3D);

bool
overright_path3D_point3D_internal(Path3D *path, Point3D *point)
{
	return FPge(path->boundbox.low.x, point->x);
}

/*
 * Is the Path3D strictly below the Point3D?
 */
Datum
below_path3D_point3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Point3D    *point = PG_GETARG_POINT3D_P(1);
	bool		result;

	result = below_path3D_point3D_internal(path, point);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(below_path3D_point3D);

bool
below_path3D_point3D_internal(Path3D *path, Point3D *point)
{
	return FPlt(path->boundbox.high.y, point->y);
}

/*
 * Is the Path3D below or overlaps the Point3D?
 */
Datum
overbelow_path3D_point3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Point3D    *point = PG_GETARG_POINT3D_P(1);
	bool		result;

	result = overbelow_path3D_point3D_internal(path, point);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overbelow_path3D_point3D);

bool
overbelow_path3D_point3D_internal(Path3D *path, Point3D *point)
{
	return FPle(path->boundbox.high.y, point->y);
}

/*
 * Is the Path3D strictly above the Point3D?
 */
Datum
above_path3D_point3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Point3D    *point = PG_GETARG_POINT3D_P(1);
	bool		result;

	result = above_path3D_point3D_internal(path, point);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(above_path3D_point3D);

bool
above_path3D_point3D_internal(Path3D *path, Point3D *point)
{
	return FPgt(path->boundbox.low.y, point->y);
}

/*
 * Is the Path3D above or overlaps the Point3D?
 */
Datum
overabove_path3D_point3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Point3D    *point = PG_GETARG_POINT3D_P(1);
	bool		result;

	result = overabove_path3D_point3D_internal(path, point);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overabove_path3D_point3D);

bool
overabove_path3D_point3D_internal(Path3D *path, Point3D *point)
{
	return FPge(path->boundbox.low.y, point->y);
}

/*
 * Is the Path3D strictly in front of the Point3D?
 */
Datum
front_path3D_point3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Point3D    *point = PG_GETARG_POINT3D_P(1);
	bool		result;

	result = front_path3D_point3D_internal(path, point);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(front_path3D_point3D);

bool
front_path3D_point3D_internal(Path3D *path, Point3D *point)
{
	return FPlt(path->boundbox.high.z, point->z);
}

/*
 * Is the Path3D in front of or overlaps the Point3D?
 */
Datum
overfront_path3D_point3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Point3D    *point = PG_GETARG_POINT3D_P(1);
	bool		result;

	result = overfront_path3D_point3D_internal(path, point);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overfront_path3D_point3D);

bool
overfront_path3D_point3D_internal(Path3D *path, Point3D *point)
{
	return FPle(path->boundbox.high.z, point->z);
}

/*
 * Is the Path3D strictly at the back of the Point3D?
 */
Datum
back_path3D_point3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Point3D    *point = PG_GETARG_POINT3D_P(1);
	bool		result;

	result = back_path3D_point3D_internal(path, point);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(back_path3D_point3D);

bool
back_path3D_point3D_internal(Path3D *path, Point3D *point)
{
	return FPgt(path->boundbox.low.z, point->z);
}

/*
 * Is the Path3D at the back of or overlaps the Point3D?
 */
Datum
overback_path3D_point3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Point3D    *point = PG_GETARG_POINT3D_P(1);
	bool		result;

	result = overback_path3D_point3D_internal(path, point);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overback_path3D_point3D);

bool
overback_path3D_point3D_internal(Path3D *path, Point3D *point)
{
	return FPge(path->boundbox.low.z, point->z);
}

/*----------------------------------------------------------*/

/* path3D OPERATOR lseg3D */

/*
 * Is the Path3D strictly to the left of the Lseg3D?
 */
Datum
left_path3D_lseg3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	bool		result;

	result = left_path3D_lseg3D_internal(path, lseg);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(left_path3D_lseg3D);

bool
left_path3D_lseg3D_internal(Path3D *path, Lseg3D *lseg)
{
	return FPlt(path->boundbox.high.x,
				Min(lseg->p[0].x,lseg->p[1].x));
}

/*
 * Is the Path3D to the left of or overlaps the Lseg3D?
 */
Datum
overleft_path3D_lseg3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	bool		result;

	result = overleft_path3D_lseg3D_internal(path, lseg);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overleft_path3D_lseg3D);

bool
overleft_path3D_lseg3D_internal(Path3D *path, Lseg3D *lseg)
{
	return FPle(path->boundbox.high.x, 
				Min(lseg->p[0].x,lseg->p[1].x));
}

/*
 * Is the Path3D strictly to the right of the Lseg3D?
 */
Datum
right_path3D_lseg3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	bool		result;

	result = right_path3D_lseg3D_internal(path, lseg);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(right_path3D_lseg3D);

bool
right_path3D_lseg3D_internal(Path3D *path, Lseg3D *lseg)
{
	return FPgt(path->boundbox.low.x, 
				Max(lseg->p[0].x,lseg->p[1].x));
}

/*
 * Is the Path3D to the right of or overlaps the Lseg3D?
 */
Datum
overright_path3D_lseg3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	bool		result;

	result = overright_path3D_lseg3D_internal(path, lseg);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overright_path3D_lseg3D);

bool
overright_path3D_lseg3D_internal(Path3D *path, Lseg3D *lseg)
{
	return FPge(path->boundbox.low.x, 
				Max(lseg->p[0].x,lseg->p[1].x));
}

/*
 * Is the Path3D strictly below the Lseg3D?
 */
Datum
below_path3D_lseg3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	bool		result;

	result = below_path3D_lseg3D_internal(path, lseg);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(below_path3D_lseg3D);

bool
below_path3D_lseg3D_internal(Path3D *path, Lseg3D *lseg)
{
	return FPlt(path->boundbox.high.y, 
				Min(lseg->p[0].y,lseg->p[1].y));
}

/*
 * Is the Path3D below or overlaps the Lseg3D?
 */
Datum
overbelow_path3D_lseg3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	bool		result;

	result = overbelow_path3D_lseg3D_internal(path, lseg);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overbelow_path3D_lseg3D);

bool
overbelow_path3D_lseg3D_internal(Path3D *path, Lseg3D *lseg)
{
	return FPle(path->boundbox.high.y,
				Min(lseg->p[0].y,lseg->p[1].y));
}

/*
 * Is the Path3D strictly above the Lseg3D?
 */
Datum
above_path3D_lseg3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	bool		result;

	result = above_path3D_lseg3D_internal(path, lseg);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(above_path3D_lseg3D);

bool
above_path3D_lseg3D_internal(Path3D *path, Lseg3D *lseg)
{
	return FPgt(path->boundbox.low.y, 
				Max(lseg->p[0].y,lseg->p[1].y));
}

/*
 * Is the Path3D above or overlaps the Lseg3D?
 */
Datum
overabove_path3D_lseg3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	bool		result;

	result = overabove_path3D_lseg3D_internal(path, lseg);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overabove_path3D_lseg3D);

bool
overabove_path3D_lseg3D_internal(Path3D *path, Lseg3D *lseg)
{
	return FPge(path->boundbox.low.y, 
				Max(lseg->p[0].y,lseg->p[1].y));
}

/*
 * Is the Path3D strictly in front of the Lseg3D?
 */
Datum
front_path3D_lseg3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	bool		result;

	result = front_path3D_lseg3D_internal(path, lseg);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(front_path3D_lseg3D);

bool
front_path3D_lseg3D_internal(Path3D *path, Lseg3D *lseg)
{
	return FPlt(path->boundbox.high.z,
				Min(lseg->p[0].z,lseg->p[1].z));
}

/*
 * Is the Path3D in front of or overlaps the Lseg3D?
 */
Datum
overfront_path3D_lseg3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	bool		result;

	result = overfront_path3D_lseg3D_internal(path, lseg);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overfront_path3D_lseg3D);

bool
overfront_path3D_lseg3D_internal(Path3D *path, Lseg3D *lseg)
{
	return FPle(path->boundbox.high.z,
				Min(lseg->p[0].z,lseg->p[1].z));
}

/*
 * Is the Path3D strictly at the back of the Lseg3D?
 */
Datum
back_path3D_lseg3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	bool		result;

	result = back_path3D_lseg3D_internal(path, lseg);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(back_path3D_lseg3D);

bool
back_path3D_lseg3D_internal(Path3D *path, Lseg3D *lseg)
{
	return FPgt(path->boundbox.low.z, 
				Max(lseg->p[0].z,lseg->p[1].z));
}

/*
 * Is the Path3D at the back of or overlaps the Lseg3D?
 */
Datum
overback_path3D_lseg3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	bool		result;

	result = overback_path3D_lseg3D_internal(path, lseg);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overback_path3D_lseg3D);

bool
overback_path3D_lseg3D_internal(Path3D *path, Lseg3D *lseg)
{
	return FPge(path->boundbox.low.z, 
				Max(lseg->p[0].z,lseg->p[1].z));
}

/*----------------------------------------------------------*/

/* path3D OPERATOR line3D */

/*
 * Is the Path3D strictly to the left of the Line3D?
 */
Datum
left_path3D_line3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	bool		result;

	result = left_path3D_line3D_internal(path, line);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(left_path3D_line3D);

bool
left_path3D_line3D_internal(Path3D *path, Line3D *line)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPlt(path->boundbox.high.x, line->p[0].x);
}

/*
 * Is the Path3D to the left of or overlaps the Line3D?
 */
Datum
overleft_path3D_line3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	bool		result;

	result = overleft_path3D_line3D_internal(path, line);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overleft_path3D_line3D);

bool
overleft_path3D_line3D_internal(Path3D *path, Line3D *line)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPle(path->boundbox.high.x, line->p[0].x);
}

/*
 * Is the Path3D strictly to the right of the Line3D?
 */
Datum
right_path3D_line3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	bool		result;

	result = right_path3D_line3D_internal(path, line);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(right_path3D_line3D);

bool
right_path3D_line3D_internal(Path3D *path, Line3D *line)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPgt(path->boundbox.low.x, line->p[0].x);
}

/*
 * Is the Path3D to the right of or overlaps the Line3D?
 */
Datum
overright_path3D_line3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	bool		result;

	result = overright_path3D_line3D_internal(path, line);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overright_path3D_line3D);

bool
overright_path3D_line3D_internal(Path3D *path, Line3D *line)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPge(path->boundbox.low.x, line->p[0].x);
}

/*
 * Is the Path3D strictly below the Line3D?
 */
Datum
below_path3D_line3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	bool		result;

	result = below_path3D_line3D_internal(path, line);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(below_path3D_line3D);

bool
below_path3D_line3D_internal(Path3D *path, Line3D *line)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPlt(path->boundbox.high.y, line->p[0].y);
}

/*
 * Is the Path3D below or overlaps the Line3D?
 */
Datum
overbelow_path3D_line3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	bool		result;

	result = overbelow_path3D_line3D_internal(path, line);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overbelow_path3D_line3D);

bool
overbelow_path3D_line3D_internal(Path3D *path, Line3D *line)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPle(path->boundbox.high.y, line->p[0].y);
}

/*
 * Is the Path3D strictly above the Line3D?
 */
Datum
above_path3D_line3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	bool		result;

	result = above_path3D_line3D_internal(path, line);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(above_path3D_line3D);

bool
above_path3D_line3D_internal(Path3D *path, Line3D *line)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPgt(path->boundbox.low.y, line->p[0].y);
}

/*
 * Is the Path3D above or overlaps the Line3D?
 */
Datum
overabove_path3D_line3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	bool		result;

	result = overabove_path3D_line3D_internal(path, line);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overabove_path3D_line3D);

bool
overabove_path3D_line3D_internal(Path3D *path, Line3D *line)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPge(path->boundbox.low.y, line->p[0].y);
}

/*
 * Is the Path3D strictly in front of the Line3D?
 */
Datum
front_path3D_line3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	bool		result;

	result = front_path3D_line3D_internal(path, line);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(front_path3D_line3D);

bool
front_path3D_line3D_internal(Path3D *path, Line3D *line)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPlt(path->boundbox.high.z, line->p[0].z);
}

/*
 * Is the Path3D in front of or overlaps the Line3D?
 */
Datum
overfront_path3D_line3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	bool		result;

	result = overfront_path3D_line3D_internal(path, line);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overfront_path3D_line3D);

bool
overfront_path3D_line3D_internal(Path3D *path, Line3D *line)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPle(path->boundbox.high.z, line->p[0].z);
}

/*
 * Is the Path3D strictly at the back of the Line3D?
 */
Datum
back_path3D_line3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	bool		result;

	result = back_path3D_line3D_internal(path, line);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(back_path3D_line3D);

bool
back_path3D_line3D_internal(Path3D *path, Line3D *line)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPgt(path->boundbox.low.z, line->p[0].z);
}

/*
 * Is the Path3D at the back of or overlaps the Line3D?
 */
Datum
overback_path3D_line3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	bool		result;

	result = overback_path3D_line3D_internal(path, line);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overback_path3D_line3D);

bool
overback_path3D_line3D_internal(Path3D *path, Line3D *line)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPge(path->boundbox.low.z, line->p[0].z);
}

/*----------------------------------------------------------*/

/* path3D OPERATOR box3D */

/*
 * Is the Path3D strictly to the left of the Box3D?
 */
Datum
left_path3D_box3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	bool		result;

	result = left_path3D_box3D_internal(path, box);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(left_path3D_box3D);

bool
left_path3D_box3D_internal(Path3D *path, Box3D *box)
{
	return FPlt(path->boundbox.high.x, box->low.x);
}

/*
 * Is the Path3D to the left of or overlaps the Box3D?
 */
Datum
overleft_path3D_box3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	bool		result;

	result = overleft_path3D_box3D_internal(path, box);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overleft_path3D_box3D);

bool
overleft_path3D_box3D_internal(Path3D *path, Box3D *box)
{
	return FPle(path->boundbox.high.x, box->high.x);
}

/*
 * Is the Path3D strictly to the right of the Box3D?
 */
Datum
right_path3D_box3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	bool		result;

	result = right_path3D_box3D_internal(path, box);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(right_path3D_box3D);

bool
right_path3D_box3D_internal(Path3D *path, Box3D *box)
{
	return FPgt(path->boundbox.low.x, box->high.x);
}

/*
 * Is the Path3D to the right of or overlaps the Box3D?
 */
Datum
overright_path3D_box3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	bool		result;

	result = overright_path3D_box3D_internal(path, box);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overright_path3D_box3D);

bool
overright_path3D_box3D_internal(Path3D *path, Box3D *box)
{
	return FPge(path->boundbox.low.x, box->low.x);
}

/*
 * Is the Path3D strictly below the Box3D?
 */
Datum
below_path3D_box3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	bool		result;

	result = below_path3D_box3D_internal(path, box);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(below_path3D_box3D);

bool
below_path3D_box3D_internal(Path3D *path, Box3D *box)
{
	return FPlt(path->boundbox.high.y, box->low.y);
}

/*
 * Is the Path3D below or overlaps the Box3D?
 */
Datum
overbelow_path3D_box3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	bool		result;

	result = overbelow_path3D_box3D_internal(path, box);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overbelow_path3D_box3D);

bool
overbelow_path3D_box3D_internal(Path3D *path, Box3D *box)
{
	return FPle(path->boundbox.high.y, box->high.y);
}

/*
 * Is the Path3D strictly above the Box3D?
 */
Datum
above_path3D_box3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	bool		result;

	result = above_path3D_box3D_internal(path, box);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(above_path3D_box3D);

bool
above_path3D_box3D_internal(Path3D *path, Box3D *box)
{
	return FPgt(path->boundbox.low.y, box->high.y);
}

/*
 * Is the Path3D above or overlaps the Box3D?
 */
Datum
overabove_path3D_box3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	bool		result;

	result = overabove_path3D_box3D_internal(path, box);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overabove_path3D_box3D);

bool
overabove_path3D_box3D_internal(Path3D *path, Box3D *box)
{
	return FPge(path->boundbox.low.y, box->low.y);
}

/*
 * Is the Path3D strictly in front of the Box3D?
 */
Datum
front_path3D_box3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	bool		result;

	result = front_path3D_box3D_internal(path, box);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(front_path3D_box3D);

bool
front_path3D_box3D_internal(Path3D *path, Box3D *box)
{
	return FPlt(path->boundbox.high.z, box->low.z);
}

/*
 * Is the Path3D in front of or overlaps the Box3D?
 */
Datum
overfront_path3D_box3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	bool		result;

	result = overfront_path3D_box3D_internal(path, box);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overfront_path3D_box3D);

bool
overfront_path3D_box3D_internal(Path3D *path, Box3D *box)
{
	return FPle(path->boundbox.high.z, box->high.z);
}

/*
 * Is the Path3D strictly at the back of the Box3D?
 */
Datum
back_path3D_box3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	bool		result;

	result = back_path3D_box3D_internal(path, box);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(back_path3D_box3D);

bool
back_path3D_box3D_internal(Path3D *path, Box3D *box)
{
	return FPgt(path->boundbox.low.z, box->high.z);
}

/*
 * Is the Path3D at the back of or overlaps the Box3D?
 */
Datum
overback_path3D_box3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	bool		result;

	result = overback_path3D_box3D_internal(path, box);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overback_path3D_box3D);

bool
overback_path3D_box3D_internal(Path3D *path, Box3D *box)
{
	return FPge(path->boundbox.low.z, box->low.z);
}

/*----------------------------------------------------------*/

/* path3D OPERATOR path3D */

/*
 * Is the first Path3D strictly to the left of the second Path3D?
 */
Datum
left_path3D_path3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path1 = PG_GETARG_PATH3D_P(0);
	Path3D     *path2 = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = left_path3D_path3D_internal(path1, path2);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path1, 0);
	PG_FREE_IF_COPY(path2, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(left_path3D_path3D);

bool
left_path3D_path3D_internal(Path3D *path1, Path3D *path2)
{
	return FPlt(path1->boundbox.high.x, path2->boundbox.low.x);
}

/*
 * Is the first Path3D to the left of or overlaps the second Path3D?
 */
Datum
overleft_path3D_path3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path1 = PG_GETARG_PATH3D_P(0);
	Path3D     *path2 = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overleft_path3D_path3D_internal(path1, path2);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path1, 0);
	PG_FREE_IF_COPY(path2, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overleft_path3D_path3D);

bool
overleft_path3D_path3D_internal(Path3D *path1, Path3D *path2)
{
	return FPle(path1->boundbox.high.x, path2->boundbox.high.x);
}

/*
 * Is the first Path3D strictly to the right of the second Path3D?
 */
Datum
right_path3D_path3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path1 = PG_GETARG_PATH3D_P(0);
	Path3D     *path2 = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = right_path3D_path3D_internal(path1, path2);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path1, 0);
	PG_FREE_IF_COPY(path2, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(right_path3D_path3D);

bool
right_path3D_path3D_internal(Path3D *path1, Path3D *path2)
{
	return FPgt(path1->boundbox.low.x, path2->boundbox.high.x);
}

/*
 * Is the first Path3D to the right of or overlaps the second Path3D?
 */
Datum
overright_path3D_path3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path1 = PG_GETARG_PATH3D_P(0);
	Path3D     *path2 = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overright_path3D_path3D_internal(path1, path2);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path1, 0);
	PG_FREE_IF_COPY(path2, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overright_path3D_path3D);

bool
overright_path3D_path3D_internal(Path3D *path1, Path3D *path2)
{
	return FPge(path1->boundbox.low.x, path2->boundbox.low.x);
}

/*
 * Is the first Path3D strictly below the second Path3D?
 */
Datum
below_path3D_path3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path1 = PG_GETARG_PATH3D_P(0);
	Path3D     *path2 = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = below_path3D_path3D_internal(path1, path2);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path1, 0);
	PG_FREE_IF_COPY(path2, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(below_path3D_path3D);

bool
below_path3D_path3D_internal(Path3D *path1, Path3D *path2)
{
	return FPlt(path1->boundbox.high.y, path2->boundbox.low.y);
}

/*
 * Is the first Path3D below or overlaps the second Path3D?
 */
Datum
overbelow_path3D_path3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path1 = PG_GETARG_PATH3D_P(0);
	Path3D     *path2 = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overbelow_path3D_path3D_internal(path1, path2);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path1, 0);
	PG_FREE_IF_COPY(path2, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overbelow_path3D_path3D);

bool
overbelow_path3D_path3D_internal(Path3D *path1, Path3D *path2)
{
	return FPle(path1->boundbox.high.y, path2->boundbox.high.y);
}

/*
 * Is the first Path3D strictly above the second Path3D?
 */
Datum
above_path3D_path3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path1 = PG_GETARG_PATH3D_P(0);
	Path3D     *path2 = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = above_path3D_path3D_internal(path1, path2);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path1, 0);
	PG_FREE_IF_COPY(path2, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(above_path3D_path3D);

bool
above_path3D_path3D_internal(Path3D *path1, Path3D *path2)
{
	return FPgt(path1->boundbox.low.y, path2->boundbox.high.y);
}

/*
 * Is the first Path3D above or overlaps the second Path3D?
 */
Datum
overabove_path3D_path3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path1 = PG_GETARG_PATH3D_P(0);
	Path3D     *path2 = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overabove_path3D_path3D_internal(path1, path2);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path1, 0);
	PG_FREE_IF_COPY(path2, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overabove_path3D_path3D);

bool
overabove_path3D_path3D_internal(Path3D *path1, Path3D *path2)
{
	return FPge(path1->boundbox.low.y, path2->boundbox.low.y);
}

/*
 * Is the first Path3D strictly in front of the second Path3D?
 */
Datum
front_path3D_path3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path1 = PG_GETARG_PATH3D_P(0);
	Path3D     *path2 = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = front_path3D_path3D_internal(path1, path2);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path1, 0);
	PG_FREE_IF_COPY(path2, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(front_path3D_path3D);

bool
front_path3D_path3D_internal(Path3D *path1, Path3D *path2)
{
	return FPlt(path1->boundbox.high.z, path2->boundbox.low.z);
}

/*
 * Is the first Path3D in front of or overlaps the second Path3D?
 */
Datum
overfront_path3D_path3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path1 = PG_GETARG_PATH3D_P(0);
	Path3D     *path2 = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overfront_path3D_path3D_internal(path1, path2);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path1, 0);
	PG_FREE_IF_COPY(path2, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overfront_path3D_path3D);

bool
overfront_path3D_path3D_internal(Path3D *path1, Path3D *path2)
{
	return FPle(path1->boundbox.high.z, path2->boundbox.high.z);
}

/*
 * Is the first Path3D strictly at the back of the second Path3D?
 */
Datum
back_path3D_path3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path1 = PG_GETARG_PATH3D_P(0);
	Path3D     *path2 = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = back_path3D_path3D_internal(path1, path2);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path1, 0);
	PG_FREE_IF_COPY(path2, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(back_path3D_path3D);

bool
back_path3D_path3D_internal(Path3D *path1, Path3D *path2)
{
	return FPgt(path1->boundbox.low.z, path2->boundbox.high.z);
}

/*
 * Is the first Path3D at the back of or overlaps the second Path3D?
 */
Datum
overback_path3D_path3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path1 = PG_GETARG_PATH3D_P(0);
	Path3D     *path2 = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overback_path3D_path3D_internal(path1, path2);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path1, 0);
	PG_FREE_IF_COPY(path2, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overback_path3D_path3D);

bool
overback_path3D_path3D_internal(Path3D *path1, Path3D *path2)
{
	return FPge(path1->boundbox.low.z, path2->boundbox.low.z);
}

/*----------------------------------------------------------*/

/* path3D OPERATOR polygon3D */

/*
 * Is the Path3D strictly to the left of the Polygon3D?
 */
Datum
left_path3D_poly3D(PG_FUNCTION_ARGS)
{
	Path3D     *path = PG_GETARG_PATH3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = left_path3D_poly3D_internal(path, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(left_path3D_poly3D);

bool
left_path3D_poly3D_internal(Path3D *path, Polygon3D *poly)
{
	return FPlt(path->boundbox.high.x, poly->boundbox.low.x);
}

/*
 * Is the Path3D to the left of or overlaps the Polygon3D?
 */
Datum
overleft_path3D_poly3D(PG_FUNCTION_ARGS)
{
	Path3D     *path = PG_GETARG_PATH3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overleft_path3D_poly3D_internal(path, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overleft_path3D_poly3D);

bool
overleft_path3D_poly3D_internal(Path3D *path, Polygon3D *poly)
{
	return FPle(path->boundbox.high.x, poly->boundbox.high.x);
}

/*
 * Is the Path3D strictly to the right of the Polygon3D?
 */
Datum
right_path3D_poly3D(PG_FUNCTION_ARGS)
{
	Path3D     *path = PG_GETARG_PATH3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = right_path3D_poly3D_internal(path, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(right_path3D_poly3D);

bool
right_path3D_poly3D_internal(Path3D *path, Polygon3D *poly)
{
	return FPgt(path->boundbox.low.x, poly->boundbox.high.x);
}

/*
 * Is the Path3D to the right of or overlaps the Polygon3D?
 */
Datum
overright_path3D_poly3D(PG_FUNCTION_ARGS)
{
	Path3D     *path = PG_GETARG_PATH3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overright_path3D_poly3D_internal(path, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overright_path3D_poly3D);

bool
overright_path3D_poly3D_internal(Path3D *path, Polygon3D *poly)
{
	return FPge(path->boundbox.low.x, poly->boundbox.low.x);
}

/*
 * Is the Path3D strictly below the Polygon3D?
 */
Datum
below_path3D_poly3D(PG_FUNCTION_ARGS)
{
	Path3D     *path = PG_GETARG_PATH3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = below_path3D_poly3D_internal(path, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(below_path3D_poly3D);

bool
below_path3D_poly3D_internal(Path3D *path, Polygon3D *poly)
{
	return FPlt(path->boundbox.high.y, poly->boundbox.low.y);
}

/*
 * Is the Path3D below or overlaps the Polygon3D?
 */
Datum
overbelow_path3D_poly3D(PG_FUNCTION_ARGS)
{
	Path3D     *path = PG_GETARG_PATH3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overbelow_path3D_poly3D_internal(path, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overbelow_path3D_poly3D);

bool
overbelow_path3D_poly3D_internal(Path3D *path, Polygon3D *poly)
{
	return FPle(path->boundbox.high.y, poly->boundbox.high.y);
}

/*
 * Is the Path3D strictly above the Polygon3D?
 */
Datum
above_path3D_poly3D(PG_FUNCTION_ARGS)
{
	Path3D     *path = PG_GETARG_PATH3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = above_path3D_poly3D_internal(path, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(above_path3D_poly3D);

bool
above_path3D_poly3D_internal(Path3D *path, Polygon3D *poly)
{
	return FPgt(path->boundbox.low.y, poly->boundbox.high.y);
}

/*
 * Is the Path3D above or overlaps the Polygon3D?
 */
Datum
overabove_path3D_poly3D(PG_FUNCTION_ARGS)
{
	Path3D     *path = PG_GETARG_PATH3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overabove_path3D_poly3D_internal(path, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overabove_path3D_poly3D);

bool
overabove_path3D_poly3D_internal(Path3D *path, Polygon3D *poly)
{
	return FPge(path->boundbox.low.y, poly->boundbox.low.y);
}

/*
 * Is the Path3D strictly in front of the Polygon3D?
 */
Datum
front_path3D_poly3D(PG_FUNCTION_ARGS)
{
	Path3D     *path = PG_GETARG_PATH3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = front_path3D_poly3D_internal(path, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(front_path3D_poly3D);

bool
front_path3D_poly3D_internal(Path3D *path, Polygon3D *poly)
{
	return FPlt(path->boundbox.high.z, poly->boundbox.low.z);
}

/*
 * Is the Path3D in front of or overlaps the Polygon3D?
 */
Datum
overfront_path3D_poly3D(PG_FUNCTION_ARGS)
{
	Path3D     *path = PG_GETARG_PATH3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overfront_path3D_poly3D_internal(path, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overfront_path3D_poly3D);

bool
overfront_path3D_poly3D_internal(Path3D *path, Polygon3D *poly)
{
	return FPle(path->boundbox.high.z, poly->boundbox.high.z);
}

/*
 * Is the Path3D strictly at the back of the Polygon3D?
 */
Datum
back_path3D_poly3D(PG_FUNCTION_ARGS)
{
	Path3D     *path = PG_GETARG_PATH3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = back_path3D_poly3D_internal(path, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(back_path3D_poly3D);

bool
back_path3D_poly3D_internal(Path3D *path, Polygon3D *poly)
{
	return FPgt(path->boundbox.low.z, poly->boundbox.high.z);
}

/*
 * Is the Path3D at the back of or overlaps the Polygon3D?
 */
Datum
overback_path3D_poly3D(PG_FUNCTION_ARGS)
{
	Path3D     *path = PG_GETARG_PATH3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overback_path3D_poly3D_internal(path, poly);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);
	PG_FREE_IF_COPY(poly, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overback_path3D_poly3D);

bool
overback_path3D_poly3D_internal(Path3D *path, Polygon3D *poly)
{
	return FPge(path->boundbox.low.z, poly->boundbox.low.z);
}

/*----------------------------------------------------------*/

/* path3D OPERATOR sphere */

/*
 * Is the Path3D strictly to the left of the Sphere?
 */
Datum
left_path3D_sphere(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	bool		result;

	result = left_path3D_sphere_internal(path, sphere);
	
	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(left_path3D_sphere);

bool
left_path3D_sphere_internal(Path3D *path, Sphere *sphere)
{
	return FPlt(path->boundbox.high.x, (sphere->center.x - sphere->radius));
}

/*
 * Is the Path3D to the left of or overlaps the Sphere?
 */
Datum
overleft_path3D_sphere(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	bool		result;

	result = overleft_path3D_sphere_internal(path, sphere);
	
	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overleft_path3D_sphere);

bool
overleft_path3D_sphere_internal(Path3D *path, Sphere *sphere)
{
	return FPle(path->boundbox.low.x, (sphere->center.x + sphere->radius));
}

/*
 * Is the Path3D strictly to the right of the Sphere?
 */
Datum
right_path3D_sphere(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	bool		result;

	result = right_path3D_sphere_internal(path, sphere);
	
	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(right_path3D_sphere);

bool
right_path3D_sphere_internal(Path3D *path, Sphere *sphere)
{
	return FPgt(path->boundbox.low.x, (sphere->center.x + sphere->radius));
}

/*
 * Is the Path3D to the right of or overlaps the Sphere?
 */
Datum
overright_path3D_sphere(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	bool		result;

	result = overright_path3D_sphere_internal(path, sphere);
	
	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overright_path3D_sphere);

bool
overright_path3D_sphere_internal(Path3D *path, Sphere *sphere)
{
	return FPge(path->boundbox.high.x, (sphere->center.x - sphere->radius));
}

/*
 * Is the Path3D strictly below the Sphere?
 */
Datum
below_path3D_sphere(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	bool		result;

	result = below_path3D_sphere_internal(path, sphere);
	
	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(below_path3D_sphere);

bool
below_path3D_sphere_internal(Path3D *path, Sphere *sphere)
{
	return FPlt(path->boundbox.high.y, (sphere->center.y - sphere->radius));
}

/*
 * Is the Path3D below or overlaps the Sphere?
 */
Datum
overbelow_path3D_sphere(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	bool		result;

	result = overbelow_path3D_sphere_internal(path, sphere);
	
	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overbelow_path3D_sphere);

bool
overbelow_path3D_sphere_internal(Path3D *path, Sphere *sphere)
{
	return FPle(path->boundbox.low.y, (sphere->center.y + sphere->radius));
}

/*
 * Is the Path3D strictly above the Sphere?
 */
Datum
above_path3D_sphere(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	bool		result;

	result = above_path3D_sphere_internal(path, sphere);
	
	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(above_path3D_sphere);

bool
above_path3D_sphere_internal(Path3D *path, Sphere *sphere)
{
	return FPgt(path->boundbox.low.y, (sphere->center.y + sphere->radius));
}

/*
 * Is the Path3D above or overlaps the Sphere?
 */
Datum
overabove_path3D_sphere(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	bool		result;

	result = overabove_path3D_sphere_internal(path, sphere);
	
	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overabove_path3D_sphere);

bool
overabove_path3D_sphere_internal(Path3D *path, Sphere *sphere)
{
	return FPge(path->boundbox.high.y, (sphere->center.y - sphere->radius));
}

/*
 * Is the Path3D strictly in front of the Sphere?
 */
Datum
front_path3D_sphere(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	bool		result;

	result = front_path3D_sphere_internal(path, sphere);
	
	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(front_path3D_sphere);

bool
front_path3D_sphere_internal(Path3D *path, Sphere *sphere)
{
	return FPlt(path->boundbox.high.z, (sphere->center.z - sphere->radius));
}

/*
 * Is the Path3D in front of or overlaps the Sphere?
 */
Datum
overfront_path3D_sphere(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	bool		result;

	result = overfront_path3D_sphere_internal(path, sphere);
	
	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overfront_path3D_sphere);

bool
overfront_path3D_sphere_internal(Path3D *path, Sphere *sphere)
{
	return FPle(path->boundbox.low.z, (sphere->center.z + sphere->radius));
}

/*
 * Is the Path3D strictly at the back of the Sphere?
 */
Datum
back_path3D_sphere(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	bool		result;

	result = back_path3D_sphere_internal(path, sphere);
	
	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(back_path3D_sphere);

bool
back_path3D_sphere_internal(Path3D *path, Sphere *sphere)
{
	return FPgt(path->boundbox.low.z, (sphere->center.z + sphere->radius));
}

/*
 * Is the Path3D at the back of or overlaps the Sphere?
 */
Datum
overback_path3D_sphere(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	bool		result;

	result = overback_path3D_sphere_internal(path, sphere);
	
	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(path, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overback_path3D_sphere);

bool
overback_path3D_sphere_internal(Path3D *path, Sphere *sphere)
{
	return FPge(path->boundbox.high.z, (sphere->center.z - sphere->radius));
}

/*****************************************************************************/

/* polygon3D OPERATOR point3D */

/*
 * Is the Polygon3D strictly to the left of the Point3D?
 */
Datum
left_poly3D_point3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Point3D    *point = PG_GETARG_POINT3D_P(1);
	bool		result;

	result = left_poly3D_point3D_internal(poly, point);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(left_poly3D_point3D);

bool
left_poly3D_point3D_internal(Polygon3D *poly, Point3D *point)
{
	return FPlt(poly->boundbox.high.x, point->x);
}

/*
 * Is the Polygon3D to the left of or overlaps the Point3D?
 */
Datum
overleft_poly3D_point3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Point3D    *point = PG_GETARG_POINT3D_P(1);
	bool		result;

	result = overleft_poly3D_point3D_internal(poly, point);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overleft_poly3D_point3D);

bool
overleft_poly3D_point3D_internal(Polygon3D *poly, Point3D *point)
{
	return FPle(poly->boundbox.high.x, point->x);
}

/*
 * Is the Polygon3D strictly to the right of the Point3D?
 */
Datum
right_poly3D_point3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Point3D    *point = PG_GETARG_POINT3D_P(1);
	bool		result;

	result = right_poly3D_point3D_internal(poly, point);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(right_poly3D_point3D);

bool
right_poly3D_point3D_internal(Polygon3D *poly, Point3D *point)
{
	return FPgt(poly->boundbox.low.x, point->x);
}

/*
 * Is the Polygon3D to the right of or overlaps the Point3D?
 */
Datum
overright_poly3D_point3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Point3D    *point = PG_GETARG_POINT3D_P(1);
	bool		result;

	result = overright_poly3D_point3D_internal(poly, point);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overright_poly3D_point3D);

bool
overright_poly3D_point3D_internal(Polygon3D *poly, Point3D *point)
{
	return FPge(poly->boundbox.low.x, point->x);
}

/*
 * Is the Polygon3D strictly below the Point3D?
 */
Datum
below_poly3D_point3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Point3D    *point = PG_GETARG_POINT3D_P(1);
	bool		result;

	result = below_poly3D_point3D_internal(poly, point);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(below_poly3D_point3D);

bool
below_poly3D_point3D_internal(Polygon3D *poly, Point3D *point)
{
	return FPlt(poly->boundbox.high.y, point->y);
}

/*
 * Is the Polygon3D below or overlaps the Point3D?
 */
Datum
overbelow_poly3D_point3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Point3D    *point = PG_GETARG_POINT3D_P(1);
	bool		result;

	result = overbelow_poly3D_point3D_internal(poly, point);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overbelow_poly3D_point3D);

bool
overbelow_poly3D_point3D_internal(Polygon3D *poly, Point3D *point)
{
	return FPle(poly->boundbox.high.y, point->y);
}

/*
 * Is the Polygon3D strictly above the Point3D?
 */
Datum
above_poly3D_point3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Point3D    *point = PG_GETARG_POINT3D_P(1);
	bool		result;

	result = above_poly3D_point3D_internal(poly, point);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(above_poly3D_point3D);

bool
above_poly3D_point3D_internal(Polygon3D *poly, Point3D *point)
{
	return FPgt(poly->boundbox.low.y, point->y);
}

/*
 * Is the Polygon3D above or overlaps the Point3D?
 */
Datum
overabove_poly3D_point3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Point3D    *point = PG_GETARG_POINT3D_P(1);
	bool		result;

	result = overabove_poly3D_point3D_internal(poly, point);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overabove_poly3D_point3D);

bool
overabove_poly3D_point3D_internal(Polygon3D *poly, Point3D *point)
{
	return FPge(poly->boundbox.low.y, point->y);
}

/*
 * Is the Polygon3D strictly in front of the Point3D?
 */
Datum
front_poly3D_point3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Point3D    *point = PG_GETARG_POINT3D_P(1);
	bool		result;

	result = front_poly3D_point3D_internal(poly, point);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(front_poly3D_point3D);

bool
front_poly3D_point3D_internal(Polygon3D *poly, Point3D *point)
{
	return FPlt(poly->boundbox.high.z, point->z);
}

/*
 * Is the Polygon3D in front of or overlaps the Point3D?
 */
Datum
overfront_poly3D_point3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Point3D    *point = PG_GETARG_POINT3D_P(1);
	bool		result;

	result = overfront_poly3D_point3D_internal(poly, point);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overfront_poly3D_point3D);

bool
overfront_poly3D_point3D_internal(Polygon3D *poly, Point3D *point)
{
	return FPle(poly->boundbox.high.z, point->z);
}

/*
 * Is the Polygon3D strictly at the back of the Point3D?
 */
Datum
back_poly3D_point3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Point3D    *point = PG_GETARG_POINT3D_P(1);
	bool		result;

	result = back_poly3D_point3D_internal(poly, point);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(back_poly3D_point3D);

bool
back_poly3D_point3D_internal(Polygon3D *poly, Point3D *point)
{
	return FPgt(poly->boundbox.low.z, point->z);
}

/*
 * Is the Polygon3D at the back of or overlaps the Point3D?
 */
Datum
overback_poly3D_point3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Point3D    *point = PG_GETARG_POINT3D_P(1);
	bool		result;

	result = overback_poly3D_point3D_internal(poly, point);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overback_poly3D_point3D);

bool
overback_poly3D_point3D_internal(Polygon3D *poly, Point3D *point)
{
	return FPge(poly->boundbox.low.z, point->z);
}

/*----------------------------------------------------------*/

/* polygon3D OPERATOR lseg3D */

/*
 * Is the Polygon3D strictly to the left of the Lseg3D?
 */
Datum
left_poly3D_lseg3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	bool		result;

	result = left_poly3D_lseg3D_internal(poly, lseg);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(left_poly3D_lseg3D);

bool
left_poly3D_lseg3D_internal(Polygon3D *poly, Lseg3D *lseg)
{
	return FPlt(poly->boundbox.high.x,
				Min(lseg->p[0].x,lseg->p[1].x));
}

/*
 * Is the Polygon3D to the left of or overlaps the Lseg3D?
 */
Datum
overleft_poly3D_lseg3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	bool		result;

	result = overleft_poly3D_lseg3D_internal(poly, lseg);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overleft_poly3D_lseg3D);

bool
overleft_poly3D_lseg3D_internal(Polygon3D *poly, Lseg3D *lseg)
{
	return FPle(poly->boundbox.high.x, 
				Min(lseg->p[0].x,lseg->p[1].x));
}

/*
 * Is the Polygon3D strictly to the right of the Lseg3D?
 */
Datum
right_poly3D_lseg3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	bool		result;

	result = right_poly3D_lseg3D_internal(poly, lseg);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(right_poly3D_lseg3D);

bool
right_poly3D_lseg3D_internal(Polygon3D *poly, Lseg3D *lseg)
{
	return FPgt(poly->boundbox.low.x, 
				Max(lseg->p[0].x,lseg->p[1].x));
}

/*
 * Is the Polygon3D to the right of or overlaps the Lseg3D?
 */
Datum
overright_poly3D_lseg3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	bool		result;

	result = overright_poly3D_lseg3D_internal(poly, lseg);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overright_poly3D_lseg3D);

bool
overright_poly3D_lseg3D_internal(Polygon3D *poly, Lseg3D *lseg)
{
	return FPge(poly->boundbox.low.x, 
				Max(lseg->p[0].x,lseg->p[1].x));
}

/*
 * Is the Polygon3D strictly below the Lseg3D?
 */
Datum
below_poly3D_lseg3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	bool		result;

	result = below_poly3D_lseg3D_internal(poly, lseg);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(below_poly3D_lseg3D);

bool
below_poly3D_lseg3D_internal(Polygon3D *poly, Lseg3D *lseg)
{
	return FPlt(poly->boundbox.high.y, 
				Min(lseg->p[0].y,lseg->p[1].y));
}

/*
 * Is the Polygon3D below or overlaps the Lseg3D?
 */
Datum
overbelow_poly3D_lseg3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	bool		result;

	result = overbelow_poly3D_lseg3D_internal(poly, lseg);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overbelow_poly3D_lseg3D);

bool
overbelow_poly3D_lseg3D_internal(Polygon3D *poly, Lseg3D *lseg)
{
	return FPle(poly->boundbox.high.y,
				Min(lseg->p[0].y,lseg->p[1].y));
}

/*
 * Is the Polygon3D strictly above the Lseg3D?
 */
Datum
above_poly3D_lseg3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	bool		result;

	result = above_poly3D_lseg3D_internal(poly, lseg);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(above_poly3D_lseg3D);

bool
above_poly3D_lseg3D_internal(Polygon3D *poly, Lseg3D *lseg)
{
	return FPgt(poly->boundbox.low.y, 
				Max(lseg->p[0].y,lseg->p[1].y));
}

/*
 * Is the Polygon3D above or overlaps the Lseg3D?
 */
Datum
overabove_poly3D_lseg3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	bool		result;

	result = overabove_poly3D_lseg3D_internal(poly, lseg);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overabove_poly3D_lseg3D);

bool
overabove_poly3D_lseg3D_internal(Polygon3D *poly, Lseg3D *lseg)
{
	return FPge(poly->boundbox.low.y, 
				Max(lseg->p[0].y,lseg->p[1].y));
}

/*
 * Is the Polygon3D strictly in front of the Lseg3D?
 */
Datum
front_poly3D_lseg3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	bool		result;

	result = front_poly3D_lseg3D_internal(poly, lseg);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(front_poly3D_lseg3D);

bool
front_poly3D_lseg3D_internal(Polygon3D *poly, Lseg3D *lseg)
{
	return FPlt(poly->boundbox.high.z,
				Min(lseg->p[0].z,lseg->p[1].z));
}

/*
 * Is the Polygon3D in front of or overlaps the Lseg3D?
 */
Datum
overfront_poly3D_lseg3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	bool		result;

	result = overfront_poly3D_lseg3D_internal(poly, lseg);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overfront_poly3D_lseg3D);

bool
overfront_poly3D_lseg3D_internal(Polygon3D *poly, Lseg3D *lseg)
{
	return FPle(poly->boundbox.high.z,
				Min(lseg->p[0].z,lseg->p[1].z));
}

/*
 * Is the Polygon3D strictly at the back of the Lseg3D?
 */
Datum
back_poly3D_lseg3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	bool		result;

	result = back_poly3D_lseg3D_internal(poly, lseg);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(back_poly3D_lseg3D);

bool
back_poly3D_lseg3D_internal(Polygon3D *poly, Lseg3D *lseg)
{
	return FPgt(poly->boundbox.low.z, 
				Max(lseg->p[0].z,lseg->p[1].z));
}

/*
 * Is the Polygon3D at the back of or overlaps the Lseg3D?
 */
Datum
overback_poly3D_lseg3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	bool		result;

	result = overback_poly3D_lseg3D_internal(poly, lseg);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overback_poly3D_lseg3D);

bool
overback_poly3D_lseg3D_internal(Polygon3D *poly, Lseg3D *lseg)
{
	return FPge(poly->boundbox.low.z, 
				Max(lseg->p[0].z,lseg->p[1].z));
}

/*----------------------------------------------------------*/

/* polygon3D OPERATOR line3D */

/*
 * Is the Polygon3D strictly to the left of the Line3D?
 */
Datum
left_poly3D_line3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	bool		result;

	result = left_poly3D_line3D_internal(poly, line);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(left_poly3D_line3D);

bool
left_poly3D_line3D_internal(Polygon3D *poly, Line3D *line)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPlt(poly->boundbox.high.x, line->p[0].x);
}

/*
 * Is the Polygon3D to the left of or overlaps the Line3D?
 */
Datum
overleft_poly3D_line3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	bool		result;

	result = overleft_poly3D_line3D_internal(poly, line);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overleft_poly3D_line3D);

bool
overleft_poly3D_line3D_internal(Polygon3D *poly, Line3D *line)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPle(poly->boundbox.high.x, line->p[0].x);
}

/*
 * Is the Polygon3D strictly to the right of the Line3D?
 */
Datum
right_poly3D_line3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	bool		result;

	result = right_poly3D_line3D_internal(poly, line);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(right_poly3D_line3D);

bool
right_poly3D_line3D_internal(Polygon3D *poly, Line3D *line)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPgt(poly->boundbox.low.x, line->p[0].x);
}

/*
 * Is the Polygon3D to the right of or overlaps the Line3D?
 */
Datum
overright_poly3D_line3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	bool		result;

	result = overright_poly3D_line3D_internal(poly, line);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overright_poly3D_line3D);

bool
overright_poly3D_line3D_internal(Polygon3D *poly, Line3D *line)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPge(poly->boundbox.low.x, line->p[0].x);
}

/*
 * Is the Polygon3D strictly below the Line3D?
 */
Datum
below_poly3D_line3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	bool		result;

	result = below_poly3D_line3D_internal(poly, line);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(below_poly3D_line3D);

bool
below_poly3D_line3D_internal(Polygon3D *poly, Line3D *line)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPlt(poly->boundbox.high.y, line->p[0].y);
}

/*
 * Is the Polygon3D below or overlaps the Line3D?
 */
Datum
overbelow_poly3D_line3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	bool		result;

	result = overbelow_poly3D_line3D_internal(poly, line);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overbelow_poly3D_line3D);

bool
overbelow_poly3D_line3D_internal(Polygon3D *poly, Line3D *line)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPle(poly->boundbox.high.y, line->p[0].y);
}

/*
 * Is the Polygon3D strictly above the Line3D?
 */
Datum
above_poly3D_line3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	bool		result;

	result = above_poly3D_line3D_internal(poly, line);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(above_poly3D_line3D);

bool
above_poly3D_line3D_internal(Polygon3D *poly, Line3D *line)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPgt(poly->boundbox.low.y, line->p[0].y);
}

/*
 * Is the Polygon3D above or overlaps the Line3D?
 */
Datum
overabove_poly3D_line3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	bool		result;

	result = overabove_poly3D_line3D_internal(poly, line);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overabove_poly3D_line3D);

bool
overabove_poly3D_line3D_internal(Polygon3D *poly, Line3D *line)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPge(poly->boundbox.low.y, line->p[0].y);
}

/*
 * Is the Polygon3D strictly in front of the Line3D?
 */
Datum
front_poly3D_line3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	bool		result;

	result = front_poly3D_line3D_internal(poly, line);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(front_poly3D_line3D);

bool
front_poly3D_line3D_internal(Polygon3D *poly, Line3D *line)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPlt(poly->boundbox.high.z, line->p[0].z);
}

/*
 * Is the Polygon3D in front of or overlaps the Line3D?
 */
Datum
overfront_poly3D_line3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	bool		result;

	result = overfront_poly3D_line3D_internal(poly, line);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overfront_poly3D_line3D);

bool
overfront_poly3D_line3D_internal(Polygon3D *poly, Line3D *line)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPle(poly->boundbox.high.z, line->p[0].z);
}

/*
 * Is the Polygon3D strictly at the back of the Line3D?
 */
Datum
back_poly3D_line3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	bool		result;

	result = back_poly3D_line3D_internal(poly, line);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(back_poly3D_line3D);

bool
back_poly3D_line3D_internal(Polygon3D *poly, Line3D *line)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPgt(poly->boundbox.low.z, line->p[0].z);
}

/*
 * Is the Polygon3D at the back of or overlaps the Line3D?
 */
Datum
overback_poly3D_line3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	bool		result;

	result = overback_poly3D_line3D_internal(poly, line);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overback_poly3D_line3D);

bool
overback_poly3D_line3D_internal(Polygon3D *poly, Line3D *line)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPge(poly->boundbox.low.z, line->p[0].z);
}

/*----------------------------------------------------------*/

/* polygon3D OPERATOR box3D */

/*
 * Is the Polygon3D strictly to the left of the Box3D?
 */
Datum
left_poly3D_box3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	bool		result;

	result = left_poly3D_box3D_internal(poly, box);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(left_poly3D_box3D);

bool
left_poly3D_box3D_internal(Polygon3D *poly, Box3D *box)
{
	return FPlt(poly->boundbox.high.x, box->low.x);
}

/*
 * Is the Polygon3D to the left of or overlaps the Box3D?
 */
Datum
overleft_poly3D_box3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	bool		result;

	result = overleft_poly3D_box3D_internal(poly, box);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overleft_poly3D_box3D);

bool
overleft_poly3D_box3D_internal(Polygon3D *poly, Box3D *box)
{
	return FPle(poly->boundbox.high.x, box->high.x);
}

/*
 * Is the Polygon3D strictly to the right of the Box3D?
 */
Datum
right_poly3D_box3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	bool		result;

	result = right_poly3D_box3D_internal(poly, box);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(right_poly3D_box3D);

bool
right_poly3D_box3D_internal(Polygon3D *poly, Box3D *box)
{
	return FPgt(poly->boundbox.low.x, box->high.x);
}

/*
 * Is the Polygon3D to the right of or overlaps the Box3D?
 */
Datum
overright_poly3D_box3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	bool		result;

	result = overright_poly3D_box3D_internal(poly, box);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overright_poly3D_box3D);

bool
overright_poly3D_box3D_internal(Polygon3D *poly, Box3D *box)
{
	return FPge(poly->boundbox.low.x, box->low.x);
}

/*
 * Is the Polygon3D strictly below the Box3D?
 */
Datum
below_poly3D_box3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	bool		result;

	result = below_poly3D_box3D_internal(poly, box);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(below_poly3D_box3D);

bool
below_poly3D_box3D_internal(Polygon3D *poly, Box3D *box)
{
	return FPlt(poly->boundbox.high.y, box->low.y);
}

/*
 * Is the Polygon3D below or overlaps the Box3D?
 */
Datum
overbelow_poly3D_box3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	bool		result;

	result = overbelow_poly3D_box3D_internal(poly, box);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overbelow_poly3D_box3D);

bool
overbelow_poly3D_box3D_internal(Polygon3D *poly, Box3D *box)
{
	return FPle(poly->boundbox.high.y, box->high.y);
}

/*
 * Is the Polygon3D strictly above the Box3D?
 */
Datum
above_poly3D_box3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	bool		result;

	result = above_poly3D_box3D_internal(poly, box);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(above_poly3D_box3D);

bool
above_poly3D_box3D_internal(Polygon3D *poly, Box3D *box)
{
	return FPgt(poly->boundbox.low.y, box->high.y);
}

/*
 * Is the Polygon3D above or overlaps the Box3D?
 */
Datum
overabove_poly3D_box3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	bool		result;

	result = overabove_poly3D_box3D_internal(poly, box);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overabove_poly3D_box3D);

bool
overabove_poly3D_box3D_internal(Polygon3D *poly, Box3D *box)
{
	return FPge(poly->boundbox.low.y, box->low.y);
}

/*
 * Is the Polygon3D strictly in front of the Box3D?
 */
Datum
front_poly3D_box3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	bool		result;

	result = front_poly3D_box3D_internal(poly, box);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(front_poly3D_box3D);

bool
front_poly3D_box3D_internal(Polygon3D *poly, Box3D *box)
{
	return FPlt(poly->boundbox.high.z, box->low.z);
}

/*
 * Is the Polygon3D in front of or overlaps the Box3D?
 */
Datum
overfront_poly3D_box3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	bool		result;

	result = overfront_poly3D_box3D_internal(poly, box);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overfront_poly3D_box3D);

bool
overfront_poly3D_box3D_internal(Polygon3D *poly, Box3D *box)
{
	return FPle(poly->boundbox.high.z, box->high.z);
}

/*
 * Is the Polygon3D strictly at the back of the Box3D?
 */
Datum
back_poly3D_box3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	bool		result;

	result = back_poly3D_box3D_internal(poly, box);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(back_poly3D_box3D);

bool
back_poly3D_box3D_internal(Polygon3D *poly, Box3D *box)
{
	return FPgt(poly->boundbox.low.z, box->high.z);
}

/*
 * Is the Polygon3D at the back of or overlaps the Box3D?
 */
Datum
overback_poly3D_box3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);
	bool		result;

	result = overback_poly3D_box3D_internal(poly, box);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overback_poly3D_box3D);

bool
overback_poly3D_box3D_internal(Polygon3D *poly, Box3D *box)
{
	return FPge(poly->boundbox.low.z, box->low.z);
}

/*----------------------------------------------------------*/

/* polygon3D OPERATOR path3D */

/*
 * Is the Polygon3D strictly to the left of the Path3D?
 */
Datum
left_poly3D_path3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Path3D     *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = left_poly3D_path3D_internal(poly, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(left_poly3D_path3D);

bool
left_poly3D_path3D_internal(Polygon3D *poly, Path3D *path)
{
	return FPlt(poly->boundbox.high.x, path->boundbox.low.x);
}

/*
 * Is the Polygon3D to the left of or overlaps the Path3D?
 */
Datum
overleft_poly3D_path3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Path3D     *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overleft_poly3D_path3D_internal(poly, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overleft_poly3D_path3D);

bool
overleft_poly3D_path3D_internal(Polygon3D *poly, Path3D *path)
{
	return FPle(poly->boundbox.high.x, path->boundbox.high.x);
}

/*
 * Is the Polygon3D strictly to the right of the Path3D?
 */
Datum
right_poly3D_path3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Path3D     *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = right_poly3D_path3D_internal(poly, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(right_poly3D_path3D);

bool
right_poly3D_path3D_internal(Polygon3D *poly, Path3D *path)
{
	return FPgt(poly->boundbox.low.x, path->boundbox.high.x);
}

/*
 * Is the Polygon3D to the right of or overlaps the Path3D?
 */
Datum
overright_poly3D_path3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Path3D     *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overright_poly3D_path3D_internal(poly, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overright_poly3D_path3D);

bool
overright_poly3D_path3D_internal(Polygon3D *poly, Path3D *path)
{
	return FPge(poly->boundbox.low.x, path->boundbox.low.x);
}

/*
 * Is the Polygon3D strictly below the Path3D?
 */
Datum
below_poly3D_path3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Path3D     *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = below_poly3D_path3D_internal(poly, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(below_poly3D_path3D);

bool
below_poly3D_path3D_internal(Polygon3D *poly, Path3D *path)
{
	return FPlt(poly->boundbox.high.y, path->boundbox.low.y);
}

/*
 * Is the Polygon3D below or overlaps the Path3D?
 */
Datum
overbelow_poly3D_path3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Path3D     *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overbelow_poly3D_path3D_internal(poly, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overbelow_poly3D_path3D);

bool
overbelow_poly3D_path3D_internal(Polygon3D *poly, Path3D *path)
{
	return FPle(poly->boundbox.high.y, path->boundbox.high.y);
}

/*
 * Is the Polygon3D strictly above the Path3D?
 */
Datum
above_poly3D_path3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Path3D     *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = above_poly3D_path3D_internal(poly, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(above_poly3D_path3D);

bool
above_poly3D_path3D_internal(Polygon3D *poly, Path3D *path)
{
	return FPgt(poly->boundbox.low.y, path->boundbox.high.y);
}

/*
 * Is the Polygon3D above or overlaps the Path3D?
 */
Datum
overabove_poly3D_path3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Path3D     *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overabove_poly3D_path3D_internal(poly, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overabove_poly3D_path3D);

bool
overabove_poly3D_path3D_internal(Polygon3D *poly, Path3D *path)
{
	return FPge(poly->boundbox.low.y, path->boundbox.low.y);
}

/*
 * Is the Polygon3D strictly in front of the Path3D?
 */
Datum
front_poly3D_path3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Path3D     *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = front_poly3D_path3D_internal(poly, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(front_poly3D_path3D);

bool
front_poly3D_path3D_internal(Polygon3D *poly, Path3D *path)
{
	return FPlt(poly->boundbox.high.z, path->boundbox.low.z);
}

/*
 * Is the Polygon3D in front of or overlaps the Path3D?
 */
Datum
overfront_poly3D_path3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Path3D     *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overfront_poly3D_path3D_internal(poly, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overfront_poly3D_path3D);

bool
overfront_poly3D_path3D_internal(Polygon3D *poly, Path3D *path)
{
	return FPle(poly->boundbox.high.z, path->boundbox.high.z);
}

/*
 * Is the Polygon3D strictly at the back of the Path3D?
 */
Datum
back_poly3D_path3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Path3D     *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = back_poly3D_path3D_internal(poly, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(back_poly3D_path3D);

bool
back_poly3D_path3D_internal(Polygon3D *poly, Path3D *path)
{
	return FPgt(poly->boundbox.low.z, path->boundbox.high.z);
}

/*
 * Is the Polygon3D at the back of or overlaps the Path3D?
 */
Datum
overback_poly3D_path3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Path3D     *path = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = overback_poly3D_path3D_internal(poly, path);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);
	PG_FREE_IF_COPY(path, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overback_poly3D_path3D);

bool
overback_poly3D_path3D_internal(Polygon3D *poly, Path3D *path)
{
	return FPge(poly->boundbox.low.z, path->boundbox.low.z);
}

/*----------------------------------------------------------*/

/* polygon3D OPERATOR polygon3D */

/*
 * Is the first Polygon3D strictly to the left of the second Polygon3D?
 */
Datum
left_poly3D_poly3D(PG_FUNCTION_ARGS)
{
	Polygon3D    *poly1 = PG_GETARG_POLYGON3D_P(0);
	Polygon3D    *poly2 = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = left_poly3D_poly3D_internal(poly1, poly2);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly1, 0);
	PG_FREE_IF_COPY(poly2, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(left_poly3D_poly3D);

bool
left_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2)
{
	return FPlt(poly1->boundbox.high.x, poly2->boundbox.low.x);
}

/*
 * Is the first Polygon3D to the left of or overlaps the second Polygon3D?
 */
Datum
overleft_poly3D_poly3D(PG_FUNCTION_ARGS)
{
	Polygon3D    *poly1 = PG_GETARG_POLYGON3D_P(0);
	Polygon3D    *poly2 = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overleft_poly3D_poly3D_internal(poly1, poly2);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly1, 0);
	PG_FREE_IF_COPY(poly2, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overleft_poly3D_poly3D);

bool
overleft_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2)
{
	return FPle(poly1->boundbox.high.x, poly2->boundbox.high.x);
}

/*
 * Is the first Polygon3D strictly to the right of the second Polygon3D?
 */
Datum
right_poly3D_poly3D(PG_FUNCTION_ARGS)
{
	Polygon3D    *poly1 = PG_GETARG_POLYGON3D_P(0);
	Polygon3D    *poly2 = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = right_poly3D_poly3D_internal(poly1, poly2);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly1, 0);
	PG_FREE_IF_COPY(poly2, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(right_poly3D_poly3D);

bool
right_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2)
{
	return FPgt(poly1->boundbox.low.x, poly2->boundbox.high.x);
}

/*
 * Is the first Polygon3D to the right of or overlaps the second Polygon3D?
 */
Datum
overright_poly3D_poly3D(PG_FUNCTION_ARGS)
{
	Polygon3D    *poly1 = PG_GETARG_POLYGON3D_P(0);
	Polygon3D    *poly2 = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overright_poly3D_poly3D_internal(poly1, poly2);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly1, 0);
	PG_FREE_IF_COPY(poly2, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overright_poly3D_poly3D);

bool
overright_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2)
{
	return FPge(poly1->boundbox.low.x, poly2->boundbox.low.x);
}

/*
 * Is the first Polygon3D strictly below the second Polygon3D?
 */
Datum
below_poly3D_poly3D(PG_FUNCTION_ARGS)
{
	Polygon3D    *poly1 = PG_GETARG_POLYGON3D_P(0);
	Polygon3D    *poly2 = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = below_poly3D_poly3D_internal(poly1, poly2);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly1, 0);
	PG_FREE_IF_COPY(poly2, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(below_poly3D_poly3D);

bool
below_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2)
{
	return FPlt(poly1->boundbox.high.y, poly2->boundbox.low.y);
}

/*
 * Is the first Polygon3D below or overlaps the second Polygon3D?
 */
Datum
overbelow_poly3D_poly3D(PG_FUNCTION_ARGS)
{
	Polygon3D    *poly1 = PG_GETARG_POLYGON3D_P(0);
	Polygon3D    *poly2 = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overbelow_poly3D_poly3D_internal(poly1, poly2);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly1, 0);
	PG_FREE_IF_COPY(poly2, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overbelow_poly3D_poly3D);

bool
overbelow_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2)
{
	return FPle(poly1->boundbox.high.y, poly2->boundbox.high.y);
}

/*
 * Is the first Polygon3D strictly above the second Polygon3D?
 */
Datum
above_poly3D_poly3D(PG_FUNCTION_ARGS)
{
	Polygon3D    *poly1 = PG_GETARG_POLYGON3D_P(0);
	Polygon3D    *poly2 = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = above_poly3D_poly3D_internal(poly1, poly2);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly1, 0);
	PG_FREE_IF_COPY(poly2, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(above_poly3D_poly3D);

bool
above_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2)
{
	return FPgt(poly1->boundbox.low.y, poly2->boundbox.high.y);
}

/*
 * Is the first Polygon3D above or overlaps the second Polygon3D?
 */
Datum
overabove_poly3D_poly3D(PG_FUNCTION_ARGS)
{
	Polygon3D    *poly1 = PG_GETARG_POLYGON3D_P(0);
	Polygon3D    *poly2 = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overabove_poly3D_poly3D_internal(poly1, poly2);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly1, 0);
	PG_FREE_IF_COPY(poly2, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overabove_poly3D_poly3D);

bool
overabove_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2)
{
	return FPge(poly1->boundbox.low.y, poly2->boundbox.low.y);
}

/*
 * Is the first Polygon3D strictly in front of the second Polygon3D?
 */
Datum
front_poly3D_poly3D(PG_FUNCTION_ARGS)
{
	Polygon3D    *poly1 = PG_GETARG_POLYGON3D_P(0);
	Polygon3D    *poly2 = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = front_poly3D_poly3D_internal(poly1, poly2);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly1, 0);
	PG_FREE_IF_COPY(poly2, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(front_poly3D_poly3D);

bool
front_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2)
{
	return FPlt(poly1->boundbox.high.z, poly2->boundbox.low.z);
}

/*
 * Is the first Polygon3D in front of or overlaps the second Polygon3D?
 */
Datum
overfront_poly3D_poly3D(PG_FUNCTION_ARGS)
{
	Polygon3D    *poly1 = PG_GETARG_POLYGON3D_P(0);
	Polygon3D    *poly2 = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overfront_poly3D_poly3D_internal(poly1, poly2);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly1, 0);
	PG_FREE_IF_COPY(poly2, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overfront_poly3D_poly3D);

bool
overfront_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2)
{
	return FPle(poly1->boundbox.high.z, poly2->boundbox.high.z);
}

/*
 * Is the first Polygon3D strictly at the back of the second Polygon3D?
 */
Datum
back_poly3D_poly3D(PG_FUNCTION_ARGS)
{
	Polygon3D    *poly1 = PG_GETARG_POLYGON3D_P(0);
	Polygon3D    *poly2 = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = back_poly3D_poly3D_internal(poly1, poly2);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly1, 0);
	PG_FREE_IF_COPY(poly2, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(back_poly3D_poly3D);

bool
back_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2)
{
	return FPgt(poly1->boundbox.low.z, poly2->boundbox.high.z);
}

/*
 * Is the first Polygon3D at the back of or overlaps the second Polygon3D?
 */
Datum
overback_poly3D_poly3D(PG_FUNCTION_ARGS)
{
	Polygon3D    *poly1 = PG_GETARG_POLYGON3D_P(0);
	Polygon3D    *poly2 = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = overback_poly3D_poly3D_internal(poly1, poly2);

	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly1, 0);
	PG_FREE_IF_COPY(poly2, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overback_poly3D_poly3D);

bool
overback_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2)
{
	return FPge(poly1->boundbox.low.z, poly2->boundbox.low.z);
}

/*----------------------------------------------------------*/

/* polygon3D OPERATOR sphere */

/*
 * Is the Polygon3D strictly to the left of the Sphere?
 */
Datum
left_poly3D_sphere(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	bool		result;

	result = left_poly3D_sphere_internal(poly, sphere);
	
	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(left_poly3D_sphere);

bool
left_poly3D_sphere_internal(Polygon3D *poly, Sphere *sphere)
{
	return FPlt(poly->boundbox.high.x, (sphere->center.x - sphere->radius));
}

/*
 * Is the Polygon3D to the left of or overlaps the Sphere?
 */
Datum
overleft_poly3D_sphere(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	bool		result;

	result = overleft_poly3D_sphere_internal(poly, sphere);
	
	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overleft_poly3D_sphere);

bool
overleft_poly3D_sphere_internal(Polygon3D *poly, Sphere *sphere)
{
	return FPle(poly->boundbox.low.x, (sphere->center.x + sphere->radius));
}

/*
 * Is the Polygon3D strictly to the right of the Sphere?
 */
Datum
right_poly3D_sphere(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	bool		result;

	result = right_poly3D_sphere_internal(poly, sphere);
	
	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(right_poly3D_sphere);

bool
right_poly3D_sphere_internal(Polygon3D *poly, Sphere *sphere)
{
	return FPgt(poly->boundbox.low.x, (sphere->center.x + sphere->radius));
}

/*
 * Is the Polygon3D to the right of or overlaps the Sphere?
 */
Datum
overright_poly3D_sphere(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	bool		result;

	result = overright_poly3D_sphere_internal(poly, sphere);
	
	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overright_poly3D_sphere);

bool
overright_poly3D_sphere_internal(Polygon3D *poly, Sphere *sphere)
{
	return FPge(poly->boundbox.high.x, (sphere->center.x - sphere->radius));
}

/*
 * Is the Polygon3D strictly below the Sphere?
 */
Datum
below_poly3D_sphere(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	bool		result;

	result = below_poly3D_sphere_internal(poly, sphere);
	
	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(below_poly3D_sphere);

bool
below_poly3D_sphere_internal(Polygon3D *poly, Sphere *sphere)
{
	return FPlt(poly->boundbox.high.y, (sphere->center.y - sphere->radius));
}

/*
 * Is the Polygon3D below or overlaps the Sphere?
 */
Datum
overbelow_poly3D_sphere(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	bool		result;

	result = overbelow_poly3D_sphere_internal(poly, sphere);
	
	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overbelow_poly3D_sphere);

bool
overbelow_poly3D_sphere_internal(Polygon3D *poly, Sphere *sphere)
{
	return FPle(poly->boundbox.low.y, (sphere->center.y + sphere->radius));
}

/*
 * Is the Polygon3D strictly above the Sphere?
 */
Datum
above_poly3D_sphere(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	bool		result;

	result = above_poly3D_sphere_internal(poly, sphere);
	
	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(above_poly3D_sphere);

bool
above_poly3D_sphere_internal(Polygon3D *poly, Sphere *sphere)
{
	return FPgt(poly->boundbox.low.y, (sphere->center.y + sphere->radius));
}

/*
 * Is the Polygon3D above or overlaps the Sphere?
 */
Datum
overabove_poly3D_sphere(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	bool		result;

	result = overabove_poly3D_sphere_internal(poly, sphere);
	
	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overabove_poly3D_sphere);

bool
overabove_poly3D_sphere_internal(Polygon3D *poly, Sphere *sphere)
{
	return FPge(poly->boundbox.high.y, (sphere->center.y - sphere->radius));
}

/*
 * Is the Polygon3D strictly in front of the Sphere?
 */
Datum
front_poly3D_sphere(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	bool		result;

	result = front_poly3D_sphere_internal(poly, sphere);
	
	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(front_poly3D_sphere);

bool
front_poly3D_sphere_internal(Polygon3D *poly, Sphere *sphere)
{
	return FPlt(poly->boundbox.high.z, (sphere->center.z - sphere->radius));
}

/*
 * Is the Polygon3D in front of or overlaps the Sphere?
 */
Datum
overfront_poly3D_sphere(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	bool		result;

	result = overfront_poly3D_sphere_internal(poly, sphere);
	
	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overfront_poly3D_sphere);

bool
overfront_poly3D_sphere_internal(Polygon3D *poly, Sphere *sphere)
{
	return FPle(poly->boundbox.low.z, (sphere->center.z + sphere->radius));
}

/*
 * Is the Polygon3D strictly at the back of the Sphere?
 */
Datum
back_poly3D_sphere(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	bool		result;

	result = back_poly3D_sphere_internal(poly, sphere);
	
	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(back_poly3D_sphere);

bool
back_poly3D_sphere_internal(Polygon3D *poly, Sphere *sphere)
{
	return FPgt(poly->boundbox.low.z, (sphere->center.z + sphere->radius));
}

/*
 * Is the Polygon3D at the back of or overlaps the Sphere?
 */
Datum
overback_poly3D_sphere(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Sphere	   *sphere = PG_GETARG_SPHERE_P(1);
	bool		result;

	result = overback_poly3D_sphere_internal(poly, sphere);
	
	/*
	 * Avoid leaking memory for toasted inputs ... needed for rtree indexes
	 */
	PG_FREE_IF_COPY(poly, 0);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(overback_poly3D_sphere);

bool
overback_poly3D_sphere_internal(Polygon3D *poly, Sphere *sphere)
{
	return FPge(poly->boundbox.high.z, (sphere->center.z - sphere->radius));
}

/*****************************************************************************/

/* sphere OPERATOR point3D */

/*
 * Is the Sphere strictly to the left of the Point3D?
 */
Datum
left_sphere_point3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(left_sphere_point3D_internal(sphere, point));
}

PG_FUNCTION_INFO_V1(left_sphere_point3D);

bool
left_sphere_point3D_internal(Sphere *sphere, Point3D *point)
{
	return FPlt((sphere->center.x + sphere->radius), point->x);
}

/*
 * Is the Sphere to the left of or overlaps the Point3D?
 */
Datum
overleft_sphere_point3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overleft_sphere_point3D_internal(sphere, point));
}

PG_FUNCTION_INFO_V1(overleft_sphere_point3D);

bool
overleft_sphere_point3D_internal(Sphere *sphere, Point3D *point)
{
	return FPle((sphere->center.x + sphere->radius), point->x);
}

/*
 * Is the Sphere strictly to the right of the Point3D?
 */
Datum
right_sphere_point3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(right_sphere_point3D_internal(sphere, point));
}

PG_FUNCTION_INFO_V1(right_sphere_point3D);

bool
right_sphere_point3D_internal(Sphere *sphere, Point3D *point)
{
	return FPgt((sphere->center.x - sphere->radius), point->x);
}

/*
 * Is the Sphere to the right of or overlaps the Point3D?
 */
Datum
overright_sphere_point3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overright_sphere_point3D_internal(sphere, point));
}

PG_FUNCTION_INFO_V1(overright_sphere_point3D);

bool
overright_sphere_point3D_internal(Sphere *sphere, Point3D *point)
{
	return FPge((sphere->center.x - sphere->radius), point->x);
}

/*
 * Is the Sphere strictly below the Point3D?
 */
Datum
below_sphere_point3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(below_sphere_point3D_internal(sphere, point));
}

PG_FUNCTION_INFO_V1(below_sphere_point3D);

bool
below_sphere_point3D_internal(Sphere *sphere, Point3D *point)
{
	return FPlt((sphere->center.y + sphere->radius), point->y);
}

/*
 * Is the Sphere below or overlaps the Point3D?
 */
Datum
overbelow_sphere_point3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overbelow_sphere_point3D_internal(sphere, point));
}

PG_FUNCTION_INFO_V1(overbelow_sphere_point3D);

bool
overbelow_sphere_point3D_internal(Sphere *sphere, Point3D *point)
{
	return FPle((sphere->center.y + sphere->radius), point->y);
}

/*
 * Is the Sphere strictly above the Point3D?
 */
Datum
above_sphere_point3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(above_sphere_point3D_internal(sphere, point));
}

PG_FUNCTION_INFO_V1(above_sphere_point3D);

bool
above_sphere_point3D_internal(Sphere *sphere, Point3D *point)
{
	return FPgt((sphere->center.y - sphere->radius), point->y);
}

/*
 * Is the Sphere above or overlaps the Point3D?
 */
Datum
overabove_sphere_point3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overabove_sphere_point3D_internal(sphere, point));
}

PG_FUNCTION_INFO_V1(overabove_sphere_point3D);

bool
overabove_sphere_point3D_internal(Sphere *sphere, Point3D *point)
{
	return FPge((sphere->center.y - sphere->radius), point->y);
}

/*
 * Is the Sphere strictly in front of the Point3D?
 */
Datum
front_sphere_point3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(front_sphere_point3D_internal(sphere, point));
}

PG_FUNCTION_INFO_V1(front_sphere_point3D);

bool
front_sphere_point3D_internal(Sphere *sphere, Point3D *point)
{
	return FPlt((sphere->center.z + sphere->radius), point->z);
}

/*
 * Is the Sphere in front of or overlaps the Point3D?
 */
Datum
overfront_sphere_point3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overfront_sphere_point3D_internal(sphere, point));
}

PG_FUNCTION_INFO_V1(overfront_sphere_point3D);

bool
overfront_sphere_point3D_internal(Sphere *sphere, Point3D *point)
{
	return FPle((sphere->center.z + sphere->radius), point->z);
}

/*
 * Is the Sphere strictly at the back of the Point3D?
 */
Datum
back_sphere_point3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(back_sphere_point3D_internal(sphere, point));
}

PG_FUNCTION_INFO_V1(back_sphere_point3D);

bool
back_sphere_point3D_internal(Sphere *sphere, Point3D *point)
{
	return FPgt((sphere->center.z - sphere->radius), point->z);
}

/*
 * Is the Sphere at the back of or overlaps the Point3D?
 */
Datum
overback_sphere_point3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(overback_sphere_point3D_internal(sphere, point));
}

PG_FUNCTION_INFO_V1(overback_sphere_point3D);

bool
overback_sphere_point3D_internal(Sphere *sphere, Point3D *point)
{
	return FPge((sphere->center.z - sphere->radius), point->z);
}

/*----------------------------------------------------------*/

/* sphere OPERATOR lseg3D */

/*
 * Is the Sphere strictly to the left of the Lseg3D?
 */
Datum
left_sphere_lseg3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(left_sphere_lseg3D_internal(sphere, lseg));
}

PG_FUNCTION_INFO_V1(left_sphere_lseg3D);

bool
left_sphere_lseg3D_internal(Sphere *sphere, Lseg3D *lseg)
{
	return FPlt((sphere->center.x + sphere->radius),
				Min(lseg->p[0].x,lseg->p[1].x));
}

/*
 * Is the Sphere to the left of or overlaps the Lseg3D?
 */
Datum
overleft_sphere_lseg3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overleft_sphere_lseg3D_internal(sphere, lseg));
}

PG_FUNCTION_INFO_V1(overleft_sphere_lseg3D);

bool
overleft_sphere_lseg3D_internal(Sphere *sphere, Lseg3D *lseg)
{
	return FPle((sphere->center.x + sphere->radius), 
				Min(lseg->p[0].x,lseg->p[1].x));
}

/*
 * Is the Sphere strictly to the right of the Lseg3D?
 */
Datum
right_sphere_lseg3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(right_sphere_lseg3D_internal(sphere, lseg));
}

PG_FUNCTION_INFO_V1(right_sphere_lseg3D);

bool
right_sphere_lseg3D_internal(Sphere *sphere, Lseg3D *lseg)
{
	return FPgt((sphere->center.x - sphere->radius), 
				Max(lseg->p[0].x,lseg->p[1].x));
}

/*
 * Is the Sphere to the right of or overlaps the Lseg3D?
 */
Datum
overright_sphere_lseg3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overright_sphere_lseg3D_internal(sphere, lseg));
}

PG_FUNCTION_INFO_V1(overright_sphere_lseg3D);

bool
overright_sphere_lseg3D_internal(Sphere *sphere, Lseg3D *lseg)
{
	return FPge((sphere->center.x - sphere->radius), 
				Max(lseg->p[0].x,lseg->p[1].x));
}

/*
 * Is the Sphere strictly below the Lseg3D?
 */
Datum
below_sphere_lseg3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(below_sphere_lseg3D_internal(sphere, lseg));
}

PG_FUNCTION_INFO_V1(below_sphere_lseg3D);

bool
below_sphere_lseg3D_internal(Sphere *sphere, Lseg3D *lseg)
{
	return FPlt((sphere->center.y + sphere->radius), 
				Min(lseg->p[0].y,lseg->p[1].y));
}

/*
 * Is the Sphere below or overlaps the Lseg3D?
 */
Datum
overbelow_sphere_lseg3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overbelow_sphere_lseg3D_internal(sphere, lseg));
}

PG_FUNCTION_INFO_V1(overbelow_sphere_lseg3D);

bool
overbelow_sphere_lseg3D_internal(Sphere *sphere, Lseg3D *lseg)
{
	return FPle((sphere->center.y + sphere->radius),
				Min(lseg->p[0].y,lseg->p[1].y));
}

/*
 * Is the Sphere strictly above the Lseg3D?
 */
Datum
above_sphere_lseg3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(above_sphere_lseg3D_internal(sphere, lseg));
}

PG_FUNCTION_INFO_V1(above_sphere_lseg3D);

bool
above_sphere_lseg3D_internal(Sphere *sphere, Lseg3D *lseg)
{
	return FPgt((sphere->center.y - sphere->radius), 
				Max(lseg->p[0].y,lseg->p[1].y));
}

/*
 * Is the Sphere above or overlaps the Lseg3D?
 */
Datum
overabove_sphere_lseg3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overabove_sphere_lseg3D_internal(sphere, lseg));
}

PG_FUNCTION_INFO_V1(overabove_sphere_lseg3D);

bool
overabove_sphere_lseg3D_internal(Sphere *sphere, Lseg3D *lseg)
{
	return FPge((sphere->center.y - sphere->radius), 
				Max(lseg->p[0].y,lseg->p[1].y));
}

/*
 * Is the Sphere strictly in front of the Lseg3D?
 */
Datum
front_sphere_lseg3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(front_sphere_lseg3D_internal(sphere, lseg));
}

PG_FUNCTION_INFO_V1(front_sphere_lseg3D);

bool
front_sphere_lseg3D_internal(Sphere *sphere, Lseg3D *lseg)
{
	return FPlt((sphere->center.z + sphere->radius),
				Min(lseg->p[0].z,lseg->p[1].z));
}

/*
 * Is the Sphere in front of or overlaps the Lseg3D?
 */
Datum
overfront_sphere_lseg3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overfront_sphere_lseg3D_internal(sphere, lseg));
}

PG_FUNCTION_INFO_V1(overfront_sphere_lseg3D);

bool
overfront_sphere_lseg3D_internal(Sphere *sphere, Lseg3D *lseg)
{
	return FPle((sphere->center.z + sphere->radius),
				Min(lseg->p[0].z,lseg->p[1].z));
}

/*
 * Is the Sphere strictly at the back of the Lseg3D?
 */
Datum
back_sphere_lseg3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(back_sphere_lseg3D_internal(sphere, lseg));
}

PG_FUNCTION_INFO_V1(back_sphere_lseg3D);

bool
back_sphere_lseg3D_internal(Sphere *sphere, Lseg3D *lseg)
{
	return FPgt((sphere->center.z - sphere->radius), 
				Max(lseg->p[0].z,lseg->p[1].z));
}

/*
 * Is the Sphere at the back of or overlaps the Lseg3D?
 */
Datum
overback_sphere_lseg3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(overback_sphere_lseg3D_internal(sphere, lseg));
}

PG_FUNCTION_INFO_V1(overback_sphere_lseg3D);

bool
overback_sphere_lseg3D_internal(Sphere *sphere, Lseg3D *lseg)
{
	return FPge((sphere->center.z - sphere->radius), 
				Max(lseg->p[0].z,lseg->p[1].z));
}

/*----------------------------------------------------------*/

/* sphere OPERATOR line3D */

/*
 * Is the Sphere strictly to the left of the Line3D?
 */
Datum
left_sphere_line3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(left_sphere_line3D_internal(sphere, line));
}

PG_FUNCTION_INFO_V1(left_sphere_line3D);

bool
left_sphere_line3D_internal(Sphere *sphere, Line3D *line)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPlt((sphere->center.x + sphere->radius), line->p[0].x);
}

/*
 * Is the Sphere to the left of or overlaps the Line3D?
 */
Datum
overleft_sphere_line3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overleft_sphere_line3D_internal(sphere, line));
}

PG_FUNCTION_INFO_V1(overleft_sphere_line3D);

bool
overleft_sphere_line3D_internal(Sphere *sphere, Line3D *line)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPle((sphere->center.x + sphere->radius), line->p[0].x);
}

/*
 * Is the Sphere strictly to the right of the Line3D?
 */
Datum
right_sphere_line3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(right_sphere_line3D_internal(sphere, line));
}

PG_FUNCTION_INFO_V1(right_sphere_line3D);

bool
right_sphere_line3D_internal(Sphere *sphere, Line3D *line)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPgt((sphere->center.x - sphere->radius), line->p[0].x);
}

/*
 * Is the Sphere to the right of or overlaps the Line3D?
 */
Datum
overright_sphere_line3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overright_sphere_line3D_internal(sphere, line));
}

PG_FUNCTION_INFO_V1(overright_sphere_line3D);

bool
overright_sphere_line3D_internal(Sphere *sphere, Line3D *line)
{
	if (!line3D_vert(line))
		return FALSE;
	
	return FPge((sphere->center.x - sphere->radius), line->p[0].x);
}

/*
 * Is the Sphere strictly below the Line3D?
 */
Datum
below_sphere_line3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(below_sphere_line3D_internal(sphere, line));
}

PG_FUNCTION_INFO_V1(below_sphere_line3D);

bool
below_sphere_line3D_internal(Sphere *sphere, Line3D *line)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPlt((sphere->center.y + sphere->radius), line->p[0].y);
}

/*
 * Is the Sphere below or overlaps the Line3D?
 */
Datum
overbelow_sphere_line3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overbelow_sphere_line3D_internal(sphere, line));
}

PG_FUNCTION_INFO_V1(overbelow_sphere_line3D);

bool
overbelow_sphere_line3D_internal(Sphere *sphere, Line3D *line)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPle((sphere->center.y + sphere->radius), line->p[0].y);
}

/*
 * Is the Sphere strictly above the Line3D?
 */
Datum
above_sphere_line3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(above_sphere_line3D_internal(sphere, line));
}

PG_FUNCTION_INFO_V1(above_sphere_line3D);

bool
above_sphere_line3D_internal(Sphere *sphere, Line3D *line)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPgt((sphere->center.y - sphere->radius), line->p[0].y);
}

/*
 * Is the Sphere above or overlaps the Line3D?
 */
Datum
overabove_sphere_line3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overabove_sphere_line3D_internal(sphere, line));
}

PG_FUNCTION_INFO_V1(overabove_sphere_line3D);

bool
overabove_sphere_line3D_internal(Sphere *sphere, Line3D *line)
{
	if (!line3D_hori(line))
		return FALSE;
	
	return FPge((sphere->center.y - sphere->radius), line->p[0].y);
}

/*
 * Is the Sphere strictly in front of the Line3D?
 */
Datum
front_sphere_line3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(front_sphere_line3D_internal(sphere, line));
}

PG_FUNCTION_INFO_V1(front_sphere_line3D);

bool
front_sphere_line3D_internal(Sphere *sphere, Line3D *line)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPlt((sphere->center.z + sphere->radius), line->p[0].z);
}

/*
 * Is the Sphere in front of or overlaps the Line3D?
 */
Datum
overfront_sphere_line3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overfront_sphere_line3D_internal(sphere, line));
}

PG_FUNCTION_INFO_V1(overfront_sphere_line3D);

bool
overfront_sphere_line3D_internal(Sphere *sphere, Line3D *line)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPle((sphere->center.z + sphere->radius), line->p[0].z);
}

/*
 * Is the Sphere strictly at the back of the Line3D?
 */
Datum
back_sphere_line3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(back_sphere_line3D_internal(sphere, line));
}

PG_FUNCTION_INFO_V1(back_sphere_line3D);

bool
back_sphere_line3D_internal(Sphere *sphere, Line3D *line)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPgt((sphere->center.z - sphere->radius), line->p[0].z);
}

/*
 * Is the Sphere at the back of or overlaps the Line3D?
 */
Datum
overback_sphere_line3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(overback_sphere_line3D_internal(sphere, line));
}

PG_FUNCTION_INFO_V1(overback_sphere_line3D);

bool
overback_sphere_line3D_internal(Sphere *sphere, Line3D *line)
{
	if (!line3D_perp(line))
		return FALSE;
	
	return FPge((sphere->center.z - sphere->radius), line->p[0].z);
}

/*----------------------------------------------------------*/

/* sphere OPERATOR box3D */

/*
 * Is the Sphere strictly to the left of the Box3D?
 */
Datum
left_sphere_box3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(left_sphere_box3D_internal(sphere, box));
}

PG_FUNCTION_INFO_V1(left_sphere_box3D);

bool
left_sphere_box3D_internal(Sphere *sphere, Box3D *box)
{
	return FPlt((sphere->center.x + sphere->radius), box->low.x);
}

/*
 * Is the Sphere to the left of or overlaps the Box3D?
 */
Datum
overleft_sphere_box3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overleft_sphere_box3D_internal(sphere, box));
}

PG_FUNCTION_INFO_V1(overleft_sphere_box3D);

bool
overleft_sphere_box3D_internal(Sphere *sphere, Box3D *box)
{
	return FPle((sphere->center.x + sphere->radius), box->high.x);
}

/*
 * Is the Sphere strictly to the right of the Box3D?
 */
Datum
right_sphere_box3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(right_sphere_box3D_internal(sphere, box));
}

PG_FUNCTION_INFO_V1(right_sphere_box3D);

bool
right_sphere_box3D_internal(Sphere *sphere, Box3D *box)
{
	return FPgt((sphere->center.x - sphere->radius), box->high.x);
}

/*
 * Is the Sphere to the right of or overlaps the Box3D?
 */
Datum
overright_sphere_box3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overright_sphere_box3D_internal(sphere, box));
}

PG_FUNCTION_INFO_V1(overright_sphere_box3D);

bool
overright_sphere_box3D_internal(Sphere *sphere, Box3D *box)
{
	return FPge((sphere->center.x - sphere->radius), box->low.x);
}

/*
 * Is the Sphere strictly below the Box3D?
 */
Datum
below_sphere_box3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(below_sphere_box3D_internal(sphere, box));
}

PG_FUNCTION_INFO_V1(below_sphere_box3D);

bool
below_sphere_box3D_internal(Sphere *sphere, Box3D *box)
{
	return FPlt((sphere->center.y + sphere->radius), box->low.y);
}

/*
 * Is the Sphere below or overlaps the Box3D?
 */
Datum
overbelow_sphere_box3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overbelow_sphere_box3D_internal(sphere, box));
}

PG_FUNCTION_INFO_V1(overbelow_sphere_box3D);

bool
overbelow_sphere_box3D_internal(Sphere *sphere, Box3D *box)
{
	return FPle((sphere->center.y + sphere->radius), box->high.y);
}

/*
 * Is the Sphere strictly above the Box3D?
 */
Datum
above_sphere_box3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(above_sphere_box3D_internal(sphere, box));
}

PG_FUNCTION_INFO_V1(above_sphere_box3D);

bool
above_sphere_box3D_internal(Sphere *sphere, Box3D *box)
{
	return FPgt((sphere->center.y - sphere->radius), box->high.y);
}

/*
 * Is the Sphere above or overlaps the Box3D?
 */
Datum
overabove_sphere_box3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overabove_sphere_box3D_internal(sphere, box));
}

PG_FUNCTION_INFO_V1(overabove_sphere_box3D);

bool
overabove_sphere_box3D_internal(Sphere *sphere, Box3D *box)
{
	return FPge((sphere->center.y - sphere->radius), box->low.y);
}

/*
 * Is the Sphere strictly in front of the Box3D?
 */
Datum
front_sphere_box3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(front_sphere_box3D_internal(sphere, box));
}

PG_FUNCTION_INFO_V1(front_sphere_box3D);

bool
front_sphere_box3D_internal(Sphere *sphere, Box3D *box)
{
	return FPlt((sphere->center.z + sphere->radius), box->low.z);
}

/*
 * Is the Sphere in front of or overlaps the Box3D?
 */
Datum
overfront_sphere_box3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overfront_sphere_box3D_internal(sphere, box));
}

PG_FUNCTION_INFO_V1(overfront_sphere_box3D);

bool
overfront_sphere_box3D_internal(Sphere *sphere, Box3D *box)
{
	return FPle((sphere->center.z + sphere->radius), box->high.z);
}

/*
 * Is the Sphere strictly at the back of the Box3D?
 */
Datum
back_sphere_box3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(back_sphere_box3D_internal(sphere, box));
}

PG_FUNCTION_INFO_V1(back_sphere_box3D);

bool
back_sphere_box3D_internal(Sphere *sphere, Box3D *box)
{
	return FPgt((sphere->center.z - sphere->radius), box->high.z);
}

/*
 * Is the Sphere at the back of or overlaps the Box3D?
 */
Datum
overback_sphere_box3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Box3D	   *box = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(overback_sphere_box3D_internal(sphere, box));
}

PG_FUNCTION_INFO_V1(overback_sphere_box3D);

bool
overback_sphere_box3D_internal(Sphere *sphere, Box3D *box)
{
	return FPge((sphere->center.z - sphere->radius), box->low.z);
}

/*----------------------------------------------------------*/

/* sphere OPERATOR path3D */

/*
 * Is the Sphere strictly to the left of the Path3D?
 */
Datum
left_sphere_path3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_BOOL(left_sphere_path3D_internal(sphere, path));
}

PG_FUNCTION_INFO_V1(left_sphere_path3D);

bool
left_sphere_path3D_internal(Sphere *sphere, Path3D *path)
{
	return FPlt((sphere->center.x + sphere->radius), path->boundbox.low.x);
}

/*
 * Is the Sphere to the left of or overlaps the Path3D?
 */
Datum
overleft_sphere_path3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_BOOL(overleft_sphere_path3D_internal(sphere, path));
}

PG_FUNCTION_INFO_V1(overleft_sphere_path3D);

bool
overleft_sphere_path3D_internal(Sphere *sphere, Path3D *path)
{
	return FPle((sphere->center.x + sphere->radius), path->boundbox.high.x);
}

/*
 * Is the Sphere strictly to the right of the Path3D?
 */
Datum
right_sphere_path3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_BOOL(right_sphere_path3D_internal(sphere, path));
}

PG_FUNCTION_INFO_V1(right_sphere_path3D);

bool
right_sphere_path3D_internal(Sphere *sphere, Path3D *path)
{
	return FPgt((sphere->center.x - sphere->radius), path->boundbox.high.x);
}

/*
 * Is the Sphere to the right of or overlaps the Path3D?
 */
Datum
overright_sphere_path3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_BOOL(overright_sphere_path3D_internal(sphere, path));
}

PG_FUNCTION_INFO_V1(overright_sphere_path3D);

bool
overright_sphere_path3D_internal(Sphere *sphere, Path3D *path)
{
	return FPge((sphere->center.x - sphere->radius), path->boundbox.low.x);
}

/*
 * Is the Sphere strictly below the Path3D?
 */
Datum
below_sphere_path3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_BOOL(below_sphere_path3D_internal(sphere, path));
}

PG_FUNCTION_INFO_V1(below_sphere_path3D);

bool
below_sphere_path3D_internal(Sphere *sphere, Path3D *path)
{
	return FPlt((sphere->center.y + sphere->radius), path->boundbox.low.y);
}

/*
 * Is the Sphere below or overlaps the Path3D?
 */
Datum
overbelow_sphere_path3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_BOOL(overbelow_sphere_path3D_internal(sphere, path));
}

PG_FUNCTION_INFO_V1(overbelow_sphere_path3D);

bool
overbelow_sphere_path3D_internal(Sphere *sphere, Path3D *path)
{
	return FPle((sphere->center.y + sphere->radius), path->boundbox.high.y);
}

/*
 * Is the Sphere strictly above the Path3D?
 */
Datum
above_sphere_path3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_BOOL(above_sphere_path3D_internal(sphere, path));
}

PG_FUNCTION_INFO_V1(above_sphere_path3D);

bool
above_sphere_path3D_internal(Sphere *sphere, Path3D *path)
{
	return FPgt((sphere->center.y - sphere->radius), path->boundbox.high.y);
}

/*
 * Is the Sphere above or overlaps the Path3D?
 */
Datum
overabove_sphere_path3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_BOOL(overabove_sphere_path3D_internal(sphere, path));
}

PG_FUNCTION_INFO_V1(overabove_sphere_path3D);

bool
overabove_sphere_path3D_internal(Sphere *sphere, Path3D *path)
{
	return FPge((sphere->center.y - sphere->radius), path->boundbox.low.y);
}

/*
 * Is the Sphere strictly in front of the Path3D?
 */
Datum
front_sphere_path3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_BOOL(front_sphere_path3D_internal(sphere, path));
}

PG_FUNCTION_INFO_V1(front_sphere_path3D);

bool
front_sphere_path3D_internal(Sphere *sphere, Path3D *path)
{
	return FPlt((sphere->center.z + sphere->radius), path->boundbox.low.z);
}

/*
 * Is the Sphere in front of or overlaps the Path3D?
 */
Datum
overfront_sphere_path3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_BOOL(overfront_sphere_path3D_internal(sphere, path));
}

PG_FUNCTION_INFO_V1(overfront_sphere_path3D);

bool
overfront_sphere_path3D_internal(Sphere *sphere, Path3D *path)
{
	return FPle((sphere->center.z + sphere->radius), path->boundbox.high.z);
}

/*
 * Is the Sphere strictly at the back of the Path3D?
 */
Datum
back_sphere_path3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_BOOL(back_sphere_path3D_internal(sphere, path));
}

PG_FUNCTION_INFO_V1(back_sphere_path3D);

bool
back_sphere_path3D_internal(Sphere *sphere, Path3D *path)
{
	return FPgt((sphere->center.z - sphere->radius), path->boundbox.high.z);
}

/*
 * Is the Sphere at the back of or overlaps the Path3D?
 */
Datum
overback_sphere_path3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);

	PG_RETURN_BOOL(overback_sphere_path3D_internal(sphere, path));
}

PG_FUNCTION_INFO_V1(overback_sphere_path3D);

bool
overback_sphere_path3D_internal(Sphere *sphere, Path3D *path)
{
	return FPge((sphere->center.z - sphere->radius), path->boundbox.low.z);
}

/*----------------------------------------------------------*/

/* sphere OPERATOR polygon3D */

/*
 * Is the Sphere strictly to the left of the Polygon3D?
 */
Datum
left_sphere_poly3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_BOOL(left_sphere_poly3D_internal(sphere, poly));
}

PG_FUNCTION_INFO_V1(left_sphere_poly3D);

bool
left_sphere_poly3D_internal(Sphere *sphere, Polygon3D *poly)
{
	return FPlt((sphere->center.x + sphere->radius), poly->boundbox.low.x);
}

/*
 * Is the Sphere to the left of or overlaps the Polygon3D?
 */
Datum
overleft_sphere_poly3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_BOOL(overleft_sphere_poly3D_internal(sphere, poly));
}

PG_FUNCTION_INFO_V1(overleft_sphere_poly3D);

bool
overleft_sphere_poly3D_internal(Sphere *sphere, Polygon3D *poly)
{
	return FPle((sphere->center.x + sphere->radius), poly->boundbox.high.x);
}

/*
 * Is the Sphere strictly to the right of the Polygon3D?
 */
Datum
right_sphere_poly3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_BOOL(right_sphere_poly3D_internal(sphere, poly));
}

PG_FUNCTION_INFO_V1(right_sphere_poly3D);

bool
right_sphere_poly3D_internal(Sphere *sphere, Polygon3D *poly)
{
	return FPgt((sphere->center.x - sphere->radius), poly->boundbox.high.x);
}

/*
 * Is the Sphere to the right of or overlaps the Polygon3D?
 */
Datum
overright_sphere_poly3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_BOOL(overright_sphere_poly3D_internal(sphere, poly));
}

PG_FUNCTION_INFO_V1(overright_sphere_poly3D);

bool
overright_sphere_poly3D_internal(Sphere *sphere, Polygon3D *poly)
{
	return FPge((sphere->center.x - sphere->radius), poly->boundbox.low.x);
}

/*
 * Is the Sphere strictly below the Polygon3D?
 */
Datum
below_sphere_poly3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_BOOL(below_sphere_poly3D_internal(sphere, poly));
}

PG_FUNCTION_INFO_V1(below_sphere_poly3D);

bool
below_sphere_poly3D_internal(Sphere *sphere, Polygon3D *poly)
{
	return FPlt((sphere->center.y + sphere->radius), poly->boundbox.low.y);
}

/*
 * Is the Sphere below or overlaps the Polygon3D?
 */
Datum
overbelow_sphere_poly3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_BOOL(overbelow_sphere_poly3D_internal(sphere, poly));
}

PG_FUNCTION_INFO_V1(overbelow_sphere_poly3D);

bool
overbelow_sphere_poly3D_internal(Sphere *sphere, Polygon3D *poly)
{
	return FPle((sphere->center.y + sphere->radius), poly->boundbox.high.y);
}

/*
 * Is the Sphere strictly above the Polygon3D?
 */
Datum
above_sphere_poly3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_BOOL(above_sphere_poly3D_internal(sphere, poly));
}

PG_FUNCTION_INFO_V1(above_sphere_poly3D);

bool
above_sphere_poly3D_internal(Sphere *sphere, Polygon3D *poly)
{
	return FPgt((sphere->center.y - sphere->radius), poly->boundbox.high.y);
}

/*
 * Is the Sphere above or overlaps the Polygon3D?
 */
Datum
overabove_sphere_poly3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_BOOL(overabove_sphere_poly3D_internal(sphere, poly));
}

PG_FUNCTION_INFO_V1(overabove_sphere_poly3D);

bool
overabove_sphere_poly3D_internal(Sphere *sphere, Polygon3D *poly)
{
	return FPge((sphere->center.y - sphere->radius), poly->boundbox.low.y);
}

/*
 * Is the Sphere strictly in front of the Polygon3D?
 */
Datum
front_sphere_poly3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_BOOL(front_sphere_poly3D_internal(sphere, poly));
}

PG_FUNCTION_INFO_V1(front_sphere_poly3D);

bool
front_sphere_poly3D_internal(Sphere *sphere, Polygon3D *poly)
{
	return FPlt((sphere->center.z + sphere->radius), poly->boundbox.low.z);
}

/*
 * Is the Sphere in front of or overlaps the Polygon3D?
 */
Datum
overfront_sphere_poly3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_BOOL(overfront_sphere_poly3D_internal(sphere, poly));
}

PG_FUNCTION_INFO_V1(overfront_sphere_poly3D);

bool
overfront_sphere_poly3D_internal(Sphere *sphere, Polygon3D *poly)
{
	return FPle((sphere->center.z + sphere->radius), poly->boundbox.high.z);
}

/*
 * Is the Sphere strictly at the back of the Polygon3D?
 */
Datum
back_sphere_poly3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_BOOL(back_sphere_poly3D_internal(sphere, poly));
}

PG_FUNCTION_INFO_V1(back_sphere_poly3D);

bool
back_sphere_poly3D_internal(Sphere *sphere, Polygon3D *poly)
{
	return FPgt((sphere->center.z - sphere->radius), poly->boundbox.high.z);
}

/*
 * Is the Sphere at the back of or overlaps the Polygon3D?
 */
Datum
overback_sphere_poly3D(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);

	PG_RETURN_BOOL(overback_sphere_poly3D_internal(sphere, poly));
}

PG_FUNCTION_INFO_V1(overback_sphere_poly3D);

bool
overback_sphere_poly3D_internal(Sphere *sphere, Polygon3D *poly)
{
	return FPge((sphere->center.z - sphere->radius), poly->boundbox.low.z);
}

/*----------------------------------------------------------*/

/* sphere OPERATOR sphere */

/*
 * Is the first Sphere strictly to the left of the second Sphere?
 */
Datum
left_sphere_sphere(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere1 = PG_GETARG_SPHERE_P(0);
	Sphere	   *sphere2 = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(left_sphere_sphere_internal(sphere1, sphere2));
}

PG_FUNCTION_INFO_V1(left_sphere_sphere);

bool
left_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2)
{
	return FPlt((sphere1->center.x + sphere1->radius),
						(sphere2->center.x - sphere2->radius));
}

/*
 * Is the first Sphere to the left of or overlaps the second Sphere?
 */
Datum
overleft_sphere_sphere(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere1 = PG_GETARG_SPHERE_P(0);
	Sphere	   *sphere2 = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overleft_sphere_sphere_internal(sphere1, sphere2));
}

PG_FUNCTION_INFO_V1(overleft_sphere_sphere);

bool
overleft_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2)
{
	return FPle((sphere1->center.x + sphere1->radius),
						(sphere2->center.x + sphere2->radius));
}

/*
 * Is the first Sphere strictly to the right of the second Sphere?
 */
Datum
right_sphere_sphere(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere1 = PG_GETARG_SPHERE_P(0);
	Sphere	   *sphere2 = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(right_sphere_sphere_internal(sphere1, sphere2));
}

PG_FUNCTION_INFO_V1(right_sphere_sphere);

bool
right_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2)
{
	return FPgt((sphere1->center.x - sphere1->radius),
						(sphere2->center.x + sphere2->radius));
}

/*
 * Is the first Sphere to the right of or overlaps the second Sphere?
 */
Datum
overright_sphere_sphere(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere1 = PG_GETARG_SPHERE_P(0);
	Sphere	   *sphere2 = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overright_sphere_sphere_internal(sphere1, sphere2));
}

PG_FUNCTION_INFO_V1(overright_sphere_sphere);

bool
overright_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2)
{
	return FPge((sphere1->center.x - sphere1->radius),
						(sphere2->center.x - sphere2->radius));
}

/*
 * Is the first Sphere strictly below the second Sphere?
 */
Datum
below_sphere_sphere(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere1 = PG_GETARG_SPHERE_P(0);
	Sphere	   *sphere2 = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(below_sphere_sphere_internal(sphere1, sphere2));
}

PG_FUNCTION_INFO_V1(below_sphere_sphere);

bool
below_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2)
{
	return FPlt((sphere1->center.y + sphere1->radius),
						(sphere2->center.y - sphere2->radius));
}

/*
 * Is the first Sphere below or overlaps the second Sphere?
 */
Datum
overbelow_sphere_sphere(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere1 = PG_GETARG_SPHERE_P(0);
	Sphere	   *sphere2 = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overbelow_sphere_sphere_internal(sphere1, sphere2));
}

PG_FUNCTION_INFO_V1(overbelow_sphere_sphere);

bool
overbelow_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2)
{
	return FPle((sphere1->center.y + sphere1->radius),
						(sphere2->center.y + sphere2->radius));
}

/*
 * Is the first Sphere strictly above the second Sphere?
 */
Datum
above_sphere_sphere(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere1 = PG_GETARG_SPHERE_P(0);
	Sphere	   *sphere2 = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(above_sphere_sphere_internal(sphere1, sphere2));
}

PG_FUNCTION_INFO_V1(above_sphere_sphere);

bool
above_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2)
{
	return FPgt((sphere1->center.y - sphere1->radius),
						(sphere2->center.y + sphere2->radius));
}

/*
 * Is the first Sphere above or overlaps the second Sphere?
 */
Datum
overabove_sphere_sphere(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere1 = PG_GETARG_SPHERE_P(0);
	Sphere	   *sphere2 = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overabove_sphere_sphere_internal(sphere1, sphere2));
}

PG_FUNCTION_INFO_V1(overabove_sphere_sphere);

bool
overabove_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2)
{
	return FPge((sphere1->center.y - sphere1->radius),
						(sphere2->center.y - sphere2->radius));
}

/*
 * Is the first Sphere strictly in front of the second Sphere?
 */
Datum
front_sphere_sphere(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere1 = PG_GETARG_SPHERE_P(0);
	Sphere	   *sphere2 = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(front_sphere_sphere_internal(sphere1, sphere2));
}

PG_FUNCTION_INFO_V1(front_sphere_sphere);

bool
front_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2)
{
	return FPlt((sphere1->center.z + sphere1->radius),
						(sphere2->center.z - sphere2->radius));
}

/*
 * Is the first Sphere in front of or overlaps the second Sphere?
 */
Datum
overfront_sphere_sphere(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere1 = PG_GETARG_SPHERE_P(0);
	Sphere	   *sphere2 = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overfront_sphere_sphere_internal(sphere1, sphere2));
}

PG_FUNCTION_INFO_V1(overfront_sphere_sphere);

bool
overfront_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2)
{
	return FPle((sphere1->center.z + sphere1->radius),
						(sphere2->center.z + sphere2->radius));
}

/*
 * Is the first Sphere strictly at the back of the second Sphere?
 */
Datum
back_sphere_sphere(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere1 = PG_GETARG_SPHERE_P(0);
	Sphere	   *sphere2 = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(back_sphere_sphere_internal(sphere1, sphere2));
}

PG_FUNCTION_INFO_V1(back_sphere_sphere);

bool
back_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2)
{
	return FPgt((sphere1->center.z - sphere1->radius),
						(sphere2->center.z + sphere2->radius));
}

/*
 * Is the first Sphere at the back of or overlaps the Sphere?
 */
Datum
overback_sphere_sphere(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere1 = PG_GETARG_SPHERE_P(0);
	Sphere	   *sphere2 = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(overback_sphere_sphere_internal(sphere1, sphere2));
}

PG_FUNCTION_INFO_V1(overback_sphere_sphere);

bool
overback_sphere_sphere_internal(Sphere *sphere1, Sphere *sphere2)
{
	return FPge((sphere1->center.z - sphere1->radius),
						(sphere2->center.z - sphere2->radius));
}

/*****************************************************************************/
