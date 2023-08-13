# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sgodin <sgodin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/13 14:39:47 by sgodin            #+#    #+#              #
#    Updated: 2023/08/13 14:41:18 by sgodin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = main.c

OBJS = ${SRCS:.c=.o}

SHELL = bash
NAME = minishell
GCC = gcc -Wall -Wextra -Werror -g -lreadline

.PHONY: all clean fclean re .c.o bonus

all: ${NAME}

${NAME}: ${OBJS}
	@${GCC} ${OBJS} -o ${NAME} && echo -ne "\r\033[1;34mCompiling ${NAME} \033[1;32mDone\033[0m\n" || echo -ne "\r\033[1;34mCompiling ${NAME} \33[1;31mError\033[0m\n"

clean:
	@echo -ne "\033[1;31mCleaning...\033[0m"
	@rm -rf ${OBJS} && echo -ne "\r\033[1;31mCleaning \033[1;32mDone\033[K\033[0m\n" || echo -ne "\r\033[1;31mCleaning \033[1;31mError\033[K\033[0m\n"

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.c.o:
	@echo -ne "\033[1;34mCompiling $<\033[0m"
	@gcc -Wall -Wextra -Werror -c $< -o ${<:.c=.o} && echo -ne "\r\033[K"