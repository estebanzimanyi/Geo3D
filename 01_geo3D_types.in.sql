/*-------------------------------------------------------------------------
 *
 * geo3D_types.sql
 *	  Type definitions for 3D geometries
 *
 * Copyright (c) 2017, Esteban Zimanyi, Université Libre de Bruxelles
 *
 *-------------------------------------------------------------------------*/

DROP TYPE IF EXISTS Point3D CASCADE;
DROP TYPE IF EXISTS Lseg3D CASCADE;
DROP TYPE IF EXISTS Path3D CASCADE;
DROP TYPE IF EXISTS Line3D CASCADE;
DROP TYPE IF EXISTS Box3D CASCADE;
DROP TYPE IF EXISTS Polygon3D CASCADE;
DROP TYPE IF EXISTS Sphere CASCADE;
CREATE TYPE Point3D;
CREATE TYPE Lseg3D;
CREATE TYPE Path3D;
CREATE TYPE Line3D;
CREATE TYPE Box3D;
CREATE TYPE Polygon3D;
CREATE TYPE Sphere;
 
/******************************************************************************/
 
CREATE OR REPLACE FUNCTION point3D_in(cstring) RETURNS Point3D AS 'MODULE_PATHNAME', 'point3D_in' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION point3D_out(Point3D) RETURNS cstring AS 'MODULE_PATHNAME', 'point3D_out' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION point3D_recv(internal) RETURNS Point3D AS 'MODULE_PATHNAME', 'point3D_recv' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION point3D_send(Point3D) RETURNS bytea AS 'MODULE_PATHNAME', 'point3D_send' LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE Point3D (
   internallength = 24,
   input = Point3D_in,
   output = Point3D_out,
   receive = Point3D_recv,
   send = Point3D_send,
   alignment = double,
   element = float8
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION Lseg3D_in(cstring) RETURNS Lseg3D AS 'MODULE_PATHNAME', 'lseg3D_in' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION Lseg3D_out(Lseg3D) RETURNS cstring AS 'MODULE_PATHNAME', 'lseg3D_out' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION Lseg3D_recv(internal) RETURNS Lseg3D AS 'MODULE_PATHNAME', 'lseg3D_recv' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION Lseg3D_send(Lseg3D) RETURNS bytea AS 'MODULE_PATHNAME', 'lseg3D_send' LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE Lseg3D (
   internallength = 48,
   input = lseg3D_in,
   output = lseg3D_out,
   receive = lseg3D_recv,
   send = lseg3D_send,
   alignment = double,
   element = Point3D
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION line3D_in(cstring) RETURNS Line3D AS 'MODULE_PATHNAME', 'line3D_in' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION line3D_out(Line3D) RETURNS cstring AS 'MODULE_PATHNAME', 'line3D_out' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION line3D_recv(internal) RETURNS Line3D AS 'MODULE_PATHNAME', 'line3D_recv' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION line3D_send(Line3D) RETURNS bytea AS 'MODULE_PATHNAME', 'line3D_send' LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE Line3D (
   internallength = 48,
   input = line3D_in,
   output = line3D_out,
   receive = line3D_recv,
   send = line3D_send,
   alignment = double,
   element = float8
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION box3D_in(cstring) RETURNS Box3D AS 'MODULE_PATHNAME', 'box3D_in' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION box3D_out(Box3D) RETURNS cstring AS 'MODULE_PATHNAME', 'box3D_out' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION box3D_recv(internal) RETURNS Box3D AS 'MODULE_PATHNAME', 'box3D_recv' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION box3D_send(Box3D) RETURNS bytea AS 'MODULE_PATHNAME', 'box3D_send' LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE Box3D (
   internallength = 48,
   input = Box3D_in,
   output = Box3D_out,
   receive = Box3D_recv,
   send = Box3D_send,
   alignment = double,
   element = Point3D
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION path3D_in(cstring) RETURNS Path3D AS 'MODULE_PATHNAME', 'path3D_in' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION path3D_out(Path3D) RETURNS cstring AS 'MODULE_PATHNAME', 'path3D_out' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION path3D_recv(internal) RETURNS Path3D AS 'MODULE_PATHNAME', 'path3D_recv' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION path3D_send(Path3D) RETURNS bytea AS 'MODULE_PATHNAME', 'path3D_send' LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE Path3D (
   internallength = variable,
   input = path3D_in,
   output = path3D_out,
   receive = path3D_recv,
   send = path3D_send,
   alignment = double
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION poly3D_in(cstring) RETURNS Polygon3D AS 'MODULE_PATHNAME', 'poly3D_in' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION poly3D_out(Polygon3D) RETURNS cstring AS 'MODULE_PATHNAME', 'poly3D_out' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION poly3D_recv(internal) RETURNS Polygon3D AS 'MODULE_PATHNAME', 'poly3D_recv' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION poly3D_send(Polygon3D) RETURNS bytea AS 'MODULE_PATHNAME', 'poly3D_send' LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE Polygon3D (
   internallength = variable,
   input = poly3D_in,
   output = poly3D_out,
   receive = poly3D_recv,
   send = poly3D_send,
   alignment = double
);

/******************************************************************************/

CREATE OR REPLACE FUNCTION sphere_in(cstring) RETURNS Sphere AS 'MODULE_PATHNAME', 'sphere_in' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION sphere_out(Sphere) RETURNS cstring AS 'MODULE_PATHNAME', 'sphere_out' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION sphere_recv(internal) RETURNS Sphere AS 'MODULE_PATHNAME', 'sphere_recv' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION sphere_send(Sphere) RETURNS bytea AS 'MODULE_PATHNAME', 'sphere_send' LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE Sphere (
   internallength = 32,
   input = sphere_in,
   output = sphere_out,
   receive = sphere_recv,
   send = sphere_send,
   alignment = double,
   element = float8
);

/******************************************************************************
 * geo3D_selfuncs.c
 *****************************************************************************/

CREATE OR REPLACE FUNCTION volumesel(internal, oid, internal, integer) RETURNS float AS 'MODULE_PATHNAME', 'volumesel' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION volumejoinsel(internal, oid, internal, integer) RETURNS float AS 'MODULE_PATHNAME', 'volumejoinsel' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION positionsel3D(internal, oid, internal, integer) RETURNS float AS 'MODULE_PATHNAME', 'positionsel3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION positionjoinsel3D(internal, oid, internal, smallint, internal) RETURNS float AS 'MODULE_PATHNAME', 'positionjoinsel3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION contsel3D(internal, oid, internal, integer) RETURNS float AS 'MODULE_PATHNAME', 'contsel3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION contjoinsel3D(internal, oid, internal, smallint, internal) RETURNS float AS 'MODULE_PATHNAME', 'contjoinsel3D' LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************
 * Point3D functions
 *****************************************************************************/

CREATE OR REPLACE FUNCTION point3D(x float8, y float8, z float8) RETURNS Point3D AS 'MODULE_PATHNAME', 'point3D_construct' LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION point3D_x(point Point3D) RETURNS float8 AS 'MODULE_PATHNAME', 'point3D_x' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION point3D_y(point Point3D) RETURNS float8 AS 'MODULE_PATHNAME', 'point3D_y' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION point3D_z(point Point3D) RETURNS float8 AS 'MODULE_PATHNAME', 'point3D_z' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION boundbox(point Point3D) RETURNS Box3D AS 'MODULE_PATHNAME', 'point3D_boundbox' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION boundsphere(point Point3D) RETURNS Sphere AS 'MODULE_PATHNAME', 'point3D_boundsphere' LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION point3D_same(point1 Point3D, point2 Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'point3D_same' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ~= (
	LEFTARG = Point3D, RIGHTARG = Point3D, PROCEDURE = point3D_same,
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);
 
CREATE OR REPLACE FUNCTION point3D_horizontal(point1 Point3D, point2 Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'point3D_horizontal' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?- (
	LEFTARG = Point3D, RIGHTARG = Point3D, PROCEDURE = point3D_horizontal,
	COMMUTATOR = '?-',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION point3D_vertical(point1 Point3D, point2 Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'point3D_vertical' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?| (
	LEFTARG = Point3D, RIGHTARG = Point3D, PROCEDURE = point3D_vertical,
	COMMUTATOR = '?|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION point3D_perpendicular(point1 Point3D, point2 Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'point3D_perpendicular' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?/ (
	LEFTARG = Point3D, RIGHTARG = Point3D, PROCEDURE = point3D_perpendicular,
	COMMUTATOR = '?/',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION point3D_collinear(point1 Point3D, point2 Point3D, point3 Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'point3D_collinear' LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION point3D_add(point1 Point3D, point2 Point3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'point3D_add' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR + (
	PROCEDURE = point3D_add,
	LEFTARG = Point3D,
	RIGHTARG = Point3D,
	COMMUTATOR = +
);

CREATE OR REPLACE FUNCTION point3D_sub(point1 Point3D, point2 Point3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'point3D_sub' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR - (
	PROCEDURE = point3D_sub,
	LEFTARG = Point3D,
	RIGHTARG = Point3D
);

CREATE OR REPLACE FUNCTION point3D_mul(point Point3D, f float) RETURNS Point3D AS 'MODULE_PATHNAME', 'point3D_mul' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR * (
	PROCEDURE = point3D_mul,
	LEFTARG = Point3D,
	RIGHTARG = float,
	COMMUTATOR = *
);

CREATE OR REPLACE FUNCTION point3D_div(point Point3D, f float) RETURNS Point3D AS 'MODULE_PATHNAME', 'point3D_div' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR / (
	PROCEDURE = point3D_div,
	LEFTARG = Point3D,
	RIGHTARG = float
);

------------------------------------------------------------------------------
--	Operator class for defining b-tree index
------------------------------------------------------------------------------

CREATE FUNCTION point3D_lt(point3D, point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'point3D_lt' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION point3D_le(point3D, point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'point3D_le' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION point3D_eq(point3D, point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'point3D_eq' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION point3D_ne(point3D, point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'point3D_ne' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION point3D_ge(point3D, point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'point3D_ge' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION point3D_gt(point3D, point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'point3D_gt' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION point3D_cmp(point3D, point3D) RETURNS int4 AS 'MODULE_PATHNAME', 'point3D_cmp' LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR < (
   leftarg = point3D, rightarg = point3D, procedure = point3D_lt,
   commutator = > , negator = >= ,
   restrict = scalarltsel, join = scalarltjoinsel
);
CREATE OPERATOR <= (
   leftarg = point3D, rightarg = point3D, procedure = point3D_le,
   commutator = >= , negator = > ,
   restrict = scalarltsel, join = scalarltjoinsel
);
CREATE OPERATOR = (
   leftarg = point3D, rightarg = point3D, procedure = point3D_eq,
   commutator = = ,
   negator = <> ,
   restrict = eqsel, join = eqjoinsel
);
CREATE OPERATOR <> (
   leftarg = point3D, rightarg = point3D, procedure = point3D_ne,
   commutator = <> ,
   negator = = ,
   restrict = neqsel, join = neqjoinsel
);
CREATE OPERATOR >= (
   leftarg = point3D, rightarg = point3D, procedure = point3D_ge,
   commutator = <= , negator = < ,
   restrict = scalargtsel, join = scalargtjoinsel
);
CREATE OPERATOR > (
   leftarg = point3D, rightarg = point3D, procedure = point3D_gt,
   commutator = < , negator = <= ,
   restrict = scalargtsel, join = scalargtjoinsel
);

CREATE OPERATOR CLASS point3D_ops
    DEFAULT FOR TYPE point3D USING btree AS
        OPERATOR        1       < ,
        OPERATOR        2       <= ,
        OPERATOR        3       = ,
        OPERATOR        4       >= ,
        OPERATOR        5       > ,
        FUNCTION        1       point3D_cmp(point3D, point3D);

/******************************************************************************
 * Lseg3D functions
 *****************************************************************************/

CREATE OR REPLACE FUNCTION lseg3D(x1 float8, y1 float8, z1 float8, x2 float8, y2 float8, z2 float8) RETURNS Lseg3D AS 'MODULE_PATHNAME', 'lseg3D_construct' LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION lseg3D(pt1 Point3D, pt2 Point3D) RETURNS Lseg3D AS 'MODULE_PATHNAME', 'lseg3D_construct_pts' LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION lseg3D_points(lseg Lseg3D) RETURNS Point3D[] AS 'MODULE_PATHNAME', 'lseg3D_points' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION boundbox(lseg Lseg3D) RETURNS Box3D AS 'MODULE_PATHNAME', 'lseg3D_boundbox' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION boundsphere(lseg Lseg3D) RETURNS Sphere AS 'MODULE_PATHNAME', 'lseg3D_boundsphere' LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION lseg3D_same(lseg1 Lseg3D, lseg2 Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'lseg3D_same' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ~= (
	LEFTARG = Lseg3D, RIGHTARG = Lseg3D, PROCEDURE = lseg3D_same,
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);

CREATE OR REPLACE FUNCTION lseg3D_parallel(lseg1 Lseg3D, lseg2 Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'lseg3D_parallel' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?|| (
	LEFTARG = Lseg3D, RIGHTARG = Lseg3D, PROCEDURE = lseg3D_parallel,
	COMMUTATOR = '?||',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);
CREATE OR REPLACE FUNCTION lseg3D_coincide(lseg1 Lseg3D, lseg2 Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'lseg3D_coincide' LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION lseg3D_skew(lseg1 Lseg3D, lseg2 Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'lseg3D_skew' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?-/ (
	LEFTARG = Lseg3D, RIGHTARG = Lseg3D, PROCEDURE = lseg3D_skew,
	COMMUTATOR = '?-/',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION lseg3D_orthogonal(lseg1 Lseg3D, lseg2 Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'lseg3D_orthogonal' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?-| (
	LEFTARG = Lseg3D, RIGHTARG = Lseg3D, PROCEDURE = lseg3D_orthogonal,
	COMMUTATOR = '?-|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION lseg3D_horizontal(lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'lseg3D_horizontal' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?- (
	RIGHTARG = Lseg3D, PROCEDURE = lseg3D_horizontal,
	RESTRICT = positionsel3D
);

CREATE OR REPLACE FUNCTION lseg3D_vertical(lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'lseg3D_vertical' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?| (
	RIGHTARG = Lseg3D, PROCEDURE = lseg3D_vertical,
	RESTRICT = positionsel3D
);

CREATE OR REPLACE FUNCTION lseg3D_perpendicular(lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'lseg3D_perpendicular' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?/ (
	RIGHTARG = Lseg3D, PROCEDURE = lseg3D_perpendicular,
	RESTRICT = positionsel3D
);

CREATE OR REPLACE FUNCTION length(lseg Lseg3D) RETURNS float AS 'MODULE_PATHNAME', 'lseg3D_length' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION center(lseg Lseg3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'lseg3D_center' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR @@ (
	RIGHTARG = Lseg3D, PROCEDURE = center
);

------------------------------------------------------------------------------
--	Operator class for defining b-tree index
------------------------------------------------------------------------------

CREATE FUNCTION lseg3D_lt(lseg3D, lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'lseg3D_lt' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION lseg3D_le(lseg3D, lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'lseg3D_le' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION lseg3D_eq(lseg3D, lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'lseg3D_eq' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION lseg3D_ne(lseg3D, lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'lseg3D_ne' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION lseg3D_ge(lseg3D, lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'lseg3D_ge' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION lseg3D_gt(lseg3D, lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'lseg3D_gt' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION lseg3D_cmp(lseg3D, lseg3D) RETURNS int4 AS 'MODULE_PATHNAME', 'lseg3D_cmp' LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR < (
   leftarg = lseg3D, rightarg = lseg3D, procedure = lseg3D_lt,
   commutator = > , negator = >= ,
   restrict = scalarltsel, join = scalarltjoinsel
);
CREATE OPERATOR <= (
   leftarg = lseg3D, rightarg = lseg3D, procedure = lseg3D_le,
   commutator = >= , negator = > ,
   restrict = scalarltsel, join = scalarltjoinsel
);
CREATE OPERATOR = (
   leftarg = lseg3D, rightarg = lseg3D, procedure = lseg3D_eq,
   commutator = = ,
   negator = <> ,
   restrict = eqsel, join = eqjoinsel
);
CREATE OPERATOR <> (
   leftarg = lseg3D, rightarg = lseg3D, procedure = lseg3D_ne,
   commutator = <> ,
   negator = = ,
   restrict = neqsel, join = neqjoinsel
);
CREATE OPERATOR >= (
   leftarg = lseg3D, rightarg = lseg3D, procedure = lseg3D_ge,
   commutator = <= , negator = < ,
   restrict = scalargtsel, join = scalargtjoinsel
);
CREATE OPERATOR > (
   leftarg = lseg3D, rightarg = lseg3D, procedure = lseg3D_gt,
   commutator = < , negator = <= ,
   restrict = scalargtsel, join = scalargtjoinsel
);

CREATE OPERATOR CLASS lseg3D_ops
    DEFAULT FOR TYPE lseg3D USING btree AS
        OPERATOR        1       < ,
        OPERATOR        2       <= ,
        OPERATOR        3       = ,
        OPERATOR        4       >= ,
        OPERATOR        5       > ,
        FUNCTION        1       lseg3D_cmp(lseg3D, lseg3D);

/******************************************************************************
 * Line3D functions
 *****************************************************************************/

CREATE OR REPLACE FUNCTION line3D(x1 float, y1 float, z1 float, x2 float, y2 float, z2 float) RETURNS Line3D AS 'MODULE_PATHNAME', 'line3D_construct' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION line3D(point1 Point3D, point2 Point3D) RETURNS Line3D AS 'MODULE_PATHNAME', 'line3D_construct_pts' LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION line3D_points(line Line3D) RETURNS Point3D[] AS 'MODULE_PATHNAME', 'line3D_points' LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION line3D_same(line1 Line3D, line2 Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'line3D_same' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ~= (
	LEFTARG = Line3D, RIGHTARG = Line3D, PROCEDURE = line3D_same,
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);

CREATE OR REPLACE FUNCTION line3D_parallel(line1 Line3D, line2 Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'line3D_parallel' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?|| (
	LEFTARG = Line3D, RIGHTARG = Line3D, PROCEDURE = line3D_parallel,
	COMMUTATOR = '?||',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);
CREATE OR REPLACE FUNCTION line3D_coincide(line1 Line3D, line2 Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'line3D_coincide' LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION line3D_skew(line1 Line3D, line2 Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'line3D_skew' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?-/ (
	LEFTARG = Line3D, RIGHTARG = Line3D, PROCEDURE = line3D_skew,
	COMMUTATOR = '?-/',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION line3D_orthogonal(line1 Line3D, line2 Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'line3D_orthogonal' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?-| (
	LEFTARG = Line3D, RIGHTARG = Line3D, PROCEDURE = line3D_orthogonal,
	COMMUTATOR = '?-|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION line3D_horizontal(line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'line3D_horizontal' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?- (
	RIGHTARG = Line3D, PROCEDURE = line3D_horizontal,
	RESTRICT = positionsel3D
);

CREATE OR REPLACE FUNCTION line3D_vertical(line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'line3D_vertical' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?| (
	RIGHTARG = Line3D, PROCEDURE = line3D_vertical,
	RESTRICT = positionsel3D
);

CREATE OR REPLACE FUNCTION line3D_perpendicular(line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'line3D_perpendicular' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ?/ (
	RIGHTARG = Line3D, PROCEDURE = line3D_perpendicular,
	RESTRICT = positionsel3D
);

------------------------------------------------------------------------------
--	Operator class for defining b-tree index
------------------------------------------------------------------------------

CREATE FUNCTION line3D_lt(line3D, line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'line3D_lt' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION line3D_le(line3D, line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'line3D_le' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION line3D_eq(line3D, line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'line3D_eq' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION line3D_ne(line3D, line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'line3D_ne' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION line3D_ge(line3D, line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'line3D_ge' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION line3D_gt(line3D, line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'line3D_gt' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION line3D_cmp(line3D, line3D) RETURNS int4 AS 'MODULE_PATHNAME', 'line3D_cmp' LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR < (
   leftarg = line3D, rightarg = line3D, procedure = line3D_lt,
   commutator = > , negator = >= ,
   restrict = scalarltsel, join = scalarltjoinsel
);
CREATE OPERATOR <= (
   leftarg = line3D, rightarg = line3D, procedure = line3D_le,
   commutator = >= , negator = > ,
   restrict = scalarltsel, join = scalarltjoinsel
);
CREATE OPERATOR = (
   leftarg = line3D, rightarg = line3D, procedure = line3D_eq,
   commutator = = ,
   negator = <> ,
   restrict = eqsel, join = eqjoinsel
);
CREATE OPERATOR <> (
   leftarg = line3D, rightarg = line3D, procedure = line3D_ne,
   commutator = <> ,
   negator = = ,
   restrict = neqsel, join = neqjoinsel
);
CREATE OPERATOR >= (
   leftarg = line3D, rightarg = line3D, procedure = line3D_ge,
   commutator = <= , negator = < ,
   restrict = scalargtsel, join = scalargtjoinsel
);
CREATE OPERATOR > (
   leftarg = line3D, rightarg = line3D, procedure = line3D_gt,
   commutator = < , negator = <= ,
   restrict = scalargtsel, join = scalargtjoinsel
);

CREATE OPERATOR CLASS line3D_ops
    DEFAULT FOR TYPE line3D USING btree AS
        OPERATOR        1       < ,
        OPERATOR        2       <= ,
        OPERATOR        3       = ,
        OPERATOR        4       >= ,
        OPERATOR        5       > ,
        FUNCTION        1       line3D_cmp(line3D, line3D);

/******************************************************************************
 * Box3D functions
 *****************************************************************************/

CREATE OR REPLACE FUNCTION box3D(x1 float8, y1 float8, z1 float8, x2 float8, y2 float8, z2 float8) RETURNS Box3D AS 'MODULE_PATHNAME', 'box3D_construct' LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION box3D(point1 Point3D, point2 Point3D) RETURNS Box3D AS 'MODULE_PATHNAME', 'box3D_construct_pts' LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION box3D_high(box Box3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'box3D_high' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION box3D_low(box Box3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'box3D_low' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION box3D_points(box Box3D) RETURNS Point3D[] AS 'MODULE_PATHNAME', 'box3D_points' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION boundsphere(box Box3D) RETURNS Sphere AS 'MODULE_PATHNAME', 'box3D_boundsphere' LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION box3D_same(box1 Box3D, box2 Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'box3D_same' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ~= (
	LEFTARG = Box3D, RIGHTARG = Box3D, PROCEDURE = box3D_same,
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);

CREATE OR REPLACE FUNCTION volume(box Box3D) RETURNS float AS 'MODULE_PATHNAME', 'box3D_volume' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION width(box Box3D) RETURNS float AS 'MODULE_PATHNAME', 'box3D_width' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION height(box Box3D) RETURNS float AS 'MODULE_PATHNAME', 'box3D_height' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION depth(box Box3D) RETURNS float AS 'MODULE_PATHNAME', 'box3D_depth' LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION center(box Box3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'box3D_center' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR @@ (
	RIGHTARG = Box3D, PROCEDURE = center
);

CREATE OR REPLACE FUNCTION bound_box(box1 Box3D, box2 Box3D) RETURNS Box3D AS 'MODULE_PATHNAME', 'boxes3D_bound_box' LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION box3D_add(box Box3D, point Point3D) RETURNS Box3D AS 'MODULE_PATHNAME', 'box3D_add' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR + (
	PROCEDURE = box3D_add,
	LEFTARG = Box3D,
	RIGHTARG = Point3D,
	COMMUTATOR = +
);

CREATE OR REPLACE FUNCTION box3D_sub(box Box3D, point Point3D) RETURNS Box3D AS 'MODULE_PATHNAME', 'box3D_sub' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR - (
	PROCEDURE = box3D_sub,
	LEFTARG = Box3D,
	RIGHTARG = Point3D
);

CREATE OR REPLACE FUNCTION box3D_mul(box Box3D, f float) RETURNS Box3D AS 'MODULE_PATHNAME', 'box3D_mul' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR * (
	PROCEDURE = box3D_mul,
	LEFTARG = Box3D,
	RIGHTARG = float,
	COMMUTATOR = *
);

CREATE OR REPLACE FUNCTION box3D_div(box Box3D, f float) RETURNS Box3D AS 'MODULE_PATHNAME', 'box3D_div' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR / (
	PROCEDURE = box3D_div,
	LEFTARG = Box3D,
	RIGHTARG = float
);

------------------------------------------------------------------------------
--	Operator class for defining b-tree index
------------------------------------------------------------------------------

CREATE FUNCTION box3D_lt(box3D, box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'box3D_lt' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION box3D_le(box3D, box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'box3D_le' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION box3D_eq(box3D, box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'box3D_eq' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION box3D_ne(box3D, box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'box3D_ne' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION box3D_ge(box3D, box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'box3D_ge' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION box3D_gt(box3D, box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'box3D_gt' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION box3D_cmp(box3D, box3D) RETURNS int4 AS 'MODULE_PATHNAME', 'box3D_cmp' LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR < (
   leftarg = box3D, rightarg = box3D, procedure = box3D_lt,
   commutator = > , negator = >= ,
   restrict = scalarltsel, join = scalarltjoinsel
);
CREATE OPERATOR <= (
   leftarg = box3D, rightarg = box3D, procedure = box3D_le,
   commutator = >= , negator = > ,
   restrict = scalarltsel, join = scalarltjoinsel
);
CREATE OPERATOR = (
   leftarg = box3D, rightarg = box3D, procedure = box3D_eq,
   commutator = = ,
   negator = <> ,
   restrict = eqsel, join = eqjoinsel
);
CREATE OPERATOR <> (
   leftarg = box3D, rightarg = box3D, procedure = box3D_ne,
   commutator = <> ,
   negator = = ,
   restrict = neqsel, join = neqjoinsel
);
CREATE OPERATOR >= (
   leftarg = box3D, rightarg = box3D, procedure = box3D_ge,
   commutator = <= , negator = < ,
   restrict = scalargtsel, join = scalargtjoinsel
);
CREATE OPERATOR > (
   leftarg = box3D, rightarg = box3D, procedure = box3D_gt,
   commutator = < , negator = <= ,
   restrict = scalargtsel, join = scalargtjoinsel
);

CREATE OPERATOR CLASS box3D_ops
    DEFAULT FOR TYPE box3D USING btree AS
        OPERATOR        1       < ,
        OPERATOR        2       <= ,
        OPERATOR        3       = ,
        OPERATOR        4       >= ,
        OPERATOR        5       > ,
        FUNCTION        1       box3D_cmp(box3D, box3D);

/******************************************************************************
 * Path3D functions
 *****************************************************************************/

CREATE OR REPLACE FUNCTION path3D_same(path1 Path3D, path2 Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'path3D_same' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ~= (
	LEFTARG = Path3D, RIGHTARG = Path3D, PROCEDURE = path3D_same,
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);

CREATE OR REPLACE FUNCTION npoints(path Path3D) RETURNS int4 AS 'MODULE_PATHNAME', 'path3D_npoints' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	RIGHTARG = Path3D, PROCEDURE = npoints
);
CREATE OR REPLACE FUNCTION path3D_points(path Path3D) RETURNS Point3D[] AS 'MODULE_PATHNAME', 'path3D_points' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION length(path Path3D) RETURNS float AS 'MODULE_PATHNAME', 'path3D_length' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION isplanar(path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'path3D_isplanar' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION area(path Path3D) RETURNS float AS 'MODULE_PATHNAME', 'path3D_area' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION center(path Path3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'path3D_center' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR @@ (
	RIGHTARG = Path3D, PROCEDURE = center
);
CREATE OR REPLACE FUNCTION isclosed(path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'path3D_isclosed' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION isopen(path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'path3D_isopen' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION boundbox(path Path3D) RETURNS Box3D AS 'MODULE_PATHNAME', 'path3D_boundbox' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION boundsphere(path Path3D) RETURNS Sphere AS 'MODULE_PATHNAME', 'path3D_boundsphere' LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION close(path Path3D) RETURNS Path3D AS 'MODULE_PATHNAME', 'path3D_close' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION open(path Path3D) RETURNS Path3D AS 'MODULE_PATHNAME', 'path3D_open' LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION path3D_concat(path1 Path3D, path2 Path3D) RETURNS Path3D AS 'MODULE_PATHNAME', 'path3D_concat' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR + (
	PROCEDURE = path3D_concat,
	LEFTARG = Path3D,
	RIGHTARG = Path3D,
	COMMUTATOR = +
);

CREATE OR REPLACE FUNCTION path3D_add(path Path3D, point Point3D) RETURNS Path3D AS 'MODULE_PATHNAME', 'path3D_add' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR + (
	PROCEDURE = path3D_add,
	LEFTARG = Path3D,
	RIGHTARG = Point3D,
	COMMUTATOR = +
);

CREATE OR REPLACE FUNCTION path3D_sub(path Path3D, point Point3D) RETURNS Path3D AS 'MODULE_PATHNAME', 'path3D_sub' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR - (
	PROCEDURE = path3D_sub,
	LEFTARG = Path3D,
	RIGHTARG = Point3D
);

CREATE OR REPLACE FUNCTION path3D_mul(path Path3D, f float) RETURNS Path3D AS 'MODULE_PATHNAME', 'path3D_mul' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR * (
	PROCEDURE = path3D_mul,
	LEFTARG = Path3D,
	RIGHTARG = float,
	COMMUTATOR = *
);
CREATE OR REPLACE FUNCTION path3D_div(path Path3D, f float) RETURNS Path3D AS 'MODULE_PATHNAME', 'path3D_div' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR / (
	PROCEDURE = path3D_div,
	LEFTARG = Path3D,
	RIGHTARG = float
);

------------------------------------------------------------------------------
--	Operator class for defining b-tree index
------------------------------------------------------------------------------

CREATE OR REPLACE FUNCTION path3D_cmp(path1 Path3D, path2 Path3D) RETURNS int4 AS 'MODULE_PATHNAME', 'path3D_cmp' LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION path3D_lt(path1 Path3D, path2 Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'path3D_lt' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR < (
	LEFTARG = Path3D, RIGHTARG = Path3D, PROCEDURE = path3D_lt,
	COMMUTATOR = '>', NEGATOR = '>=',
	RESTRICT = scalarltsel, JOIN = scalarltjoinsel
);

CREATE OR REPLACE FUNCTION path3D_le(path1 Path3D, path2 Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'path3D_le' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <= (
	LEFTARG = Path3D, RIGHTARG = Path3D, PROCEDURE = path3D_le,
	COMMUTATOR = '>=', NEGATOR = '>',
	RESTRICT = scalarltsel, JOIN = scalarltjoinsel
);

CREATE OR REPLACE FUNCTION path3D_eq(path1 Path3D, path2 Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'path3D_eq' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR = (
	LEFTARG = Path3D, RIGHTARG = Path3D, PROCEDURE = path3D_eq,
	COMMUTATOR = '=', NEGATOR = '<>',
	RESTRICT = eqsel, JOIN = eqjoinsel,
	MERGES
);

CREATE OR REPLACE FUNCTION path3D_ne(path1 Path3D, path2 Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'path3D_ne' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <> (
	LEFTARG = Path3D, RIGHTARG = Path3D, PROCEDURE = path3D_ne,
	COMMUTATOR = '<>', NEGATOR = '=',
	RESTRICT = neqsel, JOIN = neqjoinsel
);

CREATE OR REPLACE FUNCTION path3D_gt(path1 Path3D, path2 Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'path3D_gt' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR > (
	LEFTARG = Path3D, RIGHTARG = Path3D, PROCEDURE = path3D_gt,
	COMMUTATOR = '<', NEGATOR = '<=',
	RESTRICT = scalargtsel, JOIN = scalargtjoinsel
);

CREATE OR REPLACE FUNCTION path3D_ge(path1 Path3D, path2 Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'path3D_ge' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >= (
	LEFTARG = Path3D, RIGHTARG = Path3D, PROCEDURE = path3D_ge,
	COMMUTATOR = '<=', NEGATOR = '<',
	RESTRICT = scalargtsel, JOIN = scalargtjoinsel
);

CREATE OPERATOR CLASS Path3D_ops
    DEFAULT FOR TYPE Path3D USING btree AS
        OPERATOR        1       < ,
        OPERATOR        2       <= ,
        OPERATOR        3       = ,
        OPERATOR        4       >= ,
        OPERATOR        5       > ,
        FUNCTION        1       path3D_cmp(Path3D, Path3D);
		
/******************************************************************************
 * Polygon3D functions
 *****************************************************************************/

CREATE OR REPLACE FUNCTION poly3D_same(p1 Polygon3D, p2 Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'poly3D_same' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ~= (
	LEFTARG = Polygon3D, RIGHTARG = Polygon3D, PROCEDURE = poly3D_same,
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);

CREATE OR REPLACE FUNCTION npoints(poly Polygon3D) RETURNS int4 AS 'MODULE_PATHNAME', 'poly3D_npoints' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR # (
	RIGHTARG = Polygon3D, PROCEDURE = npoints
);
CREATE OR REPLACE FUNCTION poly3D_points(poly Polygon3D) RETURNS Point3D[] AS 'MODULE_PATHNAME', 'poly3D_points' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION length(poly Polygon3D) RETURNS float AS 'MODULE_PATHNAME', 'poly3D_length' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION isplanar(poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'poly3D_isplanar' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION area(poly Polygon3D) RETURNS float AS 'MODULE_PATHNAME', 'poly3D_area' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION center(poly Polygon3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'poly3D_center' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR @@ (
	RIGHTARG = Polygon3D, PROCEDURE = center
);
CREATE OR REPLACE FUNCTION boundbox(poly Polygon3D) RETURNS Box3D AS 'MODULE_PATHNAME', 'poly3D_boundbox' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION boundsphere(poly Polygon3D) RETURNS Sphere AS 'MODULE_PATHNAME', 'poly3D_boundsphere' LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION poly3D_add(poly Polygon3D, point Point3D) RETURNS Polygon3D AS 'MODULE_PATHNAME', 'poly3D_add' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR + (
	PROCEDURE = poly3D_add,
	LEFTARG = Polygon3D,
	RIGHTARG = Point3D,
	COMMUTATOR = +
);

CREATE OR REPLACE FUNCTION poly3D_sub(poly Polygon3D, point Point3D) RETURNS Polygon3D AS 'MODULE_PATHNAME', 'poly3D_sub' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR - (
	PROCEDURE = poly3D_sub,
	LEFTARG = Polygon3D,
	RIGHTARG = Point3D
);

CREATE OR REPLACE FUNCTION poly3D_mul(poly Polygon3D, f float) RETURNS Polygon3D AS 'MODULE_PATHNAME', 'poly3D_mul' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR * (
	PROCEDURE = poly3D_mul,
	LEFTARG = Polygon3D,
	RIGHTARG = float,
	COMMUTATOR = *
);
CREATE OR REPLACE FUNCTION poly3D_div(poly Polygon3D, f float) RETURNS Polygon3D AS 'MODULE_PATHNAME', 'poly3D_div' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR / (
	PROCEDURE = poly3D_div,
	LEFTARG = Polygon3D,
	RIGHTARG = float
);

/*
CREATE OR REPLACE FUNCTION poly3D_project2D(poly Polygon3D) RETURNS Polygon3D AS 'MODULE_PATHNAME', 'poly3D_project2D' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION poly3D_project2D_point3D(poly Polygon3D, point Point3D) RETURNS Point3D AS 'MODULE_PATHNAME', 'poly3D_project2D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION poly3D_project2D_lseg3D(poly Polygon3D, lseg Lseg3D) RETURNS Lseg3D AS 'MODULE_PATHNAME', 'poly3D_project2D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
*/


------------------------------------------------------------------------------
--	Operator class for defining b-tree index
------------------------------------------------------------------------------

CREATE OR REPLACE FUNCTION poly3D_cmp(p1 Polygon3D, p2 Polygon3D) RETURNS int4 AS 'MODULE_PATHNAME', 'poly3D_cmp' LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION poly3D_lt(p1 Polygon3D, p2 Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'poly3D_lt' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR < (
	LEFTARG = Polygon3D, RIGHTARG = Polygon3D, PROCEDURE = poly3D_lt,
	COMMUTATOR = '>', NEGATOR = '>=',
	RESTRICT = scalarltsel, JOIN = scalarltjoinsel
);

CREATE OR REPLACE FUNCTION poly3D_le(p1 Polygon3D, p2 Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'poly3D_le' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <= (
	LEFTARG = Polygon3D, RIGHTARG = Polygon3D, PROCEDURE = poly3D_le,
	COMMUTATOR = '>=', NEGATOR = '>',
	RESTRICT = scalarltsel, JOIN = scalarltjoinsel
);

CREATE OR REPLACE FUNCTION poly3D_eq(p1 Polygon3D, p2 Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'poly3D_eq' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR = (
	LEFTARG = Polygon3D, RIGHTARG = Polygon3D, PROCEDURE = poly3D_eq,
	COMMUTATOR = '=', NEGATOR = '<>',
	RESTRICT = eqsel, JOIN = eqjoinsel,
	MERGES
);

CREATE OR REPLACE FUNCTION poly3D_ne(p1 Polygon3D, p2 Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'poly3D_ne' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <> (
	LEFTARG = Polygon3D, RIGHTARG = Polygon3D, PROCEDURE = poly3D_ne,
	COMMUTATOR = '<>', NEGATOR = '=',
	RESTRICT = neqsel, JOIN = neqjoinsel
);

CREATE OR REPLACE FUNCTION poly3D_gt(p1 Polygon3D, p2 Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'poly3D_gt' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR > (
	LEFTARG = Polygon3D, RIGHTARG = Polygon3D, PROCEDURE = poly3D_gt,
	COMMUTATOR = '<', NEGATOR = '<=',
	RESTRICT = scalargtsel, JOIN = scalargtjoinsel
);

CREATE OR REPLACE FUNCTION poly3D_ge(p1 Polygon3D, p2 Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'poly3D_ge' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >= (
	LEFTARG = Polygon3D, RIGHTARG = Polygon3D, PROCEDURE = poly3D_ge,
	COMMUTATOR = '<=', NEGATOR = '<',
	RESTRICT = scalargtsel, JOIN = scalargtjoinsel
);

CREATE OPERATOR CLASS Polygon3D_ops
    DEFAULT FOR TYPE Polygon3D USING btree AS
        OPERATOR        1       < ,
        OPERATOR        2       <= ,
        OPERATOR        3       = ,
        OPERATOR        4       >= ,
        OPERATOR        5       > ,
        FUNCTION        1       poly3D_cmp(Polygon3D, Polygon3D);

/******************************************************************************
 * Sphere functions
 *****************************************************************************/

CREATE OR REPLACE FUNCTION sphere(c Point3D, r float8) RETURNS Sphere AS 'MODULE_PATHNAME', 'sphere_construct' LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION center(sphere Sphere) RETURNS Point3D AS 'MODULE_PATHNAME', 'sphere_center' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR @@ (
	RIGHTARG = Sphere, PROCEDURE = center
);

CREATE OR REPLACE FUNCTION radius(sphere Sphere) RETURNS float AS 'MODULE_PATHNAME', 'sphere_radius' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION diameter(sphere Sphere) RETURNS float AS 'MODULE_PATHNAME', 'sphere_diameter' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION volume(sphere Sphere) RETURNS float AS 'MODULE_PATHNAME', 'sphere_volume' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION boundbox(sphere Sphere) RETURNS Box3D AS 'MODULE_PATHNAME', 'sphere_boundbox' LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION sphere_same(sphere1 Sphere, sphere2 Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'sphere_same' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR ~= (
	LEFTARG = Sphere, RIGHTARG = Sphere, PROCEDURE = sphere_same,
	RESTRICT = contsel3D, JOIN = contjoinsel3D
);

CREATE OR REPLACE FUNCTION sphere_add(sphere Sphere, point Point3D) RETURNS Sphere AS 'MODULE_PATHNAME', 'sphere_add' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR + (
	PROCEDURE = sphere_add,
	LEFTARG = Sphere,
	RIGHTARG = Point3D,
	COMMUTATOR = +
);

CREATE OR REPLACE FUNCTION sphere_sub(sphere Sphere, point Point3D) RETURNS Sphere AS 'MODULE_PATHNAME', 'sphere_sub' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR - (
	PROCEDURE = sphere_sub,
	LEFTARG = Sphere,
	RIGHTARG = Point3D
);

CREATE OR REPLACE FUNCTION sphere_mul(sphere Sphere, f float) RETURNS Sphere AS 'MODULE_PATHNAME', 'sphere_mul' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR * (
	PROCEDURE = sphere_mul,
	LEFTARG = Sphere,
	RIGHTARG = float,
	COMMUTATOR = *
);

CREATE OR REPLACE FUNCTION sphere_div(sphere Sphere, f float) RETURNS Sphere AS 'MODULE_PATHNAME', 'sphere_div' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR / (
	PROCEDURE = sphere_div,
	LEFTARG = Sphere,
	RIGHTARG = float
);

------------------------------------------------------------------------------
--	Operator class for defining b-tree index
------------------------------------------------------------------------------

CREATE FUNCTION sphere_lt(sphere, sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'sphere_lt' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION sphere_le(sphere, sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'sphere_le' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION sphere_eq(sphere, sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'sphere_eq' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION sphere_ne(sphere, sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'sphere_ne' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION sphere_ge(sphere, sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'sphere_ge' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION sphere_gt(sphere, sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'sphere_gt' LANGUAGE C IMMUTABLE STRICT;
CREATE FUNCTION sphere_cmp(sphere, sphere) RETURNS int4 AS 'MODULE_PATHNAME', 'sphere_cmp' LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR < (
   leftarg = sphere, rightarg = sphere, procedure = sphere_lt,
   commutator = > , negator = >= ,
   restrict = scalarltsel, join = scalarltjoinsel
);
CREATE OPERATOR <= (
   leftarg = sphere, rightarg = sphere, procedure = sphere_le,
   commutator = >= , negator = > ,
   restrict = scalarltsel, join = scalarltjoinsel
);
CREATE OPERATOR = (
   leftarg = sphere, rightarg = sphere, procedure = sphere_eq,
   commutator = = ,
   negator = <> ,
   restrict = eqsel, join = eqjoinsel
);
CREATE OPERATOR <> (
   leftarg = sphere, rightarg = sphere, procedure = sphere_ne,
   commutator = <> ,
   negator = = ,
   restrict = neqsel, join = neqjoinsel
);
CREATE OPERATOR >= (
   leftarg = sphere, rightarg = sphere, procedure = sphere_ge,
   commutator = <= , negator = < ,
   restrict = scalargtsel, join = scalargtjoinsel
);
CREATE OPERATOR > (
   leftarg = sphere, rightarg = sphere, procedure = sphere_gt,
   commutator = < , negator = <= ,
   restrict = scalargtsel, join = scalargtjoinsel
);

CREATE OPERATOR CLASS sphere_ops
    DEFAULT FOR TYPE sphere USING btree AS
        OPERATOR        1       < ,
        OPERATOR        2       <= ,
        OPERATOR        3       = ,
        OPERATOR        4       >= ,
        OPERATOR        5       > ,
        FUNCTION        1       sphere_cmp(sphere, sphere);

/******************************************************************************
 * Conversion functions for a type into another type
 *****************************************************************************/

CREATE OR REPLACE FUNCTION line3D(lseg Lseg3D) RETURNS Line3D AS 'MODULE_PATHNAME', 'lseg3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION polygon3D(path Path3D) RETURNS Polygon3D AS 'MODULE_PATHNAME', 'path3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION path3D(poly Polygon3D) RETURNS Paht3D AS 'MODULE_PATHNAME', 'poly3D_path3D' LANGUAGE C IMMUTABLE STRICT;

/*****************************************************************************
 * Coplanar routines
 *****************************************************************************/

CREATE OR REPLACE FUNCTION coplanar(lseg1 Lseg3D, lseg2 Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'coplanar_lseg3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION coplanar(lseg Lseg3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'coplanar_lseg3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION coplanar(lseg Lseg3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'coplanar_lseg3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION coplanar(lseg Lseg3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'coplanar_lseg3D_poly3D' LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION coplanar(line Line3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'coplanar_line3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION coplanar(line1 Line3D, line2 Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'coplanar_line3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION coplanar(line Line3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'coplanar_line3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION coplanar(line Line3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'coplanar_line3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
 
CREATE OR REPLACE FUNCTION coplanar(path Path3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'coplanar_path3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION coplanar(path Path3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'coplanar_path3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION coplanar(path Path3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'coplanar_path3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION coplanar(path1 Path3D, path2 Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'coplanar_path3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION coplanar(path Path3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'coplanar_path3D_poly3D' LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION coplanar(poly Polygon3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'coplanar_poly3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION coplanar(poly Polygon3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'coplanar_poly3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION coplanar(poly Polygon3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'coplanar_poly3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION coplanar(poly Polygon3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'coplanar_poly3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION coplanar(poly1 Polygon3D, poly2 Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'coplanar_poly3D_poly3D' LANGUAGE C IMMUTABLE STRICT;

/******************************************************************************/

