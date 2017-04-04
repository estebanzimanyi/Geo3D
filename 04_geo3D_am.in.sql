/*-------------------------------------------------------------------------
 *
 * geo3D_am.sql
 *	  Access methods for 3D geometries: R-tree and oct-tree
 *
 * Copyright (c) 2017, Esteban Zimanyi, Université Libre de Bruxelles
 *
 *-------------------------------------------------------------------------*/

 /******************************************************************************
 * Support routines for the GiST access method 
 *****************************************************************************/

CREATE OR REPLACE FUNCTION gist_box3D_consistent(internal, Box3D, smallint, oid, internal) RETURNS bool AS 'MODULE_PATHNAME', 'gist_box3D_consistent' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION gist_box3D_union(internal, internal) RETURNS box3D AS 'MODULE_PATHNAME', 'gist_box3D_union' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION gist_box3D_compress(internal) RETURNS internal AS 'MODULE_PATHNAME', 'gist_box3D_compress' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION gist_box3D_decompress(internal) RETURNS internal AS 'MODULE_PATHNAME', 'gist_box3D_decompress' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION gist_box3D_penalty(internal, internal, internal) RETURNS internal AS 'MODULE_PATHNAME', 'gist_box3D_penalty' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION gist_box3D_picksplit(internal, internal) RETURNS internal AS 'MODULE_PATHNAME', 'gist_box3D_picksplit' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION gist_box3D_same(Box3D, Box3D, internal) RETURNS internal AS 'MODULE_PATHNAME', 'gist_box3D_same' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION gist_box3D_distance(internal, Box3D, smallint, oid, internal) RETURNS float8 AS 'MODULE_PATHNAME', 'gist_point3D_distance' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION gist_box3D_fetch(internal) RETURNS internal AS 'MODULE_PATHNAME', 'gist_box3D_fetch' LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR CLASS gist_box3D_ops
    DEFAULT FOR TYPE Box3D USING gist AS
	-- strictly left
	OPERATOR	1	<< (Box3D, Point3D),  
	OPERATOR	41	<< (Box3D, Lseg3D),  
	OPERATOR	81	<< (Box3D, Line3D),  
	OPERATOR	121	<< (Box3D, Box3D),  
	OPERATOR	161	<< (Box3D, Path3D),  
	OPERATOR	201	<< (Box3D, Polygon3D),  
	OPERATOR	241	<< (Box3D, Sphere), 
	-- overlaps or left
	OPERATOR	2	<<& (Box3D, Point3D),  
	OPERATOR	42	<<& (Box3D, Lseg3D),  
	OPERATOR	82	<<& (Box3D, Line3D),  
	OPERATOR	122	<<& (Box3D, Box3D),  
	OPERATOR	162	<<& (Box3D, Path3D),  
	OPERATOR	202	<<& (Box3D, Polygon3D),  
	OPERATOR	242	<<& (Box3D, Sphere), 
	-- overlaps
--	OPERATOR	3	&& (Box3D, Point3D),  
--	OPERATOR	43	&& (Box3D, Lseg3D),  
--	OPERATOR	83	&& (Box3D, Line3D),  
	OPERATOR	123	&& (Box3D, Box3D),  
