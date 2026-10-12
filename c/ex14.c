#include "harness.h"

int	ft_sqrt(int nb);

static int	check(int in, int out)
{
	int	got;

	got = ft_sqrt(in);
	if (got != out)
		FAIL("ft_sqrt(%d) = %d, esperado %d", in, got, out);
	return (0);
}

static int	perfect_squares(void)
{
	TRY(check(0, 0));
	TRY(check(1, 1));
	TRY(check(4, 2));
	TRY(check(9, 3));
	TRY(check(16, 4));
	TRY(check(25, 5));
	TRY(check(100, 10));
	TRY(check(144, 12));
	TRY(check(10000, 100));
	TRY(check(1000000, 1000));
	return (0);
}

static int	not_squares(void)
{
	TRY(check(2, 0));
	TRY(check(3, 0));
	TRY(check(5, 0));
	TRY(check(8, 0));
	TRY(check(15, 0));
	TRY(check(17, 0));
	TRY(check(99, 0));
	TRY(check(101, 0));
	return (0);
}

static int	negatives(void)
{
	TRY(check(-1, 0));
	TRY(check(-4, 0));
	TRY(check(-100, 0));
	TRY(check(INT_MIN, 0));
	return (0);
}

static int	big_squares(void)
{
	TRY(check(2147395600, 46340));
	TRY(check(2147302921, 46339));
	TRY(check(2147395599, 0));
	TRY(check(2147395601, 0));
	return (0);
}

static int	int_max(void)
{
	TRY(check(INT_MAX, 0));
	TRY(check(INT_MAX - 1, 0));
	return (0);
}

int	main(void)
{
	harness_init();
	T("cuadrados perfectos", perfect_squares);
	T("no cuadrados -> 0", not_squares);
	T("negativos -> 0", negatives);
	T("cuadrados grandes (46340^2) y vecinos", big_squares);
	T("INT_MAX en menos de 2 s (sin overflow de i*i)", int_max);
	return (harness_end());
}
