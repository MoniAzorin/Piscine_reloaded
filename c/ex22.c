#include "harness.h"
#include "ft_abs.h"

static int	basics(void)
{
	EXPECT_INT(ABS(-5), 5);
	EXPECT_INT(ABS(5), 5);
	EXPECT_INT(ABS(0), 0);
	EXPECT_INT(ABS(-1), 1);
	EXPECT_INT(ABS(42), 42);
	EXPECT_INT(ABS(-42), 42);
	return (0);
}

static int	limits(void)
{
	EXPECT_INT(ABS(INT_MAX), INT_MAX);
	EXPECT_INT(ABS(-INT_MAX), INT_MAX);
	EXPECT_INT(ABS(-3000000000LL), 3000000000LL);
	return (0);
}

static int	variables(void)
{
	int	v;

	v = -9;
	EXPECT_INT(ABS(v), 9);
	EXPECT_INT(ABS(v + 1), 8);
	EXPECT_INT(ABS(-v), 9);
	v = 4;
	EXPECT_INT(ABS(v), 4);
	return (0);
}

static int	arg_precedence(void)
{
	EXPECT_INT(ABS(2 - 5), 3);
	EXPECT_INT(ABS(-2 - 3), 5);
	EXPECT_INT(ABS(1 - 4 + 1), 2);
	EXPECT_INT(ABS((-7)), 7);
	return (0);
}

static int	result_precedence(void)
{
	EXPECT_INT(10 - ABS(-3), 7);
	EXPECT_INT(2 * ABS(-3), 6);
	EXPECT_INT(ABS(-3) * 2, 6);
	EXPECT_INT(-ABS(-3), -3);
	EXPECT_INT(ABS(-3) + ABS(-4), 7);
	EXPECT_INT(ABS(-3) - ABS(-4), -1);
	return (0);
}

static int	nested(void)
{
	EXPECT_INT(ABS(ABS(-4)), 4);
	EXPECT_INT(ABS(ABS(4)), 4);
	EXPECT_INT(ABS(-ABS(-4)), 4);
	return (0);
}

static int	other_types(void)
{
	double	d;

	d = ABS(-2.5);
	EXPECT_TRUE(d == 2.5, "ABS(-2.5) deberia ser 2.5");
	d = ABS(2.5);
	EXPECT_TRUE(d == 2.5, "ABS(2.5) deberia ser 2.5");
	return (0);
}

int	main(void)
{
	harness_init();
	T("casos basicos", basics);
	T("limites (INT_MAX, long long)", limits);
	T("con variables y expresiones", variables);
	T("el argumento esta entre parentesis: ABS(2 - 5) == 3", arg_precedence);
	T("el resultado esta entre parentesis: 2 * ABS(-3) == 6", result_precedence);
	T("ABS anidado", nested);
	T("tipos double", other_types);
	return (harness_end());
}
