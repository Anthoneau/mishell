# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dell <dell@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/28 15:26:33 by mel-bout          #+#    #+#              #
#    Updated: 2025/02/12 00:11:09 by dell             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
CFLAGS = -Wall -Werror -Wextra
SRCS =  env.c ft_strjoin.c ft_strlen.c ft_strncmp.c ft_strdup.c sort.c unset.c

# transforme tt les .c en .o
OBJS = ${SRCS:.c=.o}

NAME = env

# on call all avec make
all: ${NAME}

${NAME}: ${OBJS}
	@${CC} ${CFLAGS} ${OBJS} -o ${NAME} ${INC}

clean:
	@${RM} ${OBJS}
	

fclean: clean
	@${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re