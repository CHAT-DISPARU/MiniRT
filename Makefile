# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: titan <titan@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/17 17:52:56 by gajanvie          #+#    #+#              #
#    Updated: 2026/01/31 22:27:53 by titan            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := MiniRT

ESC := \033
RESET := $(ESC)[0m
TEXT_CYAN := $(ESC)[38;5;51m
TEXT_BLUE := $(ESC)[38;5;39m
TEXT_DARK_BLUE := $(ESC)[38;5;21m
TEXT_PURPLE := $(ESC)[38;5;93m
TEXT_NEON_GREEN := $(ESC)[38;5;46m
TEXT_NEON_RED := $(ESC)[38;5;196m
BG_RESET := $(RESET)
BG_FRA_BLUE := $(ESC)[48;5;19m
BG_FRA_WHITE := $(ESC)[48;5;255m
BG_FRA_RED := $(ESC)[48;5;160m
BG_BRA_GREEN := $(ESC)[48;5;28m
BG_BRA_YELLOW := $(ESC)[48;5;220m
BG_BRA_BLUE := $(ESC)[48;5;21m
BG_ESP_RED := $(ESC)[48;5;160m
BG_ESP_YELLOW := $(ESC)[48;5;220m
YELLOW=\033[0;33m

SRC_DIR := srcs/
HIT_DIR := hit_objs/
READ_DIR := read_rt/
SETTER_DIR := setters/
PARS_DIR := pars/
UTILS_DIR := utils/
BUILD_DIR := build/
LIB_MATH_DIR := srcs/lib_math
LIB_MATH := ./srcs/lib_math/gajanvielib_math.a
LIBFT = ./srcs/libft/libft.a
LIBFT_DIR = srcs/libft
LIB = MacroLibX
LIB_URL = https://github.com/seekrs/MacroLibX.git
MLX_DIR = MacroLibX
MLX = $(MLX_DIR)/libmlx.so

SRCS := $(SRC_DIR)main.c \
		$(SRC_DIR)$(READ_DIR)$(PARS_DIR)pars.c \
		$(SRC_DIR)clean.c \
		$(SRC_DIR)event.c \
		$(SRC_DIR)render.c \
		$(SRC_DIR)update.c \
		$(SRC_DIR)$(READ_DIR)$(PARS_DIR)parsing_utils.c \
		$(SRC_DIR)shadow_rays.c \
		$(SRC_DIR)$(READ_DIR)$(PARS_DIR)check.c \
		$(SRC_DIR)$(READ_DIR)$(PARS_DIR)check2.c \
		$(SRC_DIR)$(READ_DIR)$(SETTER_DIR)set_a.c \
		$(SRC_DIR)$(READ_DIR)$(SETTER_DIR)set_l.c \
		$(SRC_DIR)$(READ_DIR)$(SETTER_DIR)set_c.c \
		$(SRC_DIR)$(READ_DIR)$(SETTER_DIR)set_cy.c \
		$(SRC_DIR)$(READ_DIR)$(SETTER_DIR)set_sp.c \
		$(SRC_DIR)$(READ_DIR)$(SETTER_DIR)set_sq.c \
		$(SRC_DIR)$(READ_DIR)$(SETTER_DIR)set_pl.c \
		$(SRC_DIR)$(HIT_DIR)hit_cy.c \
		$(SRC_DIR)$(HIT_DIR)hit_sp.c \
		$(SRC_DIR)$(HIT_DIR)hit_sq.c \
		$(SRC_DIR)$(HIT_DIR)hit_pl.c \
		$(SRC_DIR)$(HIT_DIR)hit_something.c \
		$(SRC_DIR)$(READ_DIR)read_rt.c \
		$(SRC_DIR)$(UTILS_DIR)utils.c \
		$(SRC_DIR)$(UTILS_DIR)math_utils.c
		


OBJ := $(patsubst $(SRC_DIR)%.c, $(BUILD_DIR)%.o, $(SRCS))

HEADERS := includes/

CC := cc

FLAGS := -Wall -Werror -Wextra -g -I MacroLibX/includes -O3 -ffast-math

all: $(LIB) header ${NAME}

header:
	@echo " ____ ____ ____ ____ ____ ____ "
	@echo "||M |||i |||n |||i |||R |||T ||"
	@echo "||__|||__|||__|||__|||__|||__||"
	@echo "|/__\|/__\|/__\|/__\|/__\|/__\|"

$(NAME): $(LIBFT) $(LIB_MATH) ${OBJ}
	@echo "$(TEXT_NEON_GREEN)✅ Compilation of MiniRT finished !$(RESET)"
	@${CC} -o ${NAME} -I ${HEADERS} ${OBJ} ${LIBFT} $(MLX) $(LIB_MATH) ${FLAGS} -lm -lSDL2

$(LIB):
	@git clone https://github.com/seekrs/MacroLibX.git -b v2.2.2 MacroLibX --depth=1
	@$(MAKE) --no-print-directory -C MacroLibX -j
	@echo "$(GREEN)✅ Importation of MacroLibX finished!$(NC)"

${BUILD_DIR}%.o: ${SRC_DIR}%.c
	@mkdir -p $(dir $@)
	@${CC} -o $@ -I ${HEADERS} -c $< ${FLAGS}

$(LIBFT):
	@make --no-print-directory -C $(LIBFT_DIR)
	@echo "$(TEXT_NEON_GREEN)✅ Compilation of libft finished !$(RESET)"

$(LIB_MATH):
	@make --no-print-directory -C $(LIB_MATH_DIR)
	@echo "$(TEXT_NEON_GREEN)✅ Compilation of math lib finished !$(RESET)"

clean:
	@echo "$(YELLOW)🧹 file .o cleaned successfully $(RESET)"
	@make --no-print-directory -C $(LIBFT_DIR) clean
	@make --no-print-directory -C $(LIB_MATH_DIR) clean
	@rm -rf ${BUILD_DIR}

fclean: clean
	@echo "$(TEXT_NEON_RED)🧨 MiniRT deleted$(RESET)"
	@make --no-print-directory -C $(LIBFT_DIR) fclean
	@make --no-print-directory -C $(LIB_MATH_DIR) fclean
	@rm -rf $(LIB)
	@rm -f ${NAME}

re: fclean all
re_bonus:	fclean bonus

.PHONY: clean fclean re all libft header