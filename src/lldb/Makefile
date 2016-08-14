ARCHIVE_NAME := clldb.a
ARCHIVE_CXX_SOURCES := lldbcwrap.cpp
MAKE_DSYM := NO
#DYLIB_NAME := clldb
#DYLIB_PATH := $(BUILD_DIR)
#DYLIB_CXX_SOURCES := lldbcwrap.cpp
DYLIB_ONLY := yes
MAKE_EXE := no
USE_LIBCPP := 1

MY_OS = $(shell uname -s)

ifeq "$(MY_OS)" "Darwin"
    LLDB_BUILD_DIR ?= /Applications/Xcode.app/Contents/SharedFrameworks
    LD_EXTRAS ?= -framework LLDB -Wl,-rpath,"$(LLDB_BUILD_DIR)"
    FRAMEWORK_INCLUDES=-F"$(LLDB_BUILD_DIR)"
else
	#TODO configurable path
    #LD_EXTRAS ?= /usr/local/lib/liblldb.so
    LD_EXTRAS ?= $(LLDB_LIBS)
endif

include Makefile.rules
