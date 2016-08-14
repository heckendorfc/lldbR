#' lldb.load
#' 
#' @param process
#' process name
#' 
#' @export
lldb.load <- function(args){
	.Call("R_load_process",args,package="lldbR");
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
	.Call("R_set_breakpoint",handle,file,line,package="lldbR");
}

#' lldb.run
#' 
#' @param handle
#' handle returned from lldb.load
#' @param args
#' command line arguments for the process
#' 
#' @export
lldb.run <- function(handle,args){
	.Call("R_run_process",handle,args,length(args),package="lldbR");
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
	.Call("R_get_value",handle,expr,offset,size,package="lldbR");
}
