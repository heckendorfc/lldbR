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
	void(*print)(const char *, ...);
};

#define convlang(class,elem) if(c->elem) cpp->elem = static_cast<class*>(c->elem); else cpp->elem = NULL
static void convertstruct(struct lldbcdata *c, struct lldbcppdata *cpp){
	memset(cpp,0,sizeof(*cpp));
	convlang(SBDebugger,debugger);
	convlang(SBTarget,target);
	convlang(SBProcess,process);
	cpp->print = c->print;
}

static void print_process_desc(struct lldbcppdata *cpp){
	SBThread thread;
	SBStream strm;

	if(cpp->print == NULL)
		return;

	if(!cpp->process->IsValid()){
		cpp->print("Invalid process, unable to print\n");
		return;
	}

	cpp->process->GetDescription(strm);
	thread = cpp->process->GetSelectedThread();
	if(thread.IsValid()){
		thread.GetDescription(strm);
		thread.GetStatus(strm);
	}

	cpp->print("%s",strm.GetData());
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

	if(type.IsTypedefType())
		return type.GetTypedefedType().GetBasicType();
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
	if(type.IsPointerType() || type.IsArrayType())
		return extract_array(val,dst,dstsize,index,size);

	*dstsize=type.GetByteSize();
	*dst=malloc(*dstsize);
	dat.ReadRawData(error,0,*dst,*dstsize);

	if(type.IsTypedefType())
		return type.GetTypedefedType().GetBasicType();
	return type.GetBasicType();
}

int getvalue(struct lldbcdata *data, const int frame, const char *varname, void **dst, int *dstsize, int index, int size, int(*datextract)(void*,void**,int*,int,int)){
	SBThread thread;
	SBFrame frame0;
	SBValue val;
	SBData dat;
	SBType type;
	struct lldbcppdata cpp;

	if(data == NULL)
		return -1;
	convertstruct(data,&cpp);

	if(cpp.process == NULL)
		return -1;
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

	if(data == NULL)
		return 1;
	convertstruct(data,&cpp);

	if(cpp.target == NULL)
		return 1;

	if(cpp.process)
		cpp.process->Destroy();

	getcwd(pwd,MAXPATHLEN);
	process = new SBProcess(cpp.target->LaunchSimple(args, NULL, pwd));
	if(!process->IsValid())
		return 1;

	data->process = process;
	cpp.process = process;

	print_process_desc(&cpp);

	return 0;
}

int processcontinue(struct lldbcdata *data){
	SBError error;
	struct lldbcppdata cpp;

	if(data == NULL)
		return 1;
	convertstruct(data,&cpp);

	if(cpp.process == NULL)
		return 1;
	error = cpp.process->Continue();
	if(!error.Success())
		return 1;

	print_process_desc(&cpp);

	return 0;
}

int processstep(struct lldbcdata *data, const int method){
	SBThread thread;
	struct lldbcppdata cpp;

	if(data == NULL)
		return 1;
	convertstruct(data,&cpp);

	if(cpp.process == NULL)
		return 1;
	thread = cpp.process->GetSelectedThread();
	if(!thread.IsValid())
		return 1;

	switch(method){
		case STEP_OVER:
			thread.StepOver();
			break;
		case STEP_INTO:
			thread.StepInto();
			break;
		case STEP_OUT:
			thread.StepOut();
			break;
		default:
			return 5;
	}

	print_process_desc(&cpp);

	return 0;
}

int processstepto(struct lldbcdata *data, const int frame, const char *file, const uint32_t line){
	SBThread thread;
	SBFrame frame0;
	SBFileSpec filespec(file);
	struct lldbcppdata cpp;

	if(data == NULL)
		return 1;
	convertstruct(data,&cpp);

	if(cpp.process == NULL)
		return 1;
	thread = cpp.process->GetSelectedThread();
	if(!thread.IsValid())
		return 1;

	frame0 = thread.GetFrameAtIndex(frame);
	if(!frame0.IsValid())
		return 2;

	thread.StepOverUntil(frame0,filespec,line); /* neat */

	print_process_desc(&cpp);

	return 0;
}

int setbreakpoint(struct lldbcdata *data, const struct breakargs *barg){
	SBBreakpoint br;
	struct lldbcppdata cpp;

	if(data == NULL)
		return 1;
	convertstruct(data,&cpp);

	if(cpp.target == NULL)
		return 1;
	if(barg->file && barg->line>0)
		br = SBBreakpoint(cpp.target->BreakpointCreateByLocation(barg->file,barg->line));
	else if(barg->symbol)
		br = SBBreakpoint(cpp.target->BreakpointCreateByName(barg->symbol,barg->module));
	else
		return 1;

	if(!br.IsValid())
		return 1;

	if(barg->condition)
		br.SetCondition(barg->condition);

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

	if(data == NULL)
		return 1;
	convertstruct(data,&cpp);

	if(cpp.debugger == NULL)
		return 1;
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

	if(data == NULL)
		return 1;

	debugger = new SBDebugger(SBDebugger::Create());
	debugger->SetAsync(false);

	if(!debugger->IsValid())
		return 1;

	memset(data,0,sizeof(*data));

	data->debugger = debugger;

	return 0;
}

#define deleteif(x) if(cpp.x)delete cpp.x
void cleanup(struct lldbcdata *data){
	struct lldbcppdata cpp;

	if(data == NULL)
		return;

	convertstruct(data,&cpp);

	deleteif(process);
	deleteif(target);
	deleteif(debugger);
}

void lldbstartup(){
	SBDebugger::Initialize();
}

void lldbshutdown(){
	SBDebugger::Terminate();
}
