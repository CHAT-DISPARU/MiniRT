# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: titan <titan@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/17 17:52:56 by gajanvie          #+#    #+#              #
#    Updated: 2026/02/15 15:32:53 by titan            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := MiniRT
NAME_BONUS := MiniRT_bonus

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
UPDATED_DIR := update/
SRCB_DIR := srcs_bonus/
HIT_DIR := hit_objs/
READ_DIR := read_rt/
SETTER_DIR := setters/
PARS_DIR := pars/
UTILS_DIR := utils/
BUILD_DIR := build/
BVH_DIR	  := bvh/
LIB_MATH_DIR := libs/lib_math
LIB_MATH := ./libs/lib_math/gajanvielib_math.a
LIBFT = ./libs/libft/libft.a
LIBFT_DIR = libs/libft
LIB = libs/MacroLibX
LIB_URL = https://github.com/seekrs/MacroLibX.git
MLX_DIR = libs/MacroLibX
MLX = $(MLX_DIR)/libmlx.so

SRCSB := $(SRCB_DIR)main_bonus.c \
		$(SRCB_DIR)$(READ_DIR)$(PARS_DIR)pars_bonus.c \
		$(SRCB_DIR)clean_bonus.c \
		$(SRCB_DIR)event_bonus.c \
		$(SRCB_DIR)render_bonus.c \
		$(SRCB_DIR)lights_bonus.c \
		$(SRCB_DIR)$(BVH_DIR)box_cy.c \
		$(SRCB_DIR)$(BVH_DIR)box_hy.c \
		$(SRCB_DIR)$(BVH_DIR)box_sp.c \
		$(SRCB_DIR)$(BVH_DIR)slice_cost.c \
		$(SRCB_DIR)$(BVH_DIR)comp_coord.c \
		$(SRCB_DIR)$(BVH_DIR)build_bvh.c \
		$(SRCB_DIR)$(BVH_DIR)box_tr.c \
		$(SRCB_DIR)$(BVH_DIR)box_sq.c \
		$(SRCB_DIR)$(BVH_DIR)box_co.c \
		$(SRCB_DIR)$(BVH_DIR)bvh_utils.c \
		$(SRCB_DIR)$(UPDATED_DIR)update_bonus.c \
		$(SRCB_DIR)$(UPDATED_DIR)update_rot_bonus.c \
		$(SRCB_DIR)thread_calls_bonus.c \
		$(SRCB_DIR)$(READ_DIR)$(PARS_DIR)parsing_utils_bonus.c \
		$(SRCB_DIR)shadow_rays_bonus.c \
		$(SRCB_DIR)$(READ_DIR)$(PARS_DIR)check_bonus.c \
		$(SRCB_DIR)$(READ_DIR)$(PARS_DIR)check2_bonus.c \
		$(SRCB_DIR)$(READ_DIR)$(SETTER_DIR)set_a_bonus.c \
		$(SRCB_DIR)$(READ_DIR)$(SETTER_DIR)set_l_bonus.c \
		$(SRCB_DIR)$(READ_DIR)$(SETTER_DIR)set_c_bonus.c \
		$(SRCB_DIR)$(READ_DIR)$(SETTER_DIR)set_o_bonus.c \
		$(SRCB_DIR)$(READ_DIR)$(SETTER_DIR)set_cy_bonus.c \
		$(SRCB_DIR)$(READ_DIR)$(SETTER_DIR)set_sp_bonus.c \
		$(SRCB_DIR)$(READ_DIR)$(SETTER_DIR)set_tr_bonus.c \
		$(SRCB_DIR)$(READ_DIR)$(SETTER_DIR)set_sq_bonus.c \
		$(SRCB_DIR)$(READ_DIR)$(SETTER_DIR)set_pl_bonus.c \
		$(SRCB_DIR)$(READ_DIR)$(SETTER_DIR)set_hy_bonus.c \
		$(SRCB_DIR)$(READ_DIR)$(SETTER_DIR)set_co_bonus.c \
		$(SRCB_DIR)$(HIT_DIR)hit_cy_bonus.c \
		$(SRCB_DIR)$(HIT_DIR)hit_tr_bonus.c \
		$(SRCB_DIR)$(HIT_DIR)hit_sp_bonus.c \
		$(SRCB_DIR)$(HIT_DIR)hit_co_bonus.c \
		$(SRCB_DIR)$(HIT_DIR)hit_hy_bonus.c \
		$(SRCB_DIR)$(HIT_DIR)hit_sq_bonus.c \
		$(SRCB_DIR)$(HIT_DIR)hit_pl_bonus.c \
		$(SRCB_DIR)$(HIT_DIR)hit_something_bonus.c \
		$(SRCB_DIR)$(READ_DIR)read_rt_bonus.c \
		$(SRCB_DIR)$(UTILS_DIR)utils_bonus.c \
		$(SRCB_DIR)$(UTILS_DIR)texture.c \
		$(SRCB_DIR)$(UTILS_DIR)checkboard.c \
		$(SRCB_DIR)$(UTILS_DIR)fps.c \
		$(SRCB_DIR)$(UTILS_DIR)bump.c \
		$(SRCB_DIR)$(UTILS_DIR)math_utils_bonus.c

