/*-------------------------------------------------------------------------
 *
 * geo3D_selfuncs.c
 *	  Selectivity routines registered in the operator catalog in the
 *	  "oprrest" and "oprjoin" attributes.
 *
 * Portions Copyright (c) 1996-2017, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * IDENTIFICATION
 *	  src/backend/utils/adt/geo3D_selfuncs.c
 *
 *	XXX These are totally bogus.  Perhaps someone will make them do
 *	something reasonable, someday.
 *
 *-------------------------------------------------------------------------
 */
#include <postgres.h>

#include "geo3D_decls.h"


/*
 *	Selectivity functions for geometric operators.  These are bogus -- unless
 *	we know the actual key distribution in the index, we can't make a good
 *	prediction of the selectivity of these operators.
 *
 *	Note: the values used here may look unreasonably small.  Perhaps they
 *	are.  For now, we want to make sure that the optimizer will make use
 *	of a geometric index if one is available, so the selectivity had better
 *	be fairly small.
 *
 *	In general, GiST needs to search multiple subtrees in order to guarantee
 *	that all occurrences of the same key have been found.  Because of this,
 *	the estimated cost for scanning the index ought to be higher than the
 *	output selectivity would indicate.  gistcostestimate(), over in selfuncs.c,
 *	ought to be adjusted accordingly --- but until we can generate somewhat
 *	realistic numbers here, it hardly matters...
 */

/*
 * Selectivity for operators that depend on volume, such as "overlap".
 */

Datum
volumesel(PG_FUNCTION_ARGS)
{
	PG_RETURN_FLOAT8(0.005);
}

PG_FUNCTION_INFO_V1(volumesel);

Datum
volumejoinsel(PG_FUNCTION_ARGS)
{
	PG_RETURN_FLOAT8(0.005);
}

PG_FUNCTION_INFO_V1(volumejoinsel);

/*
 *	positionsel3D
 *
 * How likely is a box to be strictly left of (right of, above, below,
 * front, back) a given box?
 */

Datum
positionsel3D(PG_FUNCTION_ARGS)
{
	PG_RETURN_FLOAT8(0.1);
}

PG_FUNCTION_INFO_V1(positionsel3D);

Datum
positionjoinsel3D(PG_FUNCTION_ARGS)
{
	PG_RETURN_FLOAT8(0.1);
}

PG_FUNCTION_INFO_V1(positionjoinsel3D);

/*
 *	contsel3D: How likely is a box to contain (be contained by) a given box?
 *
 * This is a tighter constraint than "overlap", so produce a smaller
 * estimate than volumesel does.
 */

Datum
contsel3D(PG_FUNCTION_ARGS)
{
	PG_RETURN_FLOAT8(0.001);
}

PG_FUNCTION_INFO_V1(contsel3D);

Datum
contjoinsel3D(PG_FUNCTION_ARGS)
{
	PG_RETURN_FLOAT8(0.001);
}

PG_FUNCTION_INFO_V1(contjoinsel3D);
