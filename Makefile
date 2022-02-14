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

NAME	=	containers
SRCS	=	$(shell find tests -type f -name "*.cpp")
DIR_O	=	obj/
OBJS	=	$(SRCS:tests/%.cpp=obj/%.o)
INCL	=	$(shell find include -type f -name "*.hpp")

# CPP

CXX				= clang++
CXX_LANG_FLAGS	= -std=c++98
CXX_WARN_FLAGS	= -Wall -Wextra -Werror
CXX_INCL_FLAGS	= -I.  -Iinclude
# MY_CXX_MACRO_FLAGS	= -DMYFOO=32
CXX_OPTIM_FLAGS= -g
CXXFLAGS	= $(CXX_LANG_FLAGS) $(CXX_WARN_FLAGS) \
	$(CXX_INCL_FLAGS) $(CXX_OPTIM_FLAGS)

# RULES

all:	$(OBJS) $(NAME)

$(OBJS):	| $(DIR_O)

$(DIR_O):
	@mkdir -p $(DIR_O)

$(DIR_O)%.o : tests/%.cpp $(INCL)
	$(eval FNAME=$(subst .o,,$@))
	$(eval FNAME=$(subst $(DIR_O),,$(FNAME)))

	@$(eval CXXFLAGS=$(subst -std=c++11,-std=c++98,$(CXXFLAGS)))
	@$(CXX) $(CXXFLAGS) -D_IS_TEST -o $@ -c $<
	@$(CXX) $(CXXFLAGS) $@ -o ft_$(FNAME)
	@$(ECHO) "$(GREEN) ft_$(FNAME)$(QUIT)"
	@./ft_$(FNAME) > ft_$(FNAME).log

	@$(eval CXXFLAGS=$(subst -std=c++98,-std=c++11,$(CXXFLAGS)))
	@$(CXX) $(CXXFLAGS) -o $@ -c $<
	@$(CXX) $(CXXFLAGS) $@ -o std_$(FNAME)
	@$(ECHO) "$(GREEN) std_$(FNAME)$(QUIT)"
	@./std_$(FNAME) > std_$(FNAME).log

	@(diff std_$(FNAME).log ft_$(FNAME).log > $(FNAME).diff \
		&& printf "$(GREEN)No diffs ;)$(QUIT)\n" && rm $(FNAME).diff) \
		|| (printf "$(RED)Diffs :( $(FNAME).diff$(QUIT)\n" && cat $(FNAME).diff)

	@printf "$(GREY)[ ----- Time ----- ]$(QUIT)\n"
	@(time -p ./std_$(FNAME) > /dev/null) 2>&1 | grep "real" | sed 's/real/std/'
	@(time -p ./ft_$(FNAME) > /dev/null) 2>&1 | grep "real" | sed 's/real/ft /'

$(NAME):	$(OBJS)

clean:
	@$(ECHO) "$(RED)Deleting .o files$(QUIT)"
	@rm -fr $(DIR_O)

fclean:	clean
	@$(ECHO) "$(RED)Deleting 'ft_* and std_*' executables"
	@rm -fr std_* ft_* *.dSYM *.log
	@$(ECHO) "$(RED) '$(NAME)' executables DELETED$(QUIT)"

re:		fclean all

.PHONY	=	all clean fclean re

#----- Progressbar endif at end Makefile
endif