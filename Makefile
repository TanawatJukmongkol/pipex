# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/19 06:32:57 by tjukmong          #+#    #+#              #
#    Updated: 2023/04/18 00:08:26 by tjukmong         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= pipex
SRCS		= pipex.c util.c process.c child.c
SRCS_BONUS	= pipex_bonus.c util_bonus.c process_bonus.c child_bonus.c

SRC_DIR		= ./src/
LIB_DIR		= ./lib/
BUILD_DIR	= ./build/

SRC			= ${addprefix ${BUILD_DIR},${SRCS}}
SRC_BONUS	= ${addprefix ${BUILD_DIR},${SRCS_BONUS}}
OBJ			= ${SRC:.c=.o}
OBJ_BONUS	= ${SRC_BONUS:.c=.o}

CC			= gcc
CFLAGS		= -g -Wall -Werror -Wextra -O3

all: library ${BUILD_DIR} ${NAME}

library:
	find ${LIB_DIR} -mindepth 1 -maxdepth 1 -exec make -C {} \;

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f ${NAME}

re: fclean all

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}

${BUILD_DIR}%.o:${SRC_DIR}%.c
	$(CC) $(CFLAGS) -c -o $@ $^

${NAME}: ${OBJ}
	$(CC) ${OBJ} ${wildcard ${LIB_DIR}/*/*.a} -o ${NAME} $(CFLAGS)

bonus: library ${BUILD_DIR} ${OBJ_BONUS}
	$(CC) ${OBJ_BONUS} ${wildcard ${LIB_DIR}/*/*.a} -o ${NAME} $(CFLAGS)
