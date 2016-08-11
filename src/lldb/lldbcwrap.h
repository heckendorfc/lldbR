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

#ifdef __cplusplus
}
#endif

#endif
