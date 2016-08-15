#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/param.h>
#include <lldb/API/LLDB.h>
#include "lldbcwrap.h"

using namespace lldb;

struct lldbcppdata{
	SBDebugger *debugger;
	SBTarget *target;
	SBProcess *process;
};

#define convlang(class,elem) if(c->elem) cpp->elem = static_cast<class*>(c->elem)
static void convertstruct(struct lldbcdata *c, struct lldbcppdata *cpp){
	convlang(SBDebugger,debugger);
	convlang(SBTarget,target);
	convlang(SBProcess,process);
}

int extract_array(void *val, void **dst, int *dstsize, int index, int size){
	SBType type;
	SBError error;
	SBValue *value = static_cast<SBValue*>(val);
	SBData dat = value->GetPointeeData(index,size);
	if(!dat.IsValid())
		return -5;

	type = value->GetType();
	if(type.IsPointerType())
		type = type.GetPointeeType();
	else if(type.IsArrayType())
		type = type.GetArrayElementType();
	else
		return -6;

	*dstsize=size*type.GetByteSize();
	*dst=malloc(*dstsize);
	if(*dst == NULL)
		return -7;
	dat.ReadRawData(error,0,*dst,*dstsize);

	return type.GetBasicType();
}

int extract_scalar(void *val, void **dst, int *dstsize, int index, int size){
	int i;
	SBType type;
	SBError error;
	SBStream strm;
	SBValue *value = static_cast<SBValue*>(val);
	SBData dat = value->GetData();
	if(!dat.IsValid())
		return -4;

	type = value->GetType();

	*dstsize=type.GetByteSize();
	*dst=malloc(*dstsize);
	dat.ReadRawData(error,0,*dst,*dstsize);

	return type.GetBasicType();
}

int getvalue(struct lldbcdata *data, const int frame, const char *varname, void **dst, int *dstsize, int index, int size, int(*datextract)(void*,void**,int*,int,int)){
	SBThread thread;
	SBFrame frame0;
	SBValue val;
	SBData dat;
	SBType type;
	struct lldbcppdata cpp;
	convertstruct(data,&cpp);

	thread = cpp.process->GetSelectedThread();
	if(!thread.IsValid())
		return -1;

	frame0 = thread.GetFrameAtIndex(frame);
	if(!frame0.IsValid())
		return -2;

	val = frame0.EvaluateExpression(varname);
	if(!val.IsValid())
		return -3;

	return datextract(&val,dst,dstsize,index,size);
}

int startprocess(struct lldbcdata *data, const char **args){
	char pwd[MAXPATHLEN];
	SBProcess *process;
	struct lldbcppdata cpp;
	convertstruct(data,&cpp);

	getcwd(pwd,MAXPATHLEN);
	process = new SBProcess(cpp.target->LaunchSimple(args, NULL, pwd));
	if(!process->IsValid())
		return 1;

	data->process = process;

	return 0;
}

int setbreakpoint(struct lldbcdata *data, const char *file, const uint32_t line){
	SBBreakpoint *br;
	struct lldbcppdata cpp;
	convertstruct(data,&cpp);

	br = new SBBreakpoint(cpp.target->BreakpointCreateByLocation(file,line));
	if(!br->IsValid())
		return 1;

	return 0;
}

int initprocess(struct lldbcdata *data, const char *exe_file_path){
	bool verbose = false;
	const char *arch = NULL;
	const char *platform = NULL;
	const bool add_dependent_libs = false;
	SBError error;
	SBTarget *target;
	struct lldbcppdata cpp;
	convertstruct(data,&cpp);

	target = new SBTarget(cpp.debugger->CreateTarget(exe_file_path, arch, platform, add_dependent_libs, error));

	if (!error.Success())
		return 1;

	if (!target->IsValid())
		return 1;

	data->target = target;

	return 0;
}

int lldbinit(struct lldbcdata *data){
	SBDebugger *debugger;
	SBDebugger::Initialize();

	debugger = new SBDebugger(SBDebugger::Create());
	debugger->SetAsync(false);

	if(!debugger->IsValid())
		return 1;

	memset(data,0,sizeof(*data));

	data->debugger = debugger;

	return 0;
}

void cleanup(struct lldbcdata *data){
	SBDebugger::Terminate();
}