--	OPERATOR	163	&& (Box3D, Path3D),  
	OPERATOR	203	&& (Box3D, Polygon3D),  
	OPERATOR	243	&& (Box3D, Sphere), 
	-- overlaps or right
	OPERATOR	4	&>> (Box3D, Point3D),  
	OPERATOR	44	&>> (Box3D, Lseg3D),  
	OPERATOR	84	&>> (Box3D, Line3D),  
	OPERATOR	124	&>> (Box3D, Box3D),  
	OPERATOR	164	&>> (Box3D, Path3D),  
	OPERATOR	204	&>> (Box3D, Polygon3D),  
	OPERATOR	244	&>> (Box3D, Sphere), 
	-- strictly right
	OPERATOR	5	>> (Box3D, Point3D),  
	OPERATOR	45	>> (Box3D, Lseg3D),  
	OPERATOR	85	>> (Box3D, Line3D),  
	OPERATOR	125	>> (Box3D, Box3D),  
	OPERATOR	165	>> (Box3D, Path3D),  
	OPERATOR	205	>> (Box3D, Polygon3D),  
	OPERATOR	245	>> (Box3D, Sphere), 
	-- same
	OPERATOR	126	~= (Box3D, Box3D),  
	-- contains
	OPERATOR	7	@> (Box3D, Point3D),  
	OPERATOR	47	@> (Box3D, Lseg3D),  
	OPERATOR	127	@> (Box3D, Box3D),  
	OPERATOR	167	@> (Box3D, Path3D),  
	OPERATOR	207	@> (Box3D, Polygon3D),  
	OPERATOR	247	@> (Box3D, Sphere), 
	-- contained by
	OPERATOR	128	<@ (Box3D, Box3D),  
	OPERATOR	248	<@ (Box3D, Sphere), 
	-- overlaps or below
	OPERATOR	9	<<|& (Box3D, Point3D),  
	OPERATOR	49	<<|& (Box3D, Lseg3D),  
	OPERATOR	89	<<|& (Box3D, Line3D),  
	OPERATOR	129	<<|& (Box3D, Box3D),  
	OPERATOR	169	<<|& (Box3D, Path3D),  
	OPERATOR	209	<<|& (Box3D, Polygon3D),  
	OPERATOR	249	<<|& (Box3D, Sphere), 
	-- strictly below
	OPERATOR	10	<<| (Box3D, Point3D),  
	OPERATOR	50	<<| (Box3D, Lseg3D),  
	OPERATOR	90	<<| (Box3D, Line3D),  
	OPERATOR	130	<<| (Box3D, Box3D),  
	OPERATOR	170	<<| (Box3D, Path3D),  
	OPERATOR	210	<<| (Box3D, Polygon3D),  
	OPERATOR	250	<<| (Box3D, Sphere), 
	-- strictly above
	OPERATOR	11	|>> (Box3D, Point3D),  
	OPERATOR	51	|>> (Box3D, Lseg3D),  
	OPERATOR	91	|>> (Box3D, Line3D),  
	OPERATOR	131	|>> (Box3D, Box3D),  
	OPERATOR	171	|>> (Box3D, Path3D),  
	OPERATOR	211	|>> (Box3D, Polygon3D),  
	OPERATOR	251	|>> (Box3D, Sphere), 
	-- overlaps or above
	OPERATOR	12	&|>> (Box3D, Point3D),  
	OPERATOR	52	&|>> (Box3D, Lseg3D),  
	OPERATOR	92	&|>> (Box3D, Line3D),  
	OPERATOR	132	&|>> (Box3D, Box3D),  
	OPERATOR	172	&|>> (Box3D, Path3D),  
	OPERATOR	212	&|>> (Box3D, Polygon3D),  
	OPERATOR	252	&|>> (Box3D, Sphere), 
	-- distance
	OPERATOR	15	<-> (Box3D, Point3D) FOR ORDER BY pg_catalog.float_ops,
	OPERATOR	55	<-> (Box3D, Lseg3D) FOR ORDER BY pg_catalog.float_ops,
	OPERATOR	95	<-> (Box3D, Line3D) FOR ORDER BY pg_catalog.float_ops,
	OPERATOR	135	<-> (Box3D, Box3D) FOR ORDER BY pg_catalog.float_ops,
	OPERATOR	175	<-> (Box3D, Path3D) FOR ORDER BY pg_catalog.float_ops,
	OPERATOR	215	<-> (Box3D, Polygon3D) FOR ORDER BY pg_catalog.float_ops,
	OPERATOR	255	<-> (Box3D, Sphere) FOR ORDER BY pg_catalog.float_ops,
	-- overlaps or front
	OPERATOR	28	<</& (Box3D, Point3D),
	OPERATOR	68	<</& (Box3D, Lseg3D),
	OPERATOR	108	<</& (Box3D, Line3D),
	OPERATOR	148	<</& (Box3D, Box3D),
	OPERATOR	188	<</& (Box3D, Path3D),
	OPERATOR	228	<</& (Box3D, Polygon3D),
	OPERATOR	268	<</& (Box3D, Sphere),
	-- strictly front
	OPERATOR	29	<</ (Box3D, Point3D),
	OPERATOR	69	<</ (Box3D, Lseg3D),
	OPERATOR	109	<</ (Box3D, Line3D),
	OPERATOR	149	<</ (Box3D, Box3D),
	OPERATOR	189	<</ (Box3D, Path3D),
	OPERATOR	229	<</ (Box3D, Polygon3D),
	OPERATOR	269	<</ (Box3D, Sphere),
	-- strictly back
	OPERATOR	30	/>> (Box3D, Point3D),
	OPERATOR	70	/>> (Box3D, Lseg3D),
	OPERATOR	110	/>> (Box3D, Line3D),
	OPERATOR	150	/>> (Box3D, Box3D),
	OPERATOR	190	/>> (Box3D, Path3D),
	OPERATOR	230	/>> (Box3D, Polygon3D),
	OPERATOR	270	/>> (Box3D, Sphere),
	-- overlaps or back
	OPERATOR	31	&/>> (Box3D, Point3D),
	OPERATOR	71	&/>> (Box3D, Lseg3D),
	OPERATOR	111	&/>> (Box3D, Line3D),
	OPERATOR	151	&/>> (Box3D, Box3D),
	OPERATOR	191	&/>> (Box3D, Path3D),
	OPERATOR	231	&/>> (Box3D, Polygon3D),
	OPERATOR	271	&/>> (Box3D, Sphere),
	-- functions
	FUNCTION	1	gist_box3D_consistent (internal, Box3D, smallint, oid, internal),
	FUNCTION	2	gist_box3D_union (internal, internal),
	FUNCTION	3	gist_box3D_compress (internal),
	FUNCTION	4	gist_box3D_decompress (internal),
	FUNCTION	5	gist_box3D_penalty (internal, internal, internal),
	FUNCTION	6	gist_box3D_picksplit (internal, internal),
	FUNCTION	7	gist_box3D_same (Box3D, Box3D, internal),
	FUNCTION	8	gist_box3D_distance (internal, Box3D, smallint, oid, internal),
	FUNCTION	9	gist_box3D_fetch (internal);
	
