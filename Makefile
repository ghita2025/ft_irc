NAME = ft_irc_server
CXX := c++
CXXFLAGS := -std=c++98

OBJ_D := obj
SRC := $(shell find . -name "*.cpp")

OBJ := $(SRC:%.cpp=$(OBJ_D)/%.o)
DEP := $(OBJ:%.o=%.d)

.PHONY : all clean fclean re

all : $(NAME)

$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $@
	@echo "Build completed!"

$(OBJ_D)/%.o : %.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@
	@echo "Compiled: $<"

clean:
	@rm -rf $(OBJ_D)
	@echo "Object files removed"

fclean: clean
	@rm -f $(NAME)
	@echo "Executable removed"

re: fclean
	@$(MAKE) all -s

-include $(DEP)
.SECONDARY: