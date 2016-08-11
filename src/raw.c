/* This Source Code Form is subject to the terms of the BSD 2-Clause
 * License. If a copy of the this license was not distributed with this
 * file, you can obtain one from http://opensource.org/licenses/BSD-2-Clause. */

// Copyright 2016, Heckendorf.  All rights reserved.

#include "lldbr.h"

SEXP load_process(SEXP R_args)
{
	int retval;
	SEXP RET;
	char *exename = CHARPT(R_args, 0);
	int x[50];

	struct lldbcdata cdata;

	lldbinit(&cdata);
	initprocess(&cdata,exename);
	setbreakpoint(&cdata,"test.c",10);
	startprocess(&cdata,NULL);
	getvalue(&cdata,0,"x",x,0,4,extract_array);

	PROTECT(RET = allocVector(INTSXP, 4));

	for(i=0;i<4;i++)
		INTEGER(RET)[i] = x[i];

	UNPROTECT(1);

	return RET;
}

