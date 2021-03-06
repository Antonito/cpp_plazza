MK_DIR=			./mk/

include $(MK_DIR)colors.mk $(MK_DIR)defs.mk

##### General configuration
CORE=			core

MULTITHREAD=		multithread

IPCS=			ipcs

EXCEPTIONS=		exceptions

LOGGER=			logger

REGEX=			regex

###### Add your projects here #####
PROJECTS=		./$(LOGGER)/					\
			./$(REGEX)/					\
			./$(EXCEPTIONS)					\
			./$(MULTITHREAD)/				\
			./$(IPCS)/					\
			./$(CORE)/					\

# Some useful variables
DEBUG=			no

BRUTEFORCE=		no

ARGS=			DEBUG=$(DEBUG) BRUTEFORCE=$(BRUTEFORCE) -s --warn-undefined-variables -C

CURRENT_DIR=		${PWD}

PRINT_DIR=		$(ECHO) "$(YELLOW)$(path) :$(CLEAR)\n";

# Rules

all:
			@$(foreach path, $(PROJECTS),			\
			$(PRINT_DIR)					\
			$(MAKE) $(ARGS) $(path);)

$(LOGGER):
			@$(ECHO) "$(YELLOW)./$(LOGGER)/ :$(CLEAR)\n"
			@$(MAKE) $(ARGS) $(LOGGER)

$(REGEX):
			@$(ECHO) "$(YELLOW)./$(LOGGER)/ :$(CLEAR)\n"
			@$(MAKE) $(ARGS) $(LOGGER)

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
			@$(ECHO) "$(YELLOW)./ui/ :$(CLEAR)\n"
			@$(MAKE) -s -C ./ui/ clean
			@$(ECHO) "$(WHITE)[$(YELLOW)RM$(WHITE)] Removed OBJs files\n$(CLEAR)"

fclean:
			@$(foreach path, $(PROJECTS),			\
			$(PRINT_DIR)					\
			$(MAKE) $(ARGS) $(path) fclean;)
			@$(ECHO) "$(YELLOW)./ui/ :$(CLEAR)\n"
			@$(MAKE) -s -C ./ui/ clean
			@$(ECHO) "$(WHITE)[$(YELLOW)RM$(WHITE)] Removed OBJs files\n$(CLEAR)"
			@$(RM) ./plazza_ui
			@$(ECHO) "$(WHITE)[$(YELLOW)RM$(WHITE)] Removed $(CYAN)./plazza_ui\n$(CLEAR)"

re:
			@$(foreach path, $(PROJECTS),			\
			$(PRINT_DIR)					\
			$(MAKE) $(ARGS) $(path) re;)

infos:
			@$(foreach path, $(PROJECTS),			\
			$(PRINT_DIR)					\
			$(MAKE) -C $(path) infos;)

ui:
			@$(ECHO) "$(YELLOW)./ui/ :$(CLEAR)\n"
			@$(MAKE) -s -C ./ui/ all && \
			$(ECHO) "$(WHITE)[$(GREEN)OK$(WHITE)] Linked $(CYAN)./plazza_ui\n$(CLEAR)" || \
			$(ECHO) "$(WHITE)[$(RED)KO$(WHITE)] Linked $(CYAN)./plazza_ui\n$(CLEAR)"

.PHONY: all clean fclean re infos ui $(COMMON) $(EXCEPTIONS) $(MULTITHREAD) $(IPCS) $(CORE)
