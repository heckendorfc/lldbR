#' Step functions
#' 
#' Continue process execution until the next line
#'
#' @param method
#' one of c("out","over","into")
#' @param file
#' source file to step to
#' @param line
#' line number to step to
#' @param handle
#' handle returned from \code{lldb.load()} or \code{NULL} for the default handle
#'
#' @return
#' An invisible return code.
#' 
#' @name lldb.step
NULL

#' lldb.step steps either 'into', 'out' of, or 'over' a function or line
#' 
#' @rdname lldb.step
#' @export lldb.step
lldb.step <- function(method="into",handle=NULL){
	check.is.string(method)
	handle <- acquire.handle(handle)
	ret <- .Call("R_step",handle,method,package="lldbR");
	if (ret != 0){
		stop(paste("operation completed unsuccessfully: returned error code", ret))
	}
	invisible(ret)
}

#' lldb.stepto steps until reaching the specified line
#' 
#' @rdname lldb.step
#' @export lldb.stepto
lldb.stepto <- function(file=NULL,line=0,handle=NULL){
	check.is.string.or.null(file)
	check.is.posint(line)
	handle <- acquire.handle(handle)

	ret <- .Call("R_stepto",handle,file,line,package="lldbR");
	if (ret != 0){
		stop(paste("operation completed unsuccessfully: returned error code", ret))
	}

	invisible(ret)
}

