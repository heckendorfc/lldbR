#include <string.h>
#include <unistd.h>
#include <sys/param.h>
#include <lldb/API/LLDB.h>
#include "lldbcwrap.h"

using namespace lldb;

struct lldbcppdata{
	SBDebugger *debugger;
	SBTarget *target;
	SBProcess *process;
};

class LLDBSentry
{
	public:
		LLDBSentry() {
			// Initialize LLDB
			SBDebugger::Initialize();
		}
		~LLDBSentry() {
			// Terminate LLDB
			SBDebugger::Terminate();
		}
};

#define convlang(class,elem) if(c->elem) cpp->elem = static_cast<class*>(c->elem)
static void convertstruct(struct lldbcdata *c, struct lldbcppdata *cpp){
	convlang(SBDebugger,debugger);
	convlang(SBTarget,target);
	convlang(SBProcess,process);
}

int extract_array(void *val, void *dst, int index, int size){
	SBType type;
	SBError error;
	SBValue *value = static_cast<SBValue*>(val);
	SBData dat = value->GetPointeeData(index,size);
	if(!dat.IsValid())
		return -1;

	offset_t of=index;
	type = value->GetType();
	dat.ReadRawData(error,of,dst,size*type.GetByteSize());

	return type.GetBasicType();
}

int getvalue(struct lldbcdata *data, const int frame, const char *varname, void *dst, int index, int size, int(*datextract)(void*,void*,int,int)){
	SBThread thread;
	SBFrame frame0;
	SBValue val;
	SBData dat;
	struct lldbcppdata cpp;
	convertstruct(data,&cpp);

	thread = cpp.process->GetSelectedThread();
	if(!thread.IsValid())
		return -1;

	frame0 = thread.GetFrameAtIndex(frame);
	if(!frame0.IsValid())
		return -2;

	val = frame0.FindVariable(varname);
	if(!val.IsValid())
		return -3;

	return extract_array(&val,dst,index,size);
}

int startprocess(struct lldbcdata *data, const char **args){
	char pwd[MAXPATHLEN];
	SBProcess process;
	struct lldbcppdata cpp;
	convertstruct(data,&cpp);

	getcwd(pwd,MAXPATHLEN);
	process = cpp.target->LaunchSimple(args, NULL, pwd);
	if(!process.IsValid())
		return 1;

	data->process=&process;

	return 0;
}

int setbreakpoint(struct lldbcdata *data, const char *file, const int line){
	SBBreakpoint br;
	struct lldbcppdata cpp;
	convertstruct(data,&cpp);

	br = cpp.target->BreakpointCreateByLocation(file,line);
	if(!br.IsValid())
		return 1;

	return 0;
}

int initprocess(struct lldbcdata *data, const char *exe_file_path){
	bool verbose = false;
	const char *arch = NULL;
	const char *platform = NULL;
	const bool add_dependent_libs = false;
	SBError error;
	SBTarget target;
	struct lldbcppdata cpp;
	convertstruct(data,&cpp);

	target = cpp.debugger->CreateTarget (exe_file_path, arch, platform, add_dependent_libs, error);

	if (!error.Success())
		return 1;

	data->target = &target;

	return 0;
}

int lldbinit(struct lldbcdata *data){
	LLDBSentry sentry;
	SBDebugger *debugger = new SBDebugger(SBDebugger::Create());
	debugger->SetAsync(false);

	if(!debugger->IsValid())
		return 1;

	memset(data,0,sizeof(*data));

	data->debugger = debugger;

	return 0;
}

void cleanup(struct lldbcdata *data){
	SBDebugger *debugger = static_cast<SBDebugger*>(data->debugger);
}
