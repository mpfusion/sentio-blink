####################################################################
# Directories to be included                                       #
####################################################################

USERINCLUDEPATHS = src
SYSTEMDIR        = sys


####################################################################
# User source files                                                #
####################################################################

USER_C_SRC   =
USER_CXX_SRC = $(USERINCLUDEPATHS)/application.cpp
USER_ASM_SRC =


####################################################################
# Additional compiler flags for C and C++                          #
####################################################################

CPPFLAGS += \
	-DDEBUG   \

CXXFLAGS += \
	-std=c++98         \
	-fno-exceptions    \
	-fno-rtti          \
	-ftrapv            \
	-Wall              \
	-Wextra            \
	-Wfloat-equal      \
	-Wshadow           \
	-Wswitch-default   \
	-Wunreachable-code \
	-pedantic          \

CFLAGS += \
	-std=gnu99      \
	-fdata-sections \


####################################################################
# Include system make file                                         #
####################################################################

include $(SYSTEMDIR)/Makefile
