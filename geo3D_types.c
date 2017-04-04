/*****************************************************************************
 *
 * geo3D_types.c
 *	  Type definitions for 3D geometries
 *
 * Portions Copyright (c) 2017, Esteban Zimanyi, Université Libre de Bruxelles
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

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC ;
#endif

/*****************************************************************************
 * Internal routines
 *****************************************************************************/

/*
 * Delimiters for input and output strings.
 * LDELIM, RDELIM, and DELIM are left, right, and separator delimiters, respectively.
 * LDELIM_EP, RDELIM_EP are left and right delimiters for paths with endpoints.
 * LDELIM_S, RDELIM_S are left and right delimiters for spheres.
 */

#define LDELIM			'('
#define RDELIM			')'
#define DELIM			','
#define LDELIM_EP		'['
#define RDELIM_EP		']'
#define LDELIM_S		'<'
#define RDELIM_S		'>'

enum path3D_delim
{
	PATH3D_NONE, PATH3D_OPEN, PATH3D_CLOSED
};

/* Maximum number of characters printed by triple_encode() */
/* ...+3+7 : 3 accounts for extra_float_digits max value */
#define P_MAXLEN (2*(DBL_DIG+3+7)+1)
 
static int single_decode(char *str, double *x, char **ss);
static int single_encode(double x, char *str);
static int triple_decode(char *str, double *x, double *y, double *z, char **s);
static int triple_encode(double x, double y, double z, char *str);
static int triple_count(char *s, char delim);

static bool plist_same(int npts, Point3D *pt1, Point3D *pt2);
static int plist_isplanar(int npts, Point3D *plist);
static int plist_coplanar(int npts1, Point3D *pt1, int npts2, Point3D *pt2);

static Point3D *point3D_construct_internal(double x, double y, double z);
static Point3D *point3D_copy(Point3D *point);
static int point3D_cmp_internal(Point3D* point1, Point3D* point2);

static Lseg3D *lseg3D_construct_internal(double x1, double y1, double z1, double x2, double y2, double z2);
static Lseg3D *lseg3D_construct_pts_internal(Point3D *point1, Point3D *point2);
static bool lseg3D_coincide_internal(Lseg3D *lseg1, Lseg3D *lseg2);
static int lseg3D_cmp_internal(Lseg3D* lseg1, Lseg3D* lseg2);

static Lseg3D *line3D_construct_internal(double x1, double y1, double z1, double x2, double y2, double z2);
static Line3D * line3D_construct_pts_internal(Point3D *point1, Point3D *point2);
static int line3D_cmp_internal(Line3D* line1, Line3D* line2);

static Box3D *box3D_construct_internal(double x1, double y1, double z1, double x2, double y2, double z2);
static Box3D *box3D_construct_pts_internal(Point3D *point1, Point3D *point2);
static Box3D *box3D_fill(Box3D *result, double x1, double y1, double z1, double x2, double y2, double z2);
static Box3D *box3D_copy(Box3D *box);
static double box3D_ht(Box3D *box);
static double box3D_wd(Box3D *box);
static double box3D_dp(Box3D *box);
static int box3D_cmp_internal(Box3D* box1, Box3D* box2);

static int path3D_decode(int opentype, int npts, char *str, int *isopen, char **ss, Point3D *p);
static char *path3D_encode(enum path3D_delim path3D_delim, int npts, Point3D *point);
static void path3D_make_bbox(Path3D *path);
// static Path3D * path3D_copy(Path3D *path);
static double path3D_length_internal(Path3D *path);
static bool path3D_isplanar_internal(Path3D *path);
static double path3D_area_internal(Path3D *path);
static int path3D_cmp_internal(Path3D* p1, Path3D* p2);

// static Polygon3D * poly3D_copy(Polygon3D *poly);
static bool poly3D_isplanar_internal(Polygon3D *poly);
static int poly3D_cmp_internal(Polygon3D* p1, Polygon3D* p2);

static Sphere *sphere_copy(Sphere *sphere);
static double sphere_vol(Sphere *sphere);
static int sphere_cmp_internal(Sphere* s1, Sphere* s2);

/*****************************************************************************
 * Generic functions
 *****************************************************************************/

Point3D 
add(Point3D point1, Point3D point2)
{
	Point3D	   result;
	
	result.x = point1.x + point2.x;
	result.y = point1.y + point2.y;
	result.z = point1.z + point2.z;
	return result;
}

Point3D 
sub(Point3D point1, Point3D point2)
{
	Point3D	   result;
	
	result.x = point1.x - point2.x;
	result.y = point1.y - point2.y;
	result.z = point1.z - point2.z;
	return result;
}

Point3D
scalar(double k, Point3D point)
{
	Point3D	   result;
	
    result.x = k * (point.x);
    result.y = k * (point.y);
    result.z = k * (point.z);
	return result;
}

Point3D 
cross(Point3D point1, Point3D point2)
{
	Point3D	   result;
	
    result.x = point1.y * point2.z - point1.z * point2.y;
	result.y = point1.z * point2.x - point1.x * point2.z;
	result.z = point1.x * point2.y - point1.y * point2.x;
	return result;
}

Point3D 
absolute(Point3D point)
{
	Point3D	   result;
	
    result.x = fabs(point.x);
    result.y = fabs(point.y);
    result.z = fabs(point.z);
	return result;
}

double
magnitude(Point3D point)
{
	return HYPOT3D(point.x, point.y, point.z);
}

bool
proportional(Point3D point1, Point3D point2)
{
	double p;
	
	if (!FPzero(point2.x)) 
		p = point1.x / point2.x;
	else if (!FPzero(point2.y))
		p = point1.y / point2.y;
	else
		p = point1.z / point2.z;
	return (FPeq(point1.x, point2.x * p) && 
		FPeq(point1.y, point2.y * p) && FPeq(point1.z, point2.z * p));
}

/***********************************************************************
 * Private routines for multiple types
 ***********************************************************************/

static bool
plist_same(int npts, Point3D *pt1, Point3D *pt2)
{
	int			i,
				ii,
				j;

	/* find match for first point */
	for (i = 0; i < npts; i++)
	{
		if (PT3Deq(pt2[i], pt1[0]))
		{

			/* match found? then look forward through remaining points */
			for (ii = 1, j = i + 1; ii < npts; ii++, j++)
			{
				if (j >= npts)
					j = 0;
				if (PT3Dne(pt2[j], pt1[ii]))
					break;
			}
			if (ii == npts)
				return TRUE;

			/* match not found forwards? then look backwards */
			for (ii = 1, j = i - 1; ii < npts; ii++, j--)
			{
				if (j < 0)
					j = (npts - 1);
				if (PT3Dne(pt2[j], pt1[ii]))
					break;
			}
			if (ii == npts)
				return TRUE;
		}
	}

	return FALSE;
}

/*
 * plist_test3noncollinearpts: Return TRUE if there are 3 non collinear  
 * points in the array plist, return FALSE otherwise
 */
 
bool
plist_test3noncollinearpts(Point3D *plist, int npts)
{
	int			i,
				j,
				k;

	for (i = 0; i < npts - 2; i++)
		for (j = i + 1; j < npts - 1; j++)
			for (k = j + 1; k < npts; k++)
				if (point3D_collinear_internal(&plist[i], &plist[j], &plist[k]) == 0)
					return TRUE;
			
	return FALSE;
}

/*
 * plist_find3noncollinearpts: return TRUE if there are 3 non collinear points
 * in the array plist (their position in the array are given in pos1, pos2, 
 * and pos3), return FALSE otherwise
 */
 
bool
plist_find3noncollinearpts(Point3D *plist, int npts, int *pos1, int *pos2, int *pos3)
{
	int			i,
				j,
				k;

	for (i = 0; i < npts - 2; i++)
	{
		for (j = i + 1; j < npts - 1; j++)
		{
			for (k = j + 1; k < npts; k++)
			{
				if (point3D_collinear_internal(&plist[i], &plist[j], &plist[k]) == 0)
				{
					*pos1 = i;
					*pos2 = j;
					*pos3 = k;
					return TRUE;
				}
			}
		}
	}
			
	return FALSE;
}

/* 
 * plist_isplanar: Is the list of points (of a path or a polygon) planar?
 * Pick three arbitrary (but non-collinear) vertices A, B, C of the polygon. 
 * The polygon is planar if all other vertices are located in the plane 
 * defined by A, B, and C. To determine if a point P is in the plane ABC, 
 * we check if the dot product of the normal of the plane with the 
 * vector AP is equal to 0. 
 * https://www.win.tue.nl/~vanwijk/2IV60/2IV60_exercise_3_answers.pdf
 */
 
static int
plist_isplanar(int npts, Point3D *plist)
{
	Point3D		normal;
	int			i,
				pos1,
				pos2,
				pos3;
	
	if (npts < 3 ||	
		!plist_find3noncollinearpts(plist, npts, &pos1, &pos2, &pos3))
		return -1;
				 	
	normal = cross(sub(plist[pos2], plist[pos1]), sub(plist[pos3], plist[pos1]));	
				 
	for (i = 0; i < npts; i++)
	{
		if (!FPzero(DOT(sub(plist[i], plist[pos1]), normal)))
			return 0;		
	}

	return 1;		
}

static int
plist_coplanar(int npts1, Point3D *plist1, int npts2, Point3D *plist2)
{
	Point3D		normal;
	int			i,
				pos1,
				pos2,
				pos3;
	
	if (npts1 < 3 || 
		!plist_find3noncollinearpts(plist1, npts1, &pos1, &pos2, &pos3))
		return -1;
				 	
	normal = cross(sub(plist1[pos2],plist1[pos1]), 
		sub(plist1[pos3], plist1[pos1]));	
	
	// test all points in plist1
	for (i = 0; i < npts1; i++)
	{
		if (!FPzero(DOT(sub(plist1[i], plist1[pos1]), normal)))
			return 0;		
	}

	// test all points in plist2
	for (i = 0; i < npts2; i++)
	{
		if (!FPzero(DOT(sub(plist2[i], plist1[pos1]), normal)))
			return 0;		
	}
	
	return 1;		
}

/*-------------------------------------------------------------------------
 * Determine the hypotenuse.
 *
 * If required, x and y are swapped to make x the larger number. The
 * traditional formula of x^2+y^2 is rearranged to factor x outside the
 * sqrt. This allows computation of the hypotenuse for significantly
 * larger values, and with a higher precision than when using the naive
 * formula.  In particular, this cannot overflow unless the final result
 * would be out-of-range.
 *
 * sqrt( x^2 + y^2 ) = sqrt( x^2( 1 + y^2/x^2) )
 *					 = x * sqrt( 1 + y^2/x^2 )
 *					 = x * sqrt( 1 + y/x * y/x )
 *
 * It is expected that this routine will eventually be replaced with the
 * C99 hypot() function.
 *
 * This implementation conforms to IEEE Std 1003.1 and GLIBC, in that the
 * case of hypot(inf,nan) results in INF, and not NAN.
 *-----------------------------------------------------------------------
 */
double
pg_hypot(double x, double y)
{
	double		yx;

	/* Handle INF and NaN properly */
	if (isinf(x) || isinf(y))
		return get_float8_infinity();

	if (isnan(x) || isnan(y))
		return get_float8_nan();

	/* Else, drop any minus signs */
	x = fabs(x);
	y = fabs(y);

	/* Swap x and y if needed to make x the larger one */
	if (FPlt(x, y))
	{
		double		temp = x;

		x = y;
		y = temp;
	}

	/*
	 * If y is zero, the hypotenuse is x.  This test saves a few cycles in
	 * such cases, but more importantly it also protects against
	 * divide-by-zero errors, since now x >= y.
	 */
	if (FPzero(y))
		return x;

	/* Determine the hypotenuse */
	yx = y / x;
	return x * sqrt(1.0 + (yx * yx));
}

/*-------------------------------------------------------------------------
 * Determine the 3D hypotenuse.
 *
 * If required, x, y, and z are swapped to make x the larger number. The
 * traditional formula of x^2+y^2+z^2 is rearranged to factor x outside the 
 * sqrt. This allows computation of the hypotenuse for significantly
 * larger values, and with a higher precision than when using the naive
 * formula.  In particular, this cannot overflow unless the final result
 * would be out-of-range.
 *
 * sqrt( x^2 + y^2 + z^2 ) 	= sqrt( x^2( 1 + y^2/x^2 + z^2/x^2) )
 * 							= x * sqrt( 1 + y^2/x^2 + z^2/x^2)
 * 					 		= x * sqrt( 1 + y/x * y/x + z/x * z/x)
 *-----------------------------------------------------------------------
 */
double
pg_hypot3D(double x, double y, double z)
{
	double		yx;
	double		zx;

	/* Handle INF and NaN properly */
	if (isinf(x) || isinf(y)|| isinf(z))
		return get_float8_infinity();

	if (isnan(x) || isnan(y) || isnan(z))
		return get_float8_nan();

	/* Else, drop any minus signs */
	x = fabs(x);
	y = fabs(y);
	z = fabs(z);

	/* Swap x, y and z if needed to make x the larger one */
	if (FPlt(x, y))
	{
		double		temp1 = x;
		x = y;
		y = temp1;
	}
	if (FPlt(x, z))
	{
		double		temp2 = x;
		x = z;
		z = temp2;
	}
	/*
	 * If x is zero, the hypotenuse is computed with the 2D case.  
	 * This test saves a few cycles in such cases, but more importantly 
	 * it also protects against divide-by-zero errors, since now x >= y.
	 */
	if (FPzero(x))
		return pg_hypot(y,z);

	/* Determine the hypotenuse */
	yx = y / x;
	zx = z / x;
	return x * sqrt(1.0 + (yx * yx) + (zx * zx));
}

/******************************************************************************
 * 3D Geometric data types are composed of 3D points.
 * This code tries to support a common format throughout the data types,
 * to allow for more predictable usage and data type conversion.
 * The fundamental unit is Point3D. Other units are line segments,
 * open paths, boxes, closed paths, and polygons (which should be considered
 * non-intersecting closed paths).
 *
 * Data representation is as follows:
 *	Point3D:					(x,y,z)
 *	Lseg3D (line segment 3D):	[(x1,y1,z1),(x2,y2,z2)]
 *	Line3D:						((x1,y1,z1),(x2,y2,z2))
 *	Box3D:						(x1,y1,z1),(x2,y2,z2)
 *	Path3D (open):				[(x1,y1,z1),...,(xn,yn,zn)]
 *	Path3D (closed):			((x1,y1,z1),...,(xn,yn,zn))
 *	Polygon3D:					((x1,y1,z1),...,(xn,yn,zn))
 *  Sphere:						<(x1,y1,z1),radius>
 * For Box3D, the points are opposite corners with the first point 
 * at the top right back.
 * For closed Path3D and Polygon3D, the points should be maybe 
 * reordered  to allow faster internal operations.
 ******************************************************************************/

static int
single_decode(char *str, double *x, char **s)
{
	char	   *cp;

	if (!PointerIsValid(str))
		return FALSE;

	*x = strtod(str, &cp);

	if (s != NULL)
	{
		while (isspace((unsigned char) *cp))
			cp++;
		*s = cp;
	}

	return TRUE;
}	/* single_decode() */

static int
single_encode(double x, char *str)
{
	int			ndig = DBL_DIG + extra_float_digits;

	if (ndig < 1)
		ndig = 1;

	sprintf(str, "%.*g", ndig, x);
	return TRUE;
}	/* single_encode() */

static int
triple_decode(char *str, double *x, double *y, double *z, char **s)
{
	int			has_delim;
	char	   *cp;

	if (!PointerIsValid(str))
		return FALSE;

	while (isspace((unsigned char) *str))
		str++;
	if ((has_delim = (*str == LDELIM)))
		str++;

	while (isspace((unsigned char) *str))
		str++;
	*x = strtod(str, &cp);
	if (cp <= str)
		return FALSE;
	while (isspace((unsigned char) *cp))
		cp++;
	if (*cp++ != DELIM)
		return FALSE;

	while (isspace((unsigned char) *cp))
		cp++;
	*y = strtod(cp, &str);
	if (str <= cp)
		return FALSE;
	while (isspace((unsigned char) *str))
		str++;
	if (*str++ != DELIM)
		return FALSE;

	while (isspace((unsigned char) *str))
		str++;
	*z = strtod(str, &cp);
	if (cp <= str)
		return FALSE; 
	while (isspace((unsigned char) *cp))
		cp++;
	
	if (has_delim)
	{
		if (*cp != RDELIM)
			return FALSE;
		cp++;
		while (isspace((unsigned char) *cp))
			cp++;
	}
	if (s != NULL)
		*s = cp;

	return TRUE;
}	/* triple_decode() */

static int
triple_encode(double x, double y, double z, char *str)
{
	int			ndig = DBL_DIG + extra_float_digits;

	if (ndig < 1)
		ndig = 1;

	sprintf(str, "%.*g,%.*g,%.*g", ndig, x, ndig, y, ndig, z);
	return TRUE;
}	/* triple_encode() */

static int
path3D_decode(int opentype, int npts, char *str, int *isopen, 
	char **ss, Point3D *p)
{
	int			depth = 0;
	char	   *s,
			   *cp;
	int			i;

	s = str;
	while (isspace((unsigned char) *s))
		s++;
	if ((*isopen = (*s == LDELIM_EP)))
	{
		/* no open delimiter allowed? */
		if (!opentype)
			return FALSE;
		depth++;
		s++;
		while (isspace((unsigned char) *s))
			s++;

	}
	else if (*s == LDELIM)
	{
		cp = (s + 1);
		while (isspace((unsigned char) *cp))
			cp++;
		if (*cp == LDELIM)
		{
#ifdef NOT_USED
			/* nested delimiters with only one point? */
			if (npts <= 1)
				return FALSE;
#endif
			depth++;
			s = cp;
		}
		else if (strrchr(s, LDELIM) == s)
		{
			depth++;
			s = cp;
		}
	}

	for (i = 0; i < npts; i++)
	{
		if (!triple_decode(s, &(p->x), &(p->y), &(p->z), &s))
			return FALSE;

		if (*s == DELIM)
			s++;
		p++;
	}

	while (depth > 0)
	{
		if ((*s == RDELIM)
			|| ((*s == RDELIM_EP) && (*isopen) && (depth == 1)))
		{
			depth--;
			s++;
			while (isspace((unsigned char) *s))
				s++;
		}
		else
			return FALSE;
	}
	*ss = s;

	return TRUE;
}	/* path3D_decode() */

static char *
path3D_encode(enum path3D_delim path3D_delim, int npts, Point3D *point)
{
	int			size = npts * (P_MAXLEN + 3) + 2;
	char	   *result;
	char	   *cp;
	int			i;

	/* Check for integer overflow */
	if ((size - 2) / npts != (P_MAXLEN + 3))
		ereport(ERROR,
				(errcode(ERRCODE_PROGRAM_LIMIT_EXCEEDED),
				 errmsg("Too many points requested")));

	result = palloc(size);

	cp = result;
	switch (path3D_delim)
	{
		case PATH3D_CLOSED:
			*cp++ = LDELIM;
			break;
		case PATH3D_OPEN:
			*cp++ = LDELIM_EP;
			break;
		case PATH3D_NONE:
			break;
	}

	for (i = 0; i < npts; i++)
	{
		*cp++ = LDELIM;
		if (!triple_encode(point->x, point->y, point->z, cp))
			ereport(ERROR,
					(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
					 errmsg("Could not format \"path3D\" value")));

		cp += strlen(cp);
		*cp++ = RDELIM;
		*cp++ = DELIM;
		point++;
	}
	cp--;
	switch (path3D_delim)
	{
		case PATH3D_CLOSED:
			*cp++ = RDELIM;
			break;
		case PATH3D_OPEN:
			*cp++ = RDELIM_EP;
			break;
		case PATH3D_NONE:
			break;
	}
	*cp = '\0';

	return result;
}	/* path3D_encode() */

/*-------------------------------------------------------------
 * triple_count: Count the number of points
 * Allow the following notation:
 * 		'((1,2,3),(4,5,6))'
 * 		'(1,2,3,4,5,6)'
 *-------------------------------------------------------------*/
