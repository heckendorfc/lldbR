% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/raw.r
\name{lldb.load}
\alias{lldb.load}
\title{lldb.load}
\usage{
lldb.load(args, quiet = FALSE, setdefault = TRUE)
}
\arguments{
\item{args}{process name}

\item{setdefault}{if \code{TRUE}, then the default handle will be updated with the return.}
}
\value{
An object of class \code{lldb_handle}; an external pointer.
}
\description{
The initializer that creates a "handle" for lldb.
}
\details{
This function will automatically update the default handle.  If
}
\examples{
\dontrun{
library(lldbR)

lldb.load("/path/to/binary")
handle1 <- get.default.handle()

handle2 <- lldb.load("/path/to/another/binary", setdefault=FALSE)

identical(handle1, get.default.handle()) # TRUE
identical(handle2, get.default.handle()) # FALSE
set.default.handle(handle2)
identical(handle2, get.default.handle()) # TRUE
}

}
\seealso{
\code{\link{handles}, \link{lldb.exit}}
}
