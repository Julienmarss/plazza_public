##
## EPITECH PROJECT, 2025
## Tek 2 B-CCP-400-LIL-4-1-theplazza-martin.delebecque
## File description:
## Makefile
##

NAME		=	plazza
SRC		=	src/Main.cpp \
			src/Parser.cpp \
			src/Reception.cpp \
			src/Kitchen.cpp \
			src/KitchenWrapper.cpp \
			src/AFileLogger.cpp \
			src/AConsoleLogger.cpp \
			src/ADefaultLogger.cpp

OBJ		=	$(SRC:.cpp=.o)


OBJ     = $(SRC:.cpp=.o)

CXX     = g++
CXXFLAGS= -Wall -Wextra -Werror -std=c++20 -fPIC -fno-gnu-unique -I includes
LDFLAGS = -ldl

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS) 
	@ echo -e "\033[1;32m$(NAME) successfully created!\033[0m"
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
clean:
	rm -f $(OBJ)
	

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re