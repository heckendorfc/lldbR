/* This Source Code Form is subject to the terms of the BSD 2-Clause
 * License. If a copy of the this license was not distributed with this
 * file, you can obtain one from http://opensource.org/licenses/BSD-2-Clause. */

// Copyright 2016, Heckendorf.  All rights reserved.

#include "lldb/lldbcwrap.h"
#include "lldbr.h"
#include "lldbtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

SEXP R_load_process(SEXP R_args)
{
	int retval;
	SEXP RET;
	char *exename = CHARPT(R_args, 0);
	int x[50];
	int *xp;
	void *rawx;
	int rawxsize;
	int i;
	int type;

	struct lldbcdata cdata;

	lldbinit(&cdata);
	initprocess(&cdata,exename);
	setbreakpoint(&cdata,"test.c",10);
	startprocess(&cdata,NULL);
	type = getvalue(&cdata,0,"x",&rawx,&rawxsize,2,4,extract_array);

	RET = make_r_type(rawx,rawxsize,type);
	free(rawx);

	return RET;
}

#ifdef __cplusplus
}
#endif

