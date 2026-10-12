#include "harness.h"

char	*ft_strdup(char *src);

static int	check(char *src)
{
	char	*cpy;

	cpy = ft_strdup(src);
	if (!cpy)
		FAIL("ft_strdup(\"%s\") devolvio NULL", esc(src, 0));
	if (cpy == src)
		FAIL("devuelve el mismo puntero: no reserva memoria nueva");
	if (memcmp(cpy, src, strlen(src) + 1) != 0)
		FAIL("ft_strdup(\"%s\") devolvio \"%s\" (¿falta el '\\0' final?)",
			esc(src, 0), esc(cpy, 1));
	free(cpy);
	return (0);
}

static int	basic(void)
{
	TRY(check("Hello"));
	TRY(check("a"));
	TRY(check("42 Barcelona"));
	TRY(check("line1\nline2\n"));
	return (0);
}

static int	empty(void)
{
	char	*cpy;

	cpy = ft_strdup("");
	EXPECT_TRUE(cpy != NULL, "ft_strdup(\"\") no debe devolver NULL");
	EXPECT_INT(cpy[0], 0);
	free(cpy);
	return (0);
}

static int	independent_copy(void)
{
	char	src[8];
	char	*cpy;

	strcpy(src, "abcdef");
	cpy = ft_strdup(src);
	EXPECT_TRUE(cpy != NULL, "devolvio NULL");
	src[0] = 'Z';
	EXPECT_INT(cpy[0], 'a');
	cpy[1] = 'Y';
	EXPECT_INT(src[1], 'b');
	free(cpy);
	return (0);
}

static int	long_string(void)
{
	char	big[10001];
	char	*cpy;

	memset(big, 'q', 10000);
	big[10000] = '\0';
	cpy = ft_strdup(big);
	EXPECT_TRUE(cpy != NULL, "devolvio NULL");
	EXPECT_INT(strlen(cpy), 10000);
	EXPECT_TRUE(strcmp(cpy, big) == 0, "contenido distinto");
	free(cpy);
	return (0);
}

static int	two_calls(void)
{
	char	*a;
	char	*b;

	a = ft_strdup("same");
	b = ft_strdup("same");
	EXPECT_TRUE(a != NULL && b != NULL, "devolvio NULL");
	EXPECT_TRUE(a != b, "dos llamadas devuelven el mismo puntero");
	free(a);
	free(b);
	return (0);
}

int	main(void)
{
	harness_init();
	T("copias correctas (usa -fsanitize para detectar el '\\0' sin reservar)", basic);
	T("cadena vacia -> puntero valido a \"\"", empty);
	T("la copia es independiente del original", independent_copy);
	T("cadena de 10000 caracteres", long_string);
	T("dos llamadas -> dos bloques distintos", two_calls);
	return (harness_end());
}
