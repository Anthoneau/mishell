# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/24 14:47:13 by agoldber          #+#    #+#              #
#    Updated: 2025/02/25 13:00:24 by agoldber         ###   ########.fr        #
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
UTILS_DIR		=	utils/
LEXER_DIR		=	lexer/
CHECK_TKN_DIR	=	check_token/
EXPANDER_DIR	=	expander/
PARSER_DIR		=	parser/
BUILTINS_DIR	=	builtins/
EXEC_DIR		=	exec/
SRCS_DIR 		=	src/
OBJS_DIR		=	obj/
CC				=	cc
C_FLAGS			=	-g -Wall -Werror -Wextra -g -I${INCLUDES}
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
@echo "A${PASTEL_PINK}"
@echo "A${PASTEL_PURPLE}"
@echo "A${PASTEL_BLUE}"
@echo "A${PASTEL_BLUE}"
@echo "A${PASTEL_PURPLE}"
@echo "A${PASTEL_PINK}${COLOR_END}"
@echo ""
endef

#SOURCES

UTILS_FILES		=	utils utils2 utils3
LEXER_FILES		=	lexer create_word word which_token
CHECK_TKN_FILES	=	check_token end_pipe heredoc
EXPANDER_FILES	=	expander to_expand change_content expander_utils
PARSER_FILES	=	create_ast redir word ast_utils
BUILTINS_FILES	=	exit
EXEC_FILES		=	exec path get_cmd exec_builtins process get_cmd_input

UTILS			=	${addprefix ${UTILS_DIR}, ${UTILS_FILES}}
LEXER			=	${addprefix ${LEXER_DIR}, ${LEXER_FILES}}
CHECK_TKN		=	${addprefix ${CHECK_TKN_DIR}, ${CHECK_TKN_FILES}}
EXPANDER		=	${addprefix ${EXPANDER_DIR}, ${EXPANDER_FILES}}
PARSER			=	${addprefix ${PARSER_DIR}, ${PARSER_FILES}}
BUILTINS		=	${addprefix ${BUILTINS_DIR}, ${BUILTINS_FILES}}
EXEC			=	${addprefix ${EXEC_DIR}, ${EXEC_FILES}}

FILES			=	main ${UTILS} ${LEXER} ${EXPANDER} ${PARSER} ${CHECK_TKN} ${BUILTINS} ${EXEC} display get_name signals

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