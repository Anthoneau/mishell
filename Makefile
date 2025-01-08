# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/24 14:47:13 by agoldber          #+#    #+#              #
#    Updated: 2025/01/08 14:35:53 by agoldber         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#BASE
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
    READLINE_FLAGS = $(shell pkg-config --cflags readline)
    READLINE_LIBS = $(shell pkg-config --libs readline)
else
    READLINE_FLAGS =
    READLINE_LIBS = -lreadline
endif

NAME			=	minishell
INCLUDES		=	includes
LEXER_DIR		=	lexer/
SRCS_DIR 		=	src/
OBJS_DIR		=	obj/
CC				=	cc
C_FLAGS			=	-g -Wall -Werror -Wextra -I${INCLUDES}
LIBFT			=	libft.a
RM				=	rm -rf
MAKEFLAGS		+=	--no-print-directory
C_FLAGS			+=	${READLINE_FLAG}
LDFLAGS			+=	${READLINE_LIBS}

#COLORS

BGREEN			=	\033[1;32m
BBLUE			=	\033[1;34m
BCYAN			=	\033[1;36m
BPURPLE			=	\033[1;35m
BWHITE			=	\033[1;37m
BYELLOW			=	\033[1;33m
BRED			=	\033[1;31m
PASTEL_PINK		=	\033[38;5;210m
PASTEL_PURPLE	=	\033[38;5;183m
PASTEL_BLUE		=	\033[38;5;111m
COLOR_END		=	\033[0m

#GRADIENT LOGO

define gradient_logo
@echo "${PASTEL_PINK}"
@echo "${PASTEL_PURPLE}"
@echo "${PASTEL_BLUE}"
@echo "${PASTEL_BLUE}"
@echo "${PASTEL_PURPLE}"
@echo "${PASTEL_PINK}${COLOR_END}"
@echo ""
endef

#SOURCES

LEXER_FILES		=	lexer create_word

LEXER			=	${addprefix ${LEXER_DIR}, ${LEXER_FILES}}

FILES			=	main utils ${LEXER} display parser

SRCS			=	${addprefix ${SRCS_DIR}, ${addsuffix .c, ${FILES}}}
OBJS			=	${addprefix ${OBJS_DIR}, ${addsuffix .o, ${FILES}}}

#PROGRESS BAR
TOTAL_FILES		=	$(words $(FILES))

# ---------------------------------------------------------------------------- #

OBJSF			=	.cache_exists

all: check

check: display_logo ${NAME}
	@echo "${BGREEN}\nEverything up to date${COLOR_END}";

display_logo:
			$(gradient_logo)

${NAME}: ${LIBFT} ${OBJS}
			@${CC} ${OBJS} ${LIBFT} ${LDFLAGS} -o ${NAME}
			@echo "\n${BGREEN}Minishell compiled!${COLOR_END}"

${LIBFT}:
			@make -C libft/
			@mv libft/libft.a .
			@make clean -C libft/
			
${OBJS_DIR}%.o : ${SRCS_DIR}%.c | ${OBJSF}
			@mkdir -p $(dir $@)
			@${CC} ${C_FLAGS} -c $< -o $@
			@PROGRESS=$$(echo "$$(find ${OBJS_DIR} -type f | wc -l | tr -d ' ') * 100 / ${TOTAL_FILES}" | bc); \
			BAR=$$(seq -s "▇" 0 $$((PROGRESS / 2)) | tr -d '[:digit:]'); \
			SPACES=$$(seq -s " " 0 $$((50 - PROGRESS / 2)) | tr -d '[:digit:]'); \
			CURRENT_FILES=$$(find ${OBJS_DIR} -type f | wc -l | tr -d ' '); \
			printf "\r${BBLUE}Compiling: [$$BAR$$SPACES] $$PROGRESS%% ($$CURRENT_FILES/${TOTAL_FILES})${COLOR_END}"

${OBJSF}:
			@mkdir -p ${OBJS_DIR}

clean:
			@${RM} ${OBJS_DIR}
			@${RM} ${OBJSF}
			@echo "${BCYAN}.o files cleaned!${COLOR_END}"

fclean:	clean
			@${RM} ${NAME}
			@${RM} ${LIBFT}
			@echo "${BBLUE}.a files cleaned!${COLOR_END}"

re:		fclean all

.PHONY:	all clean fclean re check display_logo