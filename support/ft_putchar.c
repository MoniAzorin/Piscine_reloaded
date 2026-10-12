#include <unistd.h>

void	ft_putchar(char c)
{
	ssize_t	r;

	r = write(1, &c, 1);
	(void)r;
}
