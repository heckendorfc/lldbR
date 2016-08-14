#' lldb.load
#' 
#' @param args
#' TODO
#' 
#' @export
lldb.load <- function(args){
	.Call("R_load_process",args,package="lldbR");
}

#' lldb.break
#' 
#' @param args
#' TODO
#' 
#' @export
lldb.break <- function(args){
}

#' lldb.continue
#' 
#' @param args
#' TODO
#' 
#' @export
lldb.continue <- function(args){
}
