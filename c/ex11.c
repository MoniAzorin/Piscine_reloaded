#include "harness.h"

void	ft_div_mod(int a, int b, int *div, int *mod);

static int	check(int a, int b)
{
	int	guard[4];
	int	d;
	int	m;

	guard[0] = 111;
	guard[1] = 222;
	guard[2] = 333;
	guard[3] = 444;
	d = 777;
	m = 888;
	ft_div_mod(a, b, &d, &m);
	if (d != a / b || m != a % b)
		FAIL("ft_div_mod(%d, %d): div=%d mod=%d, esperado div=%d mod=%d",
			a, b, d, m, a / b, a % b);
	return (0);
}

static int	positives(void)
{
	TRY(check(42, 5));
	TRY(check(10, 3));
	TRY(check(100, 10));
	TRY(check(7, 7));
	TRY(check(0, 5));
	TRY(check(4, 5));
	TRY(check(1, 1));
	return (0);
}

static int	negatives(void)
{
	TRY(check(-42, 5));
	TRY(check(42, -5));
	TRY(check(-42, -5));
	TRY(check(-7, 2));
	TRY(check(7, -2));
	TRY(check(-1, 3));
	return (0);
}

static int	limits(void)
{
	TRY(check(INT_MAX, 1));
	TRY(check(INT_MAX, INT_MAX));
	TRY(check(INT_MAX, -1));
	TRY(check(INT_MIN, 1));
	TRY(check(INT_MIN, 2));
	TRY(check(INT_MIN, INT_MAX));
	TRY(check(INT_MIN, -2));
	TRY(check(1, INT_MAX));
	return (0);
}

static int	no_side_effects(void)
{
	int	r[4];

	r[0] = 1;
	r[1] = 2;
	r[2] = 3;
	r[3] = 4;
	ft_div_mod(17, 5, &r[1], &r[2]);
	EXPECT_INT(r[0], 1);
	EXPECT_INT(r[1], 3);
	EXPECT_INT(r[2], 2);
	EXPECT_INT(r[3], 4);
	return (0);
}

int	main(void)
{
	harness_init();
	T("positivos", positives);
	T("negativos (truncado hacia cero, como C)", negatives);
	T("limites INT_MIN / INT_MAX", limits);
	T("solo escribe en div y mod", no_side_effects);
	return (harness_end());
}
