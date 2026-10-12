#include "harness.h"

int	ft_count_if(char **tab, int (*f)(char*));

static char	*g_seen[16];
static int	g_seen_n;

static int	len_gt1(char *s)
{
	return (strlen(s) > 1);
}

static int	always(char *s)
{
	(void)s;
	return (1);
}

static int	never(char *s)
{
	(void)s;
	return (0);
}

static int	starts_with_4(char *s)
{
	return (s[0] == '4');
}

static int	record(char *s)
{
	if (g_seen_n < 16)
		g_seen[g_seen_n] = s;
	g_seen_n++;
	return (1);
}

static int	basic(void)
{
	char	*tab[4];

	tab[0] = "a";
	tab[1] = "bb";
	tab[2] = "ccc";
	tab[3] = 0;
	EXPECT_INT(ft_count_if(tab, &len_gt1), 2);
	return (0);
}

static int	all_and_none(void)
{
	char	*tab[4];

	tab[0] = "x";
	tab[1] = "y";
	tab[2] = "z";
	tab[3] = 0;
	EXPECT_INT(ft_count_if(tab, &always), 3);
	EXPECT_INT(ft_count_if(tab, &never), 0);
	return (0);
}

static int	empty_array(void)
{
	char	*tab[1];

	tab[0] = 0;
	g_seen_n = 0;
	EXPECT_INT(ft_count_if(tab, &record), 0);
	EXPECT_INT(g_seen_n, 0);
	return (0);
}

static int	order_and_pointers(void)
{
	char	*tab[4];

	tab[0] = "x";
	tab[1] = "y";
	tab[2] = "z";
	tab[3] = 0;
	g_seen_n = 0;
	EXPECT_INT(ft_count_if(tab, &record), 3);
	EXPECT_INT(g_seen_n, 3);
	EXPECT_TRUE(g_seen[0] == tab[0] && g_seen[1] == tab[1]
		&& g_seen[2] == tab[2], "f debe recibir cada elemento, en orden");
	return (0);
}

static int	empty_strings_dont_stop(void)
{
	char	*tab[5];

	tab[0] = "";
	tab[1] = "a";
	tab[2] = "";
	tab[3] = "b";
	tab[4] = 0;
	EXPECT_INT(ft_count_if(tab, &always), 4);
	return (0);
}

static int	mixed(void)
{
	char	*tab[6];

	tab[0] = "42";
	tab[1] = "1337";
	tab[2] = "4";
	tab[3] = "";
	tab[4] = "a42";
	tab[5] = 0;
	EXPECT_INT(ft_count_if(tab, &starts_with_4), 2);
	return (0);
}

static int	one_element(void)
{
	char	*tab[2];

	tab[0] = "hello";
	tab[1] = 0;
	EXPECT_INT(ft_count_if(tab, &len_gt1), 1);
	EXPECT_INT(ft_count_if(tab, &never), 0);
	return (0);
}

int	main(void)
{
	harness_init();
	T("cuenta los elementos para los que f devuelve 1", basic);
	T("todos (3) y ninguno (0)", all_and_none);
	T("array vacio -> 0 y f no se llama", empty_array);
	T("f recibe cada puntero, una vez y en orden", order_and_pointers);
	T("las cadenas vacias no terminan el array (solo el 0)", empty_strings_dont_stop);
	T("caso mixto", mixed);
	T("un solo elemento", one_element);
	return (harness_end());
}
