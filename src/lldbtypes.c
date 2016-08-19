#include "lldbr.h"
#include <wchar.h>
#include <complex.h>
#include <stdbool.h>

#define RTYPE_NULL 0
#define RTYPE_CHAR 1
#define RTYPE_INT 2
#define RTYPE_NUM 3
#define RTYPE_CPX 4
#define RTYPE_LGL 5

struct rinfo{
	int alloctype;
};
static struct rinfo conv_rinfo[] = {
	{NILSXP},
	{STRSXP},
	{INTSXP},
	{REALSXP},
	{CPLXSXP},
	{LGLSXP},
};

typedef SEXP(*typefun)(void *data, size_t size);

union rcast{
	void *tRTYPE_NULL;
	char *tRTYPE_CHAR;
	int *tRTYPE_INT;
	double *tRTYPE_NUM;
	Rcomplex *tRTYPE_CPX;
	int *tRTYPE_LGL;
};

#define tconv(ind,ct,rt) SEXP conv ## ind(void *data, size_t size){\
	ct *xp = data;\
	int i;\
	union rcast rtype;\
	SEXP RET;\
	int num = size/sizeof(ct);\
	PROTECT(RET = allocVector(conv_rinfo[rt].alloctype, num));\
	if(conv_rinfo[rt].alloctype==INTSXP)\
		rtype.tRTYPE_NULL=(void*)INTEGER(RET);\
	else if(conv_rinfo[rt].alloctype==REALSXP)\
		rtype.tRTYPE_NULL=(void*)REAL(RET);\
	else if(conv_rinfo[rt].alloctype==LGLSXP)\
		rtype.tRTYPE_NULL=(void*)LOGICAL(RET);\
	for(i=0;i<num;i++)\
		rtype.t ## rt [i] = xp[i];\
	UNPROTECT(1);\
	return RET;\
}

#define cpxtconv(ind,ct,ext,rt) SEXP conv ## ind(void *data, size_t size){\
	ct *xp = data;\
	int i;\
	union rcast rtype;\
	SEXP RET;\
	int num = size/sizeof(ct);\
	PROTECT(RET = allocVector(conv_rinfo[rt].alloctype, num));\
	rtype.tRTYPE_NULL=(void*)COMPLEX(RET);\
	for(i=0;i<num;i++){\
		rtype.t ## rt [i].r = creal ## ext(xp[i]);\
		rtype.t ## rt [i].i = cimag ## ext(xp[i]);\
	}\
	UNPROTECT(1);\
	return RET;\
}

#define chartconv(ind,ct,rt) SEXP conv ## ind(void *data, size_t size){\
	ct *xp = data;\
	SEXP RET;\
	PROTECT(RET = allocVector(conv_rinfo[rt].alloctype, 1));\
	SET_STRING_ELT(RET, 0, mkCharLen((const char*)xp,size));\
	UNPROTECT(1);\
	return RET;\
}

SEXP convNULL(void *data, size_t size){
	SEXP RET;
	PROTECT(RET = allocVector(NILSXP, 1));
	UNPROTECT(1);
	return RET;
}

//tconv(0,void,RTYPE_NULL) // eBasicTypeInvalid
//tconv(1,void,RTYPE_NULL) // eBasicTypeVoid
chartconv(2,char,RTYPE_CHAR) // eBasicTypeChar
chartconv(3,signed char,RTYPE_CHAR) // eBasicTypeSignedChar
chartconv(4,unsigned char,RTYPE_CHAR) // eBasicTypeUnsignedChar
chartconv(5,wchar_t,RTYPE_CHAR) // eBasicTypeWChar
chartconv(6,wchar_t,RTYPE_CHAR) // eBasicTypeSignedWChar
chartconv(7,wchar_t,RTYPE_CHAR) // eBasicTypeUnsignedWChar
chartconv(8,short,RTYPE_CHAR) // eBasicTypeChar16
chartconv(9,int,RTYPE_CHAR) // eBasicTypeChar32

tconv(10,short,RTYPE_INT) // eBasicTypeShort
tconv(11,unsigned short,RTYPE_INT) // eBasicTypeUnsignedShort
tconv(12,int,RTYPE_INT) // eBasicTypeInt
tconv(13,unsigned int,RTYPE_NUM) // eBasicTypeUnsignedInt
tconv(14,long,RTYPE_NUM) // eBasicTypeLong
tconv(15,unsigned long,RTYPE_NUM) // eBasicTypeUnsignedLong
tconv(16,long long,RTYPE_NUM) // eBasicTypeLongLong
tconv(17,unsigned long long,RTYPE_NUM) // eBasicTypeUnsignedLongLong
//tconv(18,__int128,RTYPE_NUM) // eBasicTypeInt128
//tconv(19,unsigned __int128,RTYPE_NUM) // eBasicTypeUnsignedInt128
tconv(20,bool,RTYPE_LGL) // eBasicTypeBool

//tconv(21,void,RTYPE_NULL) // eBasicTypeHalf

tconv(22,float,RTYPE_NUM) // eBasicTypeFloat
tconv(23,double,RTYPE_NUM) // eBasicTypeDouble
tconv(24,long double,RTYPE_NUM) // eBasicTypeLongDouble

cpxtconv(25,float complex,f,RTYPE_CPX) // eBasicTypeFloatComplex
cpxtconv(26,double complex,,RTYPE_CPX) // eBasicTypeDoubleComplex
cpxtconv(27,long double complex,l,RTYPE_CPX) // eBasicTypeLongDoubleComplex

//tconv(28,void,RTYPE_NULL) // eBasicTypeObjCID
//tconv(29,void,RTYPE_NULL) // eBasicTypeObjCClass
//tconv(30,void,RTYPE_NULL) // eBasicTypeObjCSel
//tconv(31,void,RTYPE_NULL) // eBasicTypeNullPtr
//tconv(32,void,RTYPE_NULL) // eBasicTypeOther


static typefun funlook[] = {
	NULL, NULL, conv2, conv3, conv4, conv5, conv6, conv7, conv8, conv9,
	conv10, conv11, conv12, conv13, conv14, conv15, conv16, conv17, NULL, NULL,
	conv20, NULL, conv22, conv23, conv24, conv25, conv26, conv27, NULL, NULL, NULL, NULL, NULL}; // convNULL 0:1, 28:32

SEXP make_r_type(void *data, size_t size, int lldbtype){
	if(!funlook[lldbtype])
		return convNULL(NULL,0);

	return funlook[lldbtype](data,size);
}
