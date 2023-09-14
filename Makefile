# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dspilleb <dspilleb@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/13 14:39:47 by sgodin            #+#    #+#              #
#    Updated: 2023/08/23 15:47:47 by dspilleb         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = src/main.c\
		src/lexer.c\
		src/lexer_utils.c\
		src/expand.c\
		src/parser.c\
		src/parser_utils.c\
		src/parser_check.c\
		src/executer.c\
		src/executer_utils.c\
		src/heredoc.c\
		src/pipe.c\
		src/redirection.c\
		src/execve.c \
		src/env_utils.c\
    	src/utils.c\
		src/expand2.c\
			builtins/builtin.c\
			builtins/builtin_tools.c\
			builtins/cd.c\
			builtins/echo.c\
			builtins/exit.c\
			builtins/export.c\
			builtins/export2.c\
			builtins/unset.c\

OBJS_DIR = ./objects/
OBJS = $(addprefix $(OBJS_DIR), $(SRCS:.c=.o))


SHELL = bash
NAME = minishell
GCC = gcc -Wall -Wextra -Werror -g -fsanitize=address

RED = \033[1;31m
GREEN = \033[1;32m
BLUE = \033[1;34m
RESET = \033[0m

READLINEINCL = -I /Users/${shell whoami}/.brew/opt/readline/include
READLINELIB = -lreadline -L /Users/${shell whoami}/.brew/opt/readline/lib

all: ${NAME}

${NAME}: ${OBJS}
	@echo -ne "${BLUE}Building libft...${RESET}"
	@make -sC ./libft
	@${GCC} ${OBJS} -o ${NAME} ${READLINELIB}  -L./libft -lft && echo -ne "\r${BLUE}Compiling ${NAME} ${GREEN}Done${RESET}\n" || echo -ne "\r${BLUE}Compiling ${NAME} ${RED}Error${RESET}\n"

$(OBJS_DIR)%.o: %.c
	@mkdir -p $(dir $@)
	@gcc $(FLAGS) ${READLINEINCL} -c $< -o $@

clean:
	@echo -ne "${RED}Cleaning...${RESET}"
	@make clean -sC ./libft
	@rm -rf ${OBJS} && echo -ne "\r${RED}Cleaning ${GREEN}Done${RESET}\n" || echo -ne "\r${RED}Cleaning ${RED}Error${RESET}\n"
	@rm -rf $(OBJS_DIR)

fclean: clean
	@make fclean -sC ./libft
	@rm -rf $(NAME)

re: fclean all

.c.o:
	@echo -ne "${BLUE}Compiling $<${RESET}"
	@gcc -Wall -Wextra -Werror -c $< -o ${<:.c=.o} && echo -ne "\r\033[K"

.PHONY: all clean fclean re .c.o bonus