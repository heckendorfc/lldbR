.onLoad <- function(libname, pkgname){
	.Call(R_lldb_startup)
	invisible()
}

.onUnload <- function(libpath){
	.Call(R_lldb_shutdown)
	invisible()
}