CREATE OR REPLACE FUNCTION gist_point3D_consistent(internal, Point3D, smallint, oid, internal) RETURNS bool AS 'MODULE_PATHNAME', 'gist_point3D_consistent' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION gist_point3D_compress(internal) RETURNS internal AS 'MODULE_PATHNAME', 'gist_point3D_compress' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION gist_point3D_distance(internal, Point3D, smallint, oid, internal) RETURNS float8 AS 'MODULE_PATHNAME', 'gist_point3D_distance' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION gist_point3D_fetch(internal) RETURNS internal AS 'MODULE_PATHNAME', 'gist_point3D_fetch' LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR CLASS gist_point3D_ops
    DEFAULT FOR TYPE point3D USING gist AS
	-- strictly left
	OPERATOR	1	<< (Point3D, Point3D),  
	OPERATOR	41	<< (Point3D, Lseg3D),  
	OPERATOR	81	<< (Point3D, Line3D),  
	OPERATOR	121	<< (Point3D, Box3D),  
	OPERATOR	161	<< (Point3D, Path3D),  
	OPERATOR	201	<< (Point3D, Polygon3D),  
	OPERATOR	241	<< (Point3D, Sphere), 
	-- overlaps or left
	OPERATOR	2	<<& (Point3D, Point3D),  
	OPERATOR	42	<<& (Point3D, Lseg3D),  
	OPERATOR	82	<<& (Point3D, Line3D),  
	OPERATOR	122	<<& (Point3D, Box3D),  
	OPERATOR	162	<<& (Point3D, Path3D),  
	OPERATOR	202	<<& (Point3D, Polygon3D),  
	OPERATOR	242	<<& (Point3D, Sphere), 
	-- overlaps
	-- overlaps or right
	OPERATOR	4	&>> (Point3D, Point3D),  
	OPERATOR	44	&>> (Point3D, Lseg3D),  
	OPERATOR	84	&>> (Point3D, Line3D),  
	OPERATOR	124	&>> (Point3D, Box3D),  
	OPERATOR	164	&>> (Point3D, Path3D),  
	OPERATOR	204	&>> (Point3D, Polygon3D),  
	OPERATOR	244	&>> (Point3D, Sphere), 
	-- strictly right
	OPERATOR	5	>> (Point3D, Point3D),  
	OPERATOR	45	>> (Point3D, Lseg3D),  
	OPERATOR	85	>> (Point3D, Line3D),  
	OPERATOR	125	>> (Point3D, Box3D),  
	OPERATOR	165	>> (Point3D, Path3D),  
	OPERATOR	205	>> (Point3D, Polygon3D),  
	OPERATOR	245	>> (Point3D, Sphere), 
	-- same
	OPERATOR	6	~= (Point3D, Point3D),  
	-- contains
	-- contained by
	OPERATOR	48	<@ (Point3D, Lseg3D),  
	OPERATOR	88	<@ (Point3D, Line3D),  
	OPERATOR	128	<@ (Point3D, Box3D),  
	OPERATOR	168	<@ (Point3D, Path3D),  
	OPERATOR	208	<@ (Point3D, Polygon3D),  
	OPERATOR	248	<@ (Point3D, Sphere), 
	-- overlaps or below
	OPERATOR	9	<<|& (Point3D, Point3D),  
	OPERATOR	49	<<|& (Point3D, Lseg3D),  
	OPERATOR	89	<<|& (Point3D, Line3D),  
	OPERATOR	129	<<|& (Point3D, Box3D),  
	OPERATOR	169	<<|& (Point3D, Path3D),  
	OPERATOR	209	<<|& (Point3D, Polygon3D),  
	OPERATOR	249	<<|& (Point3D, Sphere), 
	-- strictly below
	OPERATOR	10	<<| (Point3D, Point3D),  
	OPERATOR	50	<<| (Point3D, Lseg3D),  
	OPERATOR	90	<<| (Point3D, Line3D),  
	OPERATOR	130	<<| (Point3D, Box3D),  
	OPERATOR	170	<<| (Point3D, Path3D),  
	OPERATOR	210	<<| (Point3D, Polygon3D),  
	OPERATOR	250	<<| (Point3D, Sphere), 
	-- strictly above
	OPERATOR	11	|>> (Point3D, Point3D),  
	OPERATOR	51	|>> (Point3D, Lseg3D),  
	OPERATOR	91	|>> (Point3D, Line3D),  
	OPERATOR	131	|>> (Point3D, Box3D),  
	OPERATOR	171	|>> (Point3D, Path3D),  
	OPERATOR	211	|>> (Point3D, Polygon3D),  
	OPERATOR	251	|>> (Point3D, Sphere), 
	-- overlaps or above
	OPERATOR	12	&|>> (Point3D, Point3D),  
	OPERATOR	52	&|>> (Point3D, Lseg3D),  
	OPERATOR	92	&|>> (Point3D, Line3D),  
	OPERATOR	132	&|>> (Point3D, Box3D),  
	OPERATOR	172	&|>> (Point3D, Path3D),  
	OPERATOR	212	&|>> (Point3D, Polygon3D),  
	OPERATOR	252	&|>> (Point3D, Sphere), 
	-- distance
	OPERATOR	15	<-> (Point3D, Point3D) FOR ORDER BY pg_catalog.float_ops,
	OPERATOR	55	<-> (Point3D, Lseg3D) FOR ORDER BY pg_catalog.float_ops,
	OPERATOR	95	<-> (Point3D, Line3D) FOR ORDER BY pg_catalog.float_ops,
	OPERATOR	135	<-> (Point3D, Box3D) FOR ORDER BY pg_catalog.float_ops,
	OPERATOR	175	<-> (Point3D, Path3D) FOR ORDER BY pg_catalog.float_ops,
	OPERATOR	215	<-> (Point3D, Polygon3D) FOR ORDER BY pg_catalog.float_ops,
	OPERATOR	255	<-> (Point3D, Sphere) FOR ORDER BY pg_catalog.float_ops,
	-- overlaps or front
	OPERATOR	28	<</& (Point3D, Point3D),
	OPERATOR	68	<</& (Point3D, Lseg3D),
	OPERATOR	108	<</& (Point3D, Line3D),
	OPERATOR	148	<</& (Point3D, Box3D),
	OPERATOR	188	<</& (Point3D, Path3D),
	OPERATOR	228	<</& (Point3D, Polygon3D),
	OPERATOR	268	<</& (Point3D, Sphere),
	-- strictly front
	OPERATOR	29	<</ (Point3D, Point3D),
	OPERATOR	69	<</ (Point3D, Lseg3D),
	OPERATOR	109	<</ (Point3D, Line3D),
	OPERATOR	149	<</ (Point3D, Box3D),
	OPERATOR	189	<</ (Point3D, Path3D),
	OPERATOR	229	<</ (Point3D, Polygon3D),
	OPERATOR	269	<</ (Point3D, Sphere),
	-- strictly back
	OPERATOR	30	/>> (Point3D, Point3D),
	OPERATOR	70	/>> (Point3D, Lseg3D),
	OPERATOR	110	/>> (Point3D, Line3D),
	OPERATOR	150	/>> (Point3D, Box3D),
	OPERATOR	190	/>> (Point3D, Path3D),
	OPERATOR	230	/>> (Point3D, Polygon3D),
	OPERATOR	270	/>> (Point3D, Sphere),
	-- overlaps or back
	OPERATOR	31	&/>> (Point3D, Point3D),
	OPERATOR	71	&/>> (Point3D, Lseg3D),
	OPERATOR	111	&/>> (Point3D, Line3D),
	OPERATOR	151	&/>> (Point3D, Box3D),
	OPERATOR	191	&/>> (Point3D, Path3D),
	OPERATOR	231	&/>> (Point3D, Polygon3D),
	OPERATOR	271	&/>> (Point3D, Sphere),
	-- functions
	FUNCTION	1	gist_point3D_consistent (internal, Point3D, smallint, oid, internal),
	FUNCTION	2	gist_box3D_union (internal, internal),
	FUNCTION	3	gist_point3D_compress (internal),
	FUNCTION	4	gist_box3D_decompress (internal),
	FUNCTION	5	gist_box3D_penalty (internal, internal, internal),
	FUNCTION	6	gist_box3D_picksplit (internal, internal),
	FUNCTION	7	gist_box3D_same (Box3D, Box3D, internal),
	FUNCTION	8	gist_point3D_distance (internal, Point3D, smallint, oid, internal),
	FUNCTION	9	gist_point3D_fetch (internal);

