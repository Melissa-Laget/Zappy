##
## EPITECH PROJECT, 2021
## Makefile
## File description:
## Gautier BONHUR
##

NAME			=	zappy_server

SRC_MAIN		= src/main.c

SRC				=	src/core.c \
					src/init_server.c \
					src/ressources.c \
					src/distribute_ressource_one.c \
					src/distribute_ressource_two.c \
					src/errors/handle_error.c \
					src/clients/client_gestion.c \
					src/clients/protocol_start.c \
					src/clients/lib.c \
					src/clients/lib_two.c \
					src/clients/lib_three.c \
					src/clients/lib_four.c \
					src/clients/init.c \
					src/clients/sound_transmission.c \
					src/clients/send_message_to_graphical.c \
					src/clients/check_death_player.c \
					src/clients/gestion_command_entered.c \
					src/clients/check_win_event.c \
					src/commands/info_command.c \
					src/commands/info_command_two.c \
					src/commands/handle_command.c \
					src/commands/handle_command_lib.c \
					src/commands/graphic_command.c \
					src/commands/graphic_connection_player.c \
					src/commands/broadcast_command.c \
					src/commands/eject_command.c \
					src/commands/eject_command_lib.c \
					src/commands/bct_command.c \
					src/commands/placement_command.c \
					src/commands/food_command.c	\
					src/commands/client_command.c \
					src/commands/incantation_command.c \
					src/commands/incantation_command_lib.c \
					src/commands/fork_command.c \
					src/commands/look_command.c	\
					src/commands/look_command_tile.c \

SRC_TESTS = tests/tests_main.c

OBJ				=			$(SRC:.c=.o)

OBJ_MAIN		=			$(SRC_MAIN:.c=.o)

CFLAGS 			= 			-I./includes -g

CRITERION		= 			--coverage -lcriterion

all: 			$(NAME)

$(NAME):		$(OBJ) $(OBJ_MAIN)
	gcc -o $(NAME) $(OBJ) $(OBJ_MAIN) -lm

tests_run: $(OBJ)
	make re
	gcc -o unit_tests $(SRC) $(SRC_TESTS) $(CFLAGS) $(CRITERION)
	./unit_tests
	rm unit_tests

clean:
	rm -f $(OBJ) $(OBJ_MAIN)
	rm -f *.o
	rm -f *.gcda
	rm -f *.gcno
	rm -f unit_tests

fclean:			clean
	rm -f $(NAME)

re:	fclean all

.PHONY: all $(NAME) fclean tests_run clean re make
