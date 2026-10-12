#include "harness.h"

void	ft_swap(int *a, int *b);

static int	check(int a, int b)
{
	int	x;
	int	y;

	x = a;
	y = b;
	ft_swap(&x, &y);
	if (x != b || y != a)
		FAIL("ft_swap(%d, %d) -> a=%d b=%d, esperado a=%d b=%d",
			a, b, x, y, b, a);
	return (0);
}

static int	basic(void)
{
	TRY(check(1, 2));
	TRY(check(42, 21));
	TRY(check(0, 5));
	TRY(check(5, 0));
	return (0);
}

static int	negatives(void)
{
	TRY(check(-1, 1));
	TRY(check(-5, -9));
	TRY(check(0, -1));
	return (0);
}

static int	equal_values(void)
{
	return (check(7, 7));
}

static int	limits(void)
{
	TRY(check(INT_MIN, INT_MAX));
	TRY(check(INT_MAX, INT_MIN));
	TRY(check(INT_MIN, 0));
	TRY(check(INT_MAX, INT_MAX));
	return (0);
}

static int	same_pointer(void)
{
	int	x;

	x = 5;
	ft_swap(&x, &x);
	EXPECT_INT(x, 5);
	return (0);
}

static int	twice_restores(void)
{
	int	a;
	int	b;

	a = 3;
	b = 8;
	ft_swap(&a, &b);
	ft_swap(&a, &b);
	EXPECT_INT(a, 3);
	EXPECT_INT(b, 8);
	return (0);
}

static int	neighbours(void)
{
	int	t[4];

	t[0] = 10;
	t[1] = 20;
	t[2] = 30;
	t[3] = 40;
	ft_swap(&t[1], &t[2]);
	EXPECT_INT(t[0], 10);
	EXPECT_INT(t[1], 30);
	EXPECT_INT(t[2], 20);
	EXPECT_INT(t[3], 40);
	return (0);
}

int	main(void)
{
	harness_init();
	T("intercambia valores normales", basic);
	T("valores negativos y cero", negatives);
	T("valores iguales", equal_values);
	T("limites INT_MIN / INT_MAX (sin overflow)", limits);
	T("ft_swap(&x, &x) no destruye el valor (cuidado con XOR)", same_pointer);
	T("dos swaps restauran los valores", twice_restores);
	T("no toca la memoria vecina", neighbours);
	return (harness_end());
}