CREATE OR REPLACE FUNCTION gist_sphere_consistent(internal, Sphere, smallint, oid, internal) RETURNS bool AS 'MODULE_PATHNAME', 'gist_type3D_consistent' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION gist_sphere_compress(internal) RETURNS internal AS 'MODULE_PATHNAME', 'gist_sphere_compress' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION gist_sphere_distance(internal, Sphere, smallint, oid, internal) RETURNS float8 AS 'MODULE_PATHNAME', 'gist_sphere_distance' LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR CLASS gist_sphere_ops
    DEFAULT FOR TYPE sphere USING gist AS
	-- strictly left
	OPERATOR	1	<< (Sphere, Point3D),  
	OPERATOR	41	<< (Sphere, Lseg3D),  
	OPERATOR	81	<< (Sphere, Line3D),  
	OPERATOR	121	<< (Sphere, Box3D),  
	OPERATOR	161	<< (Sphere, Path3D),  
	OPERATOR	201	<< (Sphere, Polygon3D),  
	OPERATOR	241	<< (Sphere, Sphere), 
	-- overlaps or left
	OPERATOR	2	<<& (Sphere, Point3D),  
	OPERATOR	42	<<& (Sphere, Lseg3D),  
	OPERATOR	82	<<& (Sphere, Line3D),  
	OPERATOR	122	<<& (Sphere, Box3D),  
	OPERATOR	162	<<& (Sphere, Path3D),  
	OPERATOR	202	<<& (Sphere, Polygon3D),  
	OPERATOR	242	<<& (Sphere, Sphere), 
	-- overlaps
