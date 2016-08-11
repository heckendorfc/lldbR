#ifndef __RPKG_LLDBR_H__
#define __RPKG_LLDBR_H__

#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

#define newRptr(ptr,Rptr,fin) PROTECT(Rptr = R_MakeExternalPtr(ptr, R_NilValue, R_NilValue));R_RegisterCFinalizerEx(Rptr, fin, TRUE)
#define getRptr(ptr) R_ExternalPtrAddr(ptr);

#define CHARPT(x,i)	((char*)CHAR(STRING_ELT(x,i)))
#define INT(x) INTEGER(x)[0]
#define freeif(x) if(x)free(x);

#endif
