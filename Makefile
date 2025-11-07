CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -MMD -MP -g -std=c++17

INCLUDES = -I includes

NAME = MattDaemon
NAME_BONUS = Ben_AFK

SRCS = src/main.cpp src/MattDaemon.cpp src/Tintin_reporter.cpp

SRCS_BONUS = src/bonus/Ben_AFK.cpp

OBJDIR = obj
OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)
OBJS_BONUS = $(SRCS_BONUS:%.cpp=$(OBJDIR)/%.o)
DEPS = $(SRCS:%.cpp=$(OBJDIR)/%.d)
DEPS_BONUS = $(SRCS_BONUS:%.cpp=$(OBJDIR)/%.d)

all: $(NAME)

bonus: $(NAME_BONUS)

re: fclean all

$(NAME): $(OBJS)
	@echo Compiling $(NAME)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

$(NAME_BONUS): $(OBJS_BONUS)
	@echo Compiling $(NAME_BONUS)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ -lreadline

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo Compiling $<
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo Cleaning objects
	@rm -rf $(OBJDIR)

fclean: clean
	@echo Cleaning $(NAME)
	@rm -rf $(NAME)

run: $(NAME)
	./$(NAME)

.PHONY: all clean fclean run re

-include $(DEPS)
-include $(DEPS_BONUS)