--	OPERATOR	43	&& (Sphere, Lseg3D),  
--	OPERATOR	83	&& (Sphere, Line3D),  
	OPERATOR	123	&& (Sphere, Box3D),  
--	OPERATOR	163	&& (Sphere, Path3D),  
--	OPERATOR	203	&& (Sphere, Polygon3D),  
	OPERATOR	243	&& (Sphere, Sphere), 
	-- overlaps or right
	OPERATOR	4	&>> (Sphere, Point3D),  
	OPERATOR	44	&>> (Sphere, Lseg3D),  
	OPERATOR	84	&>> (Sphere, Line3D),  
	OPERATOR	124	&>> (Sphere, Box3D),  
	OPERATOR	164	&>> (Sphere, Path3D),  
	OPERATOR	204	&>> (Sphere, Polygon3D),  
	OPERATOR	244	&>> (Sphere, Sphere), 
	-- strictly right
	OPERATOR	5	>> (Sphere, Point3D),  
	OPERATOR	45	>> (Sphere, Lseg3D),  
	OPERATOR	85	>> (Sphere, Line3D),  
	OPERATOR	125	>> (Sphere, Box3D),  
	OPERATOR	165	>> (Sphere, Path3D),  
	OPERATOR	205	>> (Sphere, Polygon3D),  
	OPERATOR	245	>> (Sphere, Sphere), 
	-- same
	OPERATOR	246	~= (Sphere, Sphere),  
	-- contains
	OPERATOR	7	@> (Sphere, Point3D),  
	OPERATOR	47	@> (Sphere, Lseg3D),  
	OPERATOR	127	@> (Sphere, Box3D),  
	OPERATOR	167	@> (Sphere, Path3D),  
	OPERATOR	207	@> (Sphere, Polygon3D),  
	OPERATOR	247	@> (Sphere, Sphere), 
	-- contained by
	OPERATOR	128	<@ (Sphere, Box3D),  
	OPERATOR	248	<@ (Sphere, Sphere), 
	-- overlaps or below
	OPERATOR	9	<<|& (Sphere, Point3D),  
	OPERATOR	49	<<|& (Sphere, Lseg3D),  
	OPERATOR	89	<<|& (Sphere, Line3D),  
	OPERATOR	129	<<|& (Sphere, Box3D),  
	OPERATOR	169	<<|& (Sphere, Path3D),  
	OPERATOR	209	<<|& (Sphere, Polygon3D),  
	OPERATOR	249	<<|& (Sphere, Sphere), 
	-- strictly below
	OPERATOR	10	<<| (Sphere, Point3D),  
	OPERATOR	50	<<| (Sphere, Lseg3D),  
	OPERATOR	90	<<| (Sphere, Line3D),  
	OPERATOR	130	<<| (Sphere, Box3D),  
	OPERATOR	170	<<| (Sphere, Path3D),  
	OPERATOR	210	<<| (Sphere, Polygon3D),  
	OPERATOR	250	<<| (Sphere, Sphere), 
	-- strictly above
	OPERATOR	11	|>> (Sphere, Point3D),  
	OPERATOR	51	|>> (Sphere, Lseg3D),  
	OPERATOR	91	|>> (Sphere, Line3D),  
	OPERATOR	131	|>> (Sphere, Box3D),  
	OPERATOR	171	|>> (Sphere, Path3D),  
	OPERATOR	211	|>> (Sphere, Polygon3D),  
	OPERATOR	251	|>> (Sphere, Sphere), 
	-- overlaps or above
	OPERATOR	12	&|>> (Sphere, Point3D),  
	OPERATOR	52	&|>> (Sphere, Lseg3D),  
	OPERATOR	92	&|>> (Sphere, Line3D),  
	OPERATOR	132	&|>> (Sphere, Box3D),  
	OPERATOR	172	&|>> (Sphere, Path3D),  
	OPERATOR	212	&|>> (Sphere, Polygon3D),  
	OPERATOR	252	&|>> (Sphere, Sphere), 
	-- distance
	OPERATOR	15	<-> (Sphere, Point3D) FOR ORDER BY pg_catalog.float_ops,
	OPERATOR	55	<-> (Sphere, Lseg3D) FOR ORDER BY pg_catalog.float_ops,
	OPERATOR	95	<-> (Sphere, Line3D) FOR ORDER BY pg_catalog.float_ops,
	OPERATOR	135	<-> (Sphere, Box3D) FOR ORDER BY pg_catalog.float_ops,
	OPERATOR	175	<-> (Sphere, Path3D) FOR ORDER BY pg_catalog.float_ops,
	OPERATOR	215	<-> (Sphere, Polygon3D) FOR ORDER BY pg_catalog.float_ops,
	OPERATOR	255	<-> (Sphere, Sphere) FOR ORDER BY pg_catalog.float_ops,
	-- overlaps or front
	OPERATOR	28	<</& (Sphere, Point3D),
	OPERATOR	68	<</& (Sphere, Lseg3D),
	OPERATOR	108	<</& (Sphere, Line3D),
	OPERATOR	148	<</& (Sphere, Box3D),
	OPERATOR	188	<</& (Sphere, Path3D),
	OPERATOR	228	<</& (Sphere, Polygon3D),
	OPERATOR	268	<</& (Sphere, Sphere),
	-- strictly front
	OPERATOR	29	<</ (Sphere, Point3D),
	OPERATOR	69	<</ (Sphere, Lseg3D),
	OPERATOR	109	<</ (Sphere, Line3D),
	OPERATOR	149	<</ (Sphere, Box3D),
	OPERATOR	189	<</ (Sphere, Path3D),
	OPERATOR	229	<</ (Sphere, Polygon3D),
	OPERATOR	269	<</ (Sphere, Sphere),
	-- strictly back
	OPERATOR	30	/>> (Sphere, Point3D),
	OPERATOR	70	/>> (Sphere, Lseg3D),
	OPERATOR	110	/>> (Sphere, Line3D),
	OPERATOR	150	/>> (Sphere, Box3D),
	OPERATOR	190	/>> (Sphere, Path3D),
	OPERATOR	230	/>> (Sphere, Polygon3D),
	OPERATOR	270	/>> (Sphere, Sphere),
	-- overlaps or back
	OPERATOR	31	&/>> (Sphere, Point3D),
	OPERATOR	71	&/>> (Sphere, Lseg3D),
	OPERATOR	111	&/>> (Sphere, Line3D),
	OPERATOR	151	&/>> (Sphere, Box3D),
	OPERATOR	191	&/>> (Sphere, Path3D),
	OPERATOR	231	&/>> (Sphere, Polygon3D),
	OPERATOR	271	&/>> (Sphere, Sphere),
	-- functions
	FUNCTION	1	gist_sphere_consistent (internal, Sphere, smallint, oid, internal),
	FUNCTION	2	gist_box3D_union (internal, internal),
	FUNCTION	3	gist_sphere_compress (internal),
	FUNCTION	4	gist_box3D_decompress (internal),
	FUNCTION	5	gist_box3D_penalty (internal, internal, internal),
	FUNCTION	6	gist_box3D_picksplit (internal, internal),
	FUNCTION	7	gist_box3D_same (Box3D, Box3D, internal),
	FUNCTION	8	gist_sphere_distance (internal, Sphere, smallint, oid, internal);

