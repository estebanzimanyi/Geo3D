/*-------------------------------------------------------------------------
 *
 * geo3D_relpos_ops.sql
 *	  Relative position operations for 3D geometries
 *
 * Portions Copyright (c) 2017, Esteban Zimanyi, Université Libre de Bruxelles
 * Portions Copyright (c) 1996-2017, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *-------------------------------------------------------------------------*/

/* point3D op point3D */

CREATE OR REPLACE FUNCTION left_point3D_point3D(point1 Point3D, point2 Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_point3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Point3D, RIGHTARG = Point3D, PROCEDURE = left_point3D_point3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_point3D_point3D(point1 Point3D, point2 Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_point3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Point3D, RIGHTARG = Point3D, PROCEDURE = overleft_point3D_point3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_point3D_point3D(point1 Point3D, point2 Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_point3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Point3D, RIGHTARG = Point3D, PROCEDURE = right_point3D_point3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_point3D_point3D(point1 Point3D, point2 Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_point3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Point3D, RIGHTARG = Point3D, PROCEDURE = overright_point3D_point3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_point3D_point3D(point1 Point3D, point2 Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_point3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Point3D, RIGHTARG = Point3D, PROCEDURE = below_point3D_point3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_point3D_point3D(point1 Point3D, point2 Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_point3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Point3D, RIGHTARG = Point3D, PROCEDURE = overbelow_point3D_point3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_point3D_point3D(point1 Point3D, point2 Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_point3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Point3D, RIGHTARG = Point3D, PROCEDURE = above_point3D_point3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_point3D_point3D(point1 Point3D, point2 Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_point3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Point3D, RIGHTARG = Point3D, PROCEDURE = overabove_point3D_point3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_point3D_point3D(point1 Point3D, point2 Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_point3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Point3D, RIGHTARG = Point3D, PROCEDURE = front_point3D_point3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_point3D_point3D(point1 Point3D, point2 Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_point3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Point3D, RIGHTARG = Point3D, PROCEDURE = overfront_point3D_point3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_point3D_point3D(point1 Point3D, point2 Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_point3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Point3D, RIGHTARG = Point3D, PROCEDURE = back_point3D_point3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_point3D_point3D(point1 Point3D, point2 Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_point3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Point3D, RIGHTARG = Point3D, PROCEDURE = overback_point3D_point3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* point3D op lseg3D */

CREATE OR REPLACE FUNCTION left_point3D_lseg3D(point Point3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_point3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Point3D, RIGHTARG = Lseg3D, PROCEDURE = left_point3D_lseg3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_point3D_lseg3D(point Point3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_point3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Point3D, RIGHTARG = Lseg3D, PROCEDURE = overleft_point3D_lseg3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_point3D_lseg3D(point Point3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_point3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Point3D, RIGHTARG = Lseg3D, PROCEDURE = right_point3D_lseg3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_point3D_lseg3D(point Point3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_point3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Point3D, RIGHTARG = Lseg3D, PROCEDURE = overright_point3D_lseg3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_point3D_lseg3D(point Point3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_point3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Point3D, RIGHTARG = Lseg3D, PROCEDURE = below_point3D_lseg3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_point3D_lseg3D(point Point3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_point3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Point3D, RIGHTARG = Lseg3D, PROCEDURE = overbelow_point3D_lseg3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_point3D_lseg3D(point Point3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_point3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Point3D, RIGHTARG = Lseg3D, PROCEDURE = above_point3D_lseg3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_point3D_lseg3D(point Point3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_point3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Point3D, RIGHTARG = Lseg3D, PROCEDURE = overabove_point3D_lseg3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_point3D_lseg3D(point Point3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_point3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Point3D, RIGHTARG = Lseg3D, PROCEDURE = front_point3D_lseg3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_point3D_lseg3D(point Point3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_point3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Point3D, RIGHTARG = Lseg3D, PROCEDURE = overfront_point3D_lseg3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_point3D_lseg3D(point Point3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_point3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Point3D, RIGHTARG = Lseg3D, PROCEDURE = back_point3D_lseg3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_point3D_lseg3D(point Point3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_point3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Point3D, RIGHTARG = Lseg3D, PROCEDURE = overback_point3D_lseg3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* point3D op line3D */

CREATE OR REPLACE FUNCTION left_point3D_line3D(point Point3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_point3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Point3D, RIGHTARG = Line3D, PROCEDURE = left_point3D_line3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_point3D_line3D(point Point3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_point3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Point3D, RIGHTARG = Line3D, PROCEDURE = overleft_point3D_line3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_point3D_line3D(point Point3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_point3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Point3D, RIGHTARG = Line3D, PROCEDURE = right_point3D_line3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_point3D_line3D(point Point3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_point3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Point3D, RIGHTARG = Line3D, PROCEDURE = overright_point3D_line3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_point3D_line3D(point Point3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_point3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Point3D, RIGHTARG = Line3D, PROCEDURE = below_point3D_line3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_point3D_line3D(point Point3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_point3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Point3D, RIGHTARG = Line3D, PROCEDURE = overbelow_point3D_line3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_point3D_line3D(point Point3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_point3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Point3D, RIGHTARG = Line3D, PROCEDURE = above_point3D_line3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_point3D_line3D(point Point3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_point3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Point3D, RIGHTARG = Line3D, PROCEDURE = overabove_point3D_line3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_point3D_line3D(point Point3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_point3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Point3D, RIGHTARG = Line3D, PROCEDURE = front_point3D_line3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_point3D_line3D(point Point3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_point3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Point3D, RIGHTARG = Line3D, PROCEDURE = overfront_point3D_line3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_point3D_line3D(point Point3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_point3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Point3D, RIGHTARG = Line3D, PROCEDURE = back_point3D_line3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_point3D_line3D(point Point3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_point3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Point3D, RIGHTARG = Line3D, PROCEDURE = overback_point3D_line3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* point3D op box3D */

CREATE OR REPLACE FUNCTION left_point3D_box3D(point Point3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_point3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Point3D, RIGHTARG = Box3D, PROCEDURE = left_point3D_box3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_point3D_box3D(point Point3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_point3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Point3D, RIGHTARG = Box3D, PROCEDURE = overleft_point3D_box3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_point3D_box3D(point Point3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_point3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Point3D, RIGHTARG = Box3D, PROCEDURE = right_point3D_box3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_point3D_box3D(point Point3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_point3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Point3D, RIGHTARG = Box3D, PROCEDURE = overright_point3D_box3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_point3D_box3D(point Point3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_point3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Point3D, RIGHTARG = Box3D, PROCEDURE = below_point3D_box3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_point3D_box3D(point Point3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_point3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Point3D, RIGHTARG = Box3D, PROCEDURE = overbelow_point3D_box3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_point3D_box3D(point Point3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_point3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Point3D, RIGHTARG = Box3D, PROCEDURE = above_point3D_box3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_point3D_box3D(point Point3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_point3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Point3D, RIGHTARG = Box3D, PROCEDURE = overabove_point3D_box3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_point3D_box3D(point Point3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_point3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Point3D, RIGHTARG = Box3D, PROCEDURE = front_point3D_box3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_point3D_box3D(point Point3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_point3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Point3D, RIGHTARG = Box3D, PROCEDURE = overfront_point3D_box3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_point3D_box3D(point Point3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_point3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Point3D, RIGHTARG = Box3D, PROCEDURE = back_point3D_box3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_point3D_box3D(point Point3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_point3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Point3D, RIGHTARG = Box3D, PROCEDURE = overback_point3D_box3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* point3D op path3D */

CREATE OR REPLACE FUNCTION left_point3D_path3D(point Point3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_point3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Point3D, RIGHTARG = Path3D, PROCEDURE = left_point3D_path3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_point3D_path3D(point Point3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_point3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Point3D, RIGHTARG = Path3D, PROCEDURE = overleft_point3D_path3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_point3D_path3D(point Point3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_point3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Point3D, RIGHTARG = Path3D, PROCEDURE = right_point3D_path3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_point3D_path3D(point Point3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_point3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Point3D, RIGHTARG = Path3D, PROCEDURE = overright_point3D_path3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_point3D_path3D(point Point3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_point3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Point3D, RIGHTARG = Path3D, PROCEDURE = below_point3D_path3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_point3D_path3D(point Point3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_point3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Point3D, RIGHTARG = Path3D, PROCEDURE = overbelow_point3D_path3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_point3D_path3D(point Point3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_point3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Point3D, RIGHTARG = Path3D, PROCEDURE = above_point3D_path3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_point3D_path3D(point Point3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_point3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Point3D, RIGHTARG = Path3D, PROCEDURE = overabove_point3D_path3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_point3D_path3D(point Point3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_point3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Point3D, RIGHTARG = Path3D, PROCEDURE = front_point3D_path3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_point3D_path3D(point Point3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_point3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Point3D, RIGHTARG = Path3D, PROCEDURE = overfront_point3D_path3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_point3D_path3D(point Point3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_point3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Point3D, RIGHTARG = Path3D, PROCEDURE = back_point3D_path3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_point3D_path3D(point Point3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_point3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Point3D, RIGHTARG = Path3D, PROCEDURE = overback_point3D_path3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* point3D op poly3D */

CREATE OR REPLACE FUNCTION left_point3D_poly3D(point Point3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_point3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Point3D, RIGHTARG = Polygon3D, PROCEDURE = left_point3D_poly3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_point3D_poly3D(point Point3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_point3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Point3D, RIGHTARG = Polygon3D, PROCEDURE = overleft_point3D_poly3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_point3D_poly3D(point Point3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_point3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Point3D, RIGHTARG = Polygon3D, PROCEDURE = right_point3D_poly3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_point3D_poly3D(point Point3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_point3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Point3D, RIGHTARG = Polygon3D, PROCEDURE = overright_point3D_poly3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_point3D_poly3D(point Point3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_point3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Point3D, RIGHTARG = Polygon3D, PROCEDURE = below_point3D_poly3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_point3D_poly3D(point Point3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_point3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Point3D, RIGHTARG = Polygon3D, PROCEDURE = overbelow_point3D_poly3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_point3D_poly3D(point Point3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_point3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Point3D, RIGHTARG = Polygon3D, PROCEDURE = above_point3D_poly3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_point3D_poly3D(point Point3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_point3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Point3D, RIGHTARG = Polygon3D, PROCEDURE = overabove_point3D_poly3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_point3D_poly3D(point Point3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_point3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Point3D, RIGHTARG = Polygon3D, PROCEDURE = front_point3D_poly3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_point3D_poly3D(point Point3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_point3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Point3D, RIGHTARG = Polygon3D, PROCEDURE = overfront_point3D_poly3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_point3D_poly3D(point Point3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_point3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Point3D, RIGHTARG = Polygon3D, PROCEDURE = back_point3D_poly3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_point3D_poly3D(point Point3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_point3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Point3D, RIGHTARG = Polygon3D, PROCEDURE = overback_point3D_poly3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* point3D op sphere */

CREATE OR REPLACE FUNCTION left_point3D_sphere(point Point3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'left_point3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Point3D, RIGHTARG = Sphere, PROCEDURE = left_point3D_sphere,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_point3D_sphere(point Point3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_point3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Point3D, RIGHTARG = Sphere, PROCEDURE = overleft_point3D_sphere,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_point3D_sphere(point Point3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'right_point3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Point3D, RIGHTARG = Sphere, PROCEDURE = right_point3D_sphere,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_point3D_sphere(point Point3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_point3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Point3D, RIGHTARG = Sphere, PROCEDURE = overright_point3D_sphere,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_point3D_sphere(point Point3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'below_point3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Point3D, RIGHTARG = Sphere, PROCEDURE = below_point3D_sphere,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_point3D_sphere(point Point3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_point3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Point3D, RIGHTARG = Sphere, PROCEDURE = overbelow_point3D_sphere,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_point3D_sphere(point Point3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'above_point3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Point3D, RIGHTARG = Sphere, PROCEDURE = above_point3D_sphere,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_point3D_sphere(point Point3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_point3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Point3D, RIGHTARG = Sphere, PROCEDURE = overabove_point3D_sphere,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_point3D_sphere(point Point3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'front_point3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Point3D, RIGHTARG = Sphere, PROCEDURE = front_point3D_sphere,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_point3D_sphere(point Point3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_point3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Point3D, RIGHTARG = Sphere, PROCEDURE = overfront_point3D_sphere,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_point3D_sphere(point Point3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'back_point3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Point3D, RIGHTARG = Sphere, PROCEDURE = back_point3D_sphere,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_point3D_sphere(point Point3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_point3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Point3D, RIGHTARG = Sphere, PROCEDURE = overback_point3D_sphere,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/*-------------------------------------------------------------------------*/

/* lseg3D op point3D */

CREATE OR REPLACE FUNCTION left_lseg3D_point3D(lseg Lseg3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_lseg3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Lseg3D, RIGHTARG = Point3D, PROCEDURE = left_lseg3D_point3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_lseg3D_point3D(lseg Lseg3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_lseg3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Lseg3D, RIGHTARG = Point3D, PROCEDURE = overleft_lseg3D_point3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_lseg3D_point3D(lseg Lseg3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_lseg3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Lseg3D, RIGHTARG = Point3D, PROCEDURE = right_lseg3D_point3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_lseg3D_point3D(lseg Lseg3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_lseg3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Lseg3D, RIGHTARG = Point3D, PROCEDURE = overright_lseg3D_point3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_lseg3D_point3D(lseg Lseg3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_lseg3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Lseg3D, RIGHTARG = Point3D, PROCEDURE = below_lseg3D_point3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_lseg3D_point3D(lseg Lseg3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_lseg3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Lseg3D, RIGHTARG = Point3D, PROCEDURE = overbelow_lseg3D_point3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_lseg3D_point3D(lseg Lseg3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_lseg3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Lseg3D, RIGHTARG = Point3D, PROCEDURE = above_lseg3D_point3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_lseg3D_point3D(lseg Lseg3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_lseg3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Lseg3D, RIGHTARG = Point3D, PROCEDURE = overabove_lseg3D_point3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_lseg3D_point3D(lseg Lseg3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_lseg3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Lseg3D, RIGHTARG = Point3D, PROCEDURE = front_lseg3D_point3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_lseg3D_point3D(lseg Lseg3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_lseg3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Lseg3D, RIGHTARG = Point3D, PROCEDURE = overfront_lseg3D_point3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_lseg3D_point3D(lseg Lseg3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_lseg3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Lseg3D, RIGHTARG = Point3D, PROCEDURE = back_lseg3D_point3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_lseg3D_point3D(lseg Lseg3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_lseg3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Lseg3D, RIGHTARG = Point3D, PROCEDURE = overback_lseg3D_point3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* lseg3D op lseg3D */

CREATE OR REPLACE FUNCTION left_lseg3D_lseg3D(lseg1 Lseg3D, lseg2 Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_lseg3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Lseg3D, RIGHTARG = Lseg3D, PROCEDURE = left_lseg3D_lseg3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_lseg3D_lseg3D(lseg1 Lseg3D, lseg2 Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_lseg3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Lseg3D, RIGHTARG = Lseg3D, PROCEDURE = overleft_lseg3D_lseg3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_lseg3D_lseg3D(lseg1 Lseg3D, lseg2 Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_lseg3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Lseg3D, RIGHTARG = Lseg3D, PROCEDURE = right_lseg3D_lseg3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_lseg3D_lseg3D(lseg1 Lseg3D, lseg2 Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_lseg3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Lseg3D, RIGHTARG = Lseg3D, PROCEDURE = overright_lseg3D_lseg3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_lseg3D_lseg3D(lseg1 Lseg3D, lseg2 Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_lseg3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Lseg3D, RIGHTARG = Lseg3D, PROCEDURE = below_lseg3D_lseg3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_lseg3D_lseg3D(lseg1 Lseg3D, lseg2 Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_lseg3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Lseg3D, RIGHTARG = Lseg3D, PROCEDURE = overbelow_lseg3D_lseg3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_lseg3D_lseg3D(lseg1 Lseg3D, lseg2 Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_lseg3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Lseg3D, RIGHTARG = Lseg3D, PROCEDURE = above_lseg3D_lseg3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_lseg3D_lseg3D(lseg1 Lseg3D, lseg2 Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_lseg3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Lseg3D, RIGHTARG = Lseg3D, PROCEDURE = overabove_lseg3D_lseg3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_lseg3D_lseg3D(lseg1 Lseg3D, lseg2 Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_lseg3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Lseg3D, RIGHTARG = Lseg3D, PROCEDURE = front_lseg3D_lseg3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_lseg3D_lseg3D(lseg1 Lseg3D, lseg2 Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_lseg3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Lseg3D, RIGHTARG = Lseg3D, PROCEDURE = overfront_lseg3D_lseg3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_lseg3D_lseg3D(lseg1 Lseg3D, lseg2 Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_lseg3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Lseg3D, RIGHTARG = Lseg3D, PROCEDURE = back_lseg3D_lseg3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_lseg3D_lseg3D(lseg1 Lseg3D, lseg2 Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_lseg3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Lseg3D, RIGHTARG = Lseg3D, PROCEDURE = overback_lseg3D_lseg3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* lseg3D op line3D */

CREATE OR REPLACE FUNCTION left_lseg3D_line3D(lseg Lseg3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_lseg3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Lseg3D, RIGHTARG = Line3D, PROCEDURE = left_lseg3D_line3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_lseg3D_line3D(lseg Lseg3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_lseg3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Lseg3D, RIGHTARG = Line3D, PROCEDURE = overleft_lseg3D_line3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_lseg3D_line3D(lseg Lseg3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_lseg3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Lseg3D, RIGHTARG = Line3D, PROCEDURE = right_lseg3D_line3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_lseg3D_line3D(lseg Lseg3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_lseg3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Lseg3D, RIGHTARG = Line3D, PROCEDURE = overright_lseg3D_line3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_lseg3D_line3D(lseg Lseg3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_lseg3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Lseg3D, RIGHTARG = Line3D, PROCEDURE = below_lseg3D_line3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_lseg3D_line3D(lseg Lseg3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_lseg3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Lseg3D, RIGHTARG = Line3D, PROCEDURE = overbelow_lseg3D_line3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_lseg3D_line3D(lseg Lseg3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_lseg3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Lseg3D, RIGHTARG = Line3D, PROCEDURE = above_lseg3D_line3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_lseg3D_line3D(lseg Lseg3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_lseg3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Lseg3D, RIGHTARG = Line3D, PROCEDURE = overabove_lseg3D_line3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_lseg3D_line3D(lseg Lseg3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_lseg3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Lseg3D, RIGHTARG = Line3D, PROCEDURE = front_lseg3D_line3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_lseg3D_line3D(lseg Lseg3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_lseg3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Lseg3D, RIGHTARG = Line3D, PROCEDURE = overfront_lseg3D_line3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_lseg3D_line3D(lseg Lseg3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_lseg3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Lseg3D, RIGHTARG = Line3D, PROCEDURE = back_lseg3D_line3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_lseg3D_line3D(lseg Lseg3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_lseg3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Lseg3D, RIGHTARG = Line3D, PROCEDURE = overback_lseg3D_line3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* lseg3D op box3D */

CREATE OR REPLACE FUNCTION left_lseg3D_box3D(lseg Lseg3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_lseg3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Lseg3D, RIGHTARG = Box3D, PROCEDURE = left_lseg3D_box3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_lseg3D_box3D(lseg Lseg3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_lseg3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Lseg3D, RIGHTARG = Box3D, PROCEDURE = overleft_lseg3D_box3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_lseg3D_box3D(lseg Lseg3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_lseg3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Lseg3D, RIGHTARG = Box3D, PROCEDURE = right_lseg3D_box3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_lseg3D_box3D(lseg Lseg3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_lseg3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Lseg3D, RIGHTARG = Box3D, PROCEDURE = overright_lseg3D_box3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_lseg3D_box3D(lseg Lseg3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_lseg3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Lseg3D, RIGHTARG = Box3D, PROCEDURE = below_lseg3D_box3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_lseg3D_box3D(lseg Lseg3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_lseg3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Lseg3D, RIGHTARG = Box3D, PROCEDURE = overbelow_lseg3D_box3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_lseg3D_box3D(lseg Lseg3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_lseg3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Lseg3D, RIGHTARG = Box3D, PROCEDURE = above_lseg3D_box3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_lseg3D_box3D(lseg Lseg3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_lseg3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Lseg3D, RIGHTARG = Box3D, PROCEDURE = overabove_lseg3D_box3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_lseg3D_box3D(lseg Lseg3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_lseg3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Lseg3D, RIGHTARG = Box3D, PROCEDURE = front_lseg3D_box3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_lseg3D_box3D(lseg Lseg3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_lseg3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Lseg3D, RIGHTARG = Box3D, PROCEDURE = overfront_lseg3D_box3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_lseg3D_box3D(lseg Lseg3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_lseg3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Lseg3D, RIGHTARG = Box3D, PROCEDURE = back_lseg3D_box3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_lseg3D_box3D(lseg Lseg3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_lseg3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Lseg3D, RIGHTARG = Box3D, PROCEDURE = overback_lseg3D_box3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* lseg3D op path3D */

CREATE OR REPLACE FUNCTION left_lseg3D_path3D(lseg Lseg3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_lseg3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Lseg3D, RIGHTARG = Path3D, PROCEDURE = left_lseg3D_path3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_lseg3D_path3D(lseg Lseg3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_lseg3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Lseg3D, RIGHTARG = Path3D, PROCEDURE = overleft_lseg3D_path3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_lseg3D_path3D(lseg Lseg3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_lseg3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Lseg3D, RIGHTARG = Path3D, PROCEDURE = right_lseg3D_path3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_lseg3D_path3D(lseg Lseg3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_lseg3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Lseg3D, RIGHTARG = Path3D, PROCEDURE = overright_lseg3D_path3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_lseg3D_path3D(lseg Lseg3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_lseg3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Lseg3D, RIGHTARG = Path3D, PROCEDURE = below_lseg3D_path3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_lseg3D_path3D(lseg Lseg3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_lseg3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Lseg3D, RIGHTARG = Path3D, PROCEDURE = overbelow_lseg3D_path3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_lseg3D_path3D(lseg Lseg3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_lseg3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Lseg3D, RIGHTARG = Path3D, PROCEDURE = above_lseg3D_path3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_lseg3D_path3D(lseg Lseg3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_lseg3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Lseg3D, RIGHTARG = Path3D, PROCEDURE = overabove_lseg3D_path3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_lseg3D_path3D(lseg Lseg3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_lseg3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Lseg3D, RIGHTARG = Path3D, PROCEDURE = front_lseg3D_path3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_lseg3D_path3D(lseg Lseg3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_lseg3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Lseg3D, RIGHTARG = Path3D, PROCEDURE = overfront_lseg3D_path3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_lseg3D_path3D(lseg Lseg3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_lseg3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Lseg3D, RIGHTARG = Path3D, PROCEDURE = back_lseg3D_path3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_lseg3D_path3D(lseg Lseg3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_lseg3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Lseg3D, RIGHTARG = Path3D, PROCEDURE = overback_lseg3D_path3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* lseg3D op poly3D */

CREATE OR REPLACE FUNCTION left_lseg3D_poly3D(lseg Lseg3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_lseg3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Lseg3D, RIGHTARG = Polygon3D, PROCEDURE = left_lseg3D_poly3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_lseg3D_poly3D(lseg Lseg3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_lseg3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Lseg3D, RIGHTARG = Polygon3D, PROCEDURE = overleft_lseg3D_poly3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_lseg3D_poly3D(lseg Lseg3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_lseg3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Lseg3D, RIGHTARG = Polygon3D, PROCEDURE = right_lseg3D_poly3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_lseg3D_poly3D(lseg Lseg3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_lseg3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Lseg3D, RIGHTARG = Polygon3D, PROCEDURE = overright_lseg3D_poly3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_lseg3D_poly3D(lseg Lseg3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_lseg3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Lseg3D, RIGHTARG = Polygon3D, PROCEDURE = below_lseg3D_poly3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_lseg3D_poly3D(lseg Lseg3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_lseg3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Lseg3D, RIGHTARG = Polygon3D, PROCEDURE = overbelow_lseg3D_poly3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_lseg3D_poly3D(lseg Lseg3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_lseg3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Lseg3D, RIGHTARG = Polygon3D, PROCEDURE = above_lseg3D_poly3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_lseg3D_poly3D(lseg Lseg3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_lseg3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Lseg3D, RIGHTARG = Polygon3D, PROCEDURE = overabove_lseg3D_poly3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_lseg3D_poly3D(lseg Lseg3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_lseg3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Lseg3D, RIGHTARG = Polygon3D, PROCEDURE = front_lseg3D_poly3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_lseg3D_poly3D(lseg Lseg3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_lseg3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Lseg3D, RIGHTARG = Polygon3D, PROCEDURE = overfront_lseg3D_poly3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_lseg3D_poly3D(lseg Lseg3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_lseg3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Lseg3D, RIGHTARG = Polygon3D, PROCEDURE = back_lseg3D_poly3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_lseg3D_poly3D(lseg Lseg3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_lseg3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Lseg3D, RIGHTARG = Polygon3D, PROCEDURE = overback_lseg3D_poly3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* lseg3D op sphere */

CREATE OR REPLACE FUNCTION left_lseg3D_sphere(lseg Lseg3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'left_lseg3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Lseg3D, RIGHTARG = Sphere, PROCEDURE = left_lseg3D_sphere,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_lseg3D_sphere(lseg Lseg3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_lseg3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Lseg3D, RIGHTARG = Sphere, PROCEDURE = overleft_lseg3D_sphere,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_lseg3D_sphere(lseg Lseg3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'right_lseg3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Lseg3D, RIGHTARG = Sphere, PROCEDURE = right_lseg3D_sphere,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_lseg3D_sphere(lseg Lseg3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_lseg3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Lseg3D, RIGHTARG = Sphere, PROCEDURE = overright_lseg3D_sphere,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_lseg3D_sphere(lseg Lseg3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'below_lseg3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Lseg3D, RIGHTARG = Sphere, PROCEDURE = below_lseg3D_sphere,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_lseg3D_sphere(lseg Lseg3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_lseg3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Lseg3D, RIGHTARG = Sphere, PROCEDURE = overbelow_lseg3D_sphere,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_lseg3D_sphere(lseg Lseg3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'above_lseg3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Lseg3D, RIGHTARG = Sphere, PROCEDURE = above_lseg3D_sphere,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_lseg3D_sphere(lseg Lseg3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_lseg3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Lseg3D, RIGHTARG = Sphere, PROCEDURE = overabove_lseg3D_sphere,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_lseg3D_sphere(lseg Lseg3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'front_lseg3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Lseg3D, RIGHTARG = Sphere, PROCEDURE = front_lseg3D_sphere,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_lseg3D_sphere(lseg Lseg3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_lseg3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Lseg3D, RIGHTARG = Sphere, PROCEDURE = overfront_lseg3D_sphere,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_lseg3D_sphere(lseg Lseg3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'back_lseg3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Lseg3D, RIGHTARG = Sphere, PROCEDURE = back_lseg3D_sphere,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_lseg3D_sphere(lseg Lseg3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_lseg3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Lseg3D, RIGHTARG = Sphere, PROCEDURE = overback_lseg3D_sphere,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/*-------------------------------------------------------------------------*/

/* line3D op point3D */

CREATE OR REPLACE FUNCTION left_line3D_point3D(line Line3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_line3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Line3D, RIGHTARG = Point3D, PROCEDURE = left_line3D_point3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_line3D_point3D(line Line3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_line3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Line3D, RIGHTARG = Point3D, PROCEDURE = overleft_line3D_point3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_line3D_point3D(line Line3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_line3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Line3D, RIGHTARG = Point3D, PROCEDURE = right_line3D_point3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_line3D_point3D(line Line3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_line3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Line3D, RIGHTARG = Point3D, PROCEDURE = overright_line3D_point3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_line3D_point3D(line Line3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_line3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Line3D, RIGHTARG = Point3D, PROCEDURE = below_line3D_point3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_line3D_point3D(line Line3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_line3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Line3D, RIGHTARG = Point3D, PROCEDURE = overbelow_line3D_point3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_line3D_point3D(line Line3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_line3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Line3D, RIGHTARG = Point3D, PROCEDURE = above_line3D_point3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_line3D_point3D(line Line3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_line3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Line3D, RIGHTARG = Point3D, PROCEDURE = overabove_line3D_point3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_line3D_point3D(line Line3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_line3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Line3D, RIGHTARG = Point3D, PROCEDURE = front_line3D_point3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_line3D_point3D(line Line3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_line3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Line3D, RIGHTARG = Point3D, PROCEDURE = overfront_line3D_point3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_line3D_point3D(line Line3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_line3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Line3D, RIGHTARG = Point3D, PROCEDURE = back_line3D_point3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_line3D_point3D(line Line3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_line3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Line3D, RIGHTARG = Point3D, PROCEDURE = overback_line3D_point3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* line3D op lseg3D */

CREATE OR REPLACE FUNCTION left_line3D_lseg3D(line Line3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_line3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Line3D, RIGHTARG = Lseg3D, PROCEDURE = left_line3D_lseg3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_line3D_lseg3D(line Line3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_line3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Line3D, RIGHTARG = Lseg3D, PROCEDURE = overleft_line3D_lseg3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_line3D_lseg3D(line Line3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_line3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Line3D, RIGHTARG = Lseg3D, PROCEDURE = right_line3D_lseg3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_line3D_lseg3D(line Line3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_line3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Line3D, RIGHTARG = Lseg3D, PROCEDURE = overright_line3D_lseg3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_line3D_lseg3D(line Line3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_line3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Line3D, RIGHTARG = Lseg3D, PROCEDURE = below_line3D_lseg3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_line3D_lseg3D(line Line3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_line3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Line3D, RIGHTARG = Lseg3D, PROCEDURE = overbelow_line3D_lseg3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_line3D_lseg3D(line Line3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_line3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Line3D, RIGHTARG = Lseg3D, PROCEDURE = above_line3D_lseg3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_line3D_lseg3D(line Line3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_line3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Line3D, RIGHTARG = Lseg3D, PROCEDURE = overabove_line3D_lseg3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_line3D_lseg3D(line Line3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_line3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Line3D, RIGHTARG = Lseg3D, PROCEDURE = front_line3D_lseg3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_line3D_lseg3D(line Line3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_line3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Line3D, RIGHTARG = Lseg3D, PROCEDURE = overfront_line3D_lseg3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_line3D_lseg3D(line Line3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_line3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Line3D, RIGHTARG = Lseg3D, PROCEDURE = back_line3D_lseg3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_line3D_lseg3D(line Line3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_line3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Line3D, RIGHTARG = Lseg3D, PROCEDURE = overback_line3D_lseg3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* line3D op line3D */

CREATE OR REPLACE FUNCTION left_line3D_line3D(line1 Line3D, line2 Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_line3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Line3D, RIGHTARG = Line3D, PROCEDURE = left_line3D_line3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_line3D_line3D(line1 Line3D, line2 Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_line3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Line3D, RIGHTARG = Line3D, PROCEDURE = overleft_line3D_line3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_line3D_line3D(line1 Line3D, line2 Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_line3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Line3D, RIGHTARG = Line3D, PROCEDURE = right_line3D_line3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_line3D_line3D(line1 Line3D, line2 Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_line3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Line3D, RIGHTARG = Line3D, PROCEDURE = overright_line3D_line3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_line3D_line3D(line1 Line3D, line2 Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_line3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Line3D, RIGHTARG = Line3D, PROCEDURE = below_line3D_line3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_line3D_line3D(line1 Line3D, line2 Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_line3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Line3D, RIGHTARG = Line3D, PROCEDURE = overbelow_line3D_line3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_line3D_line3D(line1 Line3D, line2 Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_line3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Line3D, RIGHTARG = Line3D, PROCEDURE = above_line3D_line3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_line3D_line3D(line1 Line3D, line2 Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_line3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Line3D, RIGHTARG = Line3D, PROCEDURE = overabove_line3D_line3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_line3D_line3D(line1 Line3D, line2 Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_line3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Line3D, RIGHTARG = Line3D, PROCEDURE = front_line3D_line3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_line3D_line3D(line1 Line3D, line2 Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_line3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Line3D, RIGHTARG = Line3D, PROCEDURE = overfront_line3D_line3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_line3D_line3D(line1 Line3D, line2 Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_line3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Line3D, RIGHTARG = Line3D, PROCEDURE = back_line3D_line3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_line3D_line3D(line1 Line3D, line2 Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_line3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Line3D, RIGHTARG = Line3D, PROCEDURE = overback_line3D_line3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* line3D op box3D */

CREATE OR REPLACE FUNCTION left_line3D_box3D(line Line3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_line3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Line3D, RIGHTARG = Box3D, PROCEDURE = left_line3D_box3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_line3D_box3D(line Line3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_line3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Line3D, RIGHTARG = Box3D, PROCEDURE = overleft_line3D_box3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_line3D_box3D(line Line3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_line3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Line3D, RIGHTARG = Box3D, PROCEDURE = right_line3D_box3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_line3D_box3D(line Line3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_line3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Line3D, RIGHTARG = Box3D, PROCEDURE = overright_line3D_box3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_line3D_box3D(line Line3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_line3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Line3D, RIGHTARG = Box3D, PROCEDURE = below_line3D_box3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_line3D_box3D(line Line3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_line3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Line3D, RIGHTARG = Box3D, PROCEDURE = overbelow_line3D_box3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_line3D_box3D(line Line3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_line3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Line3D, RIGHTARG = Box3D, PROCEDURE = above_line3D_box3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_line3D_box3D(line Line3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_line3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Line3D, RIGHTARG = Box3D, PROCEDURE = overabove_line3D_box3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_line3D_box3D(line Line3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_line3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Line3D, RIGHTARG = Box3D, PROCEDURE = front_line3D_box3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_line3D_box3D(line Line3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_line3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Line3D, RIGHTARG = Box3D, PROCEDURE = overfront_line3D_box3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_line3D_box3D(line Line3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_line3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Line3D, RIGHTARG = Box3D, PROCEDURE = back_line3D_box3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_line3D_box3D(line Line3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_line3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Line3D, RIGHTARG = Box3D, PROCEDURE = overback_line3D_box3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* line3D op poly3D */

CREATE OR REPLACE FUNCTION left_line3D_path3D(line Line3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_line3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Line3D, RIGHTARG = Path3D, PROCEDURE = left_line3D_path3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_line3D_path3D(line Line3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_line3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Line3D, RIGHTARG = Path3D, PROCEDURE = overleft_line3D_path3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_line3D_path3D(line Line3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_line3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Line3D, RIGHTARG = Path3D, PROCEDURE = right_line3D_path3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_line3D_path3D(line Line3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_line3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Line3D, RIGHTARG = Path3D, PROCEDURE = overright_line3D_path3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_line3D_path3D(line Line3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_line3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Line3D, RIGHTARG = Path3D, PROCEDURE = below_line3D_path3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_line3D_path3D(line Line3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_line3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Line3D, RIGHTARG = Path3D, PROCEDURE = overbelow_line3D_path3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_line3D_path3D(line Line3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_line3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Line3D, RIGHTARG = Path3D, PROCEDURE = above_line3D_path3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_line3D_path3D(line Line3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_line3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Line3D, RIGHTARG = Path3D, PROCEDURE = overabove_line3D_path3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_line3D_path3D(line Line3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_line3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Line3D, RIGHTARG = Path3D, PROCEDURE = front_line3D_path3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_line3D_path3D(line Line3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_line3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Line3D, RIGHTARG = Path3D, PROCEDURE = overfront_line3D_path3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_line3D_path3D(line Line3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_line3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Line3D, RIGHTARG = Path3D, PROCEDURE = back_line3D_path3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_line3D_path3D(line Line3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_line3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Line3D, RIGHTARG = Path3D, PROCEDURE = overback_line3D_path3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* line3D op poly3D */

CREATE OR REPLACE FUNCTION left_line3D_poly3D(line Line3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_line3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Line3D, RIGHTARG = Polygon3D, PROCEDURE = left_line3D_poly3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_line3D_poly3D(line Line3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_line3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Line3D, RIGHTARG = Polygon3D, PROCEDURE = overleft_line3D_poly3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_line3D_poly3D(line Line3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_line3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Line3D, RIGHTARG = Polygon3D, PROCEDURE = right_line3D_poly3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_line3D_poly3D(line Line3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_line3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Line3D, RIGHTARG = Polygon3D, PROCEDURE = overright_line3D_poly3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_line3D_poly3D(line Line3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_line3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Line3D, RIGHTARG = Polygon3D, PROCEDURE = below_line3D_poly3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_line3D_poly3D(line Line3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_line3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Line3D, RIGHTARG = Polygon3D, PROCEDURE = overbelow_line3D_poly3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_line3D_poly3D(line Line3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_line3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Line3D, RIGHTARG = Polygon3D, PROCEDURE = above_line3D_poly3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_line3D_poly3D(line Line3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_line3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Line3D, RIGHTARG = Polygon3D, PROCEDURE = overabove_line3D_poly3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_line3D_poly3D(line Line3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_line3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Line3D, RIGHTARG = Polygon3D, PROCEDURE = front_line3D_poly3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_line3D_poly3D(line Line3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_line3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Line3D, RIGHTARG = Polygon3D, PROCEDURE = overfront_line3D_poly3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_line3D_poly3D(line Line3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_line3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Line3D, RIGHTARG = Polygon3D, PROCEDURE = back_line3D_poly3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_line3D_poly3D(line Line3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_line3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Line3D, RIGHTARG = Polygon3D, PROCEDURE = overback_line3D_poly3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* line3D op sphere */

CREATE OR REPLACE FUNCTION left_line3D_sphere(line Line3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'left_line3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Line3D, RIGHTARG = Sphere, PROCEDURE = left_line3D_sphere,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_line3D_sphere(line Line3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_line3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Line3D, RIGHTARG = Sphere, PROCEDURE = overleft_line3D_sphere,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_line3D_sphere(line Line3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'right_line3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Line3D, RIGHTARG = Sphere, PROCEDURE = right_line3D_sphere,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_line3D_sphere(line Line3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_line3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Line3D, RIGHTARG = Sphere, PROCEDURE = overright_line3D_sphere,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_line3D_sphere(line Line3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'below_line3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Line3D, RIGHTARG = Sphere, PROCEDURE = below_line3D_sphere,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_line3D_sphere(line Line3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_line3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Line3D, RIGHTARG = Sphere, PROCEDURE = overbelow_line3D_sphere,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_line3D_sphere(line Line3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'above_line3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Line3D, RIGHTARG = Sphere, PROCEDURE = above_line3D_sphere,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_line3D_sphere(line Line3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_line3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Line3D, RIGHTARG = Sphere, PROCEDURE = overabove_line3D_sphere,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_line3D_sphere(line Line3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'front_line3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Line3D, RIGHTARG = Sphere, PROCEDURE = front_line3D_sphere,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_line3D_sphere(line Line3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_line3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Line3D, RIGHTARG = Sphere, PROCEDURE = overfront_line3D_sphere,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_line3D_sphere(line Line3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'back_line3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Line3D, RIGHTARG = Sphere, PROCEDURE = back_line3D_sphere,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_line3D_sphere(line Line3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_line3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Line3D, RIGHTARG = Sphere, PROCEDURE = overback_line3D_sphere,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/*-------------------------------------------------------------------------*/

/* box3D op point3D */

CREATE OR REPLACE FUNCTION left_box3D_point3D(box Box3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_box3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Box3D, RIGHTARG = Point3D, PROCEDURE = left_box3D_point3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_box3D_point3D(box Box3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_box3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Box3D, RIGHTARG = Point3D, PROCEDURE = overleft_box3D_point3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_box3D_point3D(box Box3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_box3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Box3D, RIGHTARG = Point3D, PROCEDURE = right_box3D_point3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_box3D_point3D(box Box3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_box3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Box3D, RIGHTARG = Point3D, PROCEDURE = overright_box3D_point3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_box3D_point3D(box Box3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_box3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Box3D, RIGHTARG = Point3D, PROCEDURE = below_box3D_point3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_box3D_point3D(box Box3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_box3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Box3D, RIGHTARG = Point3D, PROCEDURE = overbelow_box3D_point3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_box3D_point3D(box Box3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_box3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Box3D, RIGHTARG = Point3D, PROCEDURE = above_box3D_point3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_box3D_point3D(box Box3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_box3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Box3D, RIGHTARG = Point3D, PROCEDURE = overabove_box3D_point3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_box3D_point3D(box Box3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_box3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Box3D, RIGHTARG = Point3D, PROCEDURE = front_box3D_point3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_box3D_point3D(box Box3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_box3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Box3D, RIGHTARG = Point3D, PROCEDURE = overfront_box3D_point3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_box3D_point3D(box Box3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_box3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Box3D, RIGHTARG = Point3D, PROCEDURE = back_box3D_point3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_box3D_point3D(box Box3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_box3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Box3D, RIGHTARG = Point3D, PROCEDURE = overback_box3D_point3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* box3D op lseg3D */

CREATE OR REPLACE FUNCTION left_box3D_lseg3D(box Box3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_box3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Box3D, RIGHTARG = Lseg3D, PROCEDURE = left_box3D_lseg3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_box3D_lseg3D(box Box3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_box3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Box3D, RIGHTARG = Lseg3D, PROCEDURE = overleft_box3D_lseg3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_box3D_lseg3D(box Box3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_box3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Box3D, RIGHTARG = Lseg3D, PROCEDURE = right_box3D_lseg3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_box3D_lseg3D(box Box3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_box3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Box3D, RIGHTARG = Lseg3D, PROCEDURE = overright_box3D_lseg3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_box3D_lseg3D(box Box3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_box3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Box3D, RIGHTARG = Lseg3D, PROCEDURE = below_box3D_lseg3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_box3D_lseg3D(box Box3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_box3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Box3D, RIGHTARG = Lseg3D, PROCEDURE = overbelow_box3D_lseg3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_box3D_lseg3D(box Box3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_box3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Box3D, RIGHTARG = Lseg3D, PROCEDURE = above_box3D_lseg3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_box3D_lseg3D(box Box3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_box3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Box3D, RIGHTARG = Lseg3D, PROCEDURE = overabove_box3D_lseg3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_box3D_lseg3D(box Box3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_box3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Box3D, RIGHTARG = Lseg3D, PROCEDURE = front_box3D_lseg3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_box3D_lseg3D(box Box3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_box3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Box3D, RIGHTARG = Lseg3D, PROCEDURE = overfront_box3D_lseg3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_box3D_lseg3D(box Box3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_box3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Box3D, RIGHTARG = Lseg3D, PROCEDURE = back_box3D_lseg3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_box3D_lseg3D(box Box3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_box3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Box3D, RIGHTARG = Lseg3D, PROCEDURE = overback_box3D_lseg3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* box3D op line3D */

CREATE OR REPLACE FUNCTION left_box3D_line3D(box Box3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_box3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Box3D, RIGHTARG = Line3D, PROCEDURE = left_box3D_line3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_box3D_line3D(box Box3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_box3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Box3D, RIGHTARG = Line3D, PROCEDURE = overleft_box3D_line3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_box3D_line3D(box Box3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_box3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Box3D, RIGHTARG = Line3D, PROCEDURE = right_box3D_line3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_box3D_line3D(box Box3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_box3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Box3D, RIGHTARG = Line3D, PROCEDURE = overright_box3D_line3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_box3D_line3D(box Box3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_box3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Box3D, RIGHTARG = Line3D, PROCEDURE = below_box3D_line3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_box3D_line3D(box Box3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_box3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Box3D, RIGHTARG = Line3D, PROCEDURE = overbelow_box3D_line3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_box3D_line3D(box Box3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_box3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Box3D, RIGHTARG = Line3D, PROCEDURE = above_box3D_line3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_box3D_line3D(box Box3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_box3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Box3D, RIGHTARG = Line3D, PROCEDURE = overabove_box3D_line3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_box3D_line3D(box Box3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_box3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Box3D, RIGHTARG = Line3D, PROCEDURE = front_box3D_line3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_box3D_line3D(box Box3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_box3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Box3D, RIGHTARG = Line3D, PROCEDURE = overfront_box3D_line3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_box3D_line3D(box Box3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_box3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Box3D, RIGHTARG = Line3D, PROCEDURE = back_box3D_line3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_box3D_line3D(box Box3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_box3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Box3D, RIGHTARG = Line3D, PROCEDURE = overback_box3D_line3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* box3D op box3D */

CREATE OR REPLACE FUNCTION left_box3D_box3D(box1 Box3D, box2 Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_box3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Box3D, RIGHTARG = Box3D, PROCEDURE = left_box3D_box3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_box3D_box3D(box1 Box3D, box2 Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_box3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Box3D, RIGHTARG = Box3D, PROCEDURE = overleft_box3D_box3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_box3D_box3D(box1 Box3D, box2 Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_box3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Box3D, RIGHTARG = Box3D, PROCEDURE = right_box3D_box3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_box3D_box3D(box1 Box3D, box2 Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_box3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Box3D, RIGHTARG = Box3D, PROCEDURE = overright_box3D_box3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_box3D_box3D(box1 Box3D, box2 Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_box3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Box3D, RIGHTARG = Box3D, PROCEDURE = below_box3D_box3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_box3D_box3D(box1 Box3D, box2 Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_box3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Box3D, RIGHTARG = Box3D, PROCEDURE = overbelow_box3D_box3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_box3D_box3D(box1 Box3D, box2 Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_box3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Box3D, RIGHTARG = Box3D, PROCEDURE = above_box3D_box3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_box3D_box3D(box1 Box3D, box2 Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_box3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Box3D, RIGHTARG = Box3D, PROCEDURE = overabove_box3D_box3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_box3D_box3D(box1 Box3D, box2 Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_box3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Box3D, RIGHTARG = Box3D, PROCEDURE = front_box3D_box3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_box3D_box3D(box1 Box3D, box2 Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_box3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Box3D, RIGHTARG = Box3D, PROCEDURE = overfront_box3D_box3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_box3D_box3D(box1 Box3D, box2 Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_box3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Box3D, RIGHTARG = Box3D, PROCEDURE = back_box3D_box3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_box3D_box3D(box1 Box3D, box2 Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_box3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Box3D, RIGHTARG = Box3D, PROCEDURE = overback_box3D_box3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* box3D op poly3D */

CREATE OR REPLACE FUNCTION left_box3D_path3D(box Box3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_box3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Box3D, RIGHTARG = Path3D, PROCEDURE = left_box3D_path3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_box3D_path3D(box Box3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_box3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Box3D, RIGHTARG = Path3D, PROCEDURE = overleft_box3D_path3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_box3D_path3D(box Box3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_box3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Box3D, RIGHTARG = Path3D, PROCEDURE = right_box3D_path3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_box3D_path3D(box Box3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_box3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Box3D, RIGHTARG = Path3D, PROCEDURE = overright_box3D_path3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_box3D_path3D(box Box3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_box3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Box3D, RIGHTARG = Path3D, PROCEDURE = below_box3D_path3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_box3D_path3D(box Box3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_box3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Box3D, RIGHTARG = Path3D, PROCEDURE = overbelow_box3D_path3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_box3D_path3D(box Box3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_box3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Box3D, RIGHTARG = Path3D, PROCEDURE = above_box3D_path3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_box3D_path3D(box Box3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_box3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Box3D, RIGHTARG = Path3D, PROCEDURE = overabove_box3D_path3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_box3D_path3D(box Box3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_box3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Box3D, RIGHTARG = Path3D, PROCEDURE = front_box3D_path3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_box3D_path3D(box Box3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_box3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Box3D, RIGHTARG = Path3D, PROCEDURE = overfront_box3D_path3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_box3D_path3D(box Box3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_box3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Box3D, RIGHTARG = Path3D, PROCEDURE = back_box3D_path3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_box3D_path3D(box Box3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_box3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Box3D, RIGHTARG = Path3D, PROCEDURE = overback_box3D_path3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* box3D op poly3D */

CREATE OR REPLACE FUNCTION left_box3D_poly3D(box Box3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_box3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Box3D, RIGHTARG = Polygon3D, PROCEDURE = left_box3D_poly3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_box3D_poly3D(box Box3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_box3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Box3D, RIGHTARG = Polygon3D, PROCEDURE = overleft_box3D_poly3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_box3D_poly3D(box Box3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_box3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Box3D, RIGHTARG = Polygon3D, PROCEDURE = right_box3D_poly3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_box3D_poly3D(box Box3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_box3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Box3D, RIGHTARG = Polygon3D, PROCEDURE = overright_box3D_poly3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_box3D_poly3D(box Box3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_box3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Box3D, RIGHTARG = Polygon3D, PROCEDURE = below_box3D_poly3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_box3D_poly3D(box Box3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_box3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Box3D, RIGHTARG = Polygon3D, PROCEDURE = overbelow_box3D_poly3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_box3D_poly3D(box Box3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_box3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Box3D, RIGHTARG = Polygon3D, PROCEDURE = above_box3D_poly3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_box3D_poly3D(box Box3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_box3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Box3D, RIGHTARG = Polygon3D, PROCEDURE = overabove_box3D_poly3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_box3D_poly3D(box Box3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_box3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Box3D, RIGHTARG = Polygon3D, PROCEDURE = front_box3D_poly3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_box3D_poly3D(box Box3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_box3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Box3D, RIGHTARG = Polygon3D, PROCEDURE = overfront_box3D_poly3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_box3D_poly3D(box Box3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_box3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Box3D, RIGHTARG = Polygon3D, PROCEDURE = back_box3D_poly3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_box3D_poly3D(box Box3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_box3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Box3D, RIGHTARG = Polygon3D, PROCEDURE = overback_box3D_poly3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* box3D op sphere */

CREATE OR REPLACE FUNCTION left_box3D_sphere(box Box3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'left_box3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Box3D, RIGHTARG = Sphere, PROCEDURE = left_box3D_sphere,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_box3D_sphere(box Box3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_box3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Box3D, RIGHTARG = Sphere, PROCEDURE = overleft_box3D_sphere,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_box3D_sphere(box Box3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'right_box3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Box3D, RIGHTARG = Sphere, PROCEDURE = right_box3D_sphere,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_box3D_sphere(box Box3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_box3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Box3D, RIGHTARG = Sphere, PROCEDURE = overright_box3D_sphere,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_box3D_sphere(box Box3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'below_box3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Box3D, RIGHTARG = Sphere, PROCEDURE = below_box3D_sphere,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_box3D_sphere(box Box3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_box3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Box3D, RIGHTARG = Sphere, PROCEDURE = overbelow_box3D_sphere,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_box3D_sphere(box Box3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'above_box3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Box3D, RIGHTARG = Sphere, PROCEDURE = above_box3D_sphere,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_box3D_sphere(box Box3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_box3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Box3D, RIGHTARG = Sphere, PROCEDURE = overabove_box3D_sphere,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_box3D_sphere(box Box3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'front_box3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Box3D, RIGHTARG = Sphere, PROCEDURE = front_box3D_sphere,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_box3D_sphere(box Box3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_box3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Box3D, RIGHTARG = Sphere, PROCEDURE = overfront_box3D_sphere,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_box3D_sphere(box Box3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'back_box3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Box3D, RIGHTARG = Sphere, PROCEDURE = back_box3D_sphere,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_box3D_sphere(box Box3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_box3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Box3D, RIGHTARG = Sphere, PROCEDURE = overback_box3D_sphere,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/*-------------------------------------------------------------------------*/

/* path3D op point3D */

CREATE OR REPLACE FUNCTION left_path3D_point3D(path Path3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_path3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Path3D, RIGHTARG = Point3D, PROCEDURE = left_path3D_point3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_path3D_point3D(path Path3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_path3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Path3D, RIGHTARG = Point3D, PROCEDURE = overleft_path3D_point3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_path3D_point3D(path Path3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_path3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Path3D, RIGHTARG = Point3D, PROCEDURE = right_path3D_point3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_path3D_point3D(path Path3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_path3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Path3D, RIGHTARG = Point3D, PROCEDURE = overright_path3D_point3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_path3D_point3D(path Path3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_path3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Path3D, RIGHTARG = Point3D, PROCEDURE = below_path3D_point3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_path3D_point3D(path Path3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_path3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Path3D, RIGHTARG = Point3D, PROCEDURE = overbelow_path3D_point3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_path3D_point3D(path Path3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_path3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Path3D, RIGHTARG = Point3D, PROCEDURE = above_path3D_point3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_path3D_point3D(path Path3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_path3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Path3D, RIGHTARG = Point3D, PROCEDURE = overabove_path3D_point3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_path3D_point3D(path Path3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_path3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Path3D, RIGHTARG = Point3D, PROCEDURE = front_path3D_point3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_path3D_point3D(path Path3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_path3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Path3D, RIGHTARG = Point3D, PROCEDURE = overfront_path3D_point3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_path3D_point3D(path Path3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_path3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Path3D, RIGHTARG = Point3D, PROCEDURE = back_path3D_point3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_path3D_point3D(path Path3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_path3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Path3D, RIGHTARG = Point3D, PROCEDURE = overback_path3D_point3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* path3D op lseg3D */

CREATE OR REPLACE FUNCTION left_path3D_lseg3D(path Path3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_path3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Path3D, RIGHTARG = Lseg3D, PROCEDURE = left_path3D_lseg3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_path3D_lseg3D(path Path3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_path3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Path3D, RIGHTARG = Lseg3D, PROCEDURE = overleft_path3D_lseg3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_path3D_lseg3D(path Path3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_path3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Path3D, RIGHTARG = Lseg3D, PROCEDURE = right_path3D_lseg3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_path3D_lseg3D(path Path3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_path3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Path3D, RIGHTARG = Lseg3D, PROCEDURE = overright_path3D_lseg3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_path3D_lseg3D(path Path3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_path3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Path3D, RIGHTARG = Lseg3D, PROCEDURE = below_path3D_lseg3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_path3D_lseg3D(path Path3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_path3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Path3D, RIGHTARG = Lseg3D, PROCEDURE = overbelow_path3D_lseg3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_path3D_lseg3D(path Path3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_path3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Path3D, RIGHTARG = Lseg3D, PROCEDURE = above_path3D_lseg3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_path3D_lseg3D(path Path3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_path3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Path3D, RIGHTARG = Lseg3D, PROCEDURE = overabove_path3D_lseg3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_path3D_lseg3D(path Path3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_path3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Path3D, RIGHTARG = Lseg3D, PROCEDURE = front_path3D_lseg3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_path3D_lseg3D(path Path3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_path3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Path3D, RIGHTARG = Lseg3D, PROCEDURE = overfront_path3D_lseg3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_path3D_lseg3D(path Path3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_path3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Path3D, RIGHTARG = Lseg3D, PROCEDURE = back_path3D_lseg3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_path3D_lseg3D(path Path3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_path3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Path3D, RIGHTARG = Lseg3D, PROCEDURE = overback_path3D_lseg3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);  

/* path3D op line3D */

CREATE OR REPLACE FUNCTION left_path3D_line3D(path Path3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_path3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Path3D, RIGHTARG = Line3D, PROCEDURE = left_path3D_line3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_path3D_line3D(path Path3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_path3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Path3D, RIGHTARG = Line3D, PROCEDURE = overleft_path3D_line3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_path3D_line3D(path Path3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_path3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Path3D, RIGHTARG = Line3D, PROCEDURE = right_path3D_line3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_path3D_line3D(path Path3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_path3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Path3D, RIGHTARG = Line3D, PROCEDURE = overright_path3D_line3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_path3D_line3D(path Path3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_path3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Path3D, RIGHTARG = Line3D, PROCEDURE = below_path3D_line3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_path3D_line3D(path Path3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_path3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Path3D, RIGHTARG = Line3D, PROCEDURE = overbelow_path3D_line3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_path3D_line3D(path Path3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_path3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Path3D, RIGHTARG = Line3D, PROCEDURE = above_path3D_line3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_path3D_line3D(path Path3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_path3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Path3D, RIGHTARG = Line3D, PROCEDURE = overabove_path3D_line3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_path3D_line3D(path Path3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_path3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Path3D, RIGHTARG = Line3D, PROCEDURE = front_path3D_line3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_path3D_line3D(path Path3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_path3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Path3D, RIGHTARG = Line3D, PROCEDURE = overfront_path3D_line3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_path3D_line3D(path Path3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_path3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Path3D, RIGHTARG = Line3D, PROCEDURE = back_path3D_line3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_path3D_line3D(path Path3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_path3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Path3D, RIGHTARG = Line3D, PROCEDURE = overback_path3D_line3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);  

/* path3D op box3D */

CREATE OR REPLACE FUNCTION left_path3D_box3D(path Path3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_path3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Path3D, RIGHTARG = Box3D, PROCEDURE = left_path3D_box3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_path3D_box3D(path Path3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_path3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Path3D, RIGHTARG = Box3D, PROCEDURE = overleft_path3D_box3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_path3D_box3D(path Path3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_path3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Path3D, RIGHTARG = Box3D, PROCEDURE = right_path3D_box3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_path3D_box3D(path Path3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_path3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Path3D, RIGHTARG = Box3D, PROCEDURE = overright_path3D_box3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_path3D_box3D(path Path3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_path3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Path3D, RIGHTARG = Box3D, PROCEDURE = below_path3D_box3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_path3D_box3D(path Path3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_path3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Path3D, RIGHTARG = Box3D, PROCEDURE = overbelow_path3D_box3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_path3D_box3D(path Path3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_path3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Path3D, RIGHTARG = Box3D, PROCEDURE = above_path3D_box3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_path3D_box3D(path Path3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_path3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Path3D, RIGHTARG = Box3D, PROCEDURE = overabove_path3D_box3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_path3D_box3D(path Path3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_path3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Path3D, RIGHTARG = Box3D, PROCEDURE = front_path3D_box3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_path3D_box3D(path Path3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_path3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Path3D, RIGHTARG = Box3D, PROCEDURE = overfront_path3D_box3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_path3D_box3D(path Path3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_path3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Path3D, RIGHTARG = Box3D, PROCEDURE = back_path3D_box3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_path3D_box3D(path Path3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_path3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Path3D, RIGHTARG = Box3D, PROCEDURE = overback_path3D_box3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);  

/* path3D op path3D */

CREATE OR REPLACE FUNCTION left_path3D_path3D(path1 Path3D, path2 Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_path3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Path3D, RIGHTARG = Path3D, PROCEDURE = left_path3D_path3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_path3D_path3D(path1 Path3D, path2 Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_path3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Path3D, RIGHTARG = Path3D, PROCEDURE = overleft_path3D_path3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_path3D_path3D(path1 Path3D, path2 Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_path3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Path3D, RIGHTARG = Path3D, PROCEDURE = right_path3D_path3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_path3D_path3D(path1 Path3D, path2 Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_path3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Path3D, RIGHTARG = Path3D, PROCEDURE = overright_path3D_path3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_path3D_path3D(path1 Path3D, path2 Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_path3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Path3D, RIGHTARG = Path3D, PROCEDURE = below_path3D_path3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_path3D_path3D(path1 Path3D, path2 Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_path3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Path3D, RIGHTARG = Path3D, PROCEDURE = overbelow_path3D_path3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_path3D_path3D(path1 Path3D, path2 Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_path3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Path3D, RIGHTARG = Path3D, PROCEDURE = above_path3D_path3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_path3D_path3D(path1 Path3D, path2 Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_path3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Path3D, RIGHTARG = Path3D, PROCEDURE = overabove_path3D_path3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_path3D_path3D(path1 Path3D, path2 Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_path3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Path3D, RIGHTARG = Path3D, PROCEDURE = front_path3D_path3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_path3D_path3D(path1 Path3D, path2 Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_path3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Path3D, RIGHTARG = Path3D, PROCEDURE = overfront_path3D_path3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_path3D_path3D(path1 Path3D, path2 Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_path3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Path3D, RIGHTARG = Path3D, PROCEDURE = back_path3D_path3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_path3D_path3D(path1 Path3D, path2 Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_path3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Path3D, RIGHTARG = Path3D, PROCEDURE = overback_path3D_path3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* path3D op poly3D */

CREATE OR REPLACE FUNCTION left_path3D_poly3D(path Path3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_path3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Path3D, RIGHTARG = Polygon3D, PROCEDURE = left_path3D_poly3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_path3D_poly3D(path Path3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_path3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Path3D, RIGHTARG = Polygon3D, PROCEDURE = overleft_path3D_poly3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_path3D_poly3D(path Path3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_path3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Path3D, RIGHTARG = Polygon3D, PROCEDURE = right_path3D_poly3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_path3D_poly3D(path Path3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_path3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Path3D, RIGHTARG = Polygon3D, PROCEDURE = overright_path3D_poly3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_path3D_poly3D(path Path3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_path3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Path3D, RIGHTARG = Polygon3D, PROCEDURE = below_path3D_poly3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_path3D_poly3D(path Path3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_path3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Path3D, RIGHTARG = Polygon3D, PROCEDURE = overbelow_path3D_poly3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_path3D_poly3D(path Path3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_path3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Path3D, RIGHTARG = Polygon3D, PROCEDURE = above_path3D_poly3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_path3D_poly3D(path Path3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_path3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Path3D, RIGHTARG = Polygon3D, PROCEDURE = overabove_path3D_poly3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_path3D_poly3D(path Path3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_path3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Path3D, RIGHTARG = Polygon3D, PROCEDURE = front_path3D_poly3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_path3D_poly3D(path Path3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_path3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Path3D, RIGHTARG = Polygon3D, PROCEDURE = overfront_path3D_poly3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_path3D_poly3D(path Path3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_path3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Path3D, RIGHTARG = Polygon3D, PROCEDURE = back_path3D_poly3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_path3D_poly3D(path Path3D, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_path3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Path3D, RIGHTARG = Polygon3D, PROCEDURE = overback_path3D_poly3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* path3D op sphere */

CREATE OR REPLACE FUNCTION left_path3D_sphere(path Path3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'left_path3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Path3D, RIGHTARG = Sphere, PROCEDURE = left_path3D_sphere,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_path3D_sphere(path Path3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_path3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Path3D, RIGHTARG = Sphere, PROCEDURE = overleft_path3D_sphere,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_path3D_sphere(path Path3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'right_path3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Path3D, RIGHTARG = Sphere, PROCEDURE = right_path3D_sphere,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_path3D_sphere(path Path3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_path3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Path3D, RIGHTARG = Sphere, PROCEDURE = overright_path3D_sphere,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_path3D_sphere(path Path3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'below_path3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Path3D, RIGHTARG = Sphere, PROCEDURE = below_path3D_sphere,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_path3D_sphere(path Path3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_path3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Path3D, RIGHTARG = Sphere, PROCEDURE = overbelow_path3D_sphere,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_path3D_sphere(path Path3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'above_path3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Path3D, RIGHTARG = Sphere, PROCEDURE = above_path3D_sphere,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_path3D_sphere(path Path3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_path3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Path3D, RIGHTARG = Sphere, PROCEDURE = overabove_path3D_sphere,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_path3D_sphere(path Path3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'front_path3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Path3D, RIGHTARG = Sphere, PROCEDURE = front_path3D_sphere,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_path3D_sphere(path Path3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_path3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Path3D, RIGHTARG = Sphere, PROCEDURE = overfront_path3D_sphere,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_path3D_sphere(path Path3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'back_path3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Path3D, RIGHTARG = Sphere, PROCEDURE = back_path3D_sphere,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_path3D_sphere(path Path3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_path3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Path3D, RIGHTARG = Sphere, PROCEDURE = overback_path3D_sphere,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);
 
/*-------------------------------------------------------------------------*/

/* poly3D op point3D */

CREATE OR REPLACE FUNCTION left_poly3D_point3D(poly Polygon3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_poly3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Polygon3D, RIGHTARG = Point3D, PROCEDURE = left_poly3D_point3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_poly3D_point3D(poly Polygon3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_poly3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Polygon3D, RIGHTARG = Point3D, PROCEDURE = overleft_poly3D_point3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_poly3D_point3D(poly Polygon3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_poly3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Polygon3D, RIGHTARG = Point3D, PROCEDURE = right_poly3D_point3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_poly3D_point3D(poly Polygon3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_poly3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Polygon3D, RIGHTARG = Point3D, PROCEDURE = overright_poly3D_point3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_poly3D_point3D(poly Polygon3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_poly3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Polygon3D, RIGHTARG = Point3D, PROCEDURE = below_poly3D_point3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_poly3D_point3D(poly Polygon3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_poly3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Polygon3D, RIGHTARG = Point3D, PROCEDURE = overbelow_poly3D_point3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_poly3D_point3D(poly Polygon3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_poly3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Polygon3D, RIGHTARG = Point3D, PROCEDURE = above_poly3D_point3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_poly3D_point3D(poly Polygon3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_poly3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Polygon3D, RIGHTARG = Point3D, PROCEDURE = overabove_poly3D_point3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_poly3D_point3D(poly Polygon3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_poly3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Polygon3D, RIGHTARG = Point3D, PROCEDURE = front_poly3D_point3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_poly3D_point3D(poly Polygon3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_poly3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Polygon3D, RIGHTARG = Point3D, PROCEDURE = overfront_poly3D_point3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_poly3D_point3D(poly Polygon3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_poly3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Polygon3D, RIGHTARG = Point3D, PROCEDURE = back_poly3D_point3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_poly3D_point3D(poly Polygon3D, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_poly3D_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Polygon3D, RIGHTARG = Point3D, PROCEDURE = overback_poly3D_point3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* poly3D op lseg3D */

CREATE OR REPLACE FUNCTION left_poly3D_lseg3D(poly Polygon3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_poly3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Polygon3D, RIGHTARG = Lseg3D, PROCEDURE = left_poly3D_lseg3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_poly3D_lseg3D(poly Polygon3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_poly3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Polygon3D, RIGHTARG = Lseg3D, PROCEDURE = overleft_poly3D_lseg3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_poly3D_lseg3D(poly Polygon3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_poly3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Polygon3D, RIGHTARG = Lseg3D, PROCEDURE = right_poly3D_lseg3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_poly3D_lseg3D(poly Polygon3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_poly3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Polygon3D, RIGHTARG = Lseg3D, PROCEDURE = overright_poly3D_lseg3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_poly3D_lseg3D(poly Polygon3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_poly3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Polygon3D, RIGHTARG = Lseg3D, PROCEDURE = below_poly3D_lseg3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_poly3D_lseg3D(poly Polygon3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_poly3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Polygon3D, RIGHTARG = Lseg3D, PROCEDURE = overbelow_poly3D_lseg3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_poly3D_lseg3D(poly Polygon3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_poly3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Polygon3D, RIGHTARG = Lseg3D, PROCEDURE = above_poly3D_lseg3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_poly3D_lseg3D(poly Polygon3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_poly3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Polygon3D, RIGHTARG = Lseg3D, PROCEDURE = overabove_poly3D_lseg3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_poly3D_lseg3D(poly Polygon3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_poly3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Polygon3D, RIGHTARG = Lseg3D, PROCEDURE = front_poly3D_lseg3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_poly3D_lseg3D(poly Polygon3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_poly3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Polygon3D, RIGHTARG = Lseg3D, PROCEDURE = overfront_poly3D_lseg3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_poly3D_lseg3D(poly Polygon3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_poly3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Polygon3D, RIGHTARG = Lseg3D, PROCEDURE = back_poly3D_lseg3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_poly3D_lseg3D(poly Polygon3D, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_poly3D_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Polygon3D, RIGHTARG = Lseg3D, PROCEDURE = overback_poly3D_lseg3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);  

/* poly3D op line3D */

CREATE OR REPLACE FUNCTION left_poly3D_line3D(poly Polygon3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_poly3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Polygon3D, RIGHTARG = Line3D, PROCEDURE = left_poly3D_line3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_poly3D_line3D(poly Polygon3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_poly3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Polygon3D, RIGHTARG = Line3D, PROCEDURE = overleft_poly3D_line3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_poly3D_line3D(poly Polygon3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_poly3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Polygon3D, RIGHTARG = Line3D, PROCEDURE = right_poly3D_line3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_poly3D_line3D(poly Polygon3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_poly3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Polygon3D, RIGHTARG = Line3D, PROCEDURE = overright_poly3D_line3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_poly3D_line3D(poly Polygon3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_poly3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Polygon3D, RIGHTARG = Line3D, PROCEDURE = below_poly3D_line3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_poly3D_line3D(poly Polygon3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_poly3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Polygon3D, RIGHTARG = Line3D, PROCEDURE = overbelow_poly3D_line3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_poly3D_line3D(poly Polygon3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_poly3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Polygon3D, RIGHTARG = Line3D, PROCEDURE = above_poly3D_line3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_poly3D_line3D(poly Polygon3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_poly3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Polygon3D, RIGHTARG = Line3D, PROCEDURE = overabove_poly3D_line3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_poly3D_line3D(poly Polygon3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_poly3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Polygon3D, RIGHTARG = Line3D, PROCEDURE = front_poly3D_line3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_poly3D_line3D(poly Polygon3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_poly3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Polygon3D, RIGHTARG = Line3D, PROCEDURE = overfront_poly3D_line3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_poly3D_line3D(poly Polygon3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_poly3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Polygon3D, RIGHTARG = Line3D, PROCEDURE = back_poly3D_line3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_poly3D_line3D(poly Polygon3D, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_poly3D_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Polygon3D, RIGHTARG = Line3D, PROCEDURE = overback_poly3D_line3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);  

/* poly3D op box3D */

CREATE OR REPLACE FUNCTION left_poly3D_box3D(poly Polygon3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_poly3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Polygon3D, RIGHTARG = Box3D, PROCEDURE = left_poly3D_box3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_poly3D_box3D(poly Polygon3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_poly3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Polygon3D, RIGHTARG = Box3D, PROCEDURE = overleft_poly3D_box3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_poly3D_box3D(poly Polygon3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_poly3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Polygon3D, RIGHTARG = Box3D, PROCEDURE = right_poly3D_box3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_poly3D_box3D(poly Polygon3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_poly3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Polygon3D, RIGHTARG = Box3D, PROCEDURE = overright_poly3D_box3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_poly3D_box3D(poly Polygon3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_poly3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Polygon3D, RIGHTARG = Box3D, PROCEDURE = below_poly3D_box3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_poly3D_box3D(poly Polygon3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_poly3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Polygon3D, RIGHTARG = Box3D, PROCEDURE = overbelow_poly3D_box3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_poly3D_box3D(poly Polygon3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_poly3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Polygon3D, RIGHTARG = Box3D, PROCEDURE = above_poly3D_box3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_poly3D_box3D(poly Polygon3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_poly3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Polygon3D, RIGHTARG = Box3D, PROCEDURE = overabove_poly3D_box3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_poly3D_box3D(poly Polygon3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_poly3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Polygon3D, RIGHTARG = Box3D, PROCEDURE = front_poly3D_box3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_poly3D_box3D(poly Polygon3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_poly3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Polygon3D, RIGHTARG = Box3D, PROCEDURE = overfront_poly3D_box3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_poly3D_box3D(poly Polygon3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_poly3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Polygon3D, RIGHTARG = Box3D, PROCEDURE = back_poly3D_box3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_poly3D_box3D(poly Polygon3D, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_poly3D_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Polygon3D, RIGHTARG = Box3D, PROCEDURE = overback_poly3D_box3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* poly3D op path3D */

CREATE OR REPLACE FUNCTION left_poly3D_path3D(poly Polygon3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_poly3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Polygon3D, RIGHTARG = Path3D, PROCEDURE = left_poly3D_path3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_poly3D_path3D(poly Polygon3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_poly3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Polygon3D, RIGHTARG = Path3D, PROCEDURE = overleft_poly3D_path3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_poly3D_path3D(poly Polygon3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_poly3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Polygon3D, RIGHTARG = Path3D, PROCEDURE = right_poly3D_path3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_poly3D_path3D(poly Polygon3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_poly3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Polygon3D, RIGHTARG = Path3D, PROCEDURE = overright_poly3D_path3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_poly3D_path3D(poly Polygon3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_poly3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Polygon3D, RIGHTARG = Path3D, PROCEDURE = below_poly3D_path3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_poly3D_path3D(poly Polygon3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_poly3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Polygon3D, RIGHTARG = Path3D, PROCEDURE = overbelow_poly3D_path3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_poly3D_path3D(poly Polygon3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_poly3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Polygon3D, RIGHTARG = Path3D, PROCEDURE = above_poly3D_path3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_poly3D_path3D(poly Polygon3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_poly3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Polygon3D, RIGHTARG = Path3D, PROCEDURE = overabove_poly3D_path3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_poly3D_path3D(poly Polygon3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_poly3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Polygon3D, RIGHTARG = Path3D, PROCEDURE = front_poly3D_path3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_poly3D_path3D(poly Polygon3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_poly3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Polygon3D, RIGHTARG = Path3D, PROCEDURE = overfront_poly3D_path3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_poly3D_path3D(poly Polygon3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_poly3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Polygon3D, RIGHTARG = Path3D, PROCEDURE = back_poly3D_path3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_poly3D_path3D(poly Polygon3D, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_poly3D_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Polygon3D, RIGHTARG = Path3D, PROCEDURE = overback_poly3D_path3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* poly3D op poly3D */

CREATE OR REPLACE FUNCTION left_poly3D_poly3D(poly1 Polygon3D, poly2 Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_poly3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Polygon3D, RIGHTARG = Polygon3D, PROCEDURE = left_poly3D_poly3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_poly3D_poly3D(poly1 Polygon3D, poly2 Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_poly3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Polygon3D, RIGHTARG = Polygon3D, PROCEDURE = overleft_poly3D_poly3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_poly3D_poly3D(poly1 Polygon3D, poly2 Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_poly3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Polygon3D, RIGHTARG = Polygon3D, PROCEDURE = right_poly3D_poly3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_poly3D_poly3D(poly1 Polygon3D, poly2 Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_poly3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Polygon3D, RIGHTARG = Polygon3D, PROCEDURE = overright_poly3D_poly3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_poly3D_poly3D(poly1 Polygon3D, poly2 Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_poly3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Polygon3D, RIGHTARG = Polygon3D, PROCEDURE = below_poly3D_poly3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_poly3D_poly3D(poly1 Polygon3D, poly2 Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_poly3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Polygon3D, RIGHTARG = Polygon3D, PROCEDURE = overbelow_poly3D_poly3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_poly3D_poly3D(poly1 Polygon3D, poly2 Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_poly3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Polygon3D, RIGHTARG = Polygon3D, PROCEDURE = above_poly3D_poly3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_poly3D_poly3D(poly1 Polygon3D, poly2 Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_poly3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Polygon3D, RIGHTARG = Polygon3D, PROCEDURE = overabove_poly3D_poly3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_poly3D_poly3D(poly1 Polygon3D, poly2 Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_poly3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Polygon3D, RIGHTARG = Polygon3D, PROCEDURE = front_poly3D_poly3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_poly3D_poly3D(poly1 Polygon3D, poly2 Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_poly3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Polygon3D, RIGHTARG = Polygon3D, PROCEDURE = overfront_poly3D_poly3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_poly3D_poly3D(poly1 Polygon3D, poly2 Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_poly3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Polygon3D, RIGHTARG = Polygon3D, PROCEDURE = back_poly3D_poly3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_poly3D_poly3D(poly1 Polygon3D, poly2 Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_poly3D_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Polygon3D, RIGHTARG = Polygon3D, PROCEDURE = overback_poly3D_poly3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* poly3D op sphere */

CREATE OR REPLACE FUNCTION left_poly3D_sphere(poly Polygon3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'left_poly3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Polygon3D, RIGHTARG = Sphere, PROCEDURE = left_poly3D_sphere,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_poly3D_sphere(poly Polygon3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_poly3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Polygon3D, RIGHTARG = Sphere, PROCEDURE = overleft_poly3D_sphere,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_poly3D_sphere(poly Polygon3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'right_poly3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Polygon3D, RIGHTARG = Sphere, PROCEDURE = right_poly3D_sphere,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_poly3D_sphere(poly Polygon3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_poly3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Polygon3D, RIGHTARG = Sphere, PROCEDURE = overright_poly3D_sphere,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_poly3D_sphere(poly Polygon3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'below_poly3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Polygon3D, RIGHTARG = Sphere, PROCEDURE = below_poly3D_sphere,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_poly3D_sphere(poly Polygon3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_poly3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Polygon3D, RIGHTARG = Sphere, PROCEDURE = overbelow_poly3D_sphere,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_poly3D_sphere(poly Polygon3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'above_poly3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Polygon3D, RIGHTARG = Sphere, PROCEDURE = above_poly3D_sphere,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_poly3D_sphere(poly Polygon3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_poly3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Polygon3D, RIGHTARG = Sphere, PROCEDURE = overabove_poly3D_sphere,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_poly3D_sphere(poly Polygon3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'front_poly3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Polygon3D, RIGHTARG = Sphere, PROCEDURE = front_poly3D_sphere,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_poly3D_sphere(poly Polygon3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_poly3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Polygon3D, RIGHTARG = Sphere, PROCEDURE = overfront_poly3D_sphere,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_poly3D_sphere(poly Polygon3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'back_poly3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Polygon3D, RIGHTARG = Sphere, PROCEDURE = back_poly3D_sphere,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_poly3D_sphere(poly Polygon3D, sphere Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_poly3D_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Polygon3D, RIGHTARG = Sphere, PROCEDURE = overback_poly3D_sphere,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);
 
/*-------------------------------------------------------------------------*/

/* sphere op point3D */

CREATE OR REPLACE FUNCTION left_sphere_point3D(sphere Sphere, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_sphere_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Sphere, RIGHTARG = Point3D, PROCEDURE = left_sphere_point3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_sphere_point3D(sphere Sphere, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_sphere_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Sphere, RIGHTARG = Point3D, PROCEDURE = overleft_sphere_point3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_sphere_point3D(sphere Sphere, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_sphere_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Sphere, RIGHTARG = Point3D, PROCEDURE = right_sphere_point3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_sphere_point3D(sphere Sphere, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_sphere_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Sphere, RIGHTARG = Point3D, PROCEDURE = overright_sphere_point3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_sphere_point3D(sphere Sphere, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_sphere_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Sphere, RIGHTARG = Point3D, PROCEDURE = below_sphere_point3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_sphere_point3D(sphere Sphere, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_sphere_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Sphere, RIGHTARG = Point3D, PROCEDURE = overbelow_sphere_point3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_sphere_point3D(sphere Sphere, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_sphere_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Sphere, RIGHTARG = Point3D, PROCEDURE = above_sphere_point3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_sphere_point3D(sphere Sphere, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_sphere_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Sphere, RIGHTARG = Point3D, PROCEDURE = overabove_sphere_point3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_sphere_point3D(sphere Sphere, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_sphere_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Sphere, RIGHTARG = Point3D, PROCEDURE = front_sphere_point3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_sphere_point3D(sphere Sphere, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_sphere_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Sphere, RIGHTARG = Point3D, PROCEDURE = overfront_sphere_point3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_sphere_point3D(sphere Sphere, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_sphere_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Sphere, RIGHTARG = Point3D, PROCEDURE = back_sphere_point3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_sphere_point3D(sphere Sphere, point Point3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_sphere_point3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Sphere, RIGHTARG = Point3D, PROCEDURE = overback_sphere_point3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* sphere op lseg3D */

CREATE OR REPLACE FUNCTION left_sphere_lseg3D(sphere Sphere, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_sphere_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Sphere, RIGHTARG = Lseg3D, PROCEDURE = left_sphere_lseg3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_sphere_lseg3D(sphere Sphere, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_sphere_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Sphere, RIGHTARG = Lseg3D, PROCEDURE = overleft_sphere_lseg3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_sphere_lseg3D(sphere Sphere, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_sphere_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Sphere, RIGHTARG = Lseg3D, PROCEDURE = right_sphere_lseg3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_sphere_lseg3D(sphere Sphere, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_sphere_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Sphere, RIGHTARG = Lseg3D, PROCEDURE = overright_sphere_lseg3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_sphere_lseg3D(sphere Sphere, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_sphere_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Sphere, RIGHTARG = Lseg3D, PROCEDURE = below_sphere_lseg3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_sphere_lseg3D(sphere Sphere, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_sphere_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Sphere, RIGHTARG = Lseg3D, PROCEDURE = overbelow_sphere_lseg3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_sphere_lseg3D(sphere Sphere, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_sphere_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Sphere, RIGHTARG = Lseg3D, PROCEDURE = above_sphere_lseg3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_sphere_lseg3D(sphere Sphere, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_sphere_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Sphere, RIGHTARG = Lseg3D, PROCEDURE = overabove_sphere_lseg3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_sphere_lseg3D(sphere Sphere, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_sphere_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Sphere, RIGHTARG = Lseg3D, PROCEDURE = front_sphere_lseg3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_sphere_lseg3D(sphere Sphere, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_sphere_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Sphere, RIGHTARG = Lseg3D, PROCEDURE = overfront_sphere_lseg3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_sphere_lseg3D(sphere Sphere, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_sphere_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Sphere, RIGHTARG = Lseg3D, PROCEDURE = back_sphere_lseg3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_sphere_lseg3D(sphere Sphere, lseg Lseg3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_sphere_lseg3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Sphere, RIGHTARG = Lseg3D, PROCEDURE = overback_sphere_lseg3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* sphere op line3D */

CREATE OR REPLACE FUNCTION left_sphere_line3D(sphere Sphere, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_sphere_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Sphere, RIGHTARG = Line3D, PROCEDURE = left_sphere_line3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_sphere_line3D(sphere Sphere, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_sphere_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Sphere, RIGHTARG = Line3D, PROCEDURE = overleft_sphere_line3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_sphere_line3D(sphere Sphere, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_sphere_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Sphere, RIGHTARG = Line3D, PROCEDURE = right_sphere_line3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_sphere_line3D(sphere Sphere, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_sphere_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Sphere, RIGHTARG = Line3D, PROCEDURE = overright_sphere_line3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_sphere_line3D(sphere Sphere, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_sphere_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Sphere, RIGHTARG = Line3D, PROCEDURE = below_sphere_line3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_sphere_line3D(sphere Sphere, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_sphere_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Sphere, RIGHTARG = Line3D, PROCEDURE = overbelow_sphere_line3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_sphere_line3D(sphere Sphere, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_sphere_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Sphere, RIGHTARG = Line3D, PROCEDURE = above_sphere_line3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_sphere_line3D(sphere Sphere, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_sphere_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Sphere, RIGHTARG = Line3D, PROCEDURE = overabove_sphere_line3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_sphere_line3D(sphere Sphere, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_sphere_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Sphere, RIGHTARG = Line3D, PROCEDURE = front_sphere_line3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_sphere_line3D(sphere Sphere, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_sphere_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Sphere, RIGHTARG = Line3D, PROCEDURE = overfront_sphere_line3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_sphere_line3D(sphere Sphere, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_sphere_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Sphere, RIGHTARG = Line3D, PROCEDURE = back_sphere_line3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_sphere_line3D(sphere Sphere, line Line3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_sphere_line3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Sphere, RIGHTARG = Line3D, PROCEDURE = overback_sphere_line3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* sphere op box3D */

CREATE OR REPLACE FUNCTION left_sphere_box3D(sphere Sphere, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_sphere_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Sphere, RIGHTARG = Box3D, PROCEDURE = left_sphere_box3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_sphere_box3D(sphere Sphere, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_sphere_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Sphere, RIGHTARG = Box3D, PROCEDURE = overleft_sphere_box3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_sphere_box3D(sphere Sphere, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_sphere_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Sphere, RIGHTARG = Box3D, PROCEDURE = right_sphere_box3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_sphere_box3D(sphere Sphere, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_sphere_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Sphere, RIGHTARG = Box3D, PROCEDURE = overright_sphere_box3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_sphere_box3D(sphere Sphere, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_sphere_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Sphere, RIGHTARG = Box3D, PROCEDURE = below_sphere_box3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_sphere_box3D(sphere Sphere, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_sphere_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Sphere, RIGHTARG = Box3D, PROCEDURE = overbelow_sphere_box3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_sphere_box3D(sphere Sphere, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_sphere_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Sphere, RIGHTARG = Box3D, PROCEDURE = above_sphere_box3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_sphere_box3D(sphere Sphere, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_sphere_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Sphere, RIGHTARG = Box3D, PROCEDURE = overabove_sphere_box3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_sphere_box3D(sphere Sphere, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_sphere_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Sphere, RIGHTARG = Box3D, PROCEDURE = front_sphere_box3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_sphere_box3D(sphere Sphere, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_sphere_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Sphere, RIGHTARG = Box3D, PROCEDURE = overfront_sphere_box3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_sphere_box3D(sphere Sphere, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_sphere_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Sphere, RIGHTARG = Box3D, PROCEDURE = back_sphere_box3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_sphere_box3D(sphere Sphere, box Box3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_sphere_box3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Sphere, RIGHTARG = Box3D, PROCEDURE = overback_sphere_box3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* sphere op path3D */

CREATE OR REPLACE FUNCTION left_sphere_path3D(sphere Sphere, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_sphere_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Sphere, RIGHTARG = Path3D, PROCEDURE = left_sphere_path3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_sphere_path3D(sphere Sphere, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_sphere_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Sphere, RIGHTARG = Path3D, PROCEDURE = overleft_sphere_path3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_sphere_path3D(sphere Sphere, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_sphere_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Sphere, RIGHTARG = Path3D, PROCEDURE = right_sphere_path3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_sphere_path3D(sphere Sphere, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_sphere_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Sphere, RIGHTARG = Path3D, PROCEDURE = overright_sphere_path3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_sphere_path3D(sphere Sphere, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_sphere_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Sphere, RIGHTARG = Path3D, PROCEDURE = below_sphere_path3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_sphere_path3D(sphere Sphere, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_sphere_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Sphere, RIGHTARG = Path3D, PROCEDURE = overbelow_sphere_path3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_sphere_path3D(sphere Sphere, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_sphere_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Sphere, RIGHTARG = Path3D, PROCEDURE = above_sphere_path3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_sphere_path3D(sphere Sphere, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_sphere_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Sphere, RIGHTARG = Path3D, PROCEDURE = overabove_sphere_path3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_sphere_path3D(sphere Sphere, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_sphere_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Sphere, RIGHTARG = Path3D, PROCEDURE = front_sphere_path3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_sphere_path3D(sphere Sphere, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_sphere_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Sphere, RIGHTARG = Path3D, PROCEDURE = overfront_sphere_path3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_sphere_path3D(sphere Sphere, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_sphere_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Sphere, RIGHTARG = Path3D, PROCEDURE = back_sphere_path3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_sphere_path3D(sphere Sphere, path Path3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_sphere_path3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Sphere, RIGHTARG = Path3D, PROCEDURE = overback_sphere_path3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* sphere op poly3D */

CREATE OR REPLACE FUNCTION left_sphere_poly3D(sphere Sphere, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'left_sphere_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Sphere, RIGHTARG = Polygon3D, PROCEDURE = left_sphere_poly3D,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_sphere_poly3D(sphere Sphere, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_sphere_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Sphere, RIGHTARG = Polygon3D, PROCEDURE = overleft_sphere_poly3D,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_sphere_poly3D(sphere Sphere, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'right_sphere_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Sphere, RIGHTARG = Polygon3D, PROCEDURE = right_sphere_poly3D,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_sphere_poly3D(sphere Sphere, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_sphere_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Sphere, RIGHTARG = Polygon3D, PROCEDURE = overright_sphere_poly3D,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_sphere_poly3D(sphere Sphere, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'below_sphere_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Sphere, RIGHTARG = Polygon3D, PROCEDURE = below_sphere_poly3D,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_sphere_poly3D(sphere Sphere, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_sphere_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Sphere, RIGHTARG = Polygon3D, PROCEDURE = overbelow_sphere_poly3D,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_sphere_poly3D(sphere Sphere, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'above_sphere_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Sphere, RIGHTARG = Polygon3D, PROCEDURE = above_sphere_poly3D,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_sphere_poly3D(sphere Sphere, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_sphere_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Sphere, RIGHTARG = Polygon3D, PROCEDURE = overabove_sphere_poly3D,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_sphere_poly3D(sphere Sphere, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'front_sphere_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Sphere, RIGHTARG = Polygon3D, PROCEDURE = front_sphere_poly3D,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_sphere_poly3D(sphere Sphere, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_sphere_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Sphere, RIGHTARG = Polygon3D, PROCEDURE = overfront_sphere_poly3D,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_sphere_poly3D(sphere Sphere, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'back_sphere_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Sphere, RIGHTARG = Polygon3D, PROCEDURE = back_sphere_poly3D,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_sphere_poly3D(sphere Sphere, poly Polygon3D) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_sphere_poly3D' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Sphere, RIGHTARG = Polygon3D, PROCEDURE = overback_sphere_poly3D,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/* sphere op sphere */

CREATE OR REPLACE FUNCTION left_sphere_sphere(sphere1 Sphere, sphere2 Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'left_sphere_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR << (
	LEFTARG = Sphere, RIGHTARG = Sphere, PROCEDURE = left_sphere_sphere,
	COMMUTATOR = '>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overleft_sphere_sphere(sphere1 Sphere, sphere2 Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overleft_sphere_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<& (
	LEFTARG = Sphere, RIGHTARG = Sphere, PROCEDURE = overleft_sphere_sphere,
	COMMUTATOR = '&>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION right_sphere_sphere(sphere1 Sphere, sphere2 Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'right_sphere_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR >> (
	LEFTARG = Sphere, RIGHTARG = Sphere, PROCEDURE = right_sphere_sphere,
	COMMUTATOR = '<<',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overright_sphere_sphere(sphere1 Sphere, sphere2 Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overright_sphere_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &>> (
	LEFTARG = Sphere, RIGHTARG = Sphere, PROCEDURE = overright_sphere_sphere,
	COMMUTATOR = '<<&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION below_sphere_sphere(sphere1 Sphere, sphere2 Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'below_sphere_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<| (
	LEFTARG = Sphere, RIGHTARG = Sphere, PROCEDURE = below_sphere_sphere,
	COMMUTATOR = '|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overbelow_sphere_sphere(sphere1 Sphere, sphere2 Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overbelow_sphere_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <<|& (
	LEFTARG = Sphere, RIGHTARG = Sphere, PROCEDURE = overbelow_sphere_sphere,
	COMMUTATOR = '&|>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION above_sphere_sphere(sphere1 Sphere, sphere2 Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'above_sphere_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR |>> (
	LEFTARG = Sphere, RIGHTARG = Sphere, PROCEDURE = above_sphere_sphere,
	COMMUTATOR = '<<|',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overabove_sphere_sphere(sphere1 Sphere, sphere2 Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overabove_sphere_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &|>> (
	LEFTARG = Sphere, RIGHTARG = Sphere, PROCEDURE = overabove_sphere_sphere,
	COMMUTATOR = '<<|&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION front_sphere_sphere(sphere1 Sphere, sphere2 Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'front_sphere_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</ (
	LEFTARG = Sphere, RIGHTARG = Sphere, PROCEDURE = front_sphere_sphere,
	COMMUTATOR = '/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overfront_sphere_sphere(sphere1 Sphere, sphere2 Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overfront_sphere_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR <</& (
	LEFTARG = Sphere, RIGHTARG = Sphere, PROCEDURE = overfront_sphere_sphere,
	COMMUTATOR = '&/>>',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION back_sphere_sphere(sphere1 Sphere, sphere2 Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'back_sphere_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR />> (
	LEFTARG = Sphere, RIGHTARG = Sphere, PROCEDURE = back_sphere_sphere,
	COMMUTATOR = '<</',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

CREATE OR REPLACE FUNCTION overback_sphere_sphere(sphere1 Sphere, sphere2 Sphere) RETURNS boolean AS 'MODULE_PATHNAME', 'overback_sphere_sphere' LANGUAGE C IMMUTABLE STRICT;
CREATE OPERATOR &/>> (
	LEFTARG = Sphere, RIGHTARG = Sphere, PROCEDURE = overback_sphere_sphere,
	COMMUTATOR = '<</&',
	RESTRICT = positionsel3D, JOIN = positionjoinsel3D
);

/*-------------------------------------------------------------------------*/
