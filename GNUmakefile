
SOURCE_SUBDIRS=sched periph/stm32f100xx/

BUILD_DIR=build

export SOURCE_SUBDIRS BUILD_DIR

all: libcabbage.a

.DEFAULT: 
	$(MAKE) -f makefiles/make.gmake -I$(PWD)/makefiles/ $@ 