/******************************************************************************
 * Support routines for the SP-GiST octree access method
 *****************************************************************************/

CREATE OR REPLACE FUNCTION spgist_octree_config(internal, internal) RETURNS void AS 'MODULE_PATHNAME', 'spgist_octree_config' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION spgist_octree_choose(internal, internal) RETURNS void AS 'MODULE_PATHNAME', 'spgist_octree_choose' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION spgist_octree_picksplit(internal, internal) RETURNS void AS 'MODULE_PATHNAME', 'spgist_octree_picksplit' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION spgist_octree_inner_consistent(internal, internal) RETURNS void AS 'MODULE_PATHNAME', 'spgist_octree_inner_consistent' LANGUAGE C IMMUTABLE STRICT;
CREATE OR REPLACE FUNCTION spgist_octree_leaf_consistent(internal, internal) RETURNS boolean AS 'MODULE_PATHNAME', 'spgist_octree_leaf_consistent' LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR CLASS spgist_octree_ops
	DEFAULT FOR TYPE Point3D
	USING SPGiST AS
	-- strictly left
	OPERATOR	1	<< (Point3D, Point3D),  
	OPERATOR	41	<< (Point3D, Lseg3D),  
	OPERATOR	81	<< (Point3D, Line3D),  
	OPERATOR	121	<< (Point3D, Box3D),  
	OPERATOR	161	<< (Point3D, Path3D),  
	OPERATOR	201	<< (Point3D, Polygon3D),  
	OPERATOR	241	<< (Point3D, Sphere), 
	-- strictly right
	OPERATOR	5	>> (Point3D, Point3D),  
	OPERATOR	45	>> (Point3D, Lseg3D),  
	OPERATOR	85	>> (Point3D, Line3D),  
	OPERATOR	125	>> (Point3D, Box3D),  
	OPERATOR	165	>> (Point3D, Path3D),  
	OPERATOR	205	>> (Point3D, Polygon3D),  
	OPERATOR	245	>> (Point3D, Sphere), 
	-- same
	OPERATOR	6	~= (Point3D, Point3D),  
	-- contained by
	OPERATOR	48	<@ (Point3D, Lseg3D),  
	OPERATOR	88	<@ (Point3D, Line3D),  
	OPERATOR	128	<@ (Point3D, Box3D),  
	OPERATOR	168	<@ (Point3D, Path3D),  
	OPERATOR	208	<@ (Point3D, Polygon3D),  
	OPERATOR	248	<@ (Point3D, Sphere), 
	-- strictly below
	OPERATOR	10	<<| (Sphere, Point3D),  
	OPERATOR	50	<<| (Sphere, Lseg3D),  
	OPERATOR	90	<<| (Sphere, Line3D),  
	OPERATOR	130	<<| (Sphere, Box3D),  
	OPERATOR	170	<<| (Sphere, Path3D),  
	OPERATOR	210	<<| (Sphere, Polygon3D),  
	OPERATOR	250	<<| (Sphere, Sphere), 
	-- strictly above
	OPERATOR	11	|>> (Sphere, Point3D),  
	OPERATOR	51	|>> (Sphere, Lseg3D),  
	OPERATOR	91	|>> (Sphere, Line3D),  
	OPERATOR	131	|>> (Sphere, Box3D),  
	OPERATOR	171	|>> (Sphere, Path3D),  
	OPERATOR	211	|>> (Sphere, Polygon3D),  
	OPERATOR	251	|>> (Sphere, Sphere), 
	-- strictly front
	OPERATOR	29	<</ (Point3D, Point3D),
	OPERATOR	69	<</ (Point3D, Lseg3D),
	OPERATOR	109	<</ (Point3D, Line3D),
	OPERATOR	149	<</ (Point3D, Box3D),
	OPERATOR	189	<</ (Point3D, Path3D),
	OPERATOR	229	<</ (Point3D, Polygon3D),
	OPERATOR	269	<</ (Point3D, Sphere),
	-- strictly back
	OPERATOR	30	/>> (Point3D, Point3D),
	OPERATOR	70	/>> (Point3D, Lseg3D),
	OPERATOR	110	/>> (Point3D, Line3D),
	OPERATOR	150	/>> (Point3D, Box3D),
	OPERATOR	190	/>> (Point3D, Path3D),
	OPERATOR	230	/>> (Point3D, Polygon3D),
	OPERATOR	270	/>> (Point3D, Sphere),
	-- functions
	FUNCTION	1	spgist_octree_config(internal, internal),
	FUNCTION	2	spgist_octree_choose(internal, internal),
	FUNCTION	3	spgist_octree_picksplit(internal, internal),
	FUNCTION	4	spgist_octree_inner_consistent(internal, internal),
	FUNCTION	5	spgist_octree_leaf_consistent(internal, internal);
		   
