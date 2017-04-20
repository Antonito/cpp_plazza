# Definitions for Makefile
NAME=			$(ROOT_DIR)/plazza

LOCAL_COMP_FLAGS=

LOCAL_LINK_FLAGS=	-L$(ROOT_DIR)/build/ -lplazza_regex -lplazza_multithread -lplazza_logger -pthread

LOCAL_DEBUG_FLAGS=
