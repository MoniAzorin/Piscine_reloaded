NAME     := libft.a
CC       := cc
CFLAGS   := -Wall -Wextra -Werror
CPPFLAGS := -I includes
ARFLAGS  := rcs
RM       := rm -f

SRCS := srcs/ft_putchar.c \
        srcs/ft_putstr.c \
        srcs/ft_strcmp.c \
        srcs/ft_strlen.c \
        srcs/ft_swap.c
OBJS := $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) includes/ft.h
	ar $(ARFLAGS) $@ $^

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re