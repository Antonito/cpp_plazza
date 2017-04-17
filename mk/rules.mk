MAIN_OBJ_DIR=	$(ROOT_DIR)/obj/
EMPTY=
OBJ_DIR=	$(MAIN_OBJ_DIR)$(shell basename $(CURDIR))/
OBJ=		$(SRC:$(SRC_DIR)%.cpp=$(OBJ_DIR)%.o)
OBJ_DIR_LIST=	$(DIR_LIST:$(SRC_DIR)%=$(OBJ_DIR)%)

NAME_EXTENSION=	$(suffix $(NAME))

$(NAME):	$(MAIN_OBJ_DIR) $(OBJ_DIR_LIST) $(OBJ)
ifeq ($(NAME_EXTENSION),.a)
		@$(RANLIB) $(NAME) $(OBJ) && \
		$(ECHO) "$(WHITE)[$(GREEN)OK$(WHITE)] Generated $(CYAN)"$(NAME)"\n$(CLEAR)" || \
		$(ECHO) "$(WHITE)[$(RED)KO$(WHITE)] Generated $(CYAN)"$(NAME)"\n$(CLEAR)"
else
		@$(CXX) $(OBJ) -o $(NAME) $(LDFLAGS) && \
		$(ECHO) "$(WHITE)[$(GREEN)OK$(WHITE)] Linked $(CYAN)"$(NAME)"\n$(CLEAR)" || \
		$(ECHO) "$(WHITE)[$(RED)KO$(WHITE)] Linked $(CYAN)"$(NAME)"\n$(CLEAR)"
endif

$(OBJ_DIR)%.o:	$(SRC_DIR)%.cpp
		@$(CXX) $(CXXFLAGS) -c -o $@ $< && \
		$(ECHO) "$(WHITE)[$(GREEN)OK$(WHITE)] Compiled "$<"\n$(CLEAR)" || \
		$(ECHO) "$(WHITE)[$(RED)KO$(WHITE)] Compiled "$<"\n$(CLEAR)"

$(MAIN_OBJ_DIR) $(OBJ_DIR_LIST):
		@$(MKDIR) $@ && \
		$(ECHO) "$(WHITE)[$(PURPLE)MKDIR$(WHITE)] Created obj directory $(CYAN)"$@"\n$(CLEAR)" || \
		$(ECHO) "$(WHITE)[$(PURPLE)MKDIR$(WHITE)] Cannot create obj directory $(CYAN)"$@"\n$(CLEAR)"

all:		$(NAME)

infos:
		@$(ECHO) "$(CYAN)Compiler:\t\t$(CXX)\n"
		@$(ECHO) "Compilation Flags:\t$(CXXFLAGS)\n"
ifeq ($(NAME_EXTENSION),.a)
		@$(ECHO) "Library Generation:\t$(RANLIB)\n$(CLEAR)"
else
		@$(ECHO) "Link Flags:\t\t$(LDFLAGS)\n$(CLEAR)"
endif

clean:
		@$(RM) $(OBJ)
		@$(RM_DIR) $(MAIN_OBJ_DIR)
		@$(ECHO) "$(WHITE)[$(YELLOW)RM$(WHITE)] Removed OBJs files and directory\n$(CLEAR)"

fclean:		clean
		@$(RM) $(NAME)
		@$(ECHO) "$(WHITE)[$(YELLOW)RM$(WHITE)] Removed $(CYAN)"$(NAME)"\n$(CLEAR)"

re:		fclean all

run:
		./$(NAME)

.PHONY: all clean fclean re infos run
