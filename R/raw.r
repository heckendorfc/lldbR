#' lldb.load
#' 
#' The initializer that creates a "handle" for lldb.
#' 
#' @details
#' This function will automatically update the default handle.  If 
#' 
#' @param args
#' process name
#' @param setdefault
#' if \code{TRUE}, then the default handle will be updated with the return.
#' 
#' @return
#' An object of class \code{lldb_handle}; an external pointer.
#' 
#' @seealso \code{\link{handles}}
#' 
#' @examples
#' \dontrun{
#' library(lldbR)
#' 
#' lldb.load("/path/to/binary")
#' handle1 <- get.default.handle()
#' 
#' handle2 <- lldb.load("/path/to/another/binary", setdefault=FALSE)
#' 
#' identical(handle1, get.default.handle()) # TRUE
#' identical(handle2, get.default.handle()) # FALSE
#' set.default.handle(handle2)
#' identical(handle2, get.default.handle()) # TRUE
#' }
#' 
#' @export
lldb.load <- function(args,setdefault=TRUE){
	check.is.string(args)
	check.is.flag(setdefault)
	ret <- .Call(R_load_process,args);
	class(ret) <- "lldb_handle"
	if (setdefault){
		set.default.handle(ret)
	}
	invisible(ret)
}

#' lldb.run
#' 
#' Runs the process.
#' 
#' @param args
#' command line arguments for the process or \code{NULL} (the default) for no
#' arguments. Each argument should be in its own string. \code{splitter} can be
#' used to split a single string.
#' @param handle
#' handle returned from \code{lldb.load()} or \code{NULL} for the default handle
#' 
#' @return
#' An invisible return code.
#' 
#' @seealso \code{\link{lldb.breakfun}} \code{\link{lldb.breakline}} \code{\link{splitter}} 
#' 
#' @export
lldb.run <- function(args=NULL,handle=NULL){
	if (!is.null(args) && !is.character(args))
		stop("args must be of class 'character' or NULL")
	handle <- acquire.handle(handle)
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
#' handle returned from \code{lldb.load()} or \code{NULL} for the default handle
#'
#' @return
#' An invisible return code.
#' 
#' @seealso  \code{\link{lldb.breakline}}\code{\link{lldb.breakfun}}
#' 
#' @export
lldb.continue <- function(handle=NULL){
	handle <- acquire.handle(handle)
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
#' @param expr
#' expression to evaluate
#' @param offset
#' offset index to read from
#' @param size
#' number of elements to read
#' @param handle
#' handle returned from \code{lldb.load()} or \code{NULL} for the default handle
#' 
#' @return
#' The requested data.
#' 
#' @examples
#' \dontrun{
#' library(lldbR)
#' lldb.load("/path/to/binary")
#' 
#' ### Break at line 10 of the specified source file 
#' lldb.breakline("/path/to/source.c", 10)
#' lldb.run()
#' 
#' ### Extract x[0]
#' lldb.expr("x")
#' ### Extract x[2] to x[4]
#' lldb.expr("x", 2, 3)
#' }
#' 
#' @export
lldb.expr <- function(expr,offset=0,size=1,handle=NULL){
	check.is.string(expr)
	check.is.natnum(offset)
	check.is.posint(size)
	handle <- acquire.handle(handle)
	.Call(R_get_value,handle,expr,as.integer(offset),as.integer(size));
}

# intentionally not public
lldb.exit <- function(handle=NULL){
	if(is.null(handle) || identical(handle,get.default.handle())){
		rm(handle)
		rm("handle", envir=lldb_state)
	} else {
		rm(handle, envir=-2)
	}
	
	invisible(gc())
	invisible()
}
