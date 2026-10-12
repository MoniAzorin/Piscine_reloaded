#include "harness.h"

void	ft_print_numbers(void);

static int	exact(void)
{
	char	buf[64];

	cap_start();
	ft_print_numbers();
	cap_end(buf, sizeof(buf));
	EXPECT_STR(buf, "0123456789");
	return (0);
}

static int	twice(void)
{
	char	buf[64];

	cap_start();
	ft_print_numbers();
	ft_print_numbers();
	cap_end(buf, sizeof(buf));
	EXPECT_STR(buf, "01234567890123456789");
	return (0);
}

int	main(void)
{
	harness_init();
	T("imprime 0-9 en una linea, sin salto de linea", exact);
	T("dos llamadas seguidas imprimen dos veces", twice);
	return (harness_end());
}
