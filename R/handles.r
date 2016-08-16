lldb_state <- new.env()

#' @method print lldb_handle
#' @export
print.lldb_handle <- function(x, ...){
	class(x) <- NULL
	ptr <- capture.output(x)
	cat(paste("An lldb handle:", ptr, "\n"))
}

acquire.handle <- function(handle){
	if (!is.null(handle)){
		check.is.handle(handle)
		handle
	} else {
		get.default.handle()
	}
}

#' Default Handles
#' 
#' Functions to get and set the default handle.
#' 
#' @param handle
#' The return from \code{lldb.load()}.
#' 
#' @return
#' \code{get.default.handle()} returns the default handle if set, and errors
#' otherwise. \code{set.default.handle()} sets the global default handle value.
#' 
#' @seealso \link{\link{lldb.load}}
#' 
#' @name handles
#' @rdname handles
NULL

#' @rdname handles
#' @export
get.default.handle <- function(){
	if (!exists("handle", envir=lldb_state)){
		stop("no default handle set")
	}
	get("handle", envir=lldb_state)
}

#' @rdname handles
#' @export
set.default.handle <- function(handle){
	check.is.handle(handle)
	assign("handle", handle, envir=lldb_state)
	invisible()
}
