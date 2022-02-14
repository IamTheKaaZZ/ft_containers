# PLACE AT THE TOP OF YOUR MAKEFILE
#---------------------------------
# Progress bar defs
#--------------------------------
#  words = count the number of words
ifneq ($(words $(MAKECMDGOALS)),1) # if no argument was given to make...
.DEFAULT_GOAL = all # set the default goal to all
#  http://www.gnu.org/software/make/manual/make.html
#  $@ = target name
#  %: = last resort recipe
#  --no-print-directory = don't print enter/leave messages for each output grouping
#  MAKEFILE_LIST = has a list of all the parsed Makefiles that can be found *.mk, Makefile, etc
#  -n = dry run, just print the recipes
#  -r = no builtin rules, disables implicit rules
#  -R = no builtin variables, disables implicit variables
#  -f = specify the name of the Makefile
%:                   # define a last resort default rule
		@$(MAKE) $@ --no-print-directory -rRf $(firstword $(MAKEFILE_LIST)) # recursive make call, 
else
ifndef ECHO
#  execute a dry run of make, defining echo beforehand, and count all the instances of "COUNTTHIS"
T := $(shell $(MAKE) $(MAKECMDGOALS) --no-print-directory \
		-nrRf $(firstword $(MAKEFILE_LIST)) \
		ECHO="COUNTTHIS" | grep -c "COUNTTHIS")
#  eval = evaluate the text and read the results as makefile commands
N := x
#  Recursively expand C for each instance of ECHO to count more x's
C = $(words $N)$(eval N := x $N)
#  Multipy the count of x's by 100, and divide by the count of "COUNTTHIS"
#  Followed by a percent sign
#  And wrap it all in square brackets
ECHO = echo -ne "$(QUIT)\r [`expr $C '*' 100 / $T`%]\t"
endif
#------------------
# end progress bar
#------------------

# COLORS

GREEN	= \033[0;32m
RED		= \033[0;31m
BLUE	= \033[0;34m
GREY	= \033[38;5;240m
WHITE	= \033[0;37m
QUIT	= \033[0m

# VARIABLES

NAME	=	ft_containers
SRCS	=	$(wildcard tests/%.cpp)
DIR_O	=	obj/
OBJS	=	$(SRCS:tests/%.cpp=obj/%.o)
INCL	=	$(wildcard include/%.hpp)
CC		=	clang++
CFLAGS	=	-Wall -Wextra -Werror -std=c++98 -Wc++98-compat -I$(INCL)
DBFLAGS =	-g -fsanitize=address

# RULES

all:	$(NAME)

$(NAME):	$(OBJS)
	@$(ECHO) "$(GREEN) '$(NAME)' objects compiled.$(QUIT)"
	@$(ECHO) "$(GREEN) Creating '$(NAME)'$(QUIT)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@$(ECHO) "$(GREEN) $(NAME) executable CREATED$(QUIT)"

$(DIR_O)%.o:	tests/%.cpp $(INCL)
	@$(CC) -c $(CFLAGS) $< -o $@
	@$(ECHO) "$(GREEN) $@$(QUIT)"

$(OBJS):	| $(DIR_O)

$(DIR_O):
	mkdir -p $(DIR_O)

clean:
	@$(ECHO) "$(RED)Deleting .o files$(QUIT)"
	@rm -fr $(DIR_O)
	@rm -fr *.replace

fclean:	clean
	@$(ECHO) "$(RED)Deleting '$(NAME)' executable"
	@rm -fr $(NAME) *.dSYM
	@$(ECHO) "$(RED) '$(NAME)' executable DELETED$(QUIT)"

re:		fclean all

.PHONY	=	all clean fclean re

#----- Progressbar endif at end Makefile
endif