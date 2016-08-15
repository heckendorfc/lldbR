# lldbR

Functions to access the LLDB API so R can be used as a debugger interface.


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

Say we have a file `test.c` that looks like:

```c
#include <stdlib.h>

int main(){
  int i;
  double *x = malloc(10 * sizeof(*x));
  
  for (i=1; i<10; i++){ // oopsie!
    x[i] = 0.0;
  }
  
  // imagine we do things here
  
  free(x);
  return 0;
}
```

```r
library(lldbR)
handle <- lldb.load("/tmp/a.out")
lldb.break(handle,"test.c",10)
lldb.run(handle,NULL)

val <- lldb.expr(handle,"arrayvar",1,5)
class(val)
val

val <- lldb.expr(handle,"container->elemA[6]",0,1)
class(val)
val