static int
triple_count(char *s, char delim)
{
	int			ndelim = 0;

	while ((s = strchr(s, delim)) != NULL)
	{
		ndelim++;
		s++;
	}
	return (ndelim + 1) / 3 ;
}

/***********************************************************************
 * Routines for 3D points
 ***********************************************************************/

Oid POINT3D_OID = InvalidOid ;
Oid point3d_oid() 
{
    if(POINT3D_OID == InvalidOid)
        POINT3D_OID = TypenameGetTypid("point3d") ;
    return POINT3D_OID ;
}

/*----------------------------------------------------------
 *	Input and output functions
 *---------------------------------------------------------*/
 
/*
 * point3D_in: Read in the Point3D from a string specification
 *
 * Input format:
 * 		"(x,y,z)"
 * 		"x,y,z"
 */
Datum
point3D_in(PG_FUNCTION_ARGS)
{
	char	   *str = PG_GETARG_CSTRING(0);
	Point3D	   *point;
	double		x,
				y,
				z;
	char	   *s;

	if (!triple_decode(str, &x, &y, &z, &s) || (*s != '\0'))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("Invalid input syntax for type Point3D: \"%s\"", str)));

	point = (Point3D *) palloc(sizeof(Point3D));

	point->x = x;
	point->y = y;
	point->z = z;

	PG_RETURN_POINT3D_P(point);
}

PG_FUNCTION_INFO_V1(point3D_in) ;

/*		
 * point3D_out: Convert internal Point3D representation to character string
 *
 * Output format:
 * 		"(x,y,z)"
 */
Datum
point3D_out(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);

	PG_RETURN_CSTRING(path3D_encode(PATH3D_NONE, 1, point));
}

PG_FUNCTION_INFO_V1(point3D_out) ;

/*
 *	point3D_recv: Convert external binary representation to Point3D
 */
Datum
point3D_recv(PG_FUNCTION_ARGS)
{
	StringInfo	buf = (StringInfo) PG_GETARG_POINTER(0);
	Point3D	   *point = (Point3D *) palloc(sizeof(Point3D));
	
	point->x = pq_getmsgfloat8(buf);
	point->y = pq_getmsgfloat8(buf);
	point->z = pq_getmsgfloat8(buf);
	PG_RETURN_POINT3D_P(point);
}

PG_FUNCTION_INFO_V1(point3D_recv);

/*
 * point3D_send: Convert Point3D to binary representation
 */
Datum
point3D_send(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	StringInfoData buf;

	pq_begintypsend(&buf);
	pq_sendfloat8(&buf, point->x);
	pq_sendfloat8(&buf, point->y);
	pq_sendfloat8(&buf, point->z);
	PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
}

PG_FUNCTION_INFO_V1(point3D_send);

/*----------------------------------------------------------
 *	Constructors
 *---------------------------------------------------------*/
 
/* 
 * point3D_construct: Form a Point3D from three double
 */
Datum
point3D_construct(PG_FUNCTION_ARGS)
{
	double		x = PG_GETARG_FLOAT8(0);
	double		y = PG_GETARG_FLOAT8(1);
	double		z = PG_GETARG_FLOAT8(2);

	PG_RETURN_POINT3D_P(point3D_construct_internal(x, y, z));
}

PG_FUNCTION_INFO_V1(point3D_construct);

static Point3D *
point3D_construct_internal(double x, double y, double z)
{
	Point3D	   *result = (Point3D *) palloc(sizeof(Point3D));

	result->x = x;
	result->y = y;
	result->z = z;
	return result;
}

/* 
 * point3D_copy: Form a Point3D from another Point3D
 */
static Point3D *
point3D_copy(Point3D *point)
{
	Point3D	   *result;

	if (!PointerIsValid(point))
		return NULL;

	result = (Point3D *) palloc(sizeof(Point3D));

	*result = *point;
	return result;
}

/*----------------------------------------------------------
 *	Accessor functions
 *---------------------------------------------------------*/

/* 
 * point3D_x: Return the x coordinate of a Point3D
 */
Datum
point3D_x(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);

	PG_RETURN_FLOAT8(point->x);
}

PG_FUNCTION_INFO_V1(point3D_x);

/* 
 * point3D_y: Return the y coordinate of a Point3D
 */
Datum
point3D_y(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);

	PG_RETURN_FLOAT8(point->y);
}

PG_FUNCTION_INFO_V1(point3D_y);

/* 
 * point3D_z: Return the z coordinate of a Point3D
 */
Datum
point3D_z(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);

	PG_RETURN_FLOAT8(point->z);
}

PG_FUNCTION_INFO_V1(point3D_z);

/*
 * point3D_boundbox: Return the bounding point of the Point3D
 */
Datum
point3D_boundbox(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);

	PG_RETURN_BOX3D_P(point3D_bbox(point));
}

PG_FUNCTION_INFO_V1(point3D_boundbox);

Box3D *
point3D_bbox(Point3D *point)
{
	Box3D	   *box = (Box3D *) palloc(sizeof(Box3D));

	box->high = box->low = *point;
	return box;
}

/*
 * point3D_boundsphere: Return the bounding sphere of the Point3D
 */
Datum
point3D_boundsphere(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);

	PG_RETURN_SPHERE_P(point3D_bsphere(point));
}

PG_FUNCTION_INFO_V1(point3D_boundsphere);

Sphere *
point3D_bsphere(Point3D *point)
{
	Sphere	   *sphere = (Sphere *) palloc(sizeof(Sphere));

	sphere->center = *point;
	sphere->radius = 0.0;
	return sphere;
}

/*----------------------------------------------------------
 *	Relative position routines
 *---------------------------------------------------------*/

/* 
 * point3D_vertical: Are the points parallel to the 
 * vertical plane defined by the y and z axes?
 */
Datum
point3D_vertical(PG_FUNCTION_ARGS)
{
	Point3D	   *point1 = PG_GETARG_POINT3D_P(0);
	Point3D	   *point2 = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(FPeq(point1->x, point2->x));
}

PG_FUNCTION_INFO_V1(point3D_vertical);

/* 
 * point3D_horizontal: Are the points parallel to the 
 * horizontal plane defined by the x and z axes?
 */
Datum
point3D_horizontal(PG_FUNCTION_ARGS)
{
	Point3D	   *point1 = PG_GETARG_POINT3D_P(0);
	Point3D	   *point2 = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(FPeq(point1->y, point2->y));
}

PG_FUNCTION_INFO_V1(point3D_horizontal);

/* 
 * point3D_perpendicular: Are the points parallel to the 
 * perpendicular plane defined by the x and y axes?
 */
Datum
point3D_perpendicular(PG_FUNCTION_ARGS)
{
	Point3D	   *point1 = PG_GETARG_POINT3D_P(0);
	Point3D	   *point2 = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(FPeq(point1->z, point2->z));
}

PG_FUNCTION_INFO_V1(point3D_perpendicular);

/*
 * point3D_collinear: Are the 3 points collinear?
 */
Datum
point3D_collinear(PG_FUNCTION_ARGS)
{
	Point3D	   *point1 = PG_GETARG_POINT3D_P(0);
	Point3D	   *point2 = PG_GETARG_POINT3D_P(1);	
	Point3D	   *point3 = PG_GETARG_POINT3D_P(2);
	int			result;
	
	result = point3D_collinear_internal(point1, point2, point3);	
	if (result < 0)
		PG_RETURN_NULL();	
	else if (result == 0)
		PG_RETURN_BOOL(FALSE);
	else 
		PG_RETURN_BOOL(TRUE);
}

PG_FUNCTION_INFO_V1(point3D_collinear);

/*
 * If A,B,C are collinear, then AB and AC are proportional
 */
int
point3D_collinear_internal(Point3D* point1, Point3D* point2, Point3D* point3)
{
	Point3D		u,
				v;
	
	/* ensure that the points are pairwise different */
	if (PT3DPeq(point1, point2) || PT3DPeq(point1, point3) || 
		PT3DPeq(point2, point3))
		return -1;
		
	u = sub(*point2, *point1);
	v = sub(*point3, *point1);

	if (proportional(u, v))
		return 1;
	else
		return 0;
}

/*----------------------------------------------------------
 *	"Arithmetic" routines on Point3D
 *---------------------------------------------------------*/

/*
 * point3D_add: Return the Point3D obtained by adding  
 * the two Point3D
 */
Datum
point3D_add(PG_FUNCTION_ARGS)
{
	Point3D	   *point1 = PG_GETARG_POINT3D_P(0);
	Point3D	   *point2 = PG_GETARG_POINT3D_P(1);
	Point3D	   *result = (Point3D *) palloc(sizeof(Point3D));

	result->x = (point1->x + point2->x);
	result->y = (point1->y + point2->y);
	result->z = (point1->z + point2->z);

	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(point3D_add);

/*
 * point3D_sub: Return the Point3D obtained by subtracting  
 * the two Point3D
 */
Datum
point3D_sub(PG_FUNCTION_ARGS)
{
	Point3D	   *point1 = PG_GETARG_POINT3D_P(0);
	Point3D	   *point2 = PG_GETARG_POINT3D_P(1);
	Point3D	   *result = (Point3D *) palloc(sizeof(Point3D));

	result->x = (point1->x - point2->x);
	result->y = (point1->y - point2->y);
	result->z = (point1->z - point2->z);

	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(point3D_sub);

/*
 * point3D_mul: Return the Point3D obtained by multiplying  
 * the Point3D with the double
 */
Datum
point3D_mul(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	double	   d = PG_GETARG_FLOAT8(1);
	Point3D	   *result = (Point3D *) palloc(sizeof(Point3D));

	result->x = point->x * d;
	result->y = point->y * d;
	result->z = point->z * d; 

	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(point3D_mul);

/*
 * point3D_div: Return the Point3D obtained by dividing 
 * the Point3D by the double
 */
Datum
point3D_div(PG_FUNCTION_ARGS)
{
	Point3D	   *point = PG_GETARG_POINT3D_P(0);
	double	   d = PG_GETARG_FLOAT8(1);
	Point3D	   *result;

	if (FPzero(d))
		ereport(ERROR,
				(errcode(ERRCODE_DIVISION_BY_ZERO),
				 errmsg("Division by zero")));
	
	result = (Point3D *) palloc(sizeof(Point3D));

	result->x = point->x / d;
	result->y = point->y / d;
	result->z = point->z / d; 

	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(point3D_div);

/*----------------------------------------------------------
 *	Relational routines 
 *---------------------------------------------------------*/

/*
 * point3D_same: Are the two Point3D equal?
 */
Datum
point3D_same(PG_FUNCTION_ARGS)
{
	Point3D		   *point1 = PG_GETARG_POINT3D_P(0);
	Point3D		   *point2 = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOOL(PT3DPeq(point1, point2));
}

PG_FUNCTION_INFO_V1(point3D_same);

bool
point3D_same_internal(Point3D *point1, Point3D *point2)
{
	return (PT3DPeq(point1, point2));
}

/*
 * point3D_cmp_internal: Compare the two Point3D
 */
static int
point3D_cmp_internal(Point3D* point1, Point3D* point2) 
{
	if(FPlt(point1->x, point2->x))
		return -1;
	if(FPgt(point1->x, point2->x))
		return 1;
	if(FPlt(point1->y, point2->y))
		return -1;
	if(FPgt(point1->y, point2->y))
		return 1;
	if(FPlt(point1->z, point2->z))
		return -1;
	if(FPgt(point1->z, point2->z))
		return 1;
	// both points are equal
	return 0;
}

Datum 
point3D_cmp(PG_FUNCTION_ARGS) 
{
	Point3D* point1 = PG_GETARG_POINT3D_P(0);
	Point3D* point2 = PG_GETARG_POINT3D_P(1);
	PG_RETURN_INT32(point3D_cmp_internal(point1, point2));
}

PG_FUNCTION_INFO_V1(point3D_cmp) ;

Datum 
point3D_lt(PG_FUNCTION_ARGS) 
{
	Point3D* point1 = PG_GETARG_POINT3D_P(0);
	Point3D* point2 = PG_GETARG_POINT3D_P(1);
	PG_RETURN_BOOL(point3D_cmp_internal(point1, point2) < 0);
}

PG_FUNCTION_INFO_V1(point3D_lt);

Datum 
point3D_le(PG_FUNCTION_ARGS) 
{
	Point3D* point1 = PG_GETARG_POINT3D_P(0);
	Point3D* point2 = PG_GETARG_POINT3D_P(1);
	PG_RETURN_BOOL(point3D_cmp_internal(point1, point2) <= 0);
}

PG_FUNCTION_INFO_V1(point3D_le);

Datum 
point3D_eq(PG_FUNCTION_ARGS) 
{
	Point3D* point1 = PG_GETARG_POINT3D_P(0);
	Point3D* point2 = PG_GETARG_POINT3D_P(1);
	PG_RETURN_BOOL(point3D_cmp_internal(point1, point2) == 0);
}

PG_FUNCTION_INFO_V1(point3D_eq);

Datum 
point3D_ne(PG_FUNCTION_ARGS) 
{
	Point3D* point1 = PG_GETARG_POINT3D_P(0);
	Point3D* point2 = PG_GETARG_POINT3D_P(1);
	PG_RETURN_BOOL(point3D_cmp_internal(point1, point2) != 0);
}

PG_FUNCTION_INFO_V1(point3D_ne);

Datum 
point3D_gt(PG_FUNCTION_ARGS) 
{
	Point3D* point1 = PG_GETARG_POINT3D_P(0);
	Point3D* point2 = PG_GETARG_POINT3D_P(1);
	PG_RETURN_BOOL(point3D_cmp_internal(point1, point2) > 0);
}

PG_FUNCTION_INFO_V1(point3D_gt);

Datum 
point3D_ge(PG_FUNCTION_ARGS) 
{
	Point3D* point1 = PG_GETARG_POINT3D_P(0);
	Point3D* point2 = PG_GETARG_POINT3D_P(1);
	PG_RETURN_BOOL(point3D_cmp_internal(point1, point2) >= 0);
}

PG_FUNCTION_INFO_V1(point3D_ge);

/***********************************************************************
 * Routines for 3D line segments
 ***********************************************************************/
 
/*----------------------------------------------------------
 *	Input and output functions
 *---------------------------------------------------------*/

/*
 * lseg3D_in: Read in the Lseg3D from a string specification
 *
 * Input format: 
 *		"[(x1, y1, z1), (x2, y2, z2)]"
 *		"((x1, y1, z1), (x2, y2, z2))" 
 *		"[x1, y1, z1, x2, y2, z2]" 
 *		"(x1, y1, z1, x2, y2, z2)" 
 *		"(x1, y1, z1), (x2, y2, z2)"
 *		"x1, y1, z1, x2, y2, z2"
 * The two end points of a Lseg3D may be equal
 */
Datum
lseg3D_in(PG_FUNCTION_ARGS)
{
	char	   *str = PG_GETARG_CSTRING(0);
	Lseg3D	   *lseg = (Lseg3D *) palloc(sizeof(Lseg3D));
	int			isopen;
	char	   *s;

	if ((!path3D_decode(TRUE, 2, str, &isopen, &s, &(lseg->p[0])))
		|| (*s != '\0'))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("Invalid input syntax for type Lseg3D: \"%s\"", str)));

	PG_RETURN_LSEG3D_P(lseg);
}

PG_FUNCTION_INFO_V1(lseg3D_in);

/*		
 * lseg3D_out: Convert internal Lseg3D representation to character string
 *
 * Output format: 
 *		"[(x1, y1, z1), (x2, y2, z2)]"
 */
Datum
lseg3D_out(PG_FUNCTION_ARGS)
{
	Lseg3D	   *ls = PG_GETARG_LSEG3D_P(0);

	PG_RETURN_CSTRING(path3D_encode(PATH3D_OPEN, 2, (Point3D *) &(ls->p[0])));
}

PG_FUNCTION_INFO_V1(lseg3D_out);

/*
 * lseg3D_recv: Convert external binary representation to Lseg3D
 */
Datum
lseg3D_recv(PG_FUNCTION_ARGS)
{
	StringInfo	buf = (StringInfo) PG_GETARG_POINTER(0);
	Lseg3D	   *lseg = (Lseg3D *) palloc(sizeof(Lseg3D));

	lseg->p[0].x = pq_getmsgfloat8(buf);
	lseg->p[0].y = pq_getmsgfloat8(buf);
	lseg->p[0].z = pq_getmsgfloat8(buf);
	lseg->p[1].x = pq_getmsgfloat8(buf);
	lseg->p[1].y = pq_getmsgfloat8(buf);
	lseg->p[1].z = pq_getmsgfloat8(buf);

	PG_RETURN_LSEG3D_P(lseg);
}

PG_FUNCTION_INFO_V1(lseg3D_recv);

/*
 * lseg3D_send: Convert Lseg3D to binary representation
 */
Datum
lseg3D_send(PG_FUNCTION_ARGS)
{
	Lseg3D	   *ls = PG_GETARG_LSEG3D_P(0);
	StringInfoData buf;

	pq_begintypsend(&buf);
	pq_sendfloat8(&buf, ls->p[0].x);
	pq_sendfloat8(&buf, ls->p[0].y);
	pq_sendfloat8(&buf, ls->p[0].z);
	pq_sendfloat8(&buf, ls->p[1].x);
	pq_sendfloat8(&buf, ls->p[1].y);
	pq_sendfloat8(&buf, ls->p[1].z);
	PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
}

PG_FUNCTION_INFO_V1(lseg3D_send);

/*----------------------------------------------------------
 *	Constructors
 *---------------------------------------------------------*/

/*		
 * lseg3D_construct: Form a Lseg3D from 6 double
 */
Datum
lseg3D_construct(PG_FUNCTION_ARGS)
{
	double		x1 = PG_GETARG_FLOAT8(0);
	double		y1 = PG_GETARG_FLOAT8(1);
	double		z1 = PG_GETARG_FLOAT8(2);
	double		x2 = PG_GETARG_FLOAT8(3);
	double		y2 = PG_GETARG_FLOAT8(4);
	double		z2 = PG_GETARG_FLOAT8(5);

	PG_RETURN_LSEG3D_P(lseg3D_construct_internal(x1, y1, z1, x2, y2, z2));
}

PG_FUNCTION_INFO_V1(lseg3D_construct);

static Lseg3D *
lseg3D_construct_internal(double x1, double y1, double z1, 
	double x2, double y2, double z2)
{
	Lseg3D		   *result = (Lseg3D *) palloc(sizeof(Lseg3D));

	result->p[0].x = x1;
	result->p[0].y = y1;
	result->p[0].z = z1;
	result->p[1].x = x2;
	result->p[1].y = y2;
	result->p[1].z = z2;

	return result;
}

/* 
 * lseg3D_construct_pts: Form a Lseg3D from two Point3D
 */
Datum
lseg3D_construct_pts(PG_FUNCTION_ARGS)
{
	Point3D	   *point1 = PG_GETARG_POINT3D_P(0);
	Point3D	   *point2 = PG_GETARG_POINT3D_P(1);

	PG_RETURN_LSEG3D_P(lseg3D_construct_pts_internal(point1, point2));
}

PG_FUNCTION_INFO_V1(lseg3D_construct_pts);

static Lseg3D *
lseg3D_construct_pts_internal(Point3D *point1, Point3D *point2)
{
	Lseg3D	   *result = (Lseg3D *) palloc(sizeof(Lseg3D));
	
	result->p[0] = *point1;
	result->p[1] = *point2;
	
	return result;
}
	
/* Like lseg3D_construct_pts, but assume space is already allocated */
void
lseg3D_construct_pts_stat(Lseg3D *lseg, Point3D point1, Point3D point2)
{
	lseg->p[0] = point1;
	lseg->p[1] = point2;
}

/*----------------------------------------------------------
 *	Accessor functions
 *---------------------------------------------------------*/

/*
 * lseg3D_points: Return the points of the Lseg3D
 */
Datum 
lseg3D_points(PG_FUNCTION_ARGS) 
{
	Lseg3D     *lseg = PG_GETARG_LSEG3D_P(0);
	Datum 	   *d = (Datum *) palloc(sizeof(Datum) * 2);
	ArrayType  *result;

	for (int i = 0; i < 2; i++)
		d[i] = Point3DPGetDatum(&lseg->p[i]);

	result = construct_array(d, 2, point3d_oid(), sizeof(Point3D), FALSE, 'd');

	PG_RETURN_ARRAYTYPE_P(result);
}

PG_FUNCTION_INFO_V1(lseg3D_points) ;

/*
 * lseg3D_length: Return the length of the Lseg3D
 */
Datum
lseg3D_length(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);

	PG_RETURN_FLOAT8(dist_point3D_point3D(&lseg->p[0], &lseg->p[1]));
}

PG_FUNCTION_INFO_V1(lseg3D_length);

/*
 * lseg3D_center: Return the center of the Lseg3D
 */
Datum
lseg3D_center(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Point3D	   *result = (Point3D *) palloc(sizeof(Point3D));

	result->x = (lseg->p[0].x + lseg->p[1].x) / 2.0;
	result->y = (lseg->p[0].y + lseg->p[1].y) / 2.0;
	result->z = (lseg->p[0].z + lseg->p[1].y) / 2.0;

	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(lseg3D_center);

/*
 * lseg3D_boundbox: Return the bounding box of the Lseg3D
 */
Datum
lseg3D_boundbox(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);

	PG_RETURN_BOX3D_P(lseg3D_bbox(lseg));
}

PG_FUNCTION_INFO_V1(lseg3D_boundbox);

Box3D *
lseg3D_bbox(Lseg3D *lseg)
{
	return box3D_construct_pts_internal(&lseg->p[0], &lseg->p[1]);
}

/*
 * lseg3D_boundsphere: Return the bounding sphere of the Lseg3D
 */
Datum
lseg3D_boundsphere(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);

	PG_RETURN_SPHERE_P(lseg3D_bsphere(lseg));
}

PG_FUNCTION_INFO_V1(lseg3D_boundsphere);

Sphere *
lseg3D_bsphere(Lseg3D *lseg)
{
	Sphere	   *sphere = (Sphere *) palloc(sizeof(Sphere));

	sphere->center = scalar(0.5, add(lseg->p[1], lseg->p[0]));
	sphere->radius = dist_point3D_point3D(&sphere->center, &lseg->p[0]);
	return sphere;
}

/* 
 * lseg3D_vertical: Is the Lseg3D parallel to the vertical plane 
 * defined by the y and z axes?
 */
Datum
lseg3D_vertical(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);

	PG_RETURN_BOOL(FPeq(lseg->p[0].x, lseg->p[1].x));
}

PG_FUNCTION_INFO_V1(lseg3D_vertical);

/* 
 * lseg3D_horizontal: Is the Lseg3D parallel to the horizontal plane 
 * defined by the x and z axes?
 */
Datum
lseg3D_horizontal(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);

	PG_RETURN_BOOL(FPeq(lseg->p[0].y, lseg->p[1].y));
}

PG_FUNCTION_INFO_V1(lseg3D_horizontal);

/* 
 * lseg3D_perpendicular: Is the Lseg3D parallel to the perpendicular plane 
 * defined by the x and y axes?
 */
Datum
lseg3D_perpendicular(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);

	PG_RETURN_BOOL(FPeq(lseg->p[0].z, lseg->p[1].z));
}

PG_FUNCTION_INFO_V1(lseg3D_perpendicular);

/*----------------------------------------------------------
 * Conversion routines
 *---------------------------------------------------------*/

/*
 * lseg3D_line3D: Convert the Lseg3D to a Line3D
 */
Datum
lseg3D_line3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);

	PG_RETURN_LINE3D_P(lseg3D_line3D_internal(lseg));
}

PG_FUNCTION_INFO_V1(lseg3D_line3D);

Line3D *
lseg3D_line3D_internal(Lseg3D *lseg)
{
	Line3D	   *line = (Line3D *) palloc(sizeof(Line3D));

	line->p[0] = lseg->p[0];
	line->p[1] = lseg->p[1];

	return line;
}

/*----------------------------------------------------------
 *	Relative position routines
 *---------------------------------------------------------*/

/*
 * lseg3D_parallel: Are two Lseg3D parallel and do not coincide?
 */
Datum
lseg3D_parallel(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D	   *lseg2 = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(lseg3D_parallel_internal(lseg1, lseg2));
}

PG_FUNCTION_INFO_V1(lseg3D_parallel);

/*
 * Test whether the corresponding lines are parallel
 */
bool
lseg3D_parallel_internal(Lseg3D *lseg1, Lseg3D *lseg2)
{
	double		p;
	Line3D	   *line1,
			   *line2;

	line1 = line3D_construct_pts_internal(&lseg1->p[0], &lseg1->p[1]);
	line2 = line3D_construct_pts_internal(&lseg2->p[0], &lseg2->p[1]);
	return line3D_parallel_internal(line1, line2);
}

/*
 * lseg3D_coincide: Do the two Lseg3D coincide ?
 */
Datum
lseg3D_coincide(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D	   *lseg2 = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(lseg3D_coincide_internal(lseg1, lseg2));
}

PG_FUNCTION_INFO_V1(lseg3D_coincide);

/*
 * Two line segments defined by their equations in the vector form
 *		S1 = <x1,y1,z1> + s<a1,b1,c1>
 *		S2 = <x2,y2,z2> + t<a2,b2,c2>
 * coincide if their direction vectors are proportional and if there exists 
 * points P0 and P1 in S1 that are also in S2, i.e., there exists t0 and t1 
 * such that P0 = Q(t0) and P1 = Q(t1) and the segment intervals 
 * [t0,t1] and [0,1] not are disjoint. 
 * http://geomalgorithms.com/a05-_intersect-1.html
 */
static bool
lseg3D_coincide_internal(Lseg3D *lseg1, Lseg3D *lseg2)
{
	double		t0,
				t1;
	Point3D		u,
				v,
				w0,
				w1;
	
	u = sub(lseg1->p[1], lseg1->p[0]);
	v = sub(lseg2->p[1], lseg2->p[0]);
	w0 = sub(lseg2->p[0], lseg1->p[0]);
	w1 = sub(lseg2->p[1], lseg1->p[1]);
	
	// Are their direction vectors proportional?
	if (!proportional(u, v)) 
		return FALSE;

	// Do they intersect ?
	if (!proportional(w0, v))
		return FALSE;	
	t0 = w0.x / v.x;
	
	if (!proportional(w1, v))
		return FALSE;
	t1 = w1.x / v.x;
	
	// if [t0,t1] intersects with [0,1]
	if (Min(t0,t1) <= 1 && 0 <= Max(t0,t1))
		return TRUE;
	else 
		return FALSE;
}

/*
 * lseg3D_skew: Are the two Lseg3D skew?
 */
Datum
lseg3D_skew(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D	   *lseg2 = PG_GETARG_LSEG3D_P(1);
	Line3D	   *line1,
			   *line2;

	line1 = line3D_construct_pts_internal(&lseg1->p[0], &lseg1->p[1]);
	line2 = line3D_construct_pts_internal(&lseg2->p[0], &lseg2->p[1]);
	PG_RETURN_BOOL(line3D_skew_internal(line1, line2));
}

PG_FUNCTION_INFO_V1(lseg3D_skew);

/* 
 * lseg3D_orthogonal: Are the two Lseg3D orthogonal?
 * The segments are orthogonal if the dot product of
 * their direction vectors is zero.
 */
Datum
lseg3D_orthogonal(PG_FUNCTION_ARGS)
{ 
	Lseg3D	   *lseg1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D	   *lseg2 = PG_GETARG_LSEG3D_P(1);

	/* Determine whether the dot product of their direction vectors is 0 */
	PG_RETURN_BOOL(FPzero(DOT(
		sub(lseg1->p[1],lseg1->p[0]), 
		sub(lseg2->p[1],lseg2->p[0]))));
}

PG_FUNCTION_INFO_V1(lseg3D_orthogonal);

/*----------------------------------------------------------
 *	Relational routines 
 *---------------------------------------------------------*/

/*
 * lseg3D_same: Are the two Lseg3D equal?
 */
Datum
lseg3D_same(PG_FUNCTION_ARGS)
{
	Lseg3D     *ls1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D     *ls2 = PG_GETARG_LSEG3D_P(1);

	PG_RETURN_BOOL(plist_same(2, ls1->p, ls2->p));
}

PG_FUNCTION_INFO_V1(lseg3D_same);

bool
lseg3D_same_internal(Lseg3D *ls1, Lseg3D *ls2)
{
	return plist_same(2, ls1->p, ls2->p);
}

/*
 * lseg3D_cmp_internal: Compare the two Lseg3D
 */
static int 
lseg3D_cmp_internal(Lseg3D* ls1, Lseg3D* ls2) 
{
	if(FPlt(ls1->p[0].x, ls2->p[0].x))
		return -1;
	if(FPgt(ls1->p[0].x, ls2->p[0].x))
		return 1;
	if(FPlt(ls1->p[0].y, ls2->p[0].y))
		return -1;
	if(FPgt(ls1->p[0].y, ls2->p[0].y))
		return 1;
	if(FPlt(ls1->p[0].z, ls2->p[0].z))
		return -1;
	if(FPgt(ls1->p[0].z, ls2->p[0].z))
		return 1;
	// the initial points are equal
	if(FPlt(ls1->p[1].x, ls2->p[1].x))
		return -1;
	if(FPgt(ls1->p[1].x, ls2->p[1].x))
		return 1;
	if(FPlt(ls1->p[1].y, ls2->p[1].y))
		return -1;
	if(FPgt(ls1->p[1].y, ls2->p[1].y))
		return 1;
	if(FPlt(ls1->p[1].z, ls2->p[1].z))
		return -1;
	if(FPgt(ls1->p[1].z, ls2->p[1].z))
		return 1;
	// the final points are also equal
	return 0;
}

Datum 
lseg3D_cmp(PG_FUNCTION_ARGS) 
{
	Lseg3D* ls1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D* ls2 = PG_GETARG_LSEG3D_P(1);
	PG_RETURN_INT32(lseg3D_cmp_internal(ls1, ls2));
}

PG_FUNCTION_INFO_V1(lseg3D_cmp);

Datum 
lseg3D_lt(PG_FUNCTION_ARGS) 
{
	Lseg3D* ls1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D* ls2 = PG_GETARG_LSEG3D_P(1);
	PG_RETURN_BOOL(lseg3D_cmp_internal(ls1, ls2) < 0);
}

PG_FUNCTION_INFO_V1(lseg3D_lt);

Datum 
lseg3D_le(PG_FUNCTION_ARGS) 
{
	Lseg3D* ls1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D* ls2 = PG_GETARG_LSEG3D_P(1);
	PG_RETURN_BOOL(lseg3D_cmp_internal(ls1, ls2) <= 0);
}

PG_FUNCTION_INFO_V1(lseg3D_le);

Datum 
lseg3D_eq(PG_FUNCTION_ARGS) 
{
	Lseg3D* ls1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D* ls2 = PG_GETARG_LSEG3D_P(1);
	PG_RETURN_BOOL(lseg3D_cmp_internal(ls1, ls2) == 0);
}

PG_FUNCTION_INFO_V1(lseg3D_eq);

Datum 
lseg3D_ne(PG_FUNCTION_ARGS) 
{
	Lseg3D* ls1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D* ls2 = PG_GETARG_LSEG3D_P(1);
	PG_RETURN_BOOL(lseg3D_cmp_internal(ls1, ls2) != 0);
}

PG_FUNCTION_INFO_V1(lseg3D_ne);

Datum 
lseg3D_gt(PG_FUNCTION_ARGS) 
{
	Lseg3D* ls1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D* ls2 = PG_GETARG_LSEG3D_P(1);
	PG_RETURN_BOOL(lseg3D_cmp_internal(ls1, ls2) > 0);
}

PG_FUNCTION_INFO_V1(lseg3D_gt);

Datum 
lseg3D_ge(PG_FUNCTION_ARGS) 
{
	Lseg3D* ls1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D* ls2 = PG_GETARG_LSEG3D_P(1);
	PG_RETURN_BOOL(lseg3D_cmp_internal(ls1, ls2) >= 0);
}

PG_FUNCTION_INFO_V1(lseg3D_ge);

/***********************************************************************
 * Routines for 3D lines
 ***********************************************************************/

/*----------------------------------------------------------
 *	Input and output functions
 *---------------------------------------------------------*/

/*
 * line3D_in: Read in the Line3D from a string specification
 *
 * Input format: 
 *		"[(x1, y1, z1), (x2, y2, z2)]"
 *		"((x1, y1, z1), (x2, y2, z2))" 
 *		"[x1, y1, z1, x2, y2, z2]" 
 *		"(x1, y1, z1, x2, y2, z2)" 
 *		"(x1, y1, z1), (x2, y2, z2)"
 *		"x1, y1, z1, x2, y2, z2"
 * The two end points of a Lseg3D cannot be equal
 */
Datum
line3D_in(PG_FUNCTION_ARGS)
{
	char	   *str = PG_GETARG_CSTRING(0);
	Line3D	   *line = (Line3D *) palloc(sizeof(Line3D));
	Lseg3D		lseg;
	int			isopen;
	char	   *s;

	if ((!path3D_decode(TRUE, 2, str, &isopen, &s, &(line->p[0])))
		|| (*s != '\0'))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("Invalid input syntax for type line3D: \"%s\"", str)));

	if (PT3Deq(line->p[0], line->p[1]))
			ereport(ERROR,
					(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
					 errmsg("Invalid line3D specification: the two points cannot be equal")));

	PG_RETURN_LINE3D_P(line);
}

PG_FUNCTION_INFO_V1(line3D_in);

/*		
 * line3D_out: Convert internal Line3D representation to character string
 *
 * Output format:
 *		"((x1, y1, z1), (x2, y2, z2))"
 */
Datum
line3D_out(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	int			ndig = DBL_DIG + extra_float_digits;

	if (ndig < 1)
		ndig = 1;

	PG_RETURN_CSTRING(psprintf("((%.*g,%.*g,%.*g),(%.*g,%.*g,%.*g))", ndig, 
		line->p[0].x, ndig, line->p[0].y, ndig, line->p[0].z, ndig, 
		line->p[1].x, ndig, line->p[1].y, ndig, line->p[1].z));
}

PG_FUNCTION_INFO_V1(line3D_out);

/*
 * line3D_recv: Convert external binary representation to Line3D
 */
Datum
line3D_recv(PG_FUNCTION_ARGS)
{
	StringInfo	buf = (StringInfo) PG_GETARG_POINTER(0);
	Line3D	   *line = (Line3D *) palloc(sizeof(Line3D));

	line->p[0].x = pq_getmsgfloat8(buf);
	line->p[0].y = pq_getmsgfloat8(buf);
	line->p[0].z = pq_getmsgfloat8(buf);
	line->p[1].x = pq_getmsgfloat8(buf);
	line->p[1].y = pq_getmsgfloat8(buf);
	line->p[1].z = pq_getmsgfloat8(buf);

	PG_RETURN_LINE3D_P(line);
}

PG_FUNCTION_INFO_V1(line3D_recv);

/*
 * line3D_send: Convert Line3D to binary representation
 */
Datum
line3D_send(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	StringInfoData buf;

	pq_begintypsend(&buf);
	pq_sendfloat8(&buf, line->p[0].x);
	pq_sendfloat8(&buf, line->p[0].y);
	pq_sendfloat8(&buf, line->p[0].z);
	pq_sendfloat8(&buf, line->p[1].x);
	pq_sendfloat8(&buf, line->p[1].y);
	pq_sendfloat8(&buf, line->p[1].z);
	PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
}

PG_FUNCTION_INFO_V1(line3D_send);

/*----------------------------------------------------------
 *	Constructors
 *---------------------------------------------------------*/

 /* 
 * line3D_construct: Form a Line3D from 6 double
 */
Datum
line3D_construct(PG_FUNCTION_ARGS)
{
	double		x1 = PG_GETARG_FLOAT8(0);
	double		y1 = PG_GETARG_FLOAT8(1);
	double		z1 = PG_GETARG_FLOAT8(2);
	double		x2 = PG_GETARG_FLOAT8(3);
	double		y2 = PG_GETARG_FLOAT8(4);
	double		z2 = PG_GETARG_FLOAT8(5);
	Line3D	   *result = (Line3D *) palloc(sizeof(Line3D));

	result->p[0].x = x1;
	result->p[0].y = y1;
	result->p[0].z = z1;
	result->p[1].x = x2;
	result->p[1].y = y2;
	result->p[1].z = z2;

	PG_RETURN_LSEG3D_P(result);
}

PG_FUNCTION_INFO_V1(line3D_construct);

/* 
 * line3D_construct_pts: Form a Line3D from two Point3D
 */
Datum
line3D_construct_pts(PG_FUNCTION_ARGS)
{
	Point3D	   *point1 = PG_GETARG_POINT3D_P(0);
	Point3D	   *point2 = PG_GETARG_POINT3D_P(1);

	PG_RETURN_LSEG3D_P(line3D_construct_pts_internal(point1, point2));
}

PG_FUNCTION_INFO_V1(line3D_construct_pts);

Line3D *
line3D_construct_pts_internal(Point3D *point1, Point3D *point2)
{
	Line3D	   *result = (Line3D *) palloc(sizeof(Line3D));

	result->p[0] = *point1;
	result->p[1] = *point2;
	
	return result;
}	
	
/*----------------------------------------------------------
 *	Accessor functions
 *---------------------------------------------------------*/
 
/*
 * line3D_points: Return the points of the Line3D
 */
Datum 
line3D_points(PG_FUNCTION_ARGS) 
{
	Line3D     *line = PG_GETARG_LINE3D_P(0);
	Datum 	   *d = (Datum *) palloc(sizeof(Datum) * 2);
	ArrayType  *result;

	for (int i = 0; i < 2; i++)
		d[i] = Point3DPGetDatum(&line->p[i]);

	result = construct_array(d, 2, point3d_oid(), sizeof(Point3D), FALSE, 'd');

	PG_RETURN_ARRAYTYPE_P(result);
}

PG_FUNCTION_INFO_V1(line3D_points) ;

/* 
 * line3D_vertical: Is the Line3D parallel to the vertical plane 
 * defined by the y and z axes?
 */
Datum
line3D_vertical(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);

	PG_RETURN_BOOL(FPeq(line->p[0].x, line->p[1].x));
}

PG_FUNCTION_INFO_V1(line3D_vertical);

bool
line3D_vert(Line3D *line)
{
	return FPeq(line->p[0].x, line->p[1].x);
}

/* 
 * line3D_horizontal: Is the Line3D parallel to the horizontal plane 
 * defined by the x and z axes?
 */
Datum
line3D_horizontal(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);

	PG_RETURN_BOOL(FPeq(line->p[0].y, line->p[1].y));
}

PG_FUNCTION_INFO_V1(line3D_horizontal);

bool 
line3D_hori(Line3D *line)
{
	return FPeq(line->p[0].y, line->p[1].y);
}

/* 
 * line3D_perpendicular: Is the Line3D parallel to the perpendicular plane 
 * defined by the x and y axes?
 */
Datum
line3D_perpendicular(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);

	PG_RETURN_BOOL(FPeq(line->p[0].z, line->p[1].z));
}

PG_FUNCTION_INFO_V1(line3D_perpendicular);

bool
line3D_perp(Line3D *line)
{
	return FPeq(line->p[0].z, line->p[1].z);
}

/*----------------------------------------------------------
 *	Relative position routines
 *---------------------------------------------------------*/

/*
 * line3D_parallel: Are two line3D parallel and do not coincide?
 */
Datum
line3D_parallel(PG_FUNCTION_ARGS)
{
	Line3D	   *line1 = PG_GETARG_LINE3D_P(0);
	Line3D	   *line2 = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(line3D_parallel_internal(line1, line2));
}

