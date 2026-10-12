#include "harness.h"

void	ft_foreach(int *tab, int length, void (*f)(int));

static int	g_calls[256];
static int	g_n;

static void	rec(int n)
{
	if (g_n < 256)
		g_calls[g_n] = n;
	g_n++;
}

static void	put_digit(int n)
{
	char	c;
	ssize_t	r;

	c = '0' + n;
	r = write(1, &c, 1);
	(void)r;
}

static int	in_order(void)
{
	int	tab[6];
	int	i;

	tab[0] = 42;
	tab[1] = -1;
	tab[2] = 0;
	tab[3] = INT_MAX;
	tab[4] = INT_MIN;
	tab[5] = 7;
	ft_foreach(tab, 6, &rec);
	EXPECT_INT(g_n, 6);
	i = 0;
	while (i < 6)
	{
		EXPECT_INT(g_calls[i], tab[i]);
		i++;
	}
	return (0);
}

static int	zero_length(void)
{
	int	tab[3];

	tab[0] = 1;
	tab[1] = 2;
	tab[2] = 3;
	ft_foreach(tab, 0, &rec);
	EXPECT_INT(g_n, 0);
	return (0);
}

static int	one_element(void)
{
	int	tab[1];

	tab[0] = 99;
	ft_foreach(tab, 1, &rec);
	EXPECT_INT(g_n, 1);
	EXPECT_INT(g_calls[0], 99);
	return (0);
}

static int	partial_length(void)
{
	int	tab[5];

	tab[0] = 10;
	tab[1] = 20;
	tab[2] = 30;
	tab[3] = 40;
	tab[4] = 50;
	ft_foreach(tab, 3, &rec);
	EXPECT_INT(g_n, 3);
	EXPECT_INT(g_calls[2], 30);
	return (0);
}

static int	prints_digits(void)
{
	int		tab[10];
	char	buf[32];
	int		i;

	i = 0;
	while (i < 10)
	{
		tab[i] = i;
		i++;
	}
	cap_start();
	ft_foreach(tab, 10, &put_digit);
	cap_end(buf, sizeof(buf));
	EXPECT_STR(buf, "0123456789");
	return (0);
}

static int	big_array(void)
{
	static int	tab[1000];
	int			i;

	i = 0;
	while (i < 1000)
	{
		tab[i] = i * 3;
		i++;
	}
	ft_foreach(tab, 1000, &rec);
	EXPECT_INT(g_n, 1000);
	EXPECT_INT(g_calls[255], 255 * 3);
	return (0);
}

static int	negative_length(void)
{
	int	tab[2];

	tab[0] = 1;
	tab[1] = 2;
	ft_foreach(tab, -1, &rec);
	EXPECT_INT(g_n, 0);
	return (0);
}

int	main(void)
{
	harness_init();
	T("aplica f a cada elemento, en orden", in_order);
	T("length = 0 -> no llama a f", zero_length);
	T("un solo elemento", one_element);
	T("solo recorre 'length' elementos", partial_length);
	T("con una funcion que imprime: 0123456789", prints_digits);
	T("array de 1000 elementos", big_array);
	TW("length negativo -> no llama a f", negative_length);
	return (harness_end());
}
