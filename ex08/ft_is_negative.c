
#include <unistd.h>

void	ft_putchar(char c);

void    ft_is_negative(int n)
{
    if (n < 0)
       	ft_putchar('N');
    else
        ft_putchar('P');
}
/*
void	ft_putchar(char c)
	write(1, &c, 1);
}

int main(void)
{
    ft_is_negative(-5);
    ft_is_negative(0);
    ft_is_negative(5);
    return (0);
}
*/