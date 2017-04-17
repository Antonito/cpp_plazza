MK_DIR=			./mk/

include $(MK_DIR)colors.mk $(MK_DIR)defs.mk

##### General configuration
COMMON=			common

CORE=			core

MULTITHREAD=		multithread

IPCS=			ipcs

EXCEPTIONS=		exceptions

###### Add your projects here #####
PROJECTS=		./$(COMMON)/					\
			./$(EXCEPTIONS)					\
			./$(MULTITHREAD)/				\
			./$(IPCS)/					\
			./$(CORE)/					\

# Some useful variables
DEBUG=			no

ARGS=			DEBUG=$(DEBUG) -s --warn-undefined-variables -C

CURRENT_DIR=		${PWD}

PRINT_DIR=		$(ECHO) "$(YELLOW)$(path) :$(CLEAR)\n";

# Rules

all:
			@$(foreach path, $(PROJECTS),			\
			$(PRINT_DIR)					\
			$(MAKE) $(ARGS) $(path);)

$(COMMON):
			@$(ECHO) "$(YELLOW)./$(COMMON)/ :$(CLEAR)\n"
			@$(MAKE) $(ARGS) $(COMMON)

$(EXCEPTIONS):
			@$(ECHO) "$(YELLOW)./$(EXCEPTIONS)/ :$(CLEAR)\n"
			@$(MAKE) $(ARGS) $(EXCEPTIONS)

$(MULTITHREAD):
			@$(ECHO) "$(YELLOW)./$(MULTITHREAD)/ :$(CLEAR)\n"
			@$(MAKE) $(ARGS) $(MULTITHREAD)

$(IPCS):
			@$(ECHO) "$(YELLOW)./$(IPCS)/ :$(CLEAR)\n"
			@$(MAKE) $(ARGS) $(IPCS)

$(CORE):
			@$(ECHO) "$(YELLOW)./$(CORE)/ :$(CLEAR)\n"
			@$(MAKE) $(ARGS) $(CORE)

clean:
			@$(foreach path, $(PROJECTS),			\
			$(PRINT_DIR)					\
			$(MAKE) $(ARGS) $(path) clean;)

fclean:
			@$(foreach path, $(PROJECTS),			\
			$(PRINT_DIR)					\
			$(MAKE) $(ARGS) $(path) fclean;)

re:
			@$(foreach path, $(PROJECTS),			\
			$(PRINT_DIR)					\
			$(MAKE) $(ARGS) $(path) re;)

infos:
			@$(foreach path, $(PROJECTS),			\
			$(PRINT_DIR)					\
			$(MAKE) $(ARGS) $(path) infos;)

.PHONY: all clean fclean re infos $(COMMON) $(EXCEPTIONS) $(MULTITHREAD) $(IPCS) $(CORE)
