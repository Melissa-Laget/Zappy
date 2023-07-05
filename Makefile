##
## EPITECH PROJECT, 2023
## B-YEP-400-BDX-4-1-zappy-max.peixoto
## File description:
## Makefile
##

NAME			=	zappy

NAME_SERVER		=	zappy_server

NAME_GUI		=	zappy_gui

NAME_AI			=	zappy_ai

all :			$(NAME)

$(NAME) :
				make -C server
				make -C gui
				make -C AI
				mv server/zappy_server .
				mv gui/zappy_gui .
				cp gui/assets/* assets/
				cp AI/start.sh zappy_ai
				sh -c "chmod +x zappy_ai"

$(NAME_SERVER) :
				make -C server
				mv server/zappy_server .

$(NAME_GUI) :
				make -C gui
				mv gui/zappy_gui .
				cp gui/assets/* assets/

$(NAME_AI) :
				make -C AI
				cp AI/start.sh zappy_ai
				sh -c "chmod +x zappy_ai"

clean :
				make clean -C server
				make clean -C gui
				make clean -C AI

fclean :
				make fclean -C server
				make fclean -C gui
				make fclean -C AI
				rm -f			$(NAME)
				rm -f			$(NAME_SERVER)
				rm -f			$(NAME_GUI)
				rm -f			$(NAME_AI)

re :			fclean all

.PHONY :		all $(NAME) $(NAME_SERVER) $(NAME_GUI) $(NAME_AI) clean fclean re