PG_FUNCTION_INFO_V1(line3D_parallel);

/*
 * Two lines defined by their end points
 *     L1 = (x11, y11, z11), (x12, y12, z12)
 *     L2 = (x21, y21, z21), (x22, y22, z22)
 * have their direction vectors given by
 *     L1: (a1, b1, c1) = (x12-x11, y12-y11, z12-z11)
 *     L2: (a2, b2, c2) = (x22-x21, y22-y21, z22-z21)
 * Two lines are parallel if their direction vectors are 
 * proportional and they do not intersect. For intersection we use
 * http://geomalgorithms.com/a05-_intersect-1.html
 */
bool
line3D_parallel_internal(Line3D *line1, Line3D *line2)
{
	double		p;
	Point3D		u,
				v,
				w;
				
	u = sub(line1->p[1], line1->p[0]);
	v = sub(line2->p[1], line2->p[0]);
	w = sub(line2->p[0], line1->p[0]);
	
	// Are their direction vectors proportional?
	if (!proportional(u,v)) 
		return FALSE;

	// Do they intersect ?
	return !proportional(w,v);
}	

/*
 * line3D_coincide: Do the two line3D coincide?
 *
 * Two lines defined by their equation in the vector form
 *		L1 = <x1,y1,z1> + s<a1,b1,c1>
 *		L2 = <x2,y2,z2> + t<a2,b2,c2>
 * coincide if their direction vectors are proportional and if there 
 * exists a point P0 in L1 that is also in L2, i.e., there exists t0  
 * such that P0 = Q(t0). 
 * http://geomalgorithms.com/a05-_intersect-1.html
 */
Datum
line3D_coincide(PG_FUNCTION_ARGS)
{
	Line3D	   *line1 = PG_GETARG_LINE3D_P(0);
	Line3D	   *line2 = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(line3D_coincide_internal(line1, line2));
}

PG_FUNCTION_INFO_V1(line3D_coincide);

bool
line3D_coincide_internal(Line3D *line1, Line3D *line2)
{
	double		p;
	Point3D		u,
				v,
				w;
	
	u = sub(line1->p[1], line1->p[0]);
	v = sub(line2->p[1], line2->p[0]);
	w = sub(line1->p[0], line2->p[0]);
	
	// Are their direction vectors proportional?
	if (!proportional(u,v))
		return FALSE;

	// Do they intersect ?
	return proportional(w,v);
}

/*
 * line3D_skew: Are the two line3D skew?
 */
Datum
line3D_skew(PG_FUNCTION_ARGS)
{
	Line3D	   *line1 = PG_GETARG_LINE3D_P(0);
	Line3D	   *line2 = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(line3D_skew_internal(line1, line2));
}

PG_FUNCTION_INFO_V1(line3D_skew);

bool
line3D_skew_internal(Line3D *line1, Line3D *line2)
{
	/* Do the two line3D coincide? */
	if(line3D_coincide_internal(line1, line2))
		return FALSE;
	
	/* Are the two line3D parallel? */
	if(line3D_parallel_internal(line1, line2))
		return FALSE;
	
	/* The two line3D are skew if they do not intersect */
	return (!inter_line3D_line3D(line1, line2));
}

/* 
 * line3D_orthogonal:  Are the two lines orthogonal?
 * Two lines are orthogonal if the dot product of
 * their direction vectors is zero
 */
Datum
line3D_orthogonal(PG_FUNCTION_ARGS)
{ 
	Line3D	   *line1 = PG_GETARG_LINE3D_P(0);
	Line3D	   *line2 = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(FPzero(DOT(
		sub(line2->p[1], line2->p[0]),
		sub(line1->p[0], line2->p[0]))));
}

PG_FUNCTION_INFO_V1(line3D_orthogonal);

/*----------------------------------------------------------
 *	Relational routines 
 *---------------------------------------------------------*/

/*
 * line3D_same: Are the two Line3D the same, i.e., they represent the
 * same infinite same of points?
 */
Datum
line3D_same(PG_FUNCTION_ARGS)
{
	Line3D		   *line1 = PG_GETARG_LINE3D_P(0);
	Line3D		   *line2 = PG_GETARG_LINE3D_P(1);

	PG_RETURN_BOOL(line3D_coincide_internal(line1, line2));
}

PG_FUNCTION_INFO_V1(line3D_same);

bool
line3D_same_internal(Line3D	*line1, Line3D *line2)
{
	 return (line3D_coincide_internal(line1, line2));
}

/*
 * line3D_cmp_internal: Compare the two Line3D
 */
static int 
line3D_cmp_internal(Line3D* line1, Line3D* line2) 
{
	if(FPlt(line1->p[0].x, line2->p[0].x))
		return -1;
	if(FPgt(line1->p[0].x, line2->p[0].x))
		return 1;
	if(FPlt(line1->p[0].y, line2->p[0].y))
		return -1;
	if(FPgt(line1->p[0].y, line2->p[0].y))
		return 1;
	if(FPlt(line1->p[0].z, line2->p[0].z))
		return -1;
	if(FPgt(line1->p[0].z, line2->p[0].z))
		return 1;
	// the initial points are equal
	if(FPlt(line1->p[1].x, line2->p[1].x))
		return -1;
	if(FPgt(line1->p[1].x, line2->p[1].x))
		return 1;
	if(FPlt(line1->p[1].y, line2->p[1].y))
		return -1;
	if(FPgt(line1->p[1].y, line2->p[1].y))
		return 1;
	if(FPlt(line1->p[1].z, line2->p[1].z))
		return -1;
	if(FPgt(line1->p[1].z, line2->p[1].z))
		return 1;
	// the final points are also equal
	return 0;
}

Datum 
line3D_cmp(PG_FUNCTION_ARGS) 
{
	Line3D* line1 = PG_GETARG_LINE3D_P(0);
	Line3D* line2 = PG_GETARG_LINE3D_P(1);
	PG_RETURN_INT32(line3D_cmp_internal(line1, line2));
}

PG_FUNCTION_INFO_V1(line3D_cmp);

Datum 
line3D_lt(PG_FUNCTION_ARGS) 
{
	Line3D* line1 = PG_GETARG_LINE3D_P(0);
	Line3D* line2 = PG_GETARG_LINE3D_P(1);
	PG_RETURN_BOOL(line3D_cmp_internal(line1, line2) < 0);
}

PG_FUNCTION_INFO_V1(line3D_lt);

Datum 
line3D_le(PG_FUNCTION_ARGS) 
{
	Line3D* line1 = PG_GETARG_LINE3D_P(0);
	Line3D* line2 = PG_GETARG_LINE3D_P(1);
	PG_RETURN_BOOL(line3D_cmp_internal(line1, line2) <= 0);
}

PG_FUNCTION_INFO_V1(line3D_le);

Datum 
line3D_eq(PG_FUNCTION_ARGS) 
{
	Line3D* line1 = PG_GETARG_LINE3D_P(0);
	Line3D* line2 = PG_GETARG_LINE3D_P(1);
	PG_RETURN_BOOL(line3D_cmp_internal(line1, line2) == 0);
}

PG_FUNCTION_INFO_V1(line3D_eq);

Datum 
line3D_ne(PG_FUNCTION_ARGS) 
{
	Line3D* line1 = PG_GETARG_LINE3D_P(0);
	Line3D* line2 = PG_GETARG_LINE3D_P(1);
	PG_RETURN_BOOL(line3D_cmp_internal(line1, line2) != 0);
}

PG_FUNCTION_INFO_V1(line3D_ne);

Datum 
line3D_gt(PG_FUNCTION_ARGS) 
{
	Line3D* line1 = PG_GETARG_LINE3D_P(0);
	Line3D* line2 = PG_GETARG_LINE3D_P(1);
	PG_RETURN_BOOL(line3D_cmp_internal(line1, line2) > 0);
}

PG_FUNCTION_INFO_V1(line3D_gt);

Datum 
line3D_ge(PG_FUNCTION_ARGS) 
{
	Line3D* line1 = PG_GETARG_LINE3D_P(0);
	Line3D* line2 = PG_GETARG_LINE3D_P(1);
	PG_RETURN_BOOL(line3D_cmp_internal(line1, line2) >= 0);
}

PG_FUNCTION_INFO_V1(line3D_ge);

/***********************************************************************
 * Routines for 3D boxes
 ***********************************************************************/

/*----------------------------------------------------------
 *	Input and output functions
 *---------------------------------------------------------*/
 
/*
 * box3D_in: Read in the Box3D from a string specification
 *
 * Input format: (two corners of the box)
 * 		"((f8, f8, f8), (f8, f8, f8))"
 * 		"(f8, f8, f8), (f8, f8, f8)"
 * 		"(f8, f8, f8, f8, f8, f8)" 
 * 		"f8, f8, f8, f8, f8, f8" 
 * The two corners of a Box3D may be equal
 */
Datum
box3D_in(PG_FUNCTION_ARGS)
{
	char	   *str = PG_GETARG_CSTRING(0);
	Box3D	   *box;
	int			isopen;
	char	   *s;
	double		x,
				y,
				z;
				
	box = (Box3D *) palloc(sizeof(Box3D));

	if ((!path3D_decode(FALSE, 2, str, &isopen, &s, &(box->high)))
		|| (*s != '\0'))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("Invalid input syntax for type Box3D: \"%s\"", str)));

	/* reorder corners if necessary... */
	if (FPlt(box->high.x, box->low.x))
	{
		x = box->high.x;
		box->high.x = box->low.x;
		box->low.x = x;
	}
	if (FPlt(box->high.y, box->low.y))
	{
		y = box->high.y;
		box->high.y = box->low.y;
		box->low.y = y;
	}
	if (FPlt(box->high.z, box->low.z))
	{
		z = box->high.z;
		box->high.z = box->low.z;
		box->low.z = z;
	}

	PG_RETURN_BOX3D_P(box);
}

PG_FUNCTION_INFO_V1(box3D_in);

/*		
 * box3D_out: Convert internal Box3D representation to character string
 *
 * Output format: 
 * 		"(f8, f8, f8), (f8, f8, f8)"
 */
Datum
box3D_out(PG_FUNCTION_ARGS)
{
	Box3D		   *box = PG_GETARG_BOX3D_P(0);

	PG_RETURN_CSTRING(path3D_encode(PATH3D_NONE, 2, &(box->high)));
}

PG_FUNCTION_INFO_V1(box3D_out);

/*
 * box3D_recv: Convert external binary representation to Box3D
 */
Datum
box3D_recv(PG_FUNCTION_ARGS)
{
	StringInfo	buf = (StringInfo) PG_GETARG_POINTER(0);
	Box3D		*box = (Box3D *) palloc(sizeof(Box3D));
	double		x,
				y,
				z;

	box->high.x = pq_getmsgfloat8(buf);
	box->high.y = pq_getmsgfloat8(buf);
	box->high.z = pq_getmsgfloat8(buf);
	box->low.x = pq_getmsgfloat8(buf);
	box->low.y = pq_getmsgfloat8(buf);
	box->low.z = pq_getmsgfloat8(buf);

	/* reorder corners if necessary... */
	if (FPlt(box->high.x, box->low.x))
	{
		x = box->high.x;
		box->high.x = box->low.x;
		box->low.x = x;
	}
	if (FPlt(box->high.y, box->low.y))
	{
		y = box->high.y;
		box->high.y = box->low.y;
		box->low.y = y;
	}
	if (FPlt(box->high.z, box->low.z))
	{
		z = box->high.z;
		box->high.z = box->low.z;
		box->low.z = z;
	}

	PG_RETURN_BOX3D_P(box);
}

PG_FUNCTION_INFO_V1(box3D_recv);

/*
 * box3D_send: Convert Box3D to binary representation
 */
Datum
box3D_send(PG_FUNCTION_ARGS)
{
	Box3D		   *box = PG_GETARG_BOX3D_P(0);
	StringInfoData buf;

	pq_begintypsend(&buf);
	pq_sendfloat8(&buf, box->high.x);
	pq_sendfloat8(&buf, box->high.y);
	pq_sendfloat8(&buf, box->high.z);
	pq_sendfloat8(&buf, box->low.x);
	pq_sendfloat8(&buf, box->low.y);
	pq_sendfloat8(&buf, box->low.z);
	PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
}

PG_FUNCTION_INFO_V1(box3D_send);

/*----------------------------------------------------------
 *	Constructors
 *---------------------------------------------------------*/
 
/*		
 * box3D_construct: Form a Box3D from 6 double
 */
Datum
box3D_construct(PG_FUNCTION_ARGS)
{
	double		x1 = PG_GETARG_FLOAT8(0);
	double		y1 = PG_GETARG_FLOAT8(1);
	double		z1 = PG_GETARG_FLOAT8(2);
	double		x2 = PG_GETARG_FLOAT8(3);
	double		y2 = PG_GETARG_FLOAT8(4);
	double		z2 = PG_GETARG_FLOAT8(5);

	PG_RETURN_BOX3D_P(box3D_construct_internal(x1, y1, z1, x2, y2, z2));
}

PG_FUNCTION_INFO_V1(box3D_construct);

static Box3D *
box3D_construct_internal(double x1, double y1, double z1, 
	double x2, double y2, double z2)
{
	Box3D	   *result = (Box3D *) palloc(sizeof(Box3D));

	return box3D_fill(result, x1, y1, z1, x2, y2, z2);
}

/*
 * box3D_fill: Fill in a given box struct
 */
static Box3D *
box3D_fill(Box3D *result, double x1, double y1, double z1, 
	double x2, double y2, double z2)
{
	if (FPgt(x1, x2))
	{
		result->high.x = x1;
		result->low.x = x2;
	}
	else
	{
		result->high.x = x2;
		result->low.x = x1;
	}
	if (FPgt(y1, y2))
	{
		result->high.y = y1;
		result->low.y = y2;
	}
	else
	{
		result->high.y = y2;
		result->low.y = y1;
	}
	if (FPgt(z1, z2))
	{
		result->high.z = z1;
		result->low.z = z2;
	}
	else
	{
		result->high.z = z2;
		result->low.z = z1;
	}

	return result;
}

/*		
 * box3D_construct_pts: Form a Box3D from 2 Point3D
 */
Datum
box3D_construct_pts(PG_FUNCTION_ARGS)
{
	Point3D	   *point1 = PG_GETARG_POINT3D_P(0);
	Point3D	   *point2 = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOX3D_P(box3D_construct_internal(point1->x, point1->y, 
		point1->z, point2->x, point2->y, point2->z));
}

PG_FUNCTION_INFO_V1(box3D_construct_pts);

Box3D *
box3D_construct_pts_internal(Point3D *point1, Point3D *point2)
{
	return box3D_construct_internal(point1->x, point1->y, point1->z, 
		point2->x, point2->y, point2->z);
}

/*
 * box3D_copy: Form a Box3D from another Box3D
 */
static Box3D *
box3D_copy(Box3D *box)
{
	Box3D	   *result = (Box3D *) palloc(sizeof(Box3D));

	memcpy((char *) result, (char *) box, sizeof(Box3D));

	return result;
}

/*----------------------------------------------------------
 *	Accessor functions
 *---------------------------------------------------------*/

/*
 * box3D_high: Return the top corner of a Box3D
 */
Datum
box3D_high(PG_FUNCTION_ARGS)
{
	Box3D	   *b = PG_GETARG_BOX3D_P(0);
	Point3D	   *result = (Point3D *) palloc(sizeof(Point3D));
	
	*result = b->high;

	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(box3D_high);

/*
 * box3D_high: Return the low corner of a Box3D
 */
Datum
box3D_low(PG_FUNCTION_ARGS)
{
	Box3D	   *b = PG_GETARG_BOX3D_P(0);
	Point3D	   *result = (Point3D *) palloc(sizeof(Point3D));
	
	*result = b->low;

	PG_RETURN_POINT3D_P(result);	
}

PG_FUNCTION_INFO_V1(box3D_low);

/*
 * box3D_points: Return the points of the Box3D
 */
Datum 
box3D_points(PG_FUNCTION_ARGS) 
{
	Box3D      *b = PG_GETARG_BOX3D_P(0);
	Datum 	   *d = (Datum *) palloc(sizeof(Datum) * 2);
	ArrayType  *result;

	d[0] = Point3DPGetDatum(&b->high);
	d[1] = Point3DPGetDatum(&b->low);

	result = construct_array(d, 2, point3d_oid(), sizeof(Point3D), FALSE, 'd');

	PG_RETURN_ARRAYTYPE_P(result);
}

PG_FUNCTION_INFO_V1(box3D_points) ;

/*
 * box3D_volume: Return the volume of the Box3D
 */
Datum
box3D_volume(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);

	PG_RETURN_FLOAT8(box3D_vol(box));
}

PG_FUNCTION_INFO_V1(box3D_volume);

double
box3D_vol(Box3D *box)
{
	return box3D_wd(box) * box3D_ht(box) * box3D_dp(box);
}

/*
 * box3D_width: Return the width of the Box3D (horizontal magnitude)
 */
Datum
box3D_width(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);

	PG_RETURN_FLOAT8(box->high.x - box->low.x);
}

PG_FUNCTION_INFO_V1(box3D_width);

static double
box3D_wd(Box3D *box)
{
	return box->high.x - box->low.x;
}

/*
 * box3D_height: Return the height of the Box3D (vertical magnitude)
 */
Datum
box3D_height(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);

	PG_RETURN_FLOAT8(box->high.y - box->low.y);
}

PG_FUNCTION_INFO_V1(box3D_height);

static double
box3D_ht(Box3D *box)
{
	return box->high.y - box->low.y;
}

/*
 * box3D_depth: Return the depth of the Box3D (perpendicular magnitude)
 */
Datum
box3D_depth(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);

	PG_RETURN_FLOAT8(box->high.z - box->low.z);
}

PG_FUNCTION_INFO_V1(box3D_depth);

static double
box3D_dp(Box3D *box)
{
	return box->high.z - box->low.z;
}

/*
 * box3D_center: Return the center point of the Box3D
 */
Datum
box3D_center(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);

	PG_RETURN_POINT3D_P(box3D_cn(box));
}

PG_FUNCTION_INFO_V1(box3D_center);

Point3D *
box3D_cn(Box3D *box)
{
	Point3D	   *result = (Point3D *) palloc(sizeof(Point3D));

	result->x = (box->high.x + box->low.x) / 2.0;
	result->y = (box->high.y + box->low.y) / 2.0;
	result->z = (box->high.z + box->low.z) / 2.0;
	
	return result;
}

/*
 * box3D_diagonal: Return a Lseg3D which is the
 * positive-slope diagonal of the Box3D
 */
Datum
box3D_diagonal(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);

	PG_RETURN_LSEG3D_P(lseg3D_construct_pts_internal(&box->high, &box->low));
}

PG_FUNCTION_INFO_V1(box3D_diagonal);

/*
 * box3D_boundsphere: Minimum bounding sphere of the box
 */
Datum
box3D_boundsphere(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);

	PG_RETURN_SPHERE_P(box3D_bsphere(box));
}

PG_FUNCTION_INFO_V1(box3D_boundsphere);

Sphere *
box3D_bsphere(Box3D *box)
{
	Sphere	   *sphere = (Sphere *) palloc(sizeof(Sphere));

	sphere->center = scalar(0.5, add(box->high, box->low));
	sphere->radius = dist_point3D_point3D(&sphere->center, &box->high);

	return sphere;
}

/*----------------------------------------------------------
 *	"Arithmetic" routines on Box3D
 *---------------------------------------------------------*/

/*
 * box3D_add: Return the Box3D obtained by adding to the corners 
 * of the Box3D the Point3D 
 */
Datum
box3D_add(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Point3D	   *p = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOX3D_P(box3D_construct_internal(
		(box->high.x + p->x), (box->high.y + p->y),(box->high.z + p->z),
		(box->low.x + p->x), (box->low.y + p->y), (box->low.z + p->z)));
}

PG_FUNCTION_INFO_V1(box3D_add);

/*
 * box3D_sub: Return the Box3D obtained by subtracting to the corners 
 * of the Box3D the Point3D 
 */
