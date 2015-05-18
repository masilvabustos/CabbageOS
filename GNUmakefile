
TOP_DIR		:=$(PWD)

SOURCE_DIR	:=$(PWD)

BUILD_DIR	=build

export TOP_DIR SOURCE_DIR BUILD_DIR

.DEFAULT_GOAL := libcabbage.a

.DEFAULT: 
	$(MAKE) -f makefiles/make.gmake -I$(PWD)/makefiles/ $@ 
