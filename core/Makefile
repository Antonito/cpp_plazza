# Makefile dependencies directory
MK_DIR=		./mk/
MK_COMMON_DIR=	./../mk/

include $(MK_COMMON_DIR)colors.mk $(MK_DIR)local_defs.mk $(MK_COMMON_DIR)defs.mk

# Project's directories

SRC_DIR=	./src/

DIR_LIST=	$(SRC_DIR)

# Project's files

SRC_FILES=	main.cpp	\
		Order.cpp	\
		OrderList.cpp	\
		Worker.cpp	\
		Response.cpp

SRC=		$(addprefix $(SRC_DIR), $(SRC_FILES))

INC_DIR=	$(DIR_LIST)			\
		$(ROOT_DIR)/logger/src/		\
		$(ROOT_DIR)/regex/src/		\
		$(ROOT_DIR)/multithread/src/	\
		$(ROOT_DIR)/ipcs/src/		\
		$(ROOT_DIR)/exceptions/src/

# Rules
include $(MK_COMMON_DIR)rules.mk