Datum
box3D_sub(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	Point3D	   *p = PG_GETARG_POINT3D_P(1);

	PG_RETURN_BOX3D_P(box3D_construct_internal(
		(box->high.x - p->x), (box->high.y - p->y), (box->high.z - p->z),
		(box->low.x - p->x),(box->low.y - p->y), (box->low.z - p->z)));
}

PG_FUNCTION_INFO_V1(box3D_sub);

/*
 * box3D_mul: Return the Box3D obtained by multiplying the corners 
 * of the Box3D with the double
 */
Datum
box3D_mul(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	double		d = PG_GETARG_FLOAT8(1);
	Box3D	   *result;
	Point3D	   *high,
			   *low;

	result = box3D_construct_internal(high->x * d, high->y * d, high->z * d, 
		low->x * d, low->y * d, low->z * d);

	PG_RETURN_BOX3D_P(result);
}

PG_FUNCTION_INFO_V1(box3D_mul);

/*
 * box3D_div: Return the Box3D obtained by dividing the corners 
 * of the Box3D by the double
 */
Datum
box3D_div(PG_FUNCTION_ARGS)
{
	Box3D	   *box = PG_GETARG_BOX3D_P(0);
	double		d = PG_GETARG_FLOAT8(1);
	Box3D	   *result;
	Point3D	   *high,
			   *low;

	if (FPzero(d))
		ereport(ERROR,
				(errcode(ERRCODE_DIVISION_BY_ZERO),
				 errmsg("division by zero")));
				 
	result = box3D_construct_internal(high->x / d, high->y / d, high->z / d, 
		low->x / d, low->y / d, low->z / d);

	PG_RETURN_BOX3D_P(result);
}

PG_FUNCTION_INFO_V1(box3D_div);

/*
 * Smallest bounding box that includes both of the given boxes
 */
Datum
boxes3D_bound_box(PG_FUNCTION_ARGS)
{
	Box3D	   *box1 = PG_GETARG_BOX3D_P(0),
			   *box2 = PG_GETARG_BOX3D_P(1),
			   *container = (Box3D *) palloc(sizeof(Box3D));

	container->high.x = Max(box1->high.x, box2->high.x);
	container->low.x = Min(box1->low.x, box2->low.x);
	container->high.y = Max(box1->high.y, box2->high.y);
	container->low.y = Min(box1->low.y, box2->low.y);
	container->high.z = Max(box1->high.z, box2->high.z);
	container->low.z = Min(box1->low.z, box2->low.z);

	PG_RETURN_BOX3D_P(container);
}

PG_FUNCTION_INFO_V1(boxes3D_bound_box);

/*----------------------------------------------------------
 *	Relational routines 
 *---------------------------------------------------------*/

/*
 * box3D_same: are two boxes identical?
 */
Datum
box3D_same(PG_FUNCTION_ARGS)
{
	Box3D		   *box1 = PG_GETARG_BOX3D_P(0);
	Box3D		   *box2 = PG_GETARG_BOX3D_P(1);

	PG_RETURN_BOOL(box3D_same_internal(box1, box2));
}

PG_FUNCTION_INFO_V1(box3D_same);

bool
box3D_same_internal(Box3D *box1, Box3D *box2)
{
	return (PT3Deq(box1->high, box2->high) &&
		PT3Deq(box1->low, box2->low));
}

/*
 * box3D_cmp_internal: Compare the two Box3D
 */
static int
box3D_cmp_internal(Box3D* b1, Box3D* b2) 
{
	if(FPlt(b1->low.x, b2->low.x))
		return -1;
	if(FPgt(b1->low.x, b2->low.x))
		return 1;
	if(FPlt(b1->low.y, b2->low.y))
		return -1;
	if(FPgt(b1->low.y, b2->low.y))
		return 1;
	if(FPlt(b1->low.z, b2->low.z))
		return -1;
	if(FPgt(b1->low.z, b2->low.z))
		return 1;
	// the low points are equal
	if(FPlt(b1->high.x, b2->high.x))
		return -1;
	if(FPgt(b1->high.x, b2->high.x))
		return 1;
	if(FPlt(b1->high.y, b2->high.y))
		return -1;
	if(FPgt(b1->high.y, b2->high.y))
		return 1;
	if(FPlt(b1->high.z, b2->high.z))
		return -1;
	if(FPgt(b1->high.z, b2->high.z))
		return 1;
	// the high points are also equal
	return 0;
}

Datum 
box3D_cmp(PG_FUNCTION_ARGS) 
{
	Box3D* b1 = PG_GETARG_BOX3D_P(0);
	Box3D* b2 = PG_GETARG_BOX3D_P(1);
	PG_RETURN_INT32(box3D_cmp_internal(b1, b2));
}

PG_FUNCTION_INFO_V1(box3D_cmp);

Datum 
box3D_lt(PG_FUNCTION_ARGS) 
{
	Box3D* b1 = PG_GETARG_BOX3D_P(0);
	Box3D* b2 = PG_GETARG_BOX3D_P(1);
	PG_RETURN_BOOL(box3D_cmp_internal(b1, b2) < 0);
}

PG_FUNCTION_INFO_V1(box3D_lt);

Datum 
box3D_le(PG_FUNCTION_ARGS) 
{
	Box3D* b1 = PG_GETARG_BOX3D_P(0);
	Box3D* b2 = PG_GETARG_BOX3D_P(1);
	PG_RETURN_BOOL(box3D_cmp_internal(b1, b2) <= 0);
}

PG_FUNCTION_INFO_V1(box3D_le);

Datum 
box3D_eq(PG_FUNCTION_ARGS) 
{
	Box3D* b1 = PG_GETARG_BOX3D_P(0);
	Box3D* b2 = PG_GETARG_BOX3D_P(1);
	PG_RETURN_BOOL(box3D_cmp_internal(b1, b2) == 0);
}

PG_FUNCTION_INFO_V1(box3D_eq);

Datum 
box3D_ne(PG_FUNCTION_ARGS) 
{
	Box3D* b1 = PG_GETARG_BOX3D_P(0);
	Box3D* b2 = PG_GETARG_BOX3D_P(1);
	PG_RETURN_BOOL(box3D_cmp_internal(b1, b2) != 0);
}

PG_FUNCTION_INFO_V1(box3D_ne);

Datum 
box3D_gt(PG_FUNCTION_ARGS) 
{
	Box3D* b1 = PG_GETARG_BOX3D_P(0);
	Box3D* b2 = PG_GETARG_BOX3D_P(1);
	PG_RETURN_BOOL(box3D_cmp_internal(b1, b2) > 0);
}

PG_FUNCTION_INFO_V1(box3D_gt);

Datum 
box3D_ge(PG_FUNCTION_ARGS) 
{
	Box3D* b1 = PG_GETARG_BOX3D_P(0);
	Box3D* b2 = PG_GETARG_BOX3D_P(1);
	PG_RETURN_BOOL(box3D_cmp_internal(b1, b2) >= 0);
}

PG_FUNCTION_INFO_V1(box3D_ge);

/***********************************************************************
 * Routines for 3D paths
 ***********************************************************************/

/*----------------------------------------------------------
 *	Input and output functions
 *---------------------------------------------------------*/
 
/* 
 * path3D_make_bbox: Make the smallest bounding box for the Path3D
 */
static void
path3D_make_bbox(Path3D *path)
{
	int			i;
	double		x1,
				y1,
				z1,
				x2,
				y2,
				z2;

	if (path->npts > 0)
	{
		x2 = x1 = path->p[0].x;
		y2 = y1 = path->p[0].y;
		z2 = z1 = path->p[0].z;
		for (i = 1; i < path->npts; i++)
		{
			if (FPlt(path->p[i].x, x1))
				x1 = path->p[i].x;
			if (FPgt(path->p[i].x, x2))
				x2 = path->p[i].x;
			if (FPlt(path->p[i].y, y1))
				y1 = path->p[i].y;
			if (FPgt(path->p[i].y, y2))
				y2 = path->p[i].y;
			if (FPlt(path->p[i].z, z1))
				z1 = path->p[i].z;
			if (FPgt(path->p[i].z, z2))
				z2 = path->p[i].z;
		}

		box3D_fill(&(path->boundbox), x1, y1, z1, x2, y2, z2);
	}
	else
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				 errmsg("Cannot create bounding box for empty path3D")));
}

/*
 * path3D_in: Read in the Path3D from a string specification
 *
 * Input format: Open paths are enclosed by '[...]',  
 * closed paths by '(...)' and are the default
 *		"[(x0,y0,z0),...,(xn,yn,zn)]" 
 *		"((x0,y0,z0),...,(xn,yn,zn))" 
 *		"[x0,y0,z0,...,xn,yn,zn]"
 *		"(x0,y0,z0,...,xn,yn,zn)"
 *		"x0,y0,z0,...,xn,yn,zn"
 * A path must contain at least one point
 * A closed path must contain at least 3 non collinear points 
 */
Datum
path3D_in(PG_FUNCTION_ARGS)
{
	char	   *str = PG_GETARG_CSTRING(0);
	Path3D	   *path;
	int			isopen;
	char	   *s;
	int			npts;
	int			size;
	int			base_size;
	int			depth = 0;
	
	if ((npts = triple_count(str, ',')) <= 0)
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("Invalid input syntax for type path3D: \"%s\"", str)));

	s = str;
	while (isspace((unsigned char) *s))
		s++;

	/* skip single leading paren */
	if ((*s == LDELIM) && (strrchr(s, LDELIM) == s))
	{
		s++;
		depth++;
	}

	base_size = sizeof(path->p[0]) * npts;
	size = offsetof(Path3D, p) + base_size;

	/* Check for integer overflow */
	if (base_size / npts != sizeof(path->p[0]) || size <= base_size)
		ereport(ERROR,
				(errcode(ERRCODE_PROGRAM_LIMIT_EXCEEDED),
				 errmsg("Too many points requested")));

	path = (Path3D *) palloc(size);

	SET_VARSIZE(path, size);
	path->npts = npts;

	if ((!path3D_decode(TRUE, npts, s, &isopen, &s, &(path->p[0])))
	&& (!((depth == 0) && (*s == '\0'))) && !((depth >= 1) && (*s == RDELIM)))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("Invalid input syntax for type path3D: \"%s\"", str)));

	/* Check for at least 3 non collinear points for closed paths */
	if (!isopen && !plist_test3noncollinearpts(path->p, npts))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("Invalid path3D specification: a closed path requires at least 3 non collinear points")));

	 path->closed = (!isopen);
	/* prevent instability in unused pad bytes */
	path->dummy = 0;
	
	path3D_make_bbox(path);

	PG_RETURN_PATH3D_P(path);
}

PG_FUNCTION_INFO_V1(path3D_in) ;

/*		
 * path3D_out: Convert internal Path3D representation to character string
 *
 * Output format:
 *		"[(x0,y0,z0),...,(xn,yn,zn)]" (closed)
 *		"((x0,y0,z0),...,(xn,yn,zn))" (open)
 */
Datum
path3D_out(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);

	PG_RETURN_CSTRING(path3D_encode(path->closed ? PATH3D_CLOSED : PATH3D_OPEN, path->npts, path->p));
}

PG_FUNCTION_INFO_V1(path3D_out) ;

/*
 * path3D_recv: Convert external binary representation to Path3D
 *
 * External representation is closed flag (a boolean byte), int32 number
 * of points, and the points.
 */
Datum
path3D_recv(PG_FUNCTION_ARGS)
{
	StringInfo	buf = (StringInfo) PG_GETARG_POINTER(0);
	Path3D	   *path;
	int			closed;
	int32		npts;
	int32		i;
	int			size;

	closed = pq_getmsgbyte(buf);
	npts = pq_getmsgint(buf, sizeof(int32));
	if (npts <= 0 || npts >= (int32) ((INT_MAX - offsetof(Path3D, p)) / sizeof(Point3D)))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_BINARY_REPRESENTATION),
			 errmsg("Invalid number of points in external \"path3D\" value")));

	size = offsetof(Path3D, p) +sizeof(path->p[0]) * npts;
	path = (Path3D *) palloc(size);

	SET_VARSIZE(path, size);
	path->npts = npts;
	path->closed = (closed ? 1 : 0);
	/* prevent instability in unused pad bytes */
	path->dummy = 0;

	for (i = 0; i < npts; i++)
	{
		path->p[i].x = pq_getmsgfloat8(buf);
		path->p[i].y = pq_getmsgfloat8(buf);
		path->p[i].z = pq_getmsgfloat8(buf);
	}

	path3D_make_bbox(path);
	
	PG_RETURN_PATH3D_P(path);
}

PG_FUNCTION_INFO_V1(path3D_recv) ;

/*
 * path3D_send: Convert Path3D to binary representation
 */
Datum
path3D_send(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	StringInfoData buf;
	int32		i;

	pq_begintypsend(&buf);
	pq_sendbyte(&buf, path->closed ? 1 : 0);
	pq_sendint(&buf, path->npts, sizeof(int32));
	for (i = 0; i < path->npts; i++)
	{
		pq_sendfloat8(&buf, path->p[i].x);
		pq_sendfloat8(&buf, path->p[i].y);
		pq_sendfloat8(&buf, path->p[i].z);
	}
	PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
}

PG_FUNCTION_INFO_V1(path3D_send) ;

/* 
 * path3D_copy: Form a Path3D from another Path3D
 */
Path3D *
path3D_copy(Path3D *path)
{
	if (!PointerIsValid(path))
		return NULL;
	else
	{
		Path3D	   *result;
		int			size;

		size = VARSIZE(path) ;
		result = (Path3D *) palloc(size);
		memcpy(result, path, size);

		return result;
	}
}

/*----------------------------------------------------------
 *	Accessor functions
 *---------------------------------------------------------*/

/*
 * path3D_npoints: Return the number of points of the Path3D
 */
Datum
path3D_npoints(PG_FUNCTION_ARGS)
{
	Path3D	     *path = PG_GETARG_PATH3D_P(0);

	PG_RETURN_INT32(path->npts);
}

PG_FUNCTION_INFO_V1(path3D_npoints);

/*
 * path3D_points: Return the points of the Path3D
 */
Datum 
path3D_points(PG_FUNCTION_ARGS) 
{
	Path3D     *path = PG_GETARG_PATH3D_P(0);
	Datum 	   *d = (Datum *) palloc(sizeof(Datum) * path->npts);
	ArrayType  *result;

	for (int i = 0; i < path->npts; i++)
		d[i] = Point3DPGetDatum(&path->p[i]);

	result = construct_array(d, path->npts, point3d_oid(), sizeof(Point3D), FALSE, 'd');

	PG_RETURN_ARRAYTYPE_P(result);
}

PG_FUNCTION_INFO_V1(path3D_points) ;

/*
 * path3D_isclosed: Is the Path3D closed?
 */
Datum
path3D_isclosed(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);

	PG_RETURN_BOOL(path->closed);
}

PG_FUNCTION_INFO_V1(path3D_isclosed) ;

/*
 * path3D_isopen: Is the Path3D open?
 */
Datum
path3D_isopen(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);

	PG_RETURN_BOOL(!path->closed);
}

PG_FUNCTION_INFO_V1(path3D_isopen) ;

/*
 * path3D_length: Return the length (perimeter) of the Path3D
 */
Datum
path3D_length(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);

	PG_RETURN_FLOAT8(path3D_length_internal(path));
}

double
path3D_length_internal(Path3D *path)
{
	double		result = 0.0;
	int			i,
				iprev;

	for (i = 0; i < path->npts; i++)
	{
		if (i > 0)
			iprev = i - 1;
		else
		{
			if (!path->closed)
				continue;
			iprev = path->npts - 1;		/* include the closure segment */
		}
		result += dist_point3D_point3D(&path->p[iprev], &path->p[i]);
	}

	return result;
}

/* 
 * path3D_isplanar: Is the Path3D planar?
 */
Datum
path3D_isplanar(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	int			i;
	
	i = plist_isplanar(path->npts, path->p);
	if (i < 0)
		PG_RETURN_NULL();
	else if (i == 0)			 
		PG_RETURN_BOOL(FALSE);
	else
		PG_RETURN_BOOL(TRUE);		
}

PG_FUNCTION_INFO_V1(path3D_isplanar) ;

static bool
path3D_isplanar_internal(Path3D *path)
{
	return (plist_isplanar(path->npts, path->p) == 1);	
}

/*
 * path3D_area: Return the area of a Path3D
 */
Datum
path3D_area(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	double		result;
	
	result = path3D_area_internal(path);
	if (result == DBL_MAX)
		PG_RETURN_NULL();
		
	PG_RETURN_FLOAT8(result);
}

PG_FUNCTION_INFO_V1(path3D_area) ;

/*
 * The path3D should be closed and planar so the calculation makes sense.
 * The algorithm projects the polygon onto a 2D plane. Then, the area can 
 * be computed in 2D similarly to the path_area function for 2D paths, 
 * and the 3D area is recovered by using an area scaling factor. 
 * http://geomalgorithms.com/a01-_area.html
 */
double
path3D_area_internal(Path3D *path)
{
	double		area = 0.0,
				magn;
	Point3D		normal;
    int  		coord,		// coord to ignore: 1=x, 2=y, 3=z
				i,
				j,
				pos1,		// position in the array path->p of 3 non collinear points
				pos2,
				pos3;

	if (!path->closed || !path3D_isplanar_internal(path))
		return DBL_MAX;

	plist_find3noncollinearpts(path->p, path->npts, &pos1, &pos2, &pos3);
	normal = cross(sub(path->p[pos2],path->p[pos1]), sub(path->p[pos3],path->p[pos1]));	
    magn = magnitude(normal); // magnitude of normal vector

    coord = 3; // ignore z-coord
    if (FPgt(abs(normal.x),abs(normal.y))) 
	{
        if (FPgt(abs(normal.x),abs(normal.z))) 
			coord = 1;   // ignore x-coord
    }
    else if (FPgt(abs(normal.y),abs(normal.z))) 
			coord = 2;  // ignore y-coord

    // compute area of the 2D projection
    switch (coord) 
	{
		case 1:		
			for (i = 0; i < path->npts; i++)
			{
				j = (i + 1) % path->npts;
				area += path->p[i].y * path->p[j].z;
				area -= path->p[i].z * path->p[j].y;
			}
			break;
		case 2:		
			for (i = 0; i < path->npts; i++)
			{
				j = (i + 1) % path->npts;
				area += path->p[i].x * path->p[j].z;
				area -= path->p[i].z * path->p[j].x;
			}
			break;
		case 3:		
			for (i = 0; i < path->npts; i++)
			{
				j = (i + 1) % path->npts;
				area += path->p[i].x * path->p[j].y;
				area -= path->p[i].y * path->p[j].x;
			}
			break;
	}

    // scale to get area before projection
    switch (coord) 
	{
      case 1:
        area *= (magn / (2 * normal.x));
        break;
      case 2:
        area *= (magn / (2 * normal.y));
        break;
      case 3:
        area *= (magn / (2 * normal.z));
    }	
	
	return (FPlt(area, 0.0) ? -area : area);
}

/*
 * path3D_center: Return the center of a Path3D
 */
