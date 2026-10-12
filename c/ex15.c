#include "harness.h"

void	ft_putstr(char *str);

static int	check(char *s)
{
	char	buf[8192];

	cap_start();
	ft_putstr(s);
	cap_end(buf, sizeof(buf));
	if (strcmp(buf, s) != 0)
		FAIL("ft_putstr(\"%s\") imprimio \"%s\"", esc(s, 0), esc(buf, 1));
	return (0);
}

static int	simple(void)
{
	TRY(check("Hello, 42!"));
	TRY(check("a"));
	TRY(check("   "));
	return (0);
}

static int	empty(void)
{
	return (check(""));
}

static int	special_chars(void)
{
	TRY(check("line1\nline2\n"));
	TRY(check("a\tb"));
	TRY(check("%s %d %%"));
	TRY(check("\"quoted\" 'single' \\"));
	return (0);
}

static int	utf8(void)
{
	return (check("\xc3\xb1\xe2\x82\xac\xe6\xbc\xa2"));
}

static int	long_string(void)
{
	char	big[5001];
	int		i;

	i = 0;
	while (i < 5000)
	{
		big[i] = 'a' + (i % 26);
		i++;
	}
	big[5000] = '\0';
	return (check(big));
}

static int	no_extra_newline(void)
{
	char	buf[64];

	cap_start();
	ft_putstr("abc");
	cap_end(buf, sizeof(buf));
	EXPECT_INT(strlen(buf), 3);
	return (0);
}

int	main(void)
{
	harness_init();
	T("cadenas simples", simple);
	T("cadena vacia (no imprime nada)", empty);
	T("saltos de linea, tabuladores, %, comillas, \\", special_chars);
	T("bytes UTF-8 sin alterar", utf8);
	T("cadena larga (5000 caracteres)", long_string);
	T("no anade '\\n' al final", no_extra_newline);
	return (harness_end());
}
