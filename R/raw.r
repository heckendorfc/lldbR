#' lldb.load
#' 
#' The initializer that creates a "handle" for lldb.
#' 
#' @param args
#' process name
#' 
#' @return
#' An object of class \code{lldb_handle}; an external pointer.
#' 
#' @seealso \code{\link{lldb.expr}}
#' 
#' @examples
#' \dontrun{
#' handle <- lldb.load("/path/to/binary")
#' handle
#' }
#' 
#' @export
lldb.load <- function(args){
	check.is.string(args)
	ret <- .Call(R_load_process,args);
	class(ret) <- "lldb_handle"
	return(ret)
}

#' @method print lldb_handle
#' @export
print.lldb_handle <- function(x, ...){
	cat("An lldb handle.\n")
}

#' lldb.break
#' 
#' Set a breakpoint.
#' 
#' @param handle
#' handle returned from lldb.load
#' @param file
#' source file to break in
#' @param line
#' line number to break at
#' 
#' @return
#' An invisible return code.
#' 
#' @examples
#' \dontrun{
#' handle <- lldb.load("/path/to/binary")
#' 
#' ### Break at line 10 of the specified source file 
#' lldb.break(handle, "/path/to/source.c", 10)
#' }
#' 
#' @seealso \code{\link{lldb.run}}
#' 
#' @export
lldb.break <- function(handle,file,line){
	check.is.handle(handle)
	check.is.string(file)
	check.is.posint(line)
	ret <- .Call(R_set_breakpoint,handle,file,as.integer(line));
	if (ret != 0){
		stop(paste("operation completed unsuccessfully: returned error code", ret))
	}
	invisible(ret)
}

#' lldb.run
#' 
#' Runs the process.
#' 
#' @param handle
#' handle returned from lldb.load
#' @param args
#' command line arguments for the process or \code{NULL} (the default) for no arguments
#' 
#' @return
#' An invisible return code.
#' 
#' @seealso \code{\link{lldb.break}}
#' 
#' @export
lldb.run <- function(handle,args=NULL){
	check.is.handle(handle)
	if (!is.null(args)){
		check.is.string(args)
	}
	ret <- .Call(R_run_process,handle,args,length(args));
	if (ret != 0){
		stop(paste("operation completed unsuccessfully: returned error code", ret))
	}
	invisible(ret)
}

#' lldb.continue
#' 
#' Continue process execution until the next breakpoint or exit.
#'
#' @param handle
#' handle returned from lldb.load
#'
#' @return
#' An invisible return code.
#' 
#' @seealso \code{\link{lldb.break}}
#' 
#' @export
lldb.continue <- function(handle){
	check.is.handle(handle)
	ret <- .Call("R_continue",handle,package="lldbR");
	if (ret != 0){
		stop(paste("operation completed unsuccessfully: returned error code", ret))
	}
	invisible(ret)
}

#' lldb.expr
#' 
#' Extract data from the process into R via an expression.
#' 
#' @param handle
#' handle returned from lldb.load
#' @param expr
#' expression to evaluate
#' @param offset
#' offset index to read from
#' @param size
#' number of elements to read
#' 
#' @return
#' The requested data.
#' 
#' @examples
#' \dontrun{
#' library(lldbR)
#' handle <- lldb.load("/path/to/binary")
#' 
#' ### Break at line 10 of the specified source file 
#' lldb.break(handle, "/path/to/source.c", 10)
#' lldb.run(handle)
#' 
#' ### Extract x[0]
#' lldb.expr(handle, "x")
#' ### Extract x[2] to x[4]
#' lldb.expr(handle, "x", 2, 3)
#' }
#' 
#' @export
lldb.expr <- function(handle,expr,offset=0,size=1){
	check.is.handle(handle)
	check.is.string(expr)
	check.is.natnum(offset)
	check.is.posint(size)
	.Call(R_get_value,handle,expr,as.integer(offset),as.integer(size));
}