Datum
path3D_center(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Point3D	   *result = (Point3D *) palloc(sizeof(Point3D));
	int			i;

	result->x = 0;
	result->y = 0;
	result->z = 0;

	for (i = 0; i < path->npts; i++)
	{
		result->x += path->p[i].x;
		result->y += path->p[i].y;
		result->z += path->p[i].z;
	}
	result->x /= path->npts;
	result->y /= path->npts;
	result->z /= path->npts;

	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(path3D_center);

/*
 * path3D_boundbox: Return the bounding box of the Path3D
 */
Datum
path3D_boundbox(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Box3D	   *box = (Box3D *) palloc(sizeof(Box3D));

	box->high = path->boundbox.high;
	box->low = path->boundbox.low;
	PG_RETURN_BOX3D_P(box);
}

PG_FUNCTION_INFO_V1(path3D_boundbox);

/*
 * path3D_boundsphere: Return the bounding sphere of the Path3D
 */
Datum
path3D_boundsphere(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	
	PG_RETURN_SPHERE_P(path3D_bsphere(path));
}

PG_FUNCTION_INFO_V1(path3D_boundsphere);

Sphere * 
path3D_bsphere(Path3D *path)
{
	int			i;
	Sphere	   *sphere = (Sphere *) palloc(sizeof(Sphere));

	sphere->center.x = 0;
	sphere->center.y = 0;
	sphere->center.z = 0;
	sphere->radius = 0;

	for (i = 0; i < path->npts; i++)
	{
		sphere->center.x += path->p[i].x;
		sphere->center.y += path->p[i].y;
		sphere->center.z += path->p[i].z;
	}
	sphere->center.x /= path->npts;
	sphere->center.y /= path->npts;
	sphere->center.z /= path->npts;

	for (i = 0; i < path->npts; i++)
		sphere->radius += dist_point3D_point3D(&path->p[i], &sphere->center);
	sphere->radius /= path->npts;

	return sphere;
}

/*----------------------------------------------------------
 *	Relational routines 
 *---------------------------------------------------------*/

/*
 * path3D_same: Are the two Path3D the same, i.e., are all the points the same?
 * Check all points for matches in both forward and reverse direction
 * since path are non-directional and are open/closed shapes
 */
Datum
path3D_same(PG_FUNCTION_ARGS)
{
	Path3D     *path1 = PG_GETARG_PATH3D_P(0);
	Path3D     *path2 = PG_GETARG_PATH3D_P(1);
	bool		result;

	result = path3D_same_internal(path1, path2);

	// Avoid leaking memory for toasted inputs. Needed for rtree indexes
	PG_FREE_IF_COPY(path1, 0);
	PG_FREE_IF_COPY(path2, 1);

	PG_RETURN_BOOL(result);
}

bool
path3D_same_internal(Path3D *path1, Path3D *path2)
{
	if (path1->npts != path2->npts)
		return FALSE;
	else
		return plist_same(path1->npts, path1->p, path2->p);
}

/*
 * path3D_cmp_internal: Compare the two Path3D
 */
static int 
path3D_cmp_internal(Path3D* p1, Path3D* p2) 
{
	int 		npts = p1->npts < p2->npts ? p1->npts : p2->npts,
				result = 0 ;
	Lseg3D		seg1,
				seg2;
				
    for(int i = 1 ; i < npts ; i ++) 
	{
		lseg3D_construct_pts_stat(&seg1, p1->p[i-1], p1->p[i]);
		lseg3D_construct_pts_stat(&seg2, p2->p[i-1], p2->p[i]);
		result = lseg3D_cmp_internal(&seg1, &seg2) ;
		if (result) 
			break ;
	}
	// The first npts segments of both paths are equal
	if (!result) 
		if (npts < p1->npts) // p1 has more segments than p2
			result = 1 ;
		else if (npts < p2->npts) // p2 has more segments than p1
			result = -1 ;
		else
			result = 0 ;
	return result;
}

Datum 
path3D_cmp(PG_FUNCTION_ARGS) 
{
	Path3D	   *p1 = PG_GETARG_PATH3D_P(0);
	Path3D	   *p2 = PG_GETARG_PATH3D_P(1);
    int 		cmp = path3D_cmp_internal(p1, p2) ;
    
	PG_FREE_IF_COPY(p1, 0) ;
    PG_FREE_IF_COPY(p2, 1) ;
	
	PG_RETURN_INT32(cmp);
}

PG_FUNCTION_INFO_V1(path3D_cmp);

PGDLLEXPORT Datum 
path3D_lt(PG_FUNCTION_ARGS) 
{
	Path3D	   *p1 = PG_GETARG_PATH3D_P(0);
	Path3D	   *p2 = PG_GETARG_PATH3D_P(1);
    int 		cmp = path3D_cmp_internal(p1, p2) ;
	
    PG_FREE_IF_COPY(p1, 0) ;
    PG_FREE_IF_COPY(p2, 1) ;
	
	PG_RETURN_BOOL(cmp < 0);
}

PG_FUNCTION_INFO_V1(path3D_lt);

PGDLLEXPORT Datum 
path3D_le(PG_FUNCTION_ARGS) 
{
	Path3D	   *p1 = PG_GETARG_PATH3D_P(0);
	Path3D	   *p2 = PG_GETARG_PATH3D_P(1);
    int 		cmp = path3D_cmp_internal(p1, p2) ;
	
    PG_FREE_IF_COPY(p1, 0) ;
    PG_FREE_IF_COPY(p2, 1) ;
	
	PG_RETURN_BOOL(cmp <= 0);
}

PG_FUNCTION_INFO_V1(path3D_le);

PGDLLEXPORT Datum 
path3D_eq(PG_FUNCTION_ARGS) 
{
	Path3D	   *p1 = PG_GETARG_PATH3D_P(0);
	Path3D	   *p2 = PG_GETARG_PATH3D_P(1);
    int 		cmp = path3D_cmp_internal(p1, p2) ;
    
	PG_FREE_IF_COPY(p1, 0) ;
    PG_FREE_IF_COPY(p2, 1) ;
	
	PG_RETURN_BOOL(cmp == 0);
}

PG_FUNCTION_INFO_V1(path3D_eq);

PGDLLEXPORT Datum 
path3D_ne(PG_FUNCTION_ARGS) 
{
	Path3D	   *p1 = PG_GETARG_PATH3D_P(0);
	Path3D	   *p2 = PG_GETARG_PATH3D_P(1);
    int 		cmp = path3D_cmp_internal(p1, p2) ;
    
	PG_FREE_IF_COPY(p1, 0) ;
    PG_FREE_IF_COPY(p2, 1) ;
	
	PG_RETURN_BOOL(cmp != 0);
}

PG_FUNCTION_INFO_V1(path3D_ne);

PGDLLEXPORT Datum 
path3D_gt(PG_FUNCTION_ARGS) 
{
	Path3D	   *p1 = PG_GETARG_PATH3D_P(0);
	Path3D	   *p2 = PG_GETARG_PATH3D_P(1);
    int 		cmp = path3D_cmp_internal(p1, p2) ;
    
	PG_FREE_IF_COPY(p1, 0) ;
    PG_FREE_IF_COPY(p2, 1) ;
	
	PG_RETURN_BOOL(cmp > 0);
}

PG_FUNCTION_INFO_V1(path3D_gt);

PGDLLEXPORT Datum 
path3D_ge(PG_FUNCTION_ARGS) 
{
	Path3D	   *p1 = PG_GETARG_PATH3D_P(0);
	Path3D	   *p2 = PG_GETARG_PATH3D_P(1);
    int 		cmp = path3D_cmp_internal(p1, p2) ;
    
	PG_FREE_IF_COPY(p1, 0) ;
    PG_FREE_IF_COPY(p2, 1) ;
	
	PG_RETURN_BOOL(cmp >= 0);
}

PG_FUNCTION_INFO_V1(path3D_ge);

/*----------------------------------------------------------
 * Conversion routines
 *---------------------------------------------------------*/

/*
 * path3D_close: Return the Path3D obtained by closing the given Path3D
 */
Datum
path3D_close(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P_COPY(0);

	/* Check for at least 3 non collinear points for closed paths */
	if (!plist_test3noncollinearpts(path->p, path->npts))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("Invalid path3D specification: a closed path requires at least 3 non collinear points")));
				 
	path->closed = TRUE;

	PG_RETURN_PATH3D_P(path);
}

PG_FUNCTION_INFO_V1(path3D_close) ;

/*
 * path3D_open: Return the Path3D obtained by opening the given Path3D
 */
Datum
path3D_open(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P_COPY(0);

	path->closed = FALSE;

	PG_RETURN_PATH3D_P(path);
}

PG_FUNCTION_INFO_V1(path3D_open) ;

/*
 * path3D_poly3D: Convert the Path3D to a Polygon3D
 */
Datum
path3D_poly3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Polygon3D  *poly;
	int			size;
	int			i;

	if (!path->closed)
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				 errmsg("Open Path3D cannot be converted to Polygon3D")));

	/*
	 * Never overflows: the old size fit in MaxAllocSize, and the new size is
	 * just a small constant larger.
	 */
	size = offsetof(Polygon3D, p) +sizeof(poly->p[0]) * path->npts;
	poly = (Polygon3D *) palloc(size);

	SET_VARSIZE(poly, size);
	poly->npts = path->npts;

	for (i = 0; i < path->npts; i++)
	{
		poly->p[i].x = path->p[i].x;
		poly->p[i].y = path->p[i].y;
		poly->p[i].z = path->p[i].z;
	}

	poly3D_make_bbox(poly);

	PG_RETURN_POLYGON3D_P(poly);
}

PG_FUNCTION_INFO_V1(path3D_poly3D);

/* 
 * path3D_concat: Concatenate two paths (only if they are both open)
 */
Datum
path3D_concat(PG_FUNCTION_ARGS)
{
	Path3D	   *p1 = PG_GETARG_PATH3D_P(0);
	Path3D	   *p2 = PG_GETARG_PATH3D_P(1);
	Path3D	   *result;
	int			size,
				base_size;
	int			i;

	if (p1->closed || p2->closed)
		PG_RETURN_NULL();

	base_size = sizeof(p1->p[0]) * (p1->npts + p2->npts);
	size = offsetof(Path3D, p) +base_size;

	/* Check for integer overflow */
	if (base_size / sizeof(p1->p[0]) != (p1->npts + p2->npts) ||
		size <= base_size)
		ereport(ERROR,
				(errcode(ERRCODE_PROGRAM_LIMIT_EXCEEDED),
				 errmsg("Too many points requested")));

	result = (Path3D *) palloc(size);

	SET_VARSIZE(result, size);
	result->npts = (p1->npts + p2->npts);
	result->closed = p1->closed;
	/* prevent instability in unused pad bytes */
	result->dummy = 0;

	for (i = 0; i < p1->npts; i++)
	{
		result->p[i].x = p1->p[i].x;
		result->p[i].y = p1->p[i].y;
		result->p[i].z = p1->p[i].z;
	}
	for (i = 0; i < p2->npts; i++)
	{
		result->p[i + p1->npts].x = p2->p[i].x;
		result->p[i + p1->npts].y = p2->p[i].y;
		result->p[i + p1->npts].z = p2->p[i].z;
	}

	PG_RETURN_PATH3D_P(result);
}

PG_FUNCTION_INFO_V1(path3D_concat);

/*----------------------------------------------------------
 *	"Arithmetic" routines on Path3D
 *---------------------------------------------------------*/

/* 
 * path3D_add: Return the Path3D obtained by adding to the vertices 
 * of the Path3D the Point3D 
 */
Datum
path3D_add(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P_COPY(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);
	int			i;

	for (i = 0; i < path->npts; i++)
	{
		path->p[i].x += point->x;
		path->p[i].y += point->y;
		path->p[i].z += point->z;
	}

	PG_RETURN_PATH3D_P(path);
}

PG_FUNCTION_INFO_V1(path3D_add);

/* 
 * path3D_sub: Return the Path3D obtained by subtracting to the vertices 
 * of the Path3D the Point3D 
 */
Datum
path3D_sub(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P_COPY(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);
	int			i;

	for (i = 0; i < path->npts; i++)
	{
		path->p[i].x -= point->x;
		path->p[i].y -= point->y;
		path->p[i].z -= point->z;
	}

	PG_RETURN_PATH3D_P(path);
}

PG_FUNCTION_INFO_V1(path3D_sub);

/* 
 * path3D_mul: Return the Path3D obtained by multiplying the vertices 
 * of the Path3D with the double
 */
Datum
path3D_mul(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P_COPY(0);
	double		d = PG_GETARG_FLOAT8(1);
	Point3D	   *p;
	int			i;

	for (i = 0; i < path->npts; i++)
	{
		path->p[i].x *= d;
		path->p[i].y *= d;
		path->p[i].z *= d;
	}

	PG_RETURN_PATH3D_P(path);
}

PG_FUNCTION_INFO_V1(path3D_mul);

/* 
 * path3D_div: Return the Path3D obtained by dividing the vertices 
 * of the Path3D by the double
 */
Datum
path3D_div(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P_COPY(0);
	double		d = PG_GETARG_FLOAT8(1);
	Point3D	   *p;
	int			i;

	if (FPzero(d))
		ereport(ERROR,
				(errcode(ERRCODE_DIVISION_BY_ZERO),
				 errmsg("Division by zero")));
				 
	for (i = 0; i < path->npts; i++)
	{
		path->p[i].x /= d;
		path->p[i].y /= d;
		path->p[i].z /= d;
	}

	PG_RETURN_PATH3D_P(path);
}

PG_FUNCTION_INFO_V1(path3D_div);

/***********************************************************************
 * Routines for 3D polygons
 ***********************************************************************/

/*----------------------------------------------------------
 *	Input and output functions
 *---------------------------------------------------------*/
 
/* 
 * poly3D_make_bbox:
 * Make the smallest bounding box for the given polygon.
 */
void
poly3D_make_bbox(Polygon3D *poly)
{
	int			i;
	double		x1,
				y1,
				z1,
				x2,
				y2,
				z2;

	if (poly->npts > 0)
	{
		x2 = x1 = poly->p[0].x;
		y2 = y1 = poly->p[0].y;
		z2 = z1 = poly->p[0].z;
		for (i = 1; i < poly->npts; i++)
		{
			if (FPlt(poly->p[i].x, x1))
				x1 = poly->p[i].x;
			if (FPgt(poly->p[i].x, x2))
				x2 = poly->p[i].x;
			if (FPlt(poly->p[i].y, y1))
				y1 = poly->p[i].y;
			if (FPgt(poly->p[i].y, y2))
				y2 = poly->p[i].y;
			if (FPlt(poly->p[i].z, z1))
				z1 = poly->p[i].z;
			if (FPgt(poly->p[i].z, z2))
				z2 = poly->p[i].z;
		}

		box3D_fill(&(poly->boundbox), x1, y1, z1, x2, y2, z2);
	}
	else
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				 errmsg("Cannot create bounding box for empty polygon3D")));
}

/*
 * poly3D_in: Read in the Polygon3D from a string specification
 *
 * Input format:
 *		"((x0,y0,z0),...,(xn,yn,zn))" 
 *		"(x0,y0,z0,...,xn,yn,zn)"
 *		"x0,y0,z0,...,xn,yn,zn"
 * Since polygons are closed, they must contain at least 3 non collinear points 
 */

Datum
poly3D_in(PG_FUNCTION_ARGS)
{
	char	   *str = PG_GETARG_CSTRING(0);
	Polygon3D  *poly;
	int			npts;
	int			size;
	int			base_size;
	int			isopen;
	char	   *s;

	if ((npts = triple_count(str, ',')) <= 0)
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
			  errmsg("Invalid input syntax for type polygon3D: \"%s\"", str)));

	base_size = sizeof(poly->p[0]) * npts;
	size = offsetof(Polygon3D, p) + base_size;

	/* Check for integer overflow */
	if (base_size / npts != sizeof(poly->p[0]) || size <= base_size)
		ereport(ERROR,
				(errcode(ERRCODE_PROGRAM_LIMIT_EXCEEDED),
				 errmsg("Too many points requested")));

	poly = (Polygon3D *) palloc0(size);	/* zero any holes */

	SET_VARSIZE(poly, size);
	poly->npts = npts;

	if ((!path3D_decode(FALSE, npts, str, &isopen, &s, &(poly->p[0])))
		|| (*s != '\0'))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
			  errmsg("Invalid input syntax for type polygon3D: \"%s\"", str)));

	/* Check for at least 3 non collinear points */
	if (npts < 3 || !plist_test3noncollinearpts(poly->p, npts))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("Invalid polygon3D specification: a polygon requires at least 3 non collinear points")));
			  
	poly3D_make_bbox(poly);

	PG_RETURN_POLYGON3D_P(poly);
}

PG_FUNCTION_INFO_V1(poly3D_in);

/*		
 * poly3D_out: Convert internal Polygon3D representation to character string
 * 
 * Output format:
 *		"((x0,y0,z0),...,(xn,yn,zn))"  
 */
Datum
poly3D_out(PG_FUNCTION_ARGS)
{
	Polygon3D    *poly = PG_GETARG_POLYGON3D_P(0);

	PG_RETURN_CSTRING(path3D_encode(PATH3D_CLOSED, poly->npts, poly->p));
}

PG_FUNCTION_INFO_V1(poly3D_out);

/*
 * poly3D_recv: Convert external binary representation to Polygon3D
 *
 * External representation is int32 number of points, and the points.
 * We recompute the bounding box on read, instead of trusting it to
 * be valid.  (Checking it would take just as long, so may as well
 * omit it from external representation.)
 */
Datum
poly3D_recv(PG_FUNCTION_ARGS)
{
	StringInfo	buf = (StringInfo) PG_GETARG_POINTER(0);
	Polygon3D  *poly;
	int32		npts;
	int32		i;
	int			size;

	npts = pq_getmsgint(buf, sizeof(int32));
	if (npts <= 0 || npts >= (int32) ((INT_MAX - offsetof(Polygon3D, p)) / sizeof(Point3D)))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_BINARY_REPRESENTATION),
		  errmsg("Invalid number of points in external \"polygon3D\" value")));

	size = offsetof(Polygon3D, p) +sizeof(poly->p[0]) * npts;
	poly = (Polygon3D *) palloc0(size);	/* zero any holes */

	SET_VARSIZE(poly, size);
	poly->npts = npts;

	for (i = 0; i < npts; i++)
	{
		poly->p[i].x = pq_getmsgfloat8(buf);
		poly->p[i].y = pq_getmsgfloat8(buf);
		poly->p[i].z = pq_getmsgfloat8(buf);
	}

	poly3D_make_bbox(poly);

	PG_RETURN_POLYGON3D_P(poly);
}

PG_FUNCTION_INFO_V1(poly3D_recv);

/*
 * poly3D_send: Convert Polygon3D to binary representation
 */
Datum
poly3D_send(PG_FUNCTION_ARGS)
{
	Polygon3D    *poly = PG_GETARG_POLYGON3D_P(0);
	StringInfoData buf;
	int32		i;

	pq_begintypsend(&buf);
	pq_sendint(&buf, poly->npts, sizeof(int32));
	for (i = 0; i < poly->npts; i++)
	{
		pq_sendfloat8(&buf, poly->p[i].x);
		pq_sendfloat8(&buf, poly->p[i].y);
		pq_sendfloat8(&buf, poly->p[i].z);
	}
	PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
}

PG_FUNCTION_INFO_V1(poly3D_send);

/* 
 * poly3D_copy: Form a Polygon3D from another Polygon3D
 */
Polygon3D *
poly3D_copy(Polygon3D *poly)
{
	if (!PointerIsValid(poly))
		return NULL;
	else
	{
		Polygon3D  *result;
		int			size;

		size = VARSIZE(poly) ;
		result = (Polygon3D *) palloc(size);
		memcpy(result, poly, size);

		return result;
	}
}

/*----------------------------------------------------------
 *	Accessor functions
 *---------------------------------------------------------*/

/*
 * poly3D_npoints: Return the number of points of the Polygon3D
 */
Datum
poly3D_npoints(PG_FUNCTION_ARGS)
{
	Polygon3D    *poly = PG_GETARG_POLYGON3D_P(0);

	PG_RETURN_INT32(poly->npts);
}

PG_FUNCTION_INFO_V1(poly3D_npoints);

/*
 * poly3D_points: Return the points of the Polygon3D
 */
Datum 
poly3D_points(PG_FUNCTION_ARGS) 
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Datum 	   *d = (Datum *) palloc(sizeof(Datum) * poly->npts);
	ArrayType  *result;

	for (int i = 0; i < poly->npts; i++)
		d[i] = Point3DPGetDatum(&poly->p[i]);

	result = construct_array(d, poly->npts, point3d_oid(), sizeof(Point3D), FALSE, 'd');

	PG_RETURN_ARRAYTYPE_P(result);
}

PG_FUNCTION_INFO_V1(poly3D_points); 

/*
 * poly3D_center: Return the center of the polygon
 */
