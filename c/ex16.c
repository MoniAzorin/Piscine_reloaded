#include "harness.h"

int	ft_strlen(char *str);

static int	check(char *s)
{
	int	got;

	got = ft_strlen(s);
	if (got != (int)strlen(s))
		FAIL("ft_strlen(\"%s\") = %d, esperado %d",
			esc(s, 0), got, (int)strlen(s));
	return (0);
}

static int	basic(void)
{
	TRY(check(""));
	TRY(check("a"));
	TRY(check("Hello"));
	TRY(check("42 Barcelona"));
	TRY(check("   "));
	return (0);
}

static int	special(void)
{
	TRY(check("\n"));
	TRY(check("a\tb\n"));
	TRY(check("\xc3\xb1\xe2\x82\xac"));
	return (0);
}

static int	stops_at_nul(void)
{
	char	s[] = "ab\0cd";

	EXPECT_INT(ft_strlen(s), 2);
	return (0);
}

static int	long_string(void)
{
	char	big[20001];

	memset(big, 'x', 20000);
	big[20000] = '\0';
	EXPECT_INT(ft_strlen(big), 20000);
	return (0);
}

static int	pointer_offset(void)
{
	char	*s;

	s = "Hello world";
	EXPECT_INT(ft_strlen(s + 6), 5);
	EXPECT_INT(ft_strlen(s + 11), 0);
	return (0);
}

int	main(void)
{
	harness_init();
	T("casos basicos (incluye cadena vacia)", basic);
	T("saltos de linea, tabuladores, UTF-8", special);
	T("se detiene en el primer '\\0'", stops_at_nul);
	T("cadena de 20000 caracteres", long_string);
	T("punteros intermedios (s + 6)", pointer_offset);
	return (harness_end());
}