/******************************************************************************/

/* 
	
#define RTLeftStrategyNumber			1		/* for << */
#define RTOverLeftStrategyNumber		2		/* for <<& */
#define RTOverlapStrategyNumber			3		/* for && */
#define RTOverRightStrategyNumber		4		/* for &>> */
#define RTRightStrategyNumber			5		/* for >> */
#define RTSameStrategyNumber			6		/* for ~= */
#define RTContainsStrategyNumber		7		/* for @> */
#define RTContainedByStrategyNumber		8		/* for <@ */
#define RTOverBelowStrategyNumber		9		/* for <<|& */
#define RTBelowStrategyNumber			10		/* for <<| */
#define RTAboveStrategyNumber			11		/* for |>> */
#define RTOverAboveStrategyNumber		12		/* for &|>> */
#define RTOldContainsStrategyNumber		13		/* for old spelling of @> */
#define RTOldContainedByStrategyNumber	14		/* for old spelling of <@ */
#define RTKNNSearchStrategyNumber		15		/* for <-> (distance) */
#define RTContainsElemStrategyNumber	16		/* for range types @> elem */
#define RTAdjacentStrategyNumber		17		/* for -|- */
#define RTEqualStrategyNumber			18		/* for = */
#define RTNotEqualStrategyNumber		19		/* for != */
#define RTLessStrategyNumber			20		/* for < */
#define RTLessEqualStrategyNumber		21		/* for <= */
#define RTGreaterStrategyNumber			22		/* for > */
#define RTGreaterEqualStrategyNumber	23		/* for >= */
#define RTSubStrategyNumber				24		/* for inet >> */
#define RTSubEqualStrategyNumber		25		/* for inet <<= */
#define RTSuperStrategyNumber			26		/* for inet << */
#define RTSuperEqualStrategyNumber		27		/* for inet >>= */

#define RTMaxStrategyNumber				27
*/

/******************************************************************************/
