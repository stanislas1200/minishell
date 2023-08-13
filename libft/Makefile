# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sgodin <sgodin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/15 18:32:23 by sgodin            #+#    #+#              #
#    Updated: 2023/06/05 14:50:39 by sgodin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS    = ft_atoi.c \
          ft_bzero.c \
          ft_calloc.c \
          ft_isalnum.c \
          ft_isalpha.c \
          ft_isascii.c \
          ft_isdigit.c \
          ft_isprint.c \
          ft_memchr.c \
          ft_memcpy.c \
          ft_memset.c \
          ft_memcmp.c \
          ft_memmove.c \
          ft_strchr.c \
          ft_strdup.c \
          ft_strlcpy.c \
          ft_strlen.c \
          ft_strncmp.c \
          ft_strnstr.c \
          ft_strrchr.c \
          ft_strtrim.c \
          ft_tolower.c \
          ft_toupper.c \
          ft_substr.c \
          ft_strjoin.c \
          ft_strlcat.c \
          ft_striteri.c \
          ft_putchar_fd.c \
          ft_putstr_fd.c \
          ft_putendl_fd.c \
          ft_putnbr_fd.c \
          ft_strmapi.c \
          ft_itoa.c \
          ft_split.c \

SRCSBONUS = ft_lstnew_bonus.c \
          ft_lstsize_bonus.c \
          ft_lstlast_bonus.c \
          ft_lstadd_front_bonus.c \
          ft_lstadd_back_bonus.c \
          ft_lstiter_bonus.c \
          ft_lstdelone_bonus.c \
          ft_lstclear_bonus.c \
          ft_lstmap_bonus.c \

NAME = libft.a

OBJS = ${SRCS:.c=.o}

OBJSBONUS = ${SRCSBONUS:.c=.o}

SHELL = bash

${NAME}: ${OBJS} ${OBJSBONUS}
	@echo -ne "\033[1;34mBuilding $@...\033[0m"
	@ar rcs $@ ${OBJS} ${OBJSBONUS}
	@echo -ne "\r\033[1;34mBuilding libft \033[1;32mDone\033[K\033[0m\n"
    

all: ${NAME}

clean:
	@rm -rf ${OBJS} ${OBJSBONUS}

fclean: clean
	@rm -rf ${NAME}
	
re: fclean all

.PHONY: all clean fclean re


.c.o:
	@echo -ne "\033[1;34mCompiling $<\033[0m"
	@gcc -Wall -Wextra -Werror -c $< -o ${<:.c=.o} && echo -ne "\r\033[K"
