#include "harness.h"

void	ft_is_negative(int n);

static int	check(int n, const char *exp)
{
	char	buf[32];

	cap_start();
	ft_is_negative(n);
	cap_end(buf, sizeof(buf));
	if (strcmp(buf, exp) != 0)
		FAIL("ft_is_negative(%d) imprimio \"%s\", esperado \"%s\"",
			n, esc(buf, 0), exp);
	return (0);
}

static int	negatives(void)
{
	TRY(check(-1, "N"));
	TRY(check(-42, "N"));
	TRY(check(-1337, "N"));
	return (0);
}

static int	zero(void)
{
	return (check(0, "P"));
}

static int	positives(void)
{
	TRY(check(1, "P"));
	TRY(check(42, "P"));
	TRY(check(1337, "P"));
	return (0);
}

static int	limits(void)
{
	TRY(check(INT_MIN, "N"));
	TRY(check(INT_MIN + 1, "N"));
	TRY(check(INT_MAX, "P"));
	TRY(check(INT_MAX - 1, "P"));
	return (0);
}

static int	sequence(void)
{
	char	buf[32];

	cap_start();
	ft_is_negative(-1);
	ft_is_negative(0);
	ft_is_negative(1);
	cap_end(buf, sizeof(buf));
	EXPECT_STR(buf, "NPP");
	return (0);
}

int	main(void)
{
	harness_init();
	T("negativos (-1, -42, -1337) -> N", negatives);
	T("cero -> P", zero);
	T("positivos (1, 42, 1337) -> P", positives);
	T("limites INT_MIN / INT_MAX", limits);
	T("sin salto de linea: -1,0,1 -> NPP", sequence);
	return (harness_end());
}
