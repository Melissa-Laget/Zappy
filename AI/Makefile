##
## EPITECH PROJECT, 2023
## AI
## File description:
## Makefile
##

NAME = zappy_ai

$(NAME): requirements
	chmod +x $(NAME)


requirements:
	pip install -r requirement.txt

all: $(NAME)
	chmod +x $(NAME)

clean:
	rm -rf __pycache__

fclean: clean

re: fclean all

.PHONY: $(NAME) requirements all clean fclean re run
