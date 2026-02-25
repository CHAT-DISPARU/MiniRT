# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: titan <titan@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/17 18:00:55 by gajanvie          #+#    #+#              #
#    Updated: 2026/02/01 13:38:14 by titan            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = gajanvielib_math.a

OBJDIR = ../../build/

SRCS = mat_init.c \
		mat_inverse.c \
		mat_multiply.c \
		mat_rotate.c \
		mat_scaling.c \
		mat_translation.c \
		mat_view.c \
		print_mat.c \
		vec_add.c \
		vec_cross.c \
		vec_dot.c \
		vec_len.c \
		vec_mul.c \
		vec_new.c \
		vec_norm.c \
		vec_print.c \
		vec_reflect.c \
		vec_sacl.c \
		vec_sub.c \
		vec_min_max.c

OBJS 	= $(SRCS:%.c=$(OBJDIR)%.o)

CC = cc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror

all:	${NAME}

$(NAME): ${OBJS}
	@ar rcs $(NAME) $(OBJS)

$(OBJDIR)%.o: $(SRCD)%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -I ../../includes -c $< -o $@


clean:
	@${RM} ${OBJS}

fclean:	clean
	@${RM} ${NAME}

re:	fclean all

.PHONY: all clean fclean re
