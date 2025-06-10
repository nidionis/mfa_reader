OBJS_DIR	= objects/
SRCS_DIR 	= srcs/
LIBMLX		= MLX42/

#Sources
SRC			= main.c
SRCS		= $(addprefix ${SRCS_DIR}, ${SRC})

#Object
OBJS		= $(addprefix ${OBJS_DIR}, ${SRC:.c=.o})

INCLUDES	= includes
NAME		= retromfa
RM			= rm -f
CFLAGS		= -Wall -Wextra -Werror -I ${INCLUDES} -O3

#Colors
LIGHT_GRAY	= \033[2m
ORANGE		= \033[1;33m
DARK_GRAY	= \033[0;90m
RED			= \033[0;91m
GREEN		= \033[0;92m
YELLOW		= \033[0;93m
BLUE		= \033[0;94m
MAGENTA		= \033[0;95m
CYAN		= \033[0;96m
WHITE		= \033[0;97m
RESET		= \033[0m

#Forme
BOLD		= \033[1m
ITALIC		= \033[3m
UNDERLINE	= \033[4m
CROSS		= \033[9m
FLASH		= \033[5m
NEGATIF		= \033[7m

all:			${LIBMLX} libmlx ${NAME}

${OBJS_DIR}%.o: ${SRCS_DIR}%.c | ${OBJS_DIR}
				@cc ${CFLAGS} -c $< -o $@

${NAME}:		${OBJS}
				@${CC} ${CFLAGS} ${OBJS} -L${LIBMLX}/build -lmlx42 -ldl -lglfw -pthread -lm -o $@ 
				@echo "${YELLOW}'$@' is compiled ! ✅${RESET}"

libmlx:
				@cmake ${LIBMLX} -B ${LIBMLX}/build && make -sC ${LIBMLX}/build -j4

${OBJS_DIR}:
				@mkdir -p ${OBJS_DIR}

clean:
				@${RM} ${OBJS}
				@${RM} -r ${OBJS_DIR}
				@make clean -sC ${LIBFT}
				@echo "${RED}'${NAME}' objects are deleted ! 👍${RESET}"

fclean:			clean
				@${RM} ${NAME} ${LIBFT}libft.a
				@echo "${RED}'${NAME}' and 'libft.a' are deleted ! 👍${RESET}"

re:				fclean all

bonus:			all

${LIBMLX}:
				@echo "${CYAN}${ITALIC}Cloning of MLX42 Codam...${RESET}"
				@git clone https://github.com/codam-coding-college/MLX42.git $@
				@echo "${CYAN}Clonne is ready !!!${RESET}"

.PHONY:			all clean fclean re libmlx bonus
