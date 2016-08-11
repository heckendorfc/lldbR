DYLIB_NAME := clldb
DYLIB_PATH := $(BUILD_DIR)
DYLIB_CXX_SOURCES := lldbcwrap.cpp
DYLIB_ONLY := yes
USE_LIBCPP := 1

MY_OS = $(shell uname -s)

ifeq "$(MY_OS)" "Darwin"
    LLDB_BUILD_DIR ?= /Applications/Xcode.app/Contents/SharedFrameworks
    LD_EXTRAS ?= -framework LLDB -Wl,-rpath,"$(LLDB_BUILD_DIR)"
    FRAMEWORK_INCLUDES=-F"$(LLDB_BUILD_DIR)"
else
	#TODO configurable path
    LD_EXTRAS ?= /usr/local/lib/liblldb.so
endif

include Makefile.rules
