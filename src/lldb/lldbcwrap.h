#ifndef LLDBCWRAP_H
#define LLDBCWRAP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct lldbcdata{
	void *debugger;
	void *target;
	void *process;
	void(*print)(const char *, ...);
};

int extract_array(void *val, void **dst, int *dstsize, int index, int size);
int extract_scalar(void *val, void **dst, int *dstsize, int index, int size);
int getvalue(struct lldbcdata *data, const int frame, const char *varname, void **dst, int *dstsize, int index, int size, int(*datextract)(void*,void**,int*,int,int));
int startprocess(struct lldbcdata *data, const char **args);
int processcontinue(struct lldbcdata *data);
int setbreakpoint(struct lldbcdata *data, const char *file, const uint32_t line);
int initprocess(struct lldbcdata *data, const char *exe_file_path);
int lldbinit(struct lldbcdata *data);
void cleanup(struct lldbcdata *data);

#ifdef __cplusplus
}
#endif

#endif