Datum
poly3D_center(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Point3D	   *result = (Point3D *) palloc(sizeof(Point3D));
	int			i;

	result->x = 0;
	result->y = 0;
	result->z = 0;

	for (i = 0; i < poly->npts; i++)
	{
		result->x += poly->p[i].x;
		result->y += poly->p[i].y;
		result->z += poly->p[i].z;
	}
	result->x /= poly->npts;
	result->y /= poly->npts;
	result->z /= poly->npts;

	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(poly3D_center);

/*
 * poly3D_length: Return the length (i.e., perimeter) of the Polygon3D
 */
Datum
poly3D_length(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Path3D	   *path;
	double		result;

	path = poly3D_path3D_internal(poly);
	result = path3D_length_internal(path);
	pfree(path);

	PG_RETURN_DATUM(result);
}

PG_FUNCTION_INFO_V1(poly3D_length);

/*
 * poly3D_area: Return the area of the Polygon3D
 */
Datum
poly3D_area(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Path3D	   *path;
	double		result;

	path = poly3D_path3D_internal(poly);
	result = path3D_area_internal(path);
	pfree(path);
	
	PG_RETURN_FLOAT8(result);
}

PG_FUNCTION_INFO_V1(poly3D_area);

/* 
 * poly3D_isplanar: Is the Polygon3D planar?
 */
Datum
poly3D_isplanar(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	int			i;
	
	i = plist_isplanar(poly->npts, poly->p);
	if (i < 0)
		PG_RETURN_NULL();
	else if (i == 0)			 
		PG_RETURN_BOOL(FALSE);
	else
		PG_RETURN_BOOL(TRUE);		
}

PG_FUNCTION_INFO_V1(poly3D_isplanar) ;

static bool
poly3D_isplanar_internal(Polygon3D *poly)
{
	return (plist_isplanar(poly->npts, poly->p) == 1);	
}

/*
 * poly3D_boundbox: Return the bounding box of the Polygon3D
 */
Datum
poly3D_boundbox(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);

	PG_RETURN_BOX3D_P(poly3D_bbox(poly));
}

PG_FUNCTION_INFO_V1(poly3D_boundbox);

Box3D *
poly3D_bbox(Polygon3D *poly)
{
	Box3D	   *box = (Box3D *) palloc(sizeof(Box3D));

	box->high = poly->boundbox.high;
	box->low = poly->boundbox.low;
	return box;
}

/*
 * poly3D_boundsphere: Return the bounding sphere of the Polygon3D
 */
Datum
poly3D_boundsphere(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	
	PG_RETURN_SPHERE_P(poly3D_bsphere(poly));
}

PG_FUNCTION_INFO_V1(poly3D_boundsphere);	

Sphere *
poly3D_bsphere(Polygon3D *poly)
{
	Sphere	   *sphere;
	Path3D	   *path;
	
	path = poly3D_path3D_internal(poly);
	sphere = path3D_bsphere(path);
	pfree(path);
	
	return sphere;
}

/*----------------------------------------------------------
 *	Relational routines 
 *---------------------------------------------------------*/

/*
 * poly3D_same: Are the two Polygon3D equal, i.e., are all the points the same?
 * Check all points for matches in both forward and reverse direction
 * since polygons are non-directional and are closed shapes.
 */
Datum
poly3D_same(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly1 = PG_GETARG_POLYGON3D_P(0);
	Polygon3D  *poly2 = PG_GETARG_POLYGON3D_P(1);
	bool		result;

	result = poly3D_same_internal(poly1, poly2);

	// Avoid leaking memory for toasted inputs. Needed for rtree indexes
	PG_FREE_IF_COPY(poly1, 0);
	PG_FREE_IF_COPY(poly2, 1);

	PG_RETURN_BOOL(result);
}

PG_FUNCTION_INFO_V1(poly3D_same);

bool
poly3D_same_internal(Polygon3D *poly1, Polygon3D *poly2)
{
	if (poly1->npts != poly2->npts)
		return FALSE;
	else
		return plist_same(poly1->npts, poly1->p, poly2->p);
}

/*
 * poly3D_cmp_internal: Compare the two Polygon3D
 */
static int 
poly3D_cmp_internal(Polygon3D* p1, Polygon3D* p2) 
{
	int 		npts = p1->npts < p2->npts ? p1->npts : p2->npts,
				result = 0 ;
	Lseg3D		seg1,
				seg2;
				
    for(int i = 1 ; i < npts ; i ++) 
	{
		lseg3D_construct_pts_stat(&seg1, p1->p[i-1], p1->p[i]);
		lseg3D_construct_pts_stat(&seg2, p2->p[i-1], p2->p[i]);
		result = lseg3D_cmp_internal(&seg1, &seg2) ;
		if (result) 
			break ;
	}
	// The first npts segments of both polygons are equal
	if (!result) 
		if (npts < p1->npts) // p1 has more segments than p2
			result = 1 ;
		else if (npts < p2->npts) // p2 has more segments than p1
			result = -1 ;
		else
			result = 0 ;
	return result;
}

PGDLLEXPORT Datum 
poly3D_cmp(PG_FUNCTION_ARGS) 
{
	Polygon3D  *p1 = PG_GETARG_POLYGON3D_P(0);
	Polygon3D  *p2 = PG_GETARG_POLYGON3D_P(1);
    int 		cmp = poly3D_cmp_internal(p1, p2) ;
    
	PG_FREE_IF_COPY(p1, 0) ;
    PG_FREE_IF_COPY(p2, 1) ;
	
	PG_RETURN_INT32(cmp);
}

PG_FUNCTION_INFO_V1(poly3D_cmp);

PGDLLEXPORT Datum 
poly3D_lt(PG_FUNCTION_ARGS) 
{
	Polygon3D  *p1 = PG_GETARG_POLYGON3D_P(0);
	Polygon3D  *p2 = PG_GETARG_POLYGON3D_P(1);
    int 		cmp = poly3D_cmp_internal(p1, p2) ;
	
    PG_FREE_IF_COPY(p1, 0) ;
    PG_FREE_IF_COPY(p2, 1) ;
	
	PG_RETURN_BOOL(cmp < 0);
}

PG_FUNCTION_INFO_V1(poly3D_lt);

PGDLLEXPORT Datum 
poly3D_le(PG_FUNCTION_ARGS) 
{
	Polygon3D  *p1 = PG_GETARG_POLYGON3D_P(0);
	Polygon3D  *p2 = PG_GETARG_POLYGON3D_P(1);
    int 		cmp = poly3D_cmp_internal(p1, p2) ;
	
    PG_FREE_IF_COPY(p1, 0) ;
    PG_FREE_IF_COPY(p2, 1) ;
	
	PG_RETURN_BOOL(cmp <= 0);
}

PG_FUNCTION_INFO_V1(poly3D_le);

PGDLLEXPORT Datum 
poly3D_eq(PG_FUNCTION_ARGS) 
{
	Polygon3D  *p1 = PG_GETARG_POLYGON3D_P(0);
	Polygon3D  *p2 = PG_GETARG_POLYGON3D_P(1);
    int 		cmp = poly3D_cmp_internal(p1, p2) ;
    
	PG_FREE_IF_COPY(p1, 0) ;
    PG_FREE_IF_COPY(p2, 1) ;
	
	PG_RETURN_BOOL(cmp == 0);
}

PG_FUNCTION_INFO_V1(poly3D_eq);

PGDLLEXPORT Datum 
poly3D_ne(PG_FUNCTION_ARGS) 
{
	Polygon3D  *p1 = PG_GETARG_POLYGON3D_P(0);
	Polygon3D  *p2 = PG_GETARG_POLYGON3D_P(1);
    int 		cmp = poly3D_cmp_internal(p1, p2) ;
    
	PG_FREE_IF_COPY(p1, 0) ;
    PG_FREE_IF_COPY(p2, 1) ;
	
	PG_RETURN_BOOL(cmp != 0);
}

PG_FUNCTION_INFO_V1(poly3D_ne);

PGDLLEXPORT Datum 
poly3D_gt(PG_FUNCTION_ARGS) 
{
	Polygon3D  *p1 = PG_GETARG_POLYGON3D_P(0);
	Polygon3D  *p2 = PG_GETARG_POLYGON3D_P(1);
    int 		cmp = poly3D_cmp_internal(p1, p2) ;
    
	PG_FREE_IF_COPY(p1, 0) ;
    PG_FREE_IF_COPY(p2, 1) ;
	
	PG_RETURN_BOOL(cmp > 0);
}

PG_FUNCTION_INFO_V1(poly3D_gt);

PGDLLEXPORT Datum 
poly3D_ge(PG_FUNCTION_ARGS) 
{
	Polygon3D  *p1 = PG_GETARG_POLYGON3D_P(0);
	Polygon3D  *p2 = PG_GETARG_POLYGON3D_P(1);
    int 		cmp = poly3D_cmp_internal(p1, p2) ;
    
	PG_FREE_IF_COPY(p1, 0) ;
    PG_FREE_IF_COPY(p2, 1) ;
	
	PG_RETURN_BOOL(cmp >= 0);
}

PG_FUNCTION_INFO_V1(poly3D_ge);

/*----------------------------------------------------------
 * Conversion routines
 *---------------------------------------------------------*/

/*
 * poly3D_path3D: Convert the Polygon3D to a Path3D
 */
Datum
poly3D_path3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);

	PG_RETURN_PATH3D_P(poly3D_path3D_internal(poly));
}

PG_FUNCTION_INFO_V1(poly3D_path3D);

Path3D *
poly3D_path3D_internal(Polygon3D *poly)
{
	Path3D	   *path;
	int			size;
	int			i;

	/*
	 * Never overflows: the old size fit in MaxAllocSize, and the new size is
	 * smaller by a small constant.
	 */
	size = offsetof(Path3D, p) + sizeof(path->p[0]) * poly->npts;
	path = (Path3D *) palloc(size);

	SET_VARSIZE(path, size);
	path->npts = poly->npts;
	path->closed = TRUE;
	/* prevent instability in unused pad bytes */
	path->dummy = 0;

	for (i = 0; i < poly->npts; i++)
	{
		path->p[i].x = poly->p[i].x;
		path->p[i].y = poly->p[i].y;
		path->p[i].z = poly->p[i].z;
	}

	path3D_make_bbox(path);
	
	return path;
}

/*----------------------------------------------------------
 * "Arithmetic" routines on Polygon3D
 *---------------------------------------------------------*/

/* 
 * poly3D_add: Return the Polygon3D obtained by adding to the vertices 
 * of the Polygon3D the Point3D 
 */
Datum
poly3D_add(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P_COPY(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);
	int			i;

	for (i = 0; i < poly->npts; i++)
	{
		poly->p[i].x += point->x;
		poly->p[i].y += point->y;
		poly->p[i].z += point->z;
	}

	PG_RETURN_POLYGON3D_P(poly);
}

PG_FUNCTION_INFO_V1(poly3D_add);

/* 
 * poly3D_sub: Return the Polygon3D obtained by subtracting to the vertices 
 * of the Polygon3D the Point3D 
 */
Datum
poly3D_sub(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P_COPY(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);
	int			i;

	for (i = 0; i < poly->npts; i++)
	{
		poly->p[i].x -= point->x;
		poly->p[i].y -= point->y;
		poly->p[i].z -= point->z;
	}

	PG_RETURN_POLYGON3D_P(poly);
}

PG_FUNCTION_INFO_V1(poly3D_sub);

/* 
 * poly3D_mul: Return the Polygon3D obtained by multiplying the vertices of the 
 * Polygon3D with the double
 */
Datum
poly3D_mul(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P_COPY(0);
	double		d = PG_GETARG_FLOAT8(1);
	Point3D	   *p;
	int			i;

	for (i = 0; i < poly->npts; i++)
	{
		poly->p[i].x *= d;
		poly->p[i].y *= d;
		poly->p[i].z *= d;
	}

	PG_RETURN_POLYGON3D_P(poly);
}

PG_FUNCTION_INFO_V1(poly3D_mul);

/* 
 * poly3D_div: Return the Polygon3D obtained by dividing the vertices of the 
 * Polygon3D by the double
 */
Datum
poly3D_div(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P_COPY(0);
	double		d = PG_GETARG_FLOAT8(1);
	Point3D	   *p;
	int			i;

	if (FPzero(d))
		ereport(ERROR,
				(errcode(ERRCODE_DIVISION_BY_ZERO),
				 errmsg("Division by zero")));
				 
	for (i = 0; i < poly->npts; i++)
	{
		poly->p[i].x /= d;
		poly->p[i].y /= d;
		poly->p[i].z /= d;
	}

	PG_RETURN_POLYGON3D_P(poly);
}

PG_FUNCTION_INFO_V1(poly3D_div);

/***********************************************************************
 * Routines for Spheres
 ***********************************************************************/

/*----------------------------------------------------------
 * Input and output functions
 *---------------------------------------------------------*/

/*
 * sphere_in: convert a string to internal form
 *
 * Input format: (center and radius of sphere)
 *		"((f8,f8,f8),f8)"
 *		"<(f8,f8,f8),f8>"
 *		"(f8,f8,f8,f8)"
 */
Datum
sphere_in(PG_FUNCTION_ARGS)
{
	char	   *str = PG_GETARG_CSTRING(0);
	Sphere	   *sphere = (Sphere *) palloc(sizeof(Sphere));
	char	   *s,
			   *cp;
	int			depth = 0;

	s = str;
	while (isspace((unsigned char) *s))
		s++;
	if ((*s == LDELIM_S) || (*s == LDELIM))
	{
		depth++;
		cp = (s + 1);
		while (isspace((unsigned char) *cp))
			cp++;
		if (*cp == LDELIM)
			s = cp;
	}

	if (!triple_decode(s, &sphere->center.x, &sphere->center.y, &sphere->center.z, &s))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
			   errmsg("Invalid input syntax for type sphere: \"%s\"", str)));

	if (*s == DELIM)
		s++;
	while (isspace((unsigned char) *s))
		s++;

	if ((!single_decode(s, &sphere->radius, &s)) || (FPlt(sphere->radius,0)))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
			   errmsg("Invalid input syntax for type sphere: \"%s\"", str)));

	while (depth > 0)
	{
		if ((*s == RDELIM)
			|| ((*s == RDELIM_S) && (depth == 1)))
		{
			depth--;
			s++;
			while (isspace((unsigned char) *s))
				s++;
		}
		else
			ereport(ERROR,
					(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
			   errmsg("Invalid input syntax for type sphere: \"%s\"", str)));
	}

	if (*s != '\0')
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
			   errmsg("Invalid input syntax for type sphere: \"%s\"", str)));

	PG_RETURN_SPHERE_P(sphere);
}

PG_FUNCTION_INFO_V1(sphere_in);

/*
 * sphere_out: Convert internal Sphere representation to character string
 * 
 * Output format: (center and radius of sphere)
 *		"<(f8,f8,f8),f8>"
 */
Datum
sphere_out(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	char	   *result;
	char	   *cp;

	result = palloc(2 * P_MAXLEN + 6);

	cp = result;
	*cp++ = LDELIM_S;
	*cp++ = LDELIM;
	if (!triple_encode(sphere->center.x, sphere->center.y, sphere->center.z, cp))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				 errmsg("Could not format \"sphere\" value")));

	cp += strlen(cp);
	*cp++ = RDELIM;
	*cp++ = DELIM;
	if (!single_encode(sphere->radius, cp))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				 errmsg("Could not format \"sphere\" value")));

	cp += strlen(cp);
	*cp++ = RDELIM_S;
	*cp = '\0';

	PG_RETURN_CSTRING(result);
}

PG_FUNCTION_INFO_V1(sphere_out);

/*
 *	sphere_recv: Convert external binary representation to Sphere
 */
Datum
sphere_recv(PG_FUNCTION_ARGS)
{
	StringInfo	buf = (StringInfo) PG_GETARG_POINTER(0);
	Sphere	   *sphere = (Sphere *) palloc(sizeof(Sphere));

	sphere->center.x = pq_getmsgfloat8(buf);
	sphere->center.y = pq_getmsgfloat8(buf);
	sphere->center.z = pq_getmsgfloat8(buf);
	sphere->radius = pq_getmsgfloat8(buf);

	if (FPlt(sphere->radius, 0))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_BINARY_REPRESENTATION),
				 errmsg("Invalid radius in external \"sphere\" value")));

	PG_RETURN_SPHERE_P(sphere);
}

PG_FUNCTION_INFO_V1(sphere_recv);

/*
 * sphere_send: Convert Sphere to binary representation
 */
Datum
sphere_send(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	StringInfoData buf;

	pq_begintypsend(&buf);
	pq_sendfloat8(&buf, sphere->center.x);
	pq_sendfloat8(&buf, sphere->center.y);
	pq_sendfloat8(&buf, sphere->center.z);
	pq_sendfloat8(&buf, sphere->radius);
	PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
}

PG_FUNCTION_INFO_V1(sphere_send);

/*----------------------------------------------------------
 *	Constructors
 *---------------------------------------------------------*/
 
/* 
 * sphere_construct: Form a Sphere from a Point3D and a double
 */
Datum
sphere_construct(PG_FUNCTION_ARGS)
{
	Point3D	   *center = PG_GETARG_POINT3D_P(0);
	double		radius = PG_GETARG_FLOAT8(1);
	Sphere	   *result = (Sphere *) palloc(sizeof(Sphere));

	result->center = *center;
	result->radius = radius;

	PG_RETURN_SPHERE_P(result);
}

PG_FUNCTION_INFO_V1(sphere_construct);

/* 
 * sphere_copy: Form a Sphere from another Sphere
 */
static Sphere *
sphere_copy(Sphere *sphere)
{
	Sphere	   *result;

	if (!PointerIsValid(sphere))
		return NULL;

	result = (Sphere *) palloc(sizeof(Sphere));
	memcpy((char *) result, (char *) sphere, sizeof(Sphere));
	return result;
}

/*----------------------------------------------------------
 *	Accessor functions
 *---------------------------------------------------------*/
 
/*
 * sphere_center: Return the center point of the sphere
 */
Datum
sphere_center(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Point3D	   *result = (Point3D *) palloc(sizeof(Point3D));
	
	*result = sphere->center;

	PG_RETURN_POINT3D_P(result);
}

PG_FUNCTION_INFO_V1(sphere_center);

/*
 * sphere_radius: Return the radius of the sphere
 */
Datum
sphere_radius(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);

	PG_RETURN_FLOAT8(sphere->radius);
}

PG_FUNCTION_INFO_V1(sphere_radius);

/*
 * sphere_diameter: Return the diameter of the sphere
 */
Datum
sphere_diameter(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);

	PG_RETURN_FLOAT8(2 * sphere->radius);
}

PG_FUNCTION_INFO_V1(sphere_diameter);

/*
 * sphere_vol: Return the volume of the sphere
 */
static double
sphere_vol(Sphere *sphere)
{
	return M_PI * (sphere->radius * sphere->radius * sphere->radius);
}

/*
 * sphere_volume: Return the volume of the sphere
 */
Datum
sphere_volume(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);

	PG_RETURN_FLOAT8(sphere_vol(sphere));
}

PG_FUNCTION_INFO_V1(sphere_volume);

/*
 * sphere_boundbox: Return the bounding box of the Sphere
 */
Datum
sphere_boundbox(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);

	PG_RETURN_BOX3D_P(sphere_bbox(sphere));
}

PG_FUNCTION_INFO_V1(sphere_boundbox);

Box3D *
sphere_bbox(Sphere *sphere)
{
	Box3D	   *box = (Box3D *) palloc(sizeof(Box3D));
	
	box->high.x = sphere->center.x + sphere->radius;
	box->high.y = sphere->center.y + sphere->radius;
	box->high.z = sphere->center.z + sphere->radius;
	box->low.x = sphere->center.x - sphere->radius;
	box->low.y = sphere->center.y - sphere->radius;
	box->low.z = sphere->center.z - sphere->radius;
}					 

/*----------------------------------------------------------
 *	Relational routines 
 *---------------------------------------------------------*/

/*
 * sphere_same: Are the two Sphere identical?
 */
Datum
sphere_same(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere1 = PG_GETARG_SPHERE_P(0);
	Sphere	   *sphere2 = PG_GETARG_SPHERE_P(1);

	PG_RETURN_BOOL(FPeq(sphere1->radius, sphere2->radius) &&
				   PT3Deq(sphere1->center, sphere2->center));
}

PG_FUNCTION_INFO_V1(sphere_same);

bool
sphere_same_internal(Sphere *sphere1, Sphere *sphere2)
{
	return (FPeq(sphere1->radius, sphere2->radius) &&
			PT3Deq(sphere1->center, sphere2->center));
}

/*
 * sphere_cmp_internal: Compare the two Sphere
 */
static int 
sphere_cmp_internal(Sphere* s1, Sphere* s2) 
{
	if(FPlt(s1->center.x, s2->center.x))
		return -1;
	if(FPgt(s1->center.x, s2->center.x))
		return 1;
	if(FPlt(s1->center.y, s2->center.y))
		return -1;
	if(FPgt(s1->center.y, s2->center.y))
		return 1;
	if(FPlt(s1->center.z, s2->center.z))
		return -1;
	if(FPgt(s1->center.z, s2->center.z))
		return 1;
	// the center points are equal
	if(FPlt(s1->radius, s2->radius))
		return -1;
	if(FPgt(s1->radius, s2->radius))
		return 1;
	else
		return 0;
}

Datum 
sphere_cmp(PG_FUNCTION_ARGS) 
{
	Sphere* s1 = PG_GETARG_SPHERE_P(0);
	Sphere* s2 = PG_GETARG_SPHERE_P(1);
	PG_RETURN_INT32(sphere_cmp_internal(s1, s2));
}

PG_FUNCTION_INFO_V1(sphere_cmp);

Datum 
sphere_lt(PG_FUNCTION_ARGS) 
{
	Sphere* s1 = PG_GETARG_SPHERE_P(0);
	Sphere* s2 = PG_GETARG_SPHERE_P(1);
	PG_RETURN_BOOL(sphere_cmp_internal(s1, s2) < 0);
}

PG_FUNCTION_INFO_V1(sphere_lt);

Datum 
sphere_le(PG_FUNCTION_ARGS) 
{
	Sphere* s1 = PG_GETARG_SPHERE_P(0);
	Sphere* s2 = PG_GETARG_SPHERE_P(1);
	PG_RETURN_BOOL(sphere_cmp_internal(s1, s2) <= 0);
}

PG_FUNCTION_INFO_V1(sphere_le);

Datum 
sphere_eq(PG_FUNCTION_ARGS) 
{
	Sphere* s1 = PG_GETARG_SPHERE_P(0);
	Sphere* s2 = PG_GETARG_SPHERE_P(1);
	PG_RETURN_BOOL(sphere_cmp_internal(s1, s2) == 0);
}

PG_FUNCTION_INFO_V1(sphere_eq);

Datum 
sphere_ne(PG_FUNCTION_ARGS) 
{
	Sphere* s1 = PG_GETARG_SPHERE_P(0);
	Sphere* s2 = PG_GETARG_SPHERE_P(1);
	PG_RETURN_BOOL(sphere_cmp_internal(s1, s2) != 0);
}

PG_FUNCTION_INFO_V1(sphere_ne);

Datum 
sphere_gt(PG_FUNCTION_ARGS) 
{
	Sphere* s1 = PG_GETARG_SPHERE_P(0);
	Sphere* s2 = PG_GETARG_SPHERE_P(1);
	PG_RETURN_BOOL(sphere_cmp_internal(s1, s2) > 0);
}

PG_FUNCTION_INFO_V1(sphere_gt);

Datum 
sphere_ge(PG_FUNCTION_ARGS) 
{
	Sphere* s1 = PG_GETARG_SPHERE_P(0);
	Sphere* s2 = PG_GETARG_SPHERE_P(1);
	PG_RETURN_BOOL(sphere_cmp_internal(s1, s2) >= 0);
}

PG_FUNCTION_INFO_V1(sphere_ge);

/*----------------------------------------------------------
 *	"Arithmetic" routines on Sphere
 *---------------------------------------------------------*/

/* 
 * sphere_add: Return the Sphere obtained by adding to the center
 * of the Sphere the Point3D 
 */
Datum
sphere_add(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);
	Sphere	   *result;

	result = sphere_copy(sphere);

	result->center.x += point->x;
	result->center.y += point->y;
	result->center.z += point->z;

	PG_RETURN_SPHERE_P(result);
}

PG_FUNCTION_INFO_V1(sphere_add);

/* 
 * sphere_sub: Return the Sphere obtained by subtracting to the center 
 * of the Sphere the Point3D 
 */
Datum
sphere_sub(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);
	Sphere	   *result;

	result = sphere_copy(sphere);

	result->center.x -= point->x;
	result->center.y -= point->y;
	result->center.z -= point->z;

	PG_RETURN_SPHERE_P(result);
}

PG_FUNCTION_INFO_V1(sphere_sub);

/* 
 * sphere_mul: Return the Sphere obtained by multiplying the center 
 * and the radius of the Sphere with the double
 */
Datum
sphere_mul(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	double		d = PG_GETARG_FLOAT8(1);
	Sphere	   *result;
	Point3D	   *p;

	result = sphere_copy(sphere);

	result->center.x *= d;
	result->center.y *= d;
	result->center.z *= d;
	result->radius *= d;

	PG_RETURN_SPHERE_P(result);
}

PG_FUNCTION_INFO_V1(sphere_mul);

/* 
 * sphere_div: Return the Sphere obtained by dividing the center 
 * and the radius of the Sphere with the double
 */
Datum
sphere_div(PG_FUNCTION_ARGS)
{
	Sphere	   *sphere = PG_GETARG_SPHERE_P(0);
	double		d = PG_GETARG_FLOAT8(1);
	Sphere	   *result;
	Point3D	   *p;

	if (FPzero(d))
		ereport(ERROR,
				(errcode(ERRCODE_DIVISION_BY_ZERO),
				 errmsg("Division by zero")));

	result = sphere_copy(sphere);

	result->center.x /= d;
	result->center.y /= d;
	result->center.z /= d;
	result->radius /= d;

	PG_RETURN_SPHERE_P(result);
}

PG_FUNCTION_INFO_V1(sphere_div);

/***********************************************************************
 * Coplanar routines
 ***********************************************************************/

/* lseg3D coplanar TYPE3D */

/*
 * coplanar_lseg3D_lseg3D: Are the two Lseg3D coplanar?
 */
Datum
coplanar_lseg3D_lseg3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg1 = PG_GETARG_LSEG3D_P(0);
	Lseg3D	   *lseg2 = PG_GETARG_LSEG3D_P(1);
	
	PG_RETURN_BOOL(plist_coplanar(2, lseg1->p, 2, lseg2->p) == 1);
}

PG_FUNCTION_INFO_V1(coplanar_lseg3D_lseg3D) ;

bool
coplanar_lseg3D_lseg3D_internal(Lseg3D *lseg1, Lseg3D *lseg2)
{
	return (plist_coplanar(2, lseg1->p, 2, lseg2->p) == 1);
}

/*
 * coplanar_lseg3D_line3D: Are the Lseg3D and the Line3D coplanar?
 */
Datum
coplanar_lseg3D_line3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	
	PG_RETURN_BOOL(plist_coplanar(2, lseg->p, 2, line->p) == 1);
}

PG_FUNCTION_INFO_V1(coplanar_lseg3D_line3D) ;

bool
coplanar_lseg3D_line3D_internal(Lseg3D *lseg, Line3D *line)
{
	return (plist_coplanar(2, lseg->p, 2, line->p) == 1);
}

/* 
 * coplanar_lseg3D_path3D: Are the Lseg3D and Path3D coplanar?
 */
Datum
coplanar_lseg3D_path3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	int			i;
	
	i = plist_coplanar(path->npts, path->p, 2, lseg->p);
	if (i < 0)
		PG_RETURN_NULL();
	else if (i == 0)			 
		PG_RETURN_BOOL(FALSE);
	else
		PG_RETURN_BOOL(TRUE);		
}

PG_FUNCTION_INFO_V1(coplanar_lseg3D_path3D) ;

bool
coplanar_lseg3D_path3D_internal(Lseg3D *lseg, Path3D *path)
{
	return (plist_coplanar(path->npts, path->p, 2, lseg->p) == 1);	
}

/* 
 * coplanar_lseg3D_poly3D: Are the Lseg3D and the Polygon3D coplanar?
 */
Datum
coplanar_lseg3D_poly3D(PG_FUNCTION_ARGS)
{
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	int			i;
	
	i = plist_coplanar(poly->npts, poly->p, 2, lseg->p);
	if (i < 0)
		PG_RETURN_NULL();
	else if (i == 0)			 
		PG_RETURN_BOOL(FALSE);
	else
		PG_RETURN_BOOL(TRUE);		
}

PG_FUNCTION_INFO_V1(coplanar_lseg3D_poly3D) ;

bool
coplanar_lseg3D_poly3D_internal(Lseg3D *lseg, Polygon3D *poly)
{
	return (plist_coplanar(poly->npts, poly->p, 2, lseg->p) == 1);	
}

/*****************************************************************************/

/* line3D coplanar TYPE3D */

/* 
 * coplanar_line3D_lseg3D: Are the Line3D and the Lseg3D coplanar?
 */
Datum
coplanar_line3D_lseg3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	
	PG_RETURN_BOOL(plist_coplanar(2, line->p, 2, lseg->p) == 1);
}

PG_FUNCTION_INFO_V1(coplanar_line3D_lseg3D) ;

bool
coplanar_line3D_lseg3D_internal(Line3D *line, Lseg3D *lseg)
{
	return (plist_coplanar(2, line->p, 2, lseg->p) == 1);
}


/* 
 * coplanar_line3D_line3D: Are the two Line3D coplanar?
 */
Datum
coplanar_line3D_line3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line1 = PG_GETARG_LINE3D_P(0);
	Line3D	   *line2 = PG_GETARG_LINE3D_P(1);
	
	PG_RETURN_BOOL(plist_coplanar(2, line1->p, 2, line2->p));
}

PG_FUNCTION_INFO_V1(coplanar_line3D_line3D) ;

bool
coplanar_line3D_line3D_internal(Line3D *line1, Line3D *line2)
{
	return (plist_coplanar(2, line1->p, 2, line2->p)==1);
}

/* 
 * coplanar_line3D_path3D: Are the Line3D and the Path3D coplanar?
 */
Datum
coplanar_line3D_path3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	int			i;
	
	i = plist_coplanar(path->npts, path->p, 2, line->p);
	if (i < 0)
		PG_RETURN_NULL();
	else if (i == 0)			 
		PG_RETURN_BOOL(FALSE);
	else
		PG_RETURN_BOOL(TRUE);		
}

PG_FUNCTION_INFO_V1(coplanar_line3D_path3D) ;

bool
coplanar_line3D_path3D_internal(Line3D *line, Path3D *path)
{
	return (plist_coplanar(path->npts, path->p, 2, line->p) == 1);	
}

/* 
 * coplanar_line3D_poly3D: Are the Line3D and the Polygon3D coplanar?
 */
Datum
coplanar_line3D_poly3D(PG_FUNCTION_ARGS)
{
	Line3D	   *line = PG_GETARG_LINE3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	int			i;
	
	i = plist_coplanar(poly->npts, poly->p, 2, line->p);
	if (i < 0)
		PG_RETURN_NULL();
	else if (i == 0)			 
		PG_RETURN_BOOL(FALSE);
	else
		PG_RETURN_BOOL(TRUE);		
}

PG_FUNCTION_INFO_V1(coplanar_line3D_poly3D) ;

bool
coplanar_line3D_poly3D_internal(Line3D *line, Polygon3D *poly)
{
	return (plist_coplanar(poly->npts, poly->p, 2, line->p) == 1);	
}

/*****************************************************************************/

/* path3D coplanar TYPE3D */

/* 
 * coplanar_path3D_point3D: Are the Path3D and the Point3D coplanar?
 */
Datum
coplanar_path3D_point3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);
	int			i;
	
	i = plist_coplanar(path->npts, path->p, 1, point);
	if (i < 0)
		PG_RETURN_NULL();
	else if (i == 0)			 
		PG_RETURN_BOOL(FALSE);
	else
		PG_RETURN_BOOL(TRUE);		
}

PG_FUNCTION_INFO_V1(coplanar_path3D_point3D) ;

bool
coplanar_path3D_point3D_internal(Path3D *path, Point3D *point)
{
	return (plist_coplanar(path->npts, path->p, 1, point) == 1);	
}

/* 
 * coplanar_path3D_lseg3D: Are the Path3D and the Lseg3D coplanar?
 */
Datum
coplanar_path3D_lseg3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	int			i;
	
	i = plist_coplanar(path->npts, path->p, 2, lseg->p);
	if (i < 0)
		PG_RETURN_NULL();
	else if (i == 0)			 
		PG_RETURN_BOOL(FALSE);
	else
		PG_RETURN_BOOL(TRUE);		
}

PG_FUNCTION_INFO_V1(coplanar_path3D_lseg3D) ;

bool
coplanar_path3D_lseg3D_internal(Path3D *path, Lseg3D *lseg)
{
	return (plist_coplanar(path->npts, path->p, 2, lseg->p) == 1);	
}

/* 
 * coplanar_path3D_line3D: Are the Path3D and the Line3D coplanar?
 */
Datum
coplanar_path3D_line3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	int			i;
	
	i = plist_coplanar(path->npts, path->p, 2, line->p);
	if (i < 0)
		PG_RETURN_NULL();
	else if (i == 0)			 
		PG_RETURN_BOOL(FALSE);
	else
		PG_RETURN_BOOL(TRUE);		
}

PG_FUNCTION_INFO_V1(coplanar_path3D_line3D) ;

bool
coplanar_path3D_line3D_internal(Path3D *path, Line3D *line)
{
	return (plist_coplanar(path->npts, path->p, 2, line->p) == 1);	
}

/* 
 * coplanar_path3D_path3D: Are the two Path3D coplanar?
 */
Datum
coplanar_path3D_path3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path1 = PG_GETARG_PATH3D_P(0);
	Path3D	   *path2 = PG_GETARG_PATH3D_P(1);
	int			i;
	
	i = plist_coplanar(path1->npts, path1->p, path2->npts, path2->p);
	if (i < 0)
		PG_RETURN_NULL();
	else if (i == 0)			 
		PG_RETURN_BOOL(FALSE);
	else
		PG_RETURN_BOOL(TRUE);		
}

PG_FUNCTION_INFO_V1(coplanar_path3D_path3D) ;

bool
coplanar_path3D_path3D_internal(Path3D *path1, Path3D *path2)
{
	return (plist_coplanar(path1->npts, path1->p, path2->npts, path2->p) == 1);	
}

/* 
 * coplanar_path3D_poly3D: Are the Path3D and the Polygon3D coplanar?
 */
Datum
coplanar_path3D_poly3D(PG_FUNCTION_ARGS)
{
	Path3D	   *path = PG_GETARG_PATH3D_P(0);
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(1);
	int			i;
	
	i = plist_coplanar(path->npts, path->p, poly->npts, poly->p);
	if (i < 0)
		PG_RETURN_NULL();
	else if (i == 0)			 
		PG_RETURN_BOOL(FALSE);
	else
		PG_RETURN_BOOL(TRUE);		
}

PG_FUNCTION_INFO_V1(coplanar_path3D_poly3D) ;

bool
coplanar_path3D_poly3D_internal(Path3D *path, Polygon3D *poly)
{
	return (plist_coplanar(path->npts, path->p, poly->npts, poly->p) == 1);	
}

/*****************************************************************************/

/* poly3D coplanar TYPE3D */

/* 
 * coplanar_poly3D_point3D: Are the Polygon3D and the Point3D coplanar?
 */
Datum
coplanar_poly3D_point3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Point3D	   *point = PG_GETARG_POINT3D_P(1);
	int			i;
	
	i = plist_coplanar(poly->npts, poly->p, 1, point);
	if (i < 0)
		PG_RETURN_NULL();
	else if (i == 0)			 
		PG_RETURN_BOOL(FALSE);
	else
		PG_RETURN_BOOL(TRUE);		
}

PG_FUNCTION_INFO_V1(coplanar_poly3D_point3D) ;

bool
coplanar_poly3D_point3D_internal(Polygon3D *poly, Point3D *point)
{
	return (plist_coplanar(poly->npts, poly->p, 1, point) == 1);	
}

/* 
 * coplanar_poly3D_lseg3D: Are the Polygon3D and the Lseg3D coplanar?
 */
Datum
coplanar_poly3D_lseg3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Lseg3D	   *lseg = PG_GETARG_LSEG3D_P(1);
	int			i;
	
	i = plist_coplanar(poly->npts, poly->p, 2, lseg->p);
	if (i < 0)
		PG_RETURN_NULL();
	else if (i == 0)			 
		PG_RETURN_BOOL(FALSE);
	else
		PG_RETURN_BOOL(TRUE);		
}

PG_FUNCTION_INFO_V1(coplanar_poly3D_lseg3D) ;

bool
coplanar_poly3D_lseg3D_internal(Polygon3D *poly, Lseg3D *lseg)
{
	return (plist_coplanar(poly->npts, poly->p, 2, lseg->p) == 1);	
}

/* 
 * coplanar_poly3D_line3D: Are the Polygon3D and the Line3D coplanar?
 */
Datum
coplanar_poly3D_line3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Line3D	   *line = PG_GETARG_LINE3D_P(1);
	int			i;
	
	i = plist_coplanar(poly->npts, poly->p, 2, line->p);
	if (i < 0)
		PG_RETURN_NULL();
	else if (i == 0)			 
		PG_RETURN_BOOL(FALSE);
	else
		PG_RETURN_BOOL(TRUE);		
}

PG_FUNCTION_INFO_V1(coplanar_poly3D_line3D) ;

bool
coplanar_poly3D_line3D_internal(Polygon3D *poly, Line3D *line)
{
	return (plist_coplanar(poly->npts, poly->p, 2, line->p) == 1);	
}

/* 
 * coplanar_poly3D_path3D: Are the Polygon3D and the Path3D coplanar?
 */
Datum
coplanar_poly3D_path3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly = PG_GETARG_POLYGON3D_P(0);
	Path3D	   *path = PG_GETARG_PATH3D_P(1);
	int			i;
	
	i = plist_coplanar(poly->npts, poly->p, path->npts, path->p);
	if (i < 0)
		PG_RETURN_NULL();
	else if (i == 0)			 
		PG_RETURN_BOOL(FALSE);
	else
		PG_RETURN_BOOL(TRUE);		
}

PG_FUNCTION_INFO_V1(coplanar_poly3D_path3D) ;

bool
coplanar_poly3D_path3D_internal(Polygon3D *poly, Path3D *path)
{
	return (plist_coplanar(poly->npts, poly->p, path->npts, path->p) == 1);	
}

/* 
 * coplanar_poly3D_poly3D: Are the two Polygon3D coplanar?
 */
Datum
coplanar_poly3D_poly3D(PG_FUNCTION_ARGS)
{
	Polygon3D  *poly1 = PG_GETARG_POLYGON3D_P(0);
	Polygon3D  *poly2 = PG_GETARG_POLYGON3D_P(1);
	int			i;
	
	i = plist_coplanar(poly1->npts, poly1->p, poly2->npts, poly2->p);
	if (i < 0)
		PG_RETURN_NULL();
	else if (i == 0)			 
		PG_RETURN_BOOL(FALSE);
	else
		PG_RETURN_BOOL(TRUE);		
}

PG_FUNCTION_INFO_V1(coplanar_poly3D_poly3D) ;

bool
coplanar_poly3D_poly3D_internal(Polygon3D *poly1, Polygon3D *poly2)
{
	return (plist_coplanar(poly1->npts, poly1->p, poly2->npts, poly2->p) == 1);	
}

/*****************************************************************************/
