#' lldb.load
#' 
#' @param process
#' process name
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
#' @param handle
#' handle returned from lldb.load
#' @param file
#' source file to break in
#' @param line
#' line number to break at
#' 
#' @export
lldb.break <- function(handle,file,line){
	check.is.handle(handle)
	check.is.string(file)
	check.is.posint(line)
	ret <- .Call(R_set_breakpoint,handle,file,as.integer(line));
	invisible(ret)
}

#' lldb.run
#' 
#' @param handle
#' handle returned from lldb.load
#' @param args
#' command line arguments for the process or \code{NULL} (the default) for no arguments
#' 
#' @export
lldb.run <- function(handle,args=NULL){
	check.is.handle(handle)
	if (!is.null(args))
		check.is.string(args)
	ret <- .Call(R_run_process,handle,args,length(args));
	invisible(ret)
}

#' lldb.continue
#' 
#' @param handle
#' handle returned from lldb.load
#' @param args
#' TODO
#' 
#' @export
lldb.continue <- function(handle,args){
}

#' lldb.expr
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
#' @export
lldb.expr <- function(handle,expr,offset,size){
	check.is.handle(handle)
	check.is.string(expr)
	check.is.natnum(offset)
	check.is.posint(size)
	.Call(R_get_value,handle,expr,as.integer(offset),as.integer(size));
}
