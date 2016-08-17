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

#define CHECKPTR(ptr) if(ptr==NULL)error("invalid handler; pointer is NULL")

static void cdata_finalize(SEXP ptr){
	struct lldbcdata *cdata;
	if (getRptr(ptr) == NULL)
		return;

	cdata = (struct lldbcdata*)getRptr(ptr);
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
	SEXP cdata_ptr;
	char *exename = CHARPT(R_args, 0);
	struct lldbcdata *cdata;

	cdata=malloc(sizeof(*cdata));
	if(cdata == NULL)
		goto err;

	newRptr(cdata, cdata_ptr, cdata_finalize);

	if((retval = lldbinit(cdata)))
		goto err;

	cdata->print=Rprintf;

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
	struct lldbcdata *cdata = (struct lldbcdata*)getRptr(R_cdata);
	CHECKPTR(cdata);

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

SEXP R_step(SEXP R_cdata, SEXP R_method){
	char *m_str = CHARPT(R_method,0);
	int method;
	int retval;
	SEXP RET;
	struct lldbcdata *cdata = (struct lldbcdata*)getRptr(R_cdata);
	CHECKPTR(cdata);

	if(strcmp(m_str,"over")==0)
		method=STEP_OVER;
	else if(strcmp(m_str,"into")==0)
		method=STEP_INTO;
	else if(strcmp(m_str,"out")==0)
		method=STEP_OUT;
	else
		return makerr();

	retval = processstep(cdata,method);

	PROTECT(RET = allocVector(INTSXP, 1));
	INT(RET) = retval;
	UNPROTECT(1);

	return RET;
}

SEXP R_stepto(SEXP R_cdata, SEXP R_file, SEXP R_line){
	char *file = CHARPT(R_file,0);
	int line = INT(R_line);
	int frame = 0;
	int retval;
	SEXP RET;
	struct lldbcdata *cdata = (struct lldbcdata*)getRptr(R_cdata);
	CHECKPTR(cdata);

	retval = processstepto(cdata,frame,file,line);

	PROTECT(RET = allocVector(INTSXP, 1));
	INT(RET) = retval;
	UNPROTECT(1);

	return RET;
}

SEXP R_set_breakpoint(SEXP R_args){
	int i;
	int retval;
	const char *name;
	struct breakargs barg;
	SEXP el, RET;
	struct lldbcdata *cdata;

	R_args = CDR(R_args); // name

	el = CAR(R_args);
	R_args = CDR(R_args);
	cdata = (struct lldbcdata*)getRptr(el);
	CHECKPTR(cdata);

	memset(&barg,0,sizeof(barg));

	for(i=0; R_args != R_NilValue; i++, R_args = CDR(R_args)){
		name = isNull(TAG(R_args)) ? "" : CHAR(PRINTNAME(TAG(R_args)));
		el = CAR(R_args);
		if(length(el) == 0)
			continue;

		if(strcmp(name,"line")==0 && TYPEOF(el)==INTSXP)
			barg.line = INT(el);
		else if(strcmp(name,"file")==0 && TYPEOF(el)==STRSXP)
			barg.file = CHARPT(el,0);
		else if(strcmp(name,"module")==0 && TYPEOF(el)==STRSXP)
			barg.module = CHARPT(el,0);
		else if(strcmp(name,"symbol")==0 && TYPEOF(el)==STRSXP)
			barg.symbol = CHARPT(el,0);
	}

	retval = setbreakpoint(cdata,&barg);

	PROTECT(RET = allocVector(INTSXP, 1));
	INT(RET) = retval;
	UNPROTECT(1);

	return RET;
}

SEXP R_continue(SEXP R_cdata){
	int retval;
	SEXP RET;
	struct lldbcdata *cdata = (struct lldbcdata*)R_ExternalPtrAddr(R_cdata);

	retval = processcontinue(cdata);

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
	struct lldbcdata *cdata = (struct lldbcdata*)getRptr(R_cdata);
	CHECKPTR(cdata);

	type = getvalue(cdata,0,expr,&rawx,&rawxsize,start,size,(start==0 && size==1)?extract_scalar:extract_array);
	if(type < 0 )
		error("invalid value return (%d)",type);

	RET = make_r_type(rawx,rawxsize,type);
	free(rawx);

	return RET;
}

SEXP R_lldb_startup(SEXP ptr){
	lldbstartup();

	return allocVector(NILSXP, 1);
}

SEXP R_lldb_shutdown(SEXP ptr){
	lldbshutdown();

	return allocVector(NILSXP, 1);
}

#ifdef __cplusplus
}
#endif
