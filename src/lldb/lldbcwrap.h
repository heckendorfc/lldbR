#ifndef LLDBCWRAP_H
#define LLDBCWRAP_H

#ifdef __cplusplus
extern "C" {
#endif

struct lldbcdata{
	void *debugger;
	void *target;
	void *process;
};

int extract_array(void *val, void *dst, int index, int size);
int getvalue(struct lldbcdata *data, const int frame, const char *varname, void *dst, int index, int size, int(*datextract)(void*,void*,int,int));
int startprocess(struct lldbcdata *data, const char **args);
int setbreakpoint(struct lldbcdata *data, const char *file, const int line);
int initprocess(struct lldbcdata *data, const char *exe_file_path);
int lldbinit(struct lldbcdata *data);
void cleanup(struct lldbcdata *data);

#ifdef __cplusplus
}
#endif

#endif
