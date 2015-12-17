#common makefile header

include $(BASEDIR)/prj/linux/scripts/configure.mk

MODULE_TARGET	= $(MODULE_EXEC)
LOCAL_INCLUDE	= 	\
				-I$(BASEDIR)/include	\
				-I$(BASEDIR)/src/	\
				-I$(BASEDIR)/third_party	\
				-I$(BASEDIR)/src/core/	\
				-I ./ \

LOCAL_FLAGS		= $(LOCAL_INCLUDE)

LIBS		   := -L$(COMMON_BIN_DIR)/

MODULE_DEPONS = \

MODULE_OBJ   =						\
	$(MODULE_OUTPUT)/test.o	\

#common makefile foot
include $(BASEDIR)/prj/linux/scripts/rules.mk

