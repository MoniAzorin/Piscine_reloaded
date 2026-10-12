#include "harness.h"

void	ft_print_alphabet(void);

static int	exact(void)
{
	char	buf[128];

	cap_start();
	ft_print_alphabet();
	cap_end(buf, sizeof(buf));
	EXPECT_STR(buf, "abcdefghijklmnopqrstuvwxyz");
	return (0);
}

static int	twice(void)
{
	char	buf[128];

	cap_start();
	ft_print_alphabet();
	ft_print_alphabet();
	cap_end(buf, sizeof(buf));
	EXPECT_STR(buf, "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz");
	return (0);
}

int	main(void)
{
	harness_init();
	T("imprime a-z en una linea, sin salto de linea", exact);
	T("dos llamadas seguidas imprimen dos veces", twice);
	return (harness_end());
}
