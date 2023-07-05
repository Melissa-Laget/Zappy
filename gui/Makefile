##
## EPITECH PROJECT, 2021
## Makefile
## File description:
## Gautier BONHUR
##

NAME			=	zappy_gui

SRC_MAIN		= 	src/main.cpp

SRC				=	src/gui.cpp			\
					src/assets.cpp		\
					src/server.cpp		\
					src/draw.cpp

OBJ				=	$(SRC:.cpp=.o)

OBJ_MAIN		=	$(SRC_MAIN:.cpp=.o)

CPPFLAGS		=	-Iincludes -Wall -Wextra -Werror -std=c++2a

SFML			=	-lsfml-graphics -lsfml-window -lsfml-system

all: 			$(NAME)

$(NAME):		$(OBJ) $(OBJ_MAIN)
	g++ -o $(NAME) $(OBJ) $(OBJ_MAIN) $(CPPFLAGS) $(SFML) -lpthread

clean:
	rm -f $(OBJ) $(OBJ_MAIN)
	rm -f *.o

fclean:			clean
	rm -f $(NAME)

re:	fclean all

.PHONY: all $(NAME) fclean clean re