SRCS := $(SRC_DIR)main.c \
		$(SRC_DIR)$(READ_DIR)$(PARS_DIR)pars.c \
		$(SRC_DIR)clean.c \
		$(SRC_DIR)event.c \
		$(SRC_DIR)render.c \
		$(SRC_DIR)lights.c \
		$(SRC_DIR)$(UPDATED_DIR)update.c \
		$(SRC_DIR)$(UPDATED_DIR)update_rot.c \
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
OBJB := $(patsubst $(SRCB_DIR)%.c, $(BUILD_DIR)%.o, $(SRCSB))


HEADERS := includes/
HEADERSB := includes/ -I includes_bonus/

CC := clang

FLAGS := -Wall -Werror -Wextra -g -I libs/MacroLibX/includes -O3 -mavx2 -mfma -march=native -mtune=native -funroll-loops -fvectorize -ffp-contract=fast  -freciprocal-math -ffast-math -fstrict-aliasing  -fomit-frame-pointer -flto=full -mprefer-vector-width=256

all: $(LIB) header ${NAME}

header:
	@echo " ____ ____ ____ ____ ____ ____ "
	@echo "||M |||i |||n |||i |||R |||T ||"
	@echo "||__|||__|||__|||__|||__|||__||"
	@echo "|/__\|/__\|/__\|/__\|/__\|/__\|"

bonus: $(LIB) header_bonus $(NAME_BONUS)

header_bonus:                                                                                  
	@echo "  ▄▄▄     ▄▄▄               ▄▄▄▄▄▄     ▄▄▄▄▄▄▄        ▄▄▄                          "
	@echo "   ███▄ ▄███               █▀██▀▀▀█▄  █▀▀██▀▀▀▀      ██▀▀█▄                        "
	@echo "   ██ ▀█▀ ██   ▀▀ ▄     ▀▀   ██▄▄▄█▀     ██          ██ ▄█▀       ▄                "
	@echo "   ██     ██   ██ ████▄ ██   ██▀▀█▄      ██          ██▀▀█▄ ▄███▄ ████▄ ██ ██ ▄██▀█"
	@echo "   ██     ██   ██ ██ ██ ██ ▄ ██  ██      ██        ▄ ██  ▄█ ██ ██ ██ ██ ██ ██ ▀███▄"
	@echo " ▀██▀     ▀██▄▄██▄██ ▀█▄██ ▀██▀  ▀██▀    ▀██▄      ▀██████▀▄▀███▀▄██ ▀█▄▀██▀██▄▄██▀"
	@echo ""

$(NAME): $(LIBFT) $(LIB_MATH) ${OBJ}
	@echo "$(TEXT_NEON_GREEN)✅ Compilation of MiniRT finished !$(RESET)"
	@${CC} -o ${NAME} -I ${HEADERS} ${OBJ} ${LIBFT} $(MLX) $(LIB_MATH) ${FLAGS} -lm -lSDL2

$(NAME_BONUS) : $(LIBFT) $(LIB_MATH) ${OBJB}
	@echo "$(TEXT_NEON_GREEN)✅ Compilation of MiniRT Bonus finished !$(RESET)"
	@${CC} -o ${NAME_BONUS} -I ${HEADERSB} ${OBJB} ${LIBFT} $(MLX) $(LIB_MATH) ${FLAGS} -lm -lSDL2

$(LIB):
	@git clone https://github.com/seekrs/MacroLibX.git -b v2.2.2 libs/MacroLibX --depth=1
	@$(MAKE) --no-print-directory -C libs/MacroLibX -j
	@echo "$(GREEN)✅ Importation of MacroLibX finished!$(NC)"

${BUILD_DIR}%.o: ${SRC_DIR}%.c
	@mkdir -p $(dir $@)
	@${CC} -o $@ -I ${HEADERS} -c $< ${FLAGS}

${BUILD_DIR}%.o: ${SRCB_DIR}%.c
	@mkdir -p $(dir $@)
	@${CC} -o $@ -I ${HEADERSB} -c $< ${FLAGS}

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
	@rm -f ${NAME_BONUS}

re: fclean all
re_bonus:	fclean bonus

.PHONY: clean fclean re all libft header