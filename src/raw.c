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

static void cdata_finalize(SEXP ptr){
	int i;
	struct lldbcdata *cdata;
	if (R_ExternalPtrAddr(ptr) == NULL)
		return;

	cdata = (struct lldbcdata*)R_ExternalPtrAddr(ptr);
	free(cdata);
	R_ClearExternalPtr(ptr);
}

SEXP makerr(){
	SEXP RET;
	PROTECT(RET = allocVector(NILSXP, 1));
	UNPROTECT(1);
	return RET;
}

SEXP R_load_process(SEXP R_args){
	int retval;
	SEXP RET;
	SEXP cdata_ptr;
	char *exename = CHARPT(R_args, 0);
	struct lldbcdata *cdata;

	cdata=malloc(sizeof(*cdata));
	if(cdata == NULL)
		goto err;

	newRptr(cdata, cdata_ptr, cdata_finalize);

	if((retval = lldbinit(cdata)))
		goto err;

	if((retval = initprocess(cdata,exename)))
		goto err;


	UNPROTECT(1);
	return cdata_ptr;

err:
	UNPROTECT(1);
	return makerr();
}

SEXP R_run_process(SEXP R_cdata, SEXP R_argv, SEXP R_argc){
	int retval;
	int i;
	SEXP RET;
	char **argv;
	int argc = INT(R_argc);
	struct lldbcdata *cdata = (struct lldbcdata*)R_ExternalPtrAddr(R_cdata);

	argv = malloc((argc+1)*sizeof(*argv));
	if(argv == NULL)
		return makerr();

	for(i=0;i<argc;i++)
		argv[i] = CHARPT(R_argv, i);
	argv[i] = NULL;

	retval = startprocess(cdata,(const char**)argv);

	PROTECT(RET = allocVector(INTSXP, 1));
	INT(RET) = retval;
	UNPROTECT(1);

	return RET;
}

SEXP R_set_breakpoint(SEXP R_cdata, SEXP R_fname, SEXP R_line){
	int retval;
	char *fname = CHARPT(R_fname,0);
	int line = INT(R_line);
	SEXP RET;
	struct lldbcdata *cdata = (struct lldbcdata*)R_ExternalPtrAddr(R_cdata);

	retval = setbreakpoint(cdata,fname,line);

	PROTECT(RET = allocVector(INTSXP, 1));
	INT(RET) = retval;
	UNPROTECT(1);

	return RET;
}

SEXP R_get_value(SEXP R_cdata, SEXP R_expr, SEXP R_start, SEXP R_size){
	SEXP RET;
	int start = INT(R_start);
	int size = INT(R_size);
	char *expr = CHARPT(R_expr,0);
	void *rawx;
	int rawxsize;
	int type;
	struct lldbcdata *cdata = (struct lldbcdata*)R_ExternalPtrAddr(R_cdata);

	type = getvalue(cdata,0,expr,&rawx,&rawxsize,start,size,(start==0 && size==1)?extract_scalar:extract_array);
	RET = make_r_type(rawx,rawxsize,type);
	free(rawx);

	return RET;
}

#ifdef __cplusplus
}
#endif

