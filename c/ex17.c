#include "harness.h"

int	ft_strcmp(char *s1, char *s2);

static int	sgn(int x)
{
	return ((x > 0) - (x < 0));
}

static int	check(char *a, char *b)
{
	int	got;
	int	exp;

	got = ft_strcmp(a, b);
	exp = strcmp(a, b);
	if (sgn(got) != sgn(exp))
		FAIL("ft_strcmp(\"%s\", \"%s\") = %d, strcmp = %d (el signo debe coincidir)",
			esc(a, 0), esc(b, 1), got, exp);
	return (0);
}

static int	equal(void)
{
	TRY(check("", ""));
	TRY(check("a", "a"));
	TRY(check("abc", "abc"));
	TRY(check("42 Barcelona", "42 Barcelona"));
	return (0);
}

static int	different_char(void)
{
	TRY(check("a", "b"));
	TRY(check("b", "a"));
	TRY(check("abc", "abd"));
	TRY(check("abd", "abc"));
	TRY(check("Hello", "hello"));
	TRY(check("hello", "Hello"));
	TRY(check("42 Barcelona", "42 Barcelonb"));
	return (0);
}

static int	different_length(void)
{
	TRY(check("", "a"));
	TRY(check("a", ""));
	TRY(check("ab", "abc"));
	TRY(check("abc", "ab"));
	return (0);
}

static int	high_bytes(void)
{
	TRY(check("\xff", "a"));
	TRY(check("a", "\xff"));
	TRY(check("\x80", "\x7f"));
	TRY(check("\x7f", "\x80"));
	TRY(check("\xc3\xa9", "e"));
	return (0);
}

static int	stops_at_nul(void)
{
	EXPECT_INT(ft_strcmp("abc\0def", "abc\0xyz"), 0);
	return (0);
}

static int	long_strings(void)
{
	char	a[5001];
	char	b[5001];

	memset(a, 'x', 5000);
	memset(b, 'x', 5000);
	a[5000] = '\0';
	b[5000] = '\0';
	TRY(check(a, b));
	b[4999] = 'y';
	TRY(check(a, b));
	TRY(check(b, a));
	return (0);
}

int	main(void)
{
	harness_init();
	T("cadenas iguales -> 0", equal);
	T("caracter distinto: signo correcto", different_char);
	T("distinta longitud (prefijos)", different_length);
	T("bytes > 127 comparados como unsigned char", high_bytes);
	T("se detiene en el primer '\\0'", stops_at_nul);
	T("cadenas largas", long_strings);
	return (harness_end());
}
