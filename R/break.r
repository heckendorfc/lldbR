#' Set breakpoints
#' 
#' Stop execution at certain posititions in the source
#' 
#' @param file
#' source file to break in
#' @param line
#' line number to break at
#' @param symbol
#' symbol (function) to break in
#' @param module 
#' module to search for symbol
#' @param handle
#' handle returned from \code{lldb.load()} or \code{NULL} for the default handle
#'
#' @name lldb.break
NULL

#' lldb.breakline sets a breakpoint at the specified line
#' 
#' @return
#' An invisible return code.
#' 
#' @examples
#' \dontrun{
#' library(lldbR)
#' lldb.load("/path/to/binary")
#' 
#' ### Break at line 10 of the specified source file 
#' lldb.breakline(file="/path/to/source.c", line=10)
#' }
#' 
#' @seealso \code{\link{lldb.run}}
#' 
#' @rdname lldb.break
#' @export lldb.breakline
lldb.breakline <- function(file=NULL,line=0,handle=NULL){
	check.is.string.or.null(file)
	check.is.posint(line)
	handle <- acquire.handle(handle)

	ret <- .External(R_set_breakpoint,handle=handle,file=file,line=as.integer(line));
	if (ret != 0){
		stop(paste("operation completed unsuccessfully: returned error code", ret))
	}

	invisible(ret)
}

#' lldb.breakfun sets a breakpoint at the specified symbol
#' 
#' @param handle
#' handle returned from \code{lldb.load()} or \code{NULL} for the default handle
#' 
#' @return
#' An invisible return code.
#' 
#' @examples
#' \dontrun{
#' library(lldbR)
#' lldb.load("/path/to/binary")
#' 
#' ### Break in main()
#' lldb.breakfun(symbol="main")
#' }
#' 
#' @seealso \code{\link{lldb.run}} \code{\link{lldb.breakline}}
#' 
#' @rdname lldb.break
#' @export lldb.breakfun
lldb.breakfun <- function(symbol=NULL,module=NULL,handle=NULL){
	check.is.string.or.null(symbol)
	check.is.string.or.null(module)
	handle <- acquire.handle(handle)

	ret <- .External(R_set_breakpoint,handle=handle,symbol=symbol,module=module);
	if (ret != 0){
		stop(paste("operation completed unsuccessfully: returned error code", ret))
	}

	invisible(ret)
}
