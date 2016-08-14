# lldbR

Functions to access the LLDB API

```r
library(lldbR)
handle <- lldb.load("/tmp/a.out")
ok <- lldb.break(handle,"test.c",10L)
ok <- lldb.run(handle,NULL)

val <- lldb.expr(handle,"arrayvar",1L,5L)
class(val)
val

val <- lldb.expr(handle,"container->elemA[6]",0L,1L)
class(val)
val
