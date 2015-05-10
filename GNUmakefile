
SOURCE_SUBDIRS=sched

BUILD_DIR=build

export SOURCE_SUBDIRS BUILD_DIR

all: libcabbage.a

libcabbage.a: 
	$(MAKE) -f makefiles/make.gmake -I$(PWD)/makefiles/ $@ 
