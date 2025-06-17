# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hkhrayza <hkhrayza@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/14 12:23:02 by husseinkhra       #+#    #+#              #
#    Updated: 2025/06/14 14:09:00 by hkhrayza         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = webserv

CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98
RM          = rm -f

SRC_DIR     = src
OBJ_DIR     = obj
INC_DIR     = includes

SRC         = $(wildcard $(SRC_DIR)/*.cpp)
OBJ         = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	
	@mkdir -p $(OBJ_DIR)
	@$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	@echo "Compiling and Run server ....."
	@$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	@$(RM) -r $(OBJ_DIR)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

