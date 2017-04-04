/*-------------------------------------------------------------------------
 *
 * geo3D_topo_ops.sql
 *	  Topological operations for 3D geometries
 *
 * Portions Copyright (c) 2017, Esteban Zimanyi, Université Libre de Bruxelles
 * Portions Copyright (c) 1996-2017, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *-------------------------------------------------------------------------*/

/******************************************************************************
 * Containment functions 
 *****************************************************************************/

CREATE OR REPLACE FUNCTION contained_point3D_lseg3D(point Point3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contained_point3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <@ (
	LEFTARG = Point3D, RIGHTARG = Lseg3D, PROCEDURE = contained_point3D_lseg3D,
	COMMUTATOR = '@>',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contained_point3D_line3D(point Point3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contained_point3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <@ (
	LEFTARG = Point3D, RIGHTARG = Line3D, PROCEDURE = contained_point3D_line3D,
	COMMUTATOR = '@>',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contained_point3D_box3D(point Point3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contained_point3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <@ (
	LEFTARG = Point3D, RIGHTARG = Box3D, PROCEDURE = contained_point3D_box3D,
	COMMUTATOR = '@>',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contained_point3D_path3D(point Point3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contained_point3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <@ (
	LEFTARG = Point3D, RIGHTARG = Path3D, PROCEDURE = contained_point3D_path3D,
	COMMUTATOR = '@>',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contained_point3D_poly3D(point Point3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contained_point3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <@ (
	LEFTARG = Point3D, RIGHTARG = Polygon3D, PROCEDURE = contained_point3D_poly3D,
	COMMUTATOR = '@>',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contained_point3D_sphere(point Point3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'contained_point3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <@ (
	LEFTARG = Point3D, RIGHTARG = Sphere, PROCEDURE = contained_point3D_sphere,
	COMMUTATOR = '@>',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION contain_lseg3D_point3D(lseg Lseg3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contain_lseg3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR @> (
	LEFTARG = Lseg3D, RIGHTARG = Point3D, PROCEDURE = contain_lseg3D_point3D,
	COMMUTATOR = '<@',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contain_lseg3D_lseg3D(lseg1 Lseg3D, lseg2 Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contain_lseg3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR @> (
	LEFTARG = Lseg3D, RIGHTARG = Lseg3D, PROCEDURE = contain_lseg3D_lseg3D,
	COMMUTATOR = '<@',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION contained_lseg3D_lseg3D(lseg1 Lseg3D, lseg2 Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contained_lseg3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <@ (
	LEFTARG = Lseg3D, RIGHTARG = Lseg3D, PROCEDURE = contained_lseg3D_lseg3D,
	COMMUTATOR = '@>',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contained_lseg3D_line3D(lseg Lseg3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contained_lseg3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <@ (
	LEFTARG = Lseg3D, RIGHTARG = Line3D, PROCEDURE = contained_lseg3D_line3D,
	COMMUTATOR = '@>',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contained_lseg3D_box3D(lseg Lseg3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contained_lseg3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <@ (
	LEFTARG = Lseg3D, RIGHTARG = Box3D, PROCEDURE = contained_lseg3D_box3D,
	COMMUTATOR = '@>',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contained_lseg3D_path3D(lseg Lseg3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contained_lseg3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <@ (
	LEFTARG = Lseg3D, RIGHTARG = Path3D, PROCEDURE = contained_lseg3D_path3D,
	COMMUTATOR = '@>',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contained_lseg3D_poly3D(lseg Lseg3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contained_lseg3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <@ (
	LEFTARG = Lseg3D, RIGHTARG = Polygon3D, PROCEDURE = contained_lseg3D_poly3D,
	COMMUTATOR = '@>',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contained_lseg3D_sphere(lseg Lseg3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'contained_lseg3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <@ (
	LEFTARG = Lseg3D, RIGHTARG = Sphere, PROCEDURE = contained_lseg3D_sphere,
	COMMUTATOR = '@>',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION contain_line3D_point3D(line Line3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contain_line3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR @> (
	LEFTARG = Line3D, RIGHTARG = Point3D, PROCEDURE = contain_line3D_point3D,
	COMMUTATOR = '<@',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contain_line3D_lseg3D(line Line3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contain_line3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR @> (
	LEFTARG = Line3D, RIGHTARG = Lseg3D, PROCEDURE = contain_line3D_lseg3D,
	COMMUTATOR = '<@',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION contain_box3D_point3D(box Box3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contain_box3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR @> (
	LEFTARG = Box3D, RIGHTARG = Point3D, PROCEDURE = contain_box3D_point3D,
	COMMUTATOR = '<@',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contain_box3D_lseg3D(box Box3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contain_box3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR @> (
	LEFTARG = Box3D, RIGHTARG = Lseg3D, PROCEDURE = contain_box3D_lseg3D,
	COMMUTATOR = '<@',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contain_box3D_box3D(box1 Box3D, box2 Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contain_box3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR @> (
	LEFTARG = Box3D, RIGHTARG = Box3D, PROCEDURE = contain_box3D_box3D,
	COMMUTATOR = '<@',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contain_box3D_path3D(box Box3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contain_box3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR @> (
	LEFTARG = Box3D, RIGHTARG = Path3D, PROCEDURE = contain_box3D_path3D,
	COMMUTATOR = '<@',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contain_box3D_poly3D(box Box3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contain_box3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR @> (
	LEFTARG = Box3D, RIGHTARG = Polygon3D, PROCEDURE = contain_box3D_poly3D,
	COMMUTATOR = '<@',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contain_box3D_sphere(box Box3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'contain_box3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR @> (
	LEFTARG = Box3D, RIGHTARG = Sphere, PROCEDURE = contain_box3D_sphere,
	COMMUTATOR = '<@',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION contained_box3D_box3D(box1 Box3D, box2 Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contained_box3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <@ (
	LEFTARG = Box3D, RIGHTARG = Box3D, PROCEDURE = contained_box3D_box3D,
	COMMUTATOR = '@>',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contained_box3D_sphere(box Box3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'contained_box3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <@ (
	LEFTARG = Box3D, RIGHTARG = Sphere, PROCEDURE = contained_box3D_sphere,
	COMMUTATOR = '@>',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION contain_path3D_point3D(path Path3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contain_path3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR @> (
	LEFTARG = Path3D, RIGHTARG = Point3D, PROCEDURE = contain_path3D_point3D,
	COMMUTATOR = '<@',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contain_path3D_lseg3D(path Path3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contain_path3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR @> (
	LEFTARG = Path3D, RIGHTARG = Lseg3D, PROCEDURE = contain_path3D_lseg3D,
	COMMUTATOR = '<@',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contain_path3D_path3D(path1 Path3D, path2 Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contain_path3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR @> (
	LEFTARG = Path3D, RIGHTARG = Path3D, PROCEDURE = contain_path3D_path3D,
	COMMUTATOR = '<@',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION contained_path3D_box3D(path Path3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contained_path3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <@ (
	LEFTARG = Path3D, RIGHTARG = Box3D, PROCEDURE = contained_path3D_box3D,
	COMMUTATOR = '@>',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contained_path3D_path3D(path1 Path3D, path2 Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contained_path3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <@ (
	LEFTARG = Path3D, RIGHTARG = Path3D, PROCEDURE = contained_path3D_path3D,
	COMMUTATOR = '@>',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contained_path3D_poly3D(path Path3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contained_path3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <@ (
	LEFTARG = Path3D, RIGHTARG = Polygon3D, PROCEDURE = contained_path3D_poly3D,
	COMMUTATOR = '@>',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contained_path3D_sphere(path Path3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'contained_path3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <@ (
	LEFTARG = Path3D, RIGHTARG = Sphere, PROCEDURE = contained_path3D_sphere,
	COMMUTATOR = '@>',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);

/******************************************************************************/

/*
CREATE OR REPLACE FUNCTION contain_poly3D_point3D_new(poly Polygon3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contain_poly3D_point3D_new' LANGUAGE C IMMUTABLE STRICT;
*/

CREATE OR REPLACE FUNCTION contain_poly3D_point3D(poly Polygon3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contain_poly3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR @> (
	LEFTARG = Polygon3D, RIGHTARG = Point3D, PROCEDURE = contain_poly3D_point3D,
	COMMUTATOR = '<@',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contain_poly3D_lseg3D(poly Polygon3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contain_poly3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR @> (
	LEFTARG = Polygon3D, RIGHTARG = Lseg3D, PROCEDURE = contain_poly3D_lseg3D,
	COMMUTATOR = '<@',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contain_poly3D_path3D(poly Polygon3D, pa Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contain_poly3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR @> (
	LEFTARG = Polygon3D, RIGHTARG = Path3D, PROCEDURE = contain_poly3D_path3D,
	COMMUTATOR = '<@',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contain_poly3D_poly3D(p1 Polygon3D, p2 Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contain_poly3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR @> (
	LEFTARG = Polygon3D, RIGHTARG = Polygon3D, PROCEDURE = contain_poly3D_poly3D,
	COMMUTATOR = '<@',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION contained_poly3D_box3D(poly Polygon3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contained_poly3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <@ (
	LEFTARG = Polygon3D, RIGHTARG = Box3D, PROCEDURE = contained_poly3D_box3D,
	COMMUTATOR = '@>',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contained_poly3D_poly3D(p1 Polygon3D, p2 Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contained_poly3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <@ (
	LEFTARG = Polygon3D, RIGHTARG = Polygon3D, PROCEDURE = contained_poly3D_poly3D,
	COMMUTATOR = '@>',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contained_poly3D_sphere(poly Polygon3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'contained_poly3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <@ (
	LEFTARG = Polygon3D, RIGHTARG = Sphere, PROCEDURE = contained_poly3D_sphere,
	COMMUTATOR = '@>',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION contain_sphere_point3D(sphere Sphere, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contain_sphere_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR @> (
	LEFTARG = Sphere, RIGHTARG = Point3D, PROCEDURE = contain_sphere_point3D,
	COMMUTATOR = '<@',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contain_sphere_lseg3D(sphere Sphere, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contain_sphere_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR @> (
	LEFTARG = Sphere, RIGHTARG = Lseg3D, PROCEDURE = contain_sphere_lseg3D,
	COMMUTATOR = '<@',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contain_sphere_box3D(sphere Sphere, l Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contain_sphere_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR @> (
	LEFTARG = Sphere, RIGHTARG = Box3D, PROCEDURE = contain_sphere_box3D,
	COMMUTATOR = '<@',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contain_sphere_path3D(sphere Sphere, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contain_sphere_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR @> (
	LEFTARG = Sphere, RIGHTARG = Path3D, PROCEDURE = contain_sphere_path3D,
	COMMUTATOR = '<@',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contain_sphere_poly3D(sphere Sphere, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contain_sphere_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR @> (
	LEFTARG = Sphere, RIGHTARG = Polygon3D, PROCEDURE = contain_sphere_poly3D,
	COMMUTATOR = '<@',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contain_sphere_sphere(sphere1 Sphere, sphere2 Sphere) RETURNS boolean AS 
'MODULE_PATHNAME', 'contain_sphere_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR @> (
	LEFTARG = Sphere, RIGHTARG = Sphere, PROCEDURE = contain_sphere_sphere,
	COMMUTATOR = '<@',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION contained_sphere_box3D(sphere Sphere, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'contained_sphere_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <@ (
	LEFTARG = Sphere, RIGHTARG = Box3D, PROCEDURE = contained_sphere_box3D,
	COMMUTATOR = '@>',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
CREATE OR REPLACE FUNCTION contained_sphere_sphere(sphere1 Sphere, sphere2 Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'contained_sphere_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <@ (
	LEFTARG = Sphere, RIGHTARG = Sphere, PROCEDURE = contained_sphere_sphere,
	COMMUTATOR = '@>',
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);

/******************************************************************************
 * Overlap functions 
 *****************************************************************************/

CREATE OR REPLACE FUNCTION overlap_lseg3D_lseg3D(lseg1 Lseg3D, lseg2 Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overlap_lseg3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR && (
	LEFTARG = Lseg3D, RIGHTARG = Lseg3D, PROCEDURE = overlap_lseg3D_lseg3D,
	COMMUTATOR = '&&',
	RESTRICT = areasel, JOIN = areajoinsel
);

CREATE OR REPLACE FUNCTION overlap_lseg3D_line3D(lseg Lseg3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overlap_lseg3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR && (
	LEFTARG = Lseg3D, RIGHTARG = Line3D, PROCEDURE = overlap_lseg3D_line3D,
	COMMUTATOR = '&&',
	RESTRICT = areasel, JOIN = areajoinsel
);
 
/******************************************************************************/

CREATE OR REPLACE FUNCTION overlap_line3D_lseg3D(line Line3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overlap_line3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR && (
	LEFTARG = Line3D, RIGHTARG = Lseg3D, PROCEDURE = overlap_line3D_lseg3D,
	COMMUTATOR = '&&',
	RESTRICT = areasel, JOIN = areajoinsel
);
 
/******************************************************************************/

CREATE OR REPLACE FUNCTION overlap_box3D_box3D(box1 Box3D, box2 Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overlap_box3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR && (
	LEFTARG = Box3D, RIGHTARG = Box3D, PROCEDURE = overlap_box3D_box3D,
	COMMUTATOR = '&&',
	RESTRICT = areasel, JOIN = areajoinsel
);
CREATE OR REPLACE FUNCTION overlap_box3D_poly3D(box Box3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overlap_box3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR && (
	LEFTARG = Box3D, RIGHTARG = Polygon3D, PROCEDURE = overlap_box3D_poly3D,
	COMMUTATOR = '&&',
	RESTRICT = areasel, JOIN = areajoinsel
);
CREATE OR REPLACE FUNCTION overlap_box3D_sphere(box Box3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overlap_box3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR && (
	LEFTARG = Box3D, RIGHTARG = Sphere, PROCEDURE = overlap_box3D_sphere,
	COMMUTATOR = '&&',
	RESTRICT = areasel, JOIN = areajoinsel
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION overlap_poly3D_box3D(poly Polygon3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overlap_poly3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR && (
	LEFTARG = Polygon3D, RIGHTARG = Box3D, PROCEDURE = overlap_poly3D_box3D,
	COMMUTATOR = '&&',
	RESTRICT = areasel, JOIN = areajoinsel
);
CREATE OR REPLACE FUNCTION overlap_poly3D_poly3D(poly1 Polygon3D, poly2 Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overlap_poly3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR && (
	LEFTARG = Polygon3D, RIGHTARG = Polygon3D, PROCEDURE = overlap_poly3D_poly3D,
	COMMUTATOR = '&&',
	RESTRICT = areasel, JOIN = areajoinsel
);
/*
CREATE OR REPLACE FUNCTION overlap_poly3D_sphere(poly Polygon3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overlap_poly3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR && (
	LEFTARG = Polygon3D, RIGHTARG = Sphere, PROCEDURE = overlap_poly3D_sphere,
	COMMUTATOR = '&&',
	RESTRICT = areasel, JOIN = areajoinsel
);
*/

/******************************************************************************/

CREATE OR REPLACE FUNCTION overlap_sphere_box3D(sphere Sphere, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overlap_sphere_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR && (
	LEFTARG = Sphere, RIGHTARG = Box3D, PROCEDURE = overlap_sphere_box3D,
	COMMUTATOR = '&&',
	RESTRICT = areasel, JOIN = areajoinsel
);
/*
CREATE OR REPLACE FUNCTION overlap_sphere_poly3D(sphere Sphere, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overlap_sphere_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR && (
	LEFTARG = Sphere, RIGHTARG = Polygon3D, PROCEDURE = overlap_sphere_poly3D,
	COMMUTATOR = '&&',
	RESTRICT = areasel, JOIN = areajoinsel
);
*/
CREATE OR REPLACE FUNCTION overlap_sphere_sphere(sphere1 Sphere, sphere2 Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overlap_sphere_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR && (
	LEFTARG = Sphere, RIGHTARG = Sphere, PROCEDURE = overlap_sphere_sphere,
	COMMUTATOR = '&&',
	RESTRICT = areasel, JOIN = areajoinsel
);

/******************************************************************************
 * Intersect functions
 *****************************************************************************/

CREATE OR REPLACE FUNCTION intersect_lseg3D_lseg3D(s1 Lseg3D, s2 Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_lseg3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Lseg3D, RIGHTARG = Lseg3D, PROCEDURE = intersect_lseg3D_lseg3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_lseg3D_line3D(lseg Lseg3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_lseg3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Lseg3D, RIGHTARG = Line3D, PROCEDURE = intersect_lseg3D_line3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_lseg3D_box3D(lseg Lseg3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_lseg3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Lseg3D, RIGHTARG = Box3D, PROCEDURE = intersect_lseg3D_box3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_lseg3D_path3D(lseg Lseg3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_lseg3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Lseg3D, RIGHTARG = Path3D, PROCEDURE = intersect_lseg3D_path3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_lseg3D_poly3D(lseg Lseg3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_lseg3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Lseg3D, RIGHTARG = Polygon3D, PROCEDURE = intersect_lseg3D_poly3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_lseg3D_sphere(lseg Lseg3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_lseg3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Lseg3D, RIGHTARG = Sphere, PROCEDURE = intersect_lseg3D_sphere,
	COMMUTATOR = ?#
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION intersect_line3D_lseg3D(line Line3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_line3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Line3D, RIGHTARG = Lseg3D, PROCEDURE = intersect_line3D_lseg3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_line3D_line3D(line1 Line3D, line2 Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_line3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Line3D, RIGHTARG = Line3D, PROCEDURE = intersect_line3D_line3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_line3D_box3D(line Line3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_line3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Line3D, RIGHTARG = Box3D, PROCEDURE = intersect_line3D_box3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_line3D_path3D(line Line3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_line3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Line3D, RIGHTARG = Path3D, PROCEDURE = intersect_line3D_path3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_line3D_poly3D(line Line3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_line3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Line3D, RIGHTARG = Polygon3D, PROCEDURE = intersect_line3D_poly3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_line3D_sphere(line Line3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_line3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Line3D, RIGHTARG = Sphere, PROCEDURE = intersect_line3D_sphere,
	COMMUTATOR = ?#
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION intersect_box3D_lseg3D(box Box3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_box3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Box3D, RIGHTARG = Lseg3D, PROCEDURE = intersect_box3D_lseg3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_box3D_line3D(box Box3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_box3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Box3D, RIGHTARG = Line3D, PROCEDURE = intersect_box3D_line3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_box3D_box3D(box1 Box3D, box2 Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_box3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Box3D, RIGHTARG = Box3D, PROCEDURE = intersect_box3D_box3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_box3D_path3D(box Box3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_box3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Box3D, RIGHTARG = Path3D, PROCEDURE = intersect_box3D_path3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_box3D_poly3D(box Box3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_box3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Box3D, RIGHTARG = Polygon3D, PROCEDURE = intersect_box3D_poly3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_box3D_sphere(box Box3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_box3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Box3D, RIGHTARG = Sphere, PROCEDURE = intersect_box3D_sphere,
	COMMUTATOR = ?#
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION intersect_path3D_lseg3D(path Path3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_path3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Path3D, RIGHTARG = Lseg3D, PROCEDURE = intersect_path3D_lseg3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_path3D_line3D(path Path3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_path3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Path3D, RIGHTARG = Line3D, PROCEDURE = intersect_path3D_line3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_path3D_box3D(path Path3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_path3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Path3D, RIGHTARG = Box3D, PROCEDURE = intersect_path3D_box3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_path3D_path3D(path1 Path3D, path2 Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_path3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Path3D, RIGHTARG = Path3D, PROCEDURE = intersect_path3D_path3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_path3D_poly3D(path Path3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_path3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Path3D, RIGHTARG = Polygon3D, PROCEDURE = intersect_path3D_poly3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_path3D_sphere(path Path3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_path3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Path3D, RIGHTARG = Sphere, PROCEDURE = intersect_path3D_sphere,
	COMMUTATOR = ?#
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION intersect_poly3D_lseg3D(poly Polygon3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_poly3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Polygon3D, RIGHTARG = Lseg3D, PROCEDURE = intersect_poly3D_lseg3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_poly3D_line3D(poly Polygon3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_poly3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Polygon3D, RIGHTARG = Line3D, PROCEDURE = intersect_poly3D_line3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_poly3D_box3D(poly Polygon3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_poly3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Polygon3D, RIGHTARG = Box3D, PROCEDURE = intersect_poly3D_box3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_poly3D_path3D(poly Polygon3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_poly3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Polygon3D, RIGHTARG = Path3D, PROCEDURE = intersect_poly3D_path3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_poly3D_poly3D(poly1 Polygon3D, poly2 Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_poly3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Polygon3D, RIGHTARG = Polygon3D, PROCEDURE = intersect_poly3D_poly3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_poly3D_sphere(poly Polygon3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_poly3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Polygon3D, RIGHTARG = Sphere, PROCEDURE = intersect_poly3D_sphere,
	COMMUTATOR = ?#
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION intersect_sphere_lseg3D(sphere Sphere, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_sphere_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Sphere, RIGHTARG = Lseg3D, PROCEDURE = intersect_sphere_lseg3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_sphere_line3D(sphere Sphere, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_sphere_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Sphere, RIGHTARG = Line3D, PROCEDURE = intersect_sphere_line3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_sphere_box3D(sphere Sphere, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_sphere_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Sphere, RIGHTARG = Box3D, PROCEDURE = intersect_sphere_box3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_sphere_path3D(sphere Sphere, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_sphere_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Sphere, RIGHTARG = Path3D, PROCEDURE = intersect_sphere_path3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_sphere_poly3D(sphere Sphere, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_sphere_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Sphere, RIGHTARG = Polygon3D, PROCEDURE = intersect_sphere_poly3D,
	COMMUTATOR = ?#
);
CREATE OR REPLACE FUNCTION intersect_sphere_sphere(sphere1 Sphere, sphere2 Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'intersect_sphere_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?# (
	LEFTARG = Sphere, RIGHTARG = Sphere, PROCEDURE = intersect_sphere_sphere,
	COMMUTATOR = ?#
);

/******************************************************************************
 * Intersection point functions
 *****************************************************************************/

CREATE OR REPLACE FUNCTION interpoint_lseg3D_lseg3D(lseg1 Lseg3D, lseg2 Lseg3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_lseg3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Lseg3D, RIGHTARG = Lseg3D, PROCEDURE = interpoint_lseg3D_lseg3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_lseg3D_line3D(lseg Lseg3D, line Line3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_lseg3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Lseg3D, RIGHTARG = Line3D, PROCEDURE = interpoint_lseg3D_line3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_lseg3D_box3D(lseg Lseg3D, box Box3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_lseg3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Lseg3D, RIGHTARG = Box3D, PROCEDURE = interpoint_lseg3D_box3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_lseg3D_path3D(lseg Lseg3D, path Path3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_lseg3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Lseg3D, RIGHTARG = Path3D, PROCEDURE = interpoint_lseg3D_path3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_lseg3D_poly3D(lseg Lseg3D, path Polygon3D) RETURNS Polygon3D AS 'MODULE_PATHNAME', 'interpoint_lseg3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Lseg3D, RIGHTARG = Polygon3D, PROCEDURE = interpoint_lseg3D_poly3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_lseg3D_sphere(lseg Lseg3D, sphere Sphere) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_lseg3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Lseg3D, RIGHTARG = Sphere, PROCEDURE = interpoint_lseg3D_sphere,
	COMMUTATOR = #
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION interpoint_line3D_lseg3D(line Line3D, lseg Lseg3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_line3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Line3D, RIGHTARG = Lseg3D, PROCEDURE = interpoint_line3D_lseg3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_line3D_line3D(line1 Line3D, line2 Line3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_line3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Line3D, RIGHTARG = Line3D, PROCEDURE = interpoint_line3D_line3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_line3D_box3D(line Line3D, box Box3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_line3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Line3D, RIGHTARG = Box3D, PROCEDURE = interpoint_line3D_box3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_line3D_path3D(line Line3D, path Path3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_line3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Line3D, RIGHTARG = Path3D, PROCEDURE = interpoint_line3D_path3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_line3D_poly3D(line Line3D, poly Polygon3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_line3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Line3D, RIGHTARG = Polygon3D, PROCEDURE = interpoint_line3D_poly3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_line3D_sphere(line Line3D, sphere Sphere) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_line3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Line3D, RIGHTARG = Sphere, PROCEDURE = interpoint_line3D_sphere,
	COMMUTATOR = #
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION interpoint_box3D_lseg3D(box Box3D, lseg Lseg3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_box3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Box3D, RIGHTARG = Lseg3D, PROCEDURE = interpoint_box3D_lseg3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_box3D_line3D(box Box3D, line Line3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_box3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Box3D, RIGHTARG = Line3D, PROCEDURE = interpoint_box3D_line3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_box3D_box3D(box1 Box3D, box2 Box3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_box3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Box3D, RIGHTARG = Box3D, PROCEDURE = interpoint_box3D_box3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_box3D_path3D(box Box3D, path Path3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_box3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Box3D, RIGHTARG = Path3D, PROCEDURE = interpoint_box3D_path3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_box3D_poly3D(box Box3D, poly Polygon3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_box3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Box3D, RIGHTARG = Polygon3D, PROCEDURE = interpoint_box3D_poly3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_box3D_sphere(box Box3D, sphere Sphere) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_box3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Box3D, RIGHTARG = Sphere, PROCEDURE = interpoint_box3D_sphere,
	COMMUTATOR = #
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION interpoint_path3D_lseg3D(path Path3D, lseg Lseg3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_path3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Path3D, RIGHTARG = Lseg3D, PROCEDURE = interpoint_path3D_lseg3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_path3D_line3D(path Path3D, line Line3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_path3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Path3D, RIGHTARG = Line3D, PROCEDURE = interpoint_path3D_line3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_path3D_box3D(path Path3D, box Box3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_path3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Path3D, RIGHTARG = Box3D, PROCEDURE = interpoint_path3D_box3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_path3D_path3D(path1 Path3D, path2 Path3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_path3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Path3D, RIGHTARG = Path3D, PROCEDURE = interpoint_path3D_path3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_path3D_poly3D(path Path3D, poly Polygon3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_path3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Path3D, RIGHTARG = Polygon3D, PROCEDURE = interpoint_path3D_poly3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_path3D_sphere(path Path3D, sphere Sphere) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_path3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Path3D, RIGHTARG = Sphere, PROCEDURE = interpoint_path3D_sphere,
	COMMUTATOR = #
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION interpoint_poly3D_lseg3D(poly Polygon3D, lseg Lseg3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_poly3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Polygon3D, RIGHTARG = Lseg3D, PROCEDURE = interpoint_poly3D_lseg3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_poly3D_line3D(poly Polygon3D, line Line3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_poly3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Polygon3D, RIGHTARG = Line3D, PROCEDURE = interpoint_poly3D_line3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_poly3D_box3D(poly Polygon3D, box Box3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_poly3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Polygon3D, RIGHTARG = Box3D, PROCEDURE = interpoint_poly3D_box3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_poly3D_path3D(poly Polygon3D, path Path3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_poly3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Polygon3D, RIGHTARG = Path3D, PROCEDURE = interpoint_poly3D_path3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_poly3D_poly3D(poly1 Polygon3D, poly2 Polygon3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_poly3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Polygon3D, RIGHTARG = Polygon3D, PROCEDURE = interpoint_poly3D_poly3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_poly3D_sphere(poly Polygon3D, sphere Sphere) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_poly3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Polygon3D, RIGHTARG = Sphere, PROCEDURE = interpoint_poly3D_sphere,
	COMMUTATOR = #
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION interpoint_sphere_lseg3D(sphere Sphere, lseg Lseg3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_sphere_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Sphere, RIGHTARG = Lseg3D, PROCEDURE = interpoint_sphere_lseg3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_sphere_line3D(sphere Sphere, line Line3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_sphere_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Sphere, RIGHTARG = Line3D, PROCEDURE = interpoint_sphere_line3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_sphere_box3D(sphere Sphere, box Box3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_sphere_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Sphere, RIGHTARG = Box3D, PROCEDURE = interpoint_sphere_box3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_sphere_path3D(sphere Sphere, path Path3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_sphere_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Sphere, RIGHTARG = Path3D, PROCEDURE = interpoint_sphere_path3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_sphere_poly3D(sphere Sphere, poly Polygon3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_sphere_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Sphere, RIGHTARG = Polygon3D, PROCEDURE = interpoint_sphere_poly3D,
	COMMUTATOR = #
);
CREATE OR REPLACE FUNCTION interpoint_sphere_sphere(sphere1 Sphere, sphere2 Sphere) RETURNS Point3D AS 'MODULE_PATHNAME', 'interpoint_sphere_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	LEFTARG = Sphere, RIGHTARG = Sphere, PROCEDURE = interpoint_sphere_sphere,
	COMMUTATOR = #
);

/******************************************************************************
 * Intersection functions
 *****************************************************************************/

CREATE OR REPLACE FUNCTION box3D_intersection(box1 Box3D, box2 Box3D) RETURNS Box3D AS 'MODULE_PATHNAME', 'box3D_intersection' LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************
 * Distance functions 
 *****************************************************************************/

CREATE OR REPLACE FUNCTION distance_point3D_point3D(point1 Point3D, point2 Point3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_point3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Point3D, RIGHTARG = Point3D, PROCEDURE = distance_point3D_point3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_point3D_lseg3D(point Point3D, lseg Lseg3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_point3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Point3D, RIGHTARG = Lseg3D, PROCEDURE = distance_point3D_lseg3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_point3D_line3D(point Point3D, line Line3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_point3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Point3D, RIGHTARG = Line3D, PROCEDURE = distance_point3D_line3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_point3D_box3D(point Point3D, box Box3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_point3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Point3D, RIGHTARG = Box3D, PROCEDURE = distance_point3D_box3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_point3D_path3D(point Point3D, path Path3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_point3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Point3D, RIGHTARG = Path3D, PROCEDURE = distance_point3D_path3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_point3D_poly3D(point Point3D, poly Polygon3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_point3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Point3D, RIGHTARG = Polygon3D, PROCEDURE = distance_point3D_poly3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_point3D_sphere(point Point3D, sphere Sphere) RETURNS float AS 'MODULE_PATHNAME', 'distance_point3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Point3D, RIGHTARG = Sphere, PROCEDURE = distance_point3D_sphere,
	COMMUTATOR = '<->'
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION distance_lseg3D_point3D(lseg Lseg3D, point Point3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_lseg3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Lseg3D, RIGHTARG = Point3D, PROCEDURE = distance_lseg3D_point3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_lseg3D_lseg3D(lseg1 Lseg3D, lseg2 Lseg3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_lseg3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Lseg3D, RIGHTARG = Lseg3D, PROCEDURE = distance_lseg3D_lseg3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_lseg3D_line3D(lseg Lseg3D, line Line3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_lseg3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Lseg3D, RIGHTARG = Line3D, PROCEDURE = distance_lseg3D_line3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_lseg3D_box3D(lseg Lseg3D, box Box3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_lseg3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Lseg3D, RIGHTARG = Box3D, PROCEDURE = distance_lseg3D_box3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_lseg3D_path3D(lseg Lseg3D, path Path3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_lseg3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Lseg3D, RIGHTARG = Path3D, PROCEDURE = distance_lseg3D_path3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_lseg3D_poly3D(lseg Lseg3D, poly Polygon3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_lseg3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Lseg3D, RIGHTARG = Polygon3D, PROCEDURE = distance_lseg3D_poly3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_lseg3D_sphere(lseg Lseg3D, sphere Sphere) RETURNS float AS 'MODULE_PATHNAME', 'distance_lseg3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Lseg3D, RIGHTARG = Sphere, PROCEDURE = distance_lseg3D_sphere,
	COMMUTATOR = '<->'
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION distance_line3D_point3D(line Line3D, point Point3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_line3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Line3D, RIGHTARG = Point3D, PROCEDURE = distance_line3D_point3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_line3D_lseg3D(line Line3D, lseg Lseg3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_line3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Line3D, RIGHTARG = Lseg3D, PROCEDURE = distance_line3D_lseg3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_line3D_line3D(line1 Line3D, line2 Line3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_line3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Line3D, RIGHTARG = Line3D, PROCEDURE = distance_line3D_line3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_line3D_box3D(line Line3D, box Box3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_line3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Line3D, RIGHTARG = Box3D, PROCEDURE = distance_line3D_box3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_line3D_path3D(line Line3D, path Path3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_line3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Line3D, RIGHTARG = Path3D, PROCEDURE = distance_line3D_path3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_line3D_poly3D(line Line3D, poly Polygon3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_line3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Line3D, RIGHTARG = Polygon3D, PROCEDURE = distance_line3D_poly3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_line3D_sphere(line Line3D, sphere Sphere) RETURNS float AS 'MODULE_PATHNAME', 'distance_line3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Line3D, RIGHTARG = Sphere, PROCEDURE = distance_line3D_sphere,
	COMMUTATOR = '<->'
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION distance_box3D_point3D(box Box3D, point Point3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_box3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Box3D, RIGHTARG = Point3D, PROCEDURE = distance_box3D_point3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_box3D_lseg3D(box Box3D, lseg Lseg3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_box3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Box3D, RIGHTARG = Lseg3D, PROCEDURE = distance_box3D_lseg3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_box3D_line3D(box Box3D, line Line3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_box3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Box3D, RIGHTARG = Line3D, PROCEDURE = distance_box3D_line3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_box3D_box3D(box1 Box3D, box2 Box3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_box3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Box3D, RIGHTARG = Box3D, PROCEDURE = distance_box3D_box3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_box3D_path3D(box Box3D, path Path3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_box3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Box3D, RIGHTARG = Path3D, PROCEDURE = distance_box3D_path3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_box3D_poly3D(box Box3D, poly Polygon3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_box3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Box3D, RIGHTARG = Polygon3D, PROCEDURE = distance_box3D_poly3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_box3D_sphere(box Box3D, sphere Sphere) RETURNS float AS 'MODULE_PATHNAME', 'distance_box3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Box3D, RIGHTARG = Sphere, PROCEDURE = distance_box3D_sphere,
	COMMUTATOR = '<->'
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION distance_path3D_point3D(path Path3D, point Point3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_path3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Path3D, RIGHTARG = Point3D, PROCEDURE = distance_path3D_point3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_path3D_lseg3D(path Path3D, lseg Lseg3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_path3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Path3D, RIGHTARG = Lseg3D, PROCEDURE = distance_path3D_lseg3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_path3D_line3D(path Path3D, line Line3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_path3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Path3D, RIGHTARG = Line3D, PROCEDURE = distance_path3D_line3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_path3D_box3D(path Path3D, box Box3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_path3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Path3D, RIGHTARG = Box3D, PROCEDURE = distance_path3D_box3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_path3D_path3D(path1 Path3D, path2 Path3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_path3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Path3D, RIGHTARG = Path3D, PROCEDURE = distance_path3D_path3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_path3D_poly3D(path Path3D, poly Polygon3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_path3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Path3D, RIGHTARG = Polygon3D, PROCEDURE = distance_path3D_poly3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_path3D_sphere(path Path3D, sphere Sphere) RETURNS float AS 'MODULE_PATHNAME', 'distance_path3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Path3D, RIGHTARG = Sphere, PROCEDURE = distance_path3D_sphere,
	COMMUTATOR = '<->'
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION distance_poly3D_point3D(poly Polygon3D, point Point3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_poly3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Polygon3D, RIGHTARG = Point3D, PROCEDURE = distance_poly3D_point3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_poly3D_lseg3D(poly Polygon3D, lseg Lseg3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_poly3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Polygon3D, RIGHTARG = Lseg3D, PROCEDURE = distance_poly3D_lseg3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_poly3D_line3D(poly Polygon3D, line Line3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_poly3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Polygon3D, RIGHTARG = Line3D, PROCEDURE = distance_poly3D_line3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_poly3D_box3D(poly Polygon3D, box Box3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_poly3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Polygon3D, RIGHTARG = Box3D, PROCEDURE = distance_poly3D_box3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_poly3D_path3D(poly Polygon3D, path Path3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_poly3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Polygon3D, RIGHTARG = Path3D, PROCEDURE = distance_poly3D_path3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_poly3D_poly3D(poly1 Polygon3D, poly2 Polygon3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_poly3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Polygon3D, RIGHTARG = Polygon3D, PROCEDURE = distance_poly3D_poly3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_poly3D_sphere(poly Polygon3D, sphere Sphere) RETURNS float AS 'MODULE_PATHNAME', 'distance_poly3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Polygon3D, RIGHTARG = Sphere, PROCEDURE = distance_poly3D_sphere,
	COMMUTATOR = '<->'
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION distance_sphere_point3D(sphere Sphere, point Point3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_sphere_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Sphere, RIGHTARG = Point3D, PROCEDURE = distance_sphere_point3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_sphere_lseg3D(sphere Sphere, lseg Lseg3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_sphere_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Sphere, RIGHTARG = Lseg3D, PROCEDURE = distance_sphere_lseg3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_sphere_line3D(sphere Sphere, line Line3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_sphere_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Sphere, RIGHTARG = Line3D, PROCEDURE = distance_sphere_line3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_sphere_box3D(sphere Sphere, box Box3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_sphere_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Sphere, RIGHTARG = Box3D, PROCEDURE = distance_sphere_box3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_sphere_path3D(sphere Sphere, path Path3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_sphere_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Sphere, RIGHTARG = Path3D, PROCEDURE = distance_sphere_path3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_sphere_poly3D(sphere Sphere, poly Polygon3D) RETURNS float AS 'MODULE_PATHNAME', 'distance_sphere_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Sphere, RIGHTARG = Polygon3D, PROCEDURE = distance_sphere_poly3D,
	COMMUTATOR = '<->'
);
CREATE OR REPLACE FUNCTION distance_sphere_sphere(sphere1 Sphere, sphere2 Sphere) RETURNS float AS 'MODULE_PATHNAME', 'distance_sphere_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <-> (
	LEFTARG = Sphere, RIGHTARG = Sphere, PROCEDURE = distance_sphere_sphere,
	COMMUTATOR = '<->'
);

/******************************************************************************
 * Closest point functions
 *****************************************************************************/

CREATE OR REPLACE FUNCTION closestpoint_point3D_lseg3D(point Point3D, lseg Lseg3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_point3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Point3D, RIGHTARG = Lseg3D, PROCEDURE = closestpoint_point3D_lseg3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_point3D_line3D(point Point3D, line Line3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_point3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Point3D, RIGHTARG = Line3D, PROCEDURE = closestpoint_point3D_line3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_point3D_box3D(point Point3D, box Box3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_point3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Point3D, RIGHTARG = Box3D, PROCEDURE = closestpoint_point3D_box3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_point3D_path3D(point Point3D, path Path3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_point3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Point3D, RIGHTARG = Path3D, PROCEDURE = closestpoint_point3D_path3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_point3D_poly3D(point Point3D, poly Polygon3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_point3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Point3D, RIGHTARG = Polygon3D, PROCEDURE = closestpoint_point3D_poly3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_point3D_sphere(point Point3D, sphere Sphere) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_point3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Point3D, RIGHTARG = Sphere, PROCEDURE = closestpoint_point3D_sphere,
	COMMUTATOR = ##
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION closestpoint_lseg3D_lseg3D(s1 Lseg3D, s2 Lseg3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_lseg3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Lseg3D, RIGHTARG = Lseg3D, PROCEDURE = closestpoint_lseg3D_lseg3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_lseg3D_line3D(lseg Lseg3D, line Line3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_lseg3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Lseg3D, RIGHTARG = Line3D, PROCEDURE = closestpoint_lseg3D_line3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_lseg3D_box3D(lseg Lseg3D, box Box3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_lseg3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Lseg3D, RIGHTARG = Box3D, PROCEDURE = closestpoint_lseg3D_box3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_lseg3D_path3D(lseg Lseg3D, path Path3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_lseg3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Lseg3D, RIGHTARG = Path3D, PROCEDURE = closestpoint_lseg3D_path3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_lseg3D_poly3D(lseg Lseg3D, poly Polygon3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_lseg3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Lseg3D, RIGHTARG = Polygon3D, PROCEDURE = closestpoint_lseg3D_poly3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_lseg3D_sphere(lseg Lseg3D, sphere Sphere) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_lseg3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Lseg3D, RIGHTARG = Sphere, PROCEDURE = closestpoint_lseg3D_sphere,
	COMMUTATOR = ##
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION closestpoint_line3D_lseg3D(line Line3D, lseg Lseg3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_line3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Line3D, RIGHTARG = Lseg3D, PROCEDURE = closestpoint_line3D_lseg3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_line3D_line3D(line1 Line3D, line2 Line3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_line3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Line3D, RIGHTARG = Line3D, PROCEDURE = closestpoint_line3D_line3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_line3D_box3D(line Line3D, box Box3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_line3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Line3D, RIGHTARG = Box3D, PROCEDURE = closestpoint_line3D_box3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_line3D_path3D(line Line3D, path Path3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_line3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Line3D, RIGHTARG = Path3D, PROCEDURE = closestpoint_line3D_path3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_line3D_poly3D(line Line3D, poly Polygon3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_line3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Line3D, RIGHTARG = Polygon3D, PROCEDURE = closestpoint_line3D_poly3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_line3D_sphere(line Line3D, sphere Sphere) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_line3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Line3D, RIGHTARG = Sphere, PROCEDURE = closestpoint_line3D_sphere,
	COMMUTATOR = ##
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION closestpoint_box3D_lseg3D(box Box3D, lseg Lseg3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_box3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Box3D, RIGHTARG = Lseg3D, PROCEDURE = closestpoint_box3D_lseg3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_box3D_line3D(box Box3D, line Line3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_box3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Box3D, RIGHTARG = Line3D, PROCEDURE = closestpoint_box3D_line3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_box3D_box3D(box1 Box3D, box2 Box3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_box3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Box3D, RIGHTARG = Box3D, PROCEDURE = closestpoint_box3D_box3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_box3D_path3D(box Box3D, path Path3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_box3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Box3D, RIGHTARG = Path3D, PROCEDURE = closestpoint_box3D_path3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_box3D_poly3D(box Box3D, poly Polygon3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_box3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Box3D, RIGHTARG = Polygon3D, PROCEDURE = closestpoint_box3D_poly3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_box3D_sphere(box Box3D, sphere Sphere) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_box3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Box3D, RIGHTARG = Sphere, PROCEDURE = closestpoint_box3D_sphere,
	COMMUTATOR = ##
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION closestpoint_path3D_lseg3D(path Path3D, lseg Lseg3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_path3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Path3D, RIGHTARG = Lseg3D, PROCEDURE = closestpoint_path3D_lseg3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_path3D_line3D(path Path3D, line Line3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_path3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Path3D, RIGHTARG = Line3D, PROCEDURE = closestpoint_path3D_line3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_path3D_box3D(path Path3D, box Box3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_path3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Path3D, RIGHTARG = Box3D, PROCEDURE = closestpoint_path3D_box3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_path3D_path3D(path1 Path3D, path2 Path3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_path3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Path3D, RIGHTARG = Path3D, PROCEDURE = closestpoint_path3D_path3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_path3D_poly3D(path Path3D, poly Polygon3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_path3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Path3D, RIGHTARG = Polygon3D, PROCEDURE = closestpoint_path3D_poly3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_path3D_sphere(path Path3D, sphere Sphere) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_path3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Path3D, RIGHTARG = Sphere, PROCEDURE = closestpoint_path3D_sphere,
	COMMUTATOR = ##
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION closestpoint_poly3D_lseg3D(poly Polygon3D, lseg Lseg3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_poly3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Polygon3D, RIGHTARG = Lseg3D, PROCEDURE = closestpoint_poly3D_lseg3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_poly3D_line3D(poly Polygon3D, line Line3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_poly3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Polygon3D, RIGHTARG = Line3D, PROCEDURE = closestpoint_poly3D_line3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_poly3D_box3D(poly Polygon3D, box Box3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_poly3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Polygon3D, RIGHTARG = Box3D, PROCEDURE = closestpoint_poly3D_box3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_poly3D_path3D(poly Polygon3D, path Path3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_poly3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Polygon3D, RIGHTARG = Path3D, PROCEDURE = closestpoint_poly3D_path3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_poly3D_poly3D(poly1 Polygon3D, poly2 Polygon3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_poly3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Polygon3D, RIGHTARG = Polygon3D, PROCEDURE = closestpoint_poly3D_poly3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_poly3D_sphere(poly Polygon3D, sphere Sphere) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_poly3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Polygon3D, RIGHTARG = Sphere, PROCEDURE = closestpoint_poly3D_sphere,
	COMMUTATOR = ##
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION closestpoint_sphere_lseg3D(sphere Sphere, lseg Lseg3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_sphere_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Sphere, RIGHTARG = Lseg3D, PROCEDURE = closestpoint_sphere_lseg3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_sphere_line3D(sphere Sphere, line Line3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_sphere_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Sphere, RIGHTARG = Line3D, PROCEDURE = closestpoint_sphere_line3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_sphere_box3D(sphere Sphere, box Box3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_sphere_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Sphere, RIGHTARG = Box3D, PROCEDURE = closestpoint_sphere_box3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_sphere_path3D(sphere Sphere, path Path3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_sphere_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Sphere, RIGHTARG = Path3D, PROCEDURE = closestpoint_sphere_path3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_sphere_poly3D(sphere Sphere, poly Polygon3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_sphere_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Sphere, RIGHTARG = Polygon3D, PROCEDURE = closestpoint_sphere_poly3D,
	COMMUTATOR = ##
);
CREATE OR REPLACE FUNCTION closestpoint_sphere_sphere(sphere1 Sphere, sphere2 Sphere) RETURNS Point3D AS 'MODULE_PATHNAME', 'closestpoint_sphere_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ## (
	LEFTARG = Sphere, RIGHTARG = Sphere, PROCEDURE = closestpoint_sphere_sphere,
	COMMUTATOR = ##
);

/******************************************************************************/


