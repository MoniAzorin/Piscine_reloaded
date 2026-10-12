#include "harness.h"

void	ft_ft(int *nbr);

static int	from_zero(void)
{
	int	n;

	n = 0;
	ft_ft(&n);
	EXPECT_INT(n, 42);
	return (0);
}

static int	overwrites(void)
{
	int	n;

	n = -7;
	ft_ft(&n);
	EXPECT_INT(n, 42);
	n = INT_MAX;
	ft_ft(&n);
	EXPECT_INT(n, 42);
	n = INT_MIN;
	ft_ft(&n);
	EXPECT_INT(n, 42);
	return (0);
}

static int	neighbours(void)
{
	int	a[3];

	a[0] = 1;
	a[1] = 2;
	a[2] = 3;
	ft_ft(&a[1]);
	EXPECT_INT(a[0], 1);
	EXPECT_INT(a[1], 42);
	EXPECT_INT(a[2], 3);
	return (0);
}

int	main(void)
{
	harness_init();
	T("pone 42 desde 0", from_zero);
	T("sobrescribe valores previos (-7, INT_MAX, INT_MIN)", overwrites);
	T("no toca las posiciones vecinas", neighbours);
	return (harness_end());
}
