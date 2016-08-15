# lldbR

**lldbR** is an R package for using R as a debugger interface via the LLDB API.  It is not a debugger for R; it allows you to use R as a debugger for another program.  See the examples below for more information.


## Installation

Currently some lldb paths are hard-coded and the package is hard to install on any machine that isn't mine. It'll be fixed in the future maybe.  Ignoring that...

The development version is maintained on GitHub, and can (possibly) be installed by any of the packages that offer installations from GitHub:

```r
### Pick your preference
devtools::install_github("heckendorfc/lldbR")
ghit::install_github("heckendorfc/lldbR")
remotes::install_github("heckendorfc/lldbR")
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
handle <- lldb.load("/tmp/test")
lldb.break(handle, "/tmp/test.c", 10)
lldb.run(handle)

lldb.expr(handle, "x", 0)
## [1] 0
lldb.expr(handle, "x", 1)
## [1] 15
```

We can also do more interesting things, like take a histogram of the first 10 elements of the array:

```r
vals <- lldb.expr(handle, "x", 0, 10)
hist(vals)
```

Which gives the expected histogram:

![Generated histogram](./hist.png)
