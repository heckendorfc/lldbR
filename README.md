# lldbR

**lldbR** is an R package for using R as a debugger interface via the LLDB API.  It is not a debugger for R; it allows you to use R as a debugger for another program.  See the examples below for more information.



## Installation

The package requires an installation of LLDB.  By default, the package will look for this in `/usr/local`. If your installation is elsewhere, you can specify it via the configure argument `--with-lldb-home`.  For example, on a recent version of Ubuntu, you might install it via:

```bash
R CMD INSTALL lldbR_1.0.tar.gz --configure-args="--with-lldb-home='/usr/lib/llvm-3.8/'"
```



## Package Use

Say we have a file `/tmp/test.c` that looks like:

```c
#include <stdlib.h>

int main(){
  int i;
  double *x = malloc(10 * sizeof(*x));
  
  for (i=1; i<10; i++){ // oopsie!
    x[i] = 15.0;
  }
  
  // imagine we do things here
  
  free(x);
  return 0;
}
```

If we build this as usual with something like `clang -g -o test /tmp/test.c`, then we can debug this from R using lldbR:

```r
library(lldbR)
lldb.load("/tmp/test")
lldb.breakline("/tmp/test.c", 10)
lldb.run()

lldb.expr("x", 0)
## [1] 0
lldb.expr("x", 1)
## [1] 15
```

We can also do more interesting things, like take a histogram of the first 10 elements of the array:

```r
vals <- lldb.expr("x", 0, 10)
hist(vals)
```

Which gives the expected histogram:

![Generated histogram](./hist.png)
