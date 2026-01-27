# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gajanvie <gajanvie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/14 20:10:19 by gajanvie          #+#    #+#              #
#    Updated: 2025/11/14 10:36:15 by gajanvie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SRCD = .
OBJDIR = ../../objs/

SRCS = $(SRCD)/ft_isalnum.c \
       $(SRCD)/ft_isprint.c \
       $(SRCD)/ft_memcmp.c \
       $(SRCD)/ft_putchar_fd.c \
       $(SRCD)/ft_split.c \
       $(SRCD)/ft_strlcat.c \
       $(SRCD)/ft_strncmp.c \
       $(SRCD)/ft_substr.c \
       $(SRCD)/ft_atoi.c \
       $(SRCD)/ft_isalpha.c \
       $(SRCD)/ft_itoa.c \
       $(SRCD)/ft_memcpy.c \
       $(SRCD)/ft_putendl_fd.c \
       $(SRCD)/ft_strchr.c \
       $(SRCD)/ft_strlcpy.c \
       $(SRCD)/ft_strnstr.c \
       $(SRCD)/ft_tolower.c \
       $(SRCD)/ft_bzero.c \
       $(SRCD)/ft_isascii.c \
       $(SRCD)/ft_memcpy.c \
       $(SRCD)/ft_memmove.c \
       $(SRCD)/ft_putnbr_fd.c \
       $(SRCD)/ft_strdup.c \
       $(SRCD)/ft_strlen.c \
       $(SRCD)/ft_strrchr.c \
       $(SRCD)/ft_toupper.c \
       $(SRCD)/ft_calloc.c \
       $(SRCD)/ft_isdigit.c \
       $(SRCD)/ft_memchr.c \
       $(SRCD)/ft_memset.c \
       $(SRCD)/ft_putstr_fd.c \
       $(SRCD)/ft_strjoin.c \
       $(SRCD)/ft_strmapi.c \
       $(SRCD)/ft_strtrim.c \
       $(SRCD)/ft_striteri.c \
       $(SRCD)/ft_lstadd_back_bonus.c \
       $(SRCD)/ft_lstadd_front_bonus.c \
       $(SRCD)/ft_lstclear_bonus.c \
       $(SRCD)/ft_lstdelone_bonus.c \
       $(SRCD)/ft_lstlast_bonus.c \
       $(SRCD)/ft_lstnew_bonus.c \
       $(SRCD)/ft_lstsize_bonus.c \
       $(SRCD)/get_next_line.c \
	$(SRCD)/get_next_line_utils.c \
       $(SRCD)/ft_print_char.c \
       $(SRCD)/ft_printf.c \
       $(SRCD)/ft_print_integer.c \
       $(SRCD)/ft_print_hex.c \
       $(SRCD)/ft_print_percent.c \
       $(SRCD)/ft_print_pointer.c \
       $(SRCD)/ft_print_str.c \
       $(SRCD)/ft_print_uinteger.c \
       $(SRCD)/ft_readers.c \
       $(SRCD)/ft_putstr.c \
       $(SRCD)/ft_putchar.c \
       $(SRCD)/ft_atol_base.c \
       $(SRCD)/ft_atol.c

OBJS 	= $(SRCS:$(SRCD)%.c=$(OBJDIR)%.o)

CC = cc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror

all:	${NAME}

$(NAME): ${OBJS}
	@ar rcs $(NAME) $(OBJS)

$(OBJDIR)%.o: $(SRCD)%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -I ../../include -c $< -o $@


clean:
	@${RM} ${OBJS}

fclean:	clean
	@${RM} ${NAME}

re:	fclean all

.PHONY: all clean fclean re
