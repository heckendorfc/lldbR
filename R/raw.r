#' lldb.load
#' 
#' @param args
#' TODO
#' 
#' @export
lldb.load <- function(args){
	.Call("load_process",package="lldbR",args);
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
