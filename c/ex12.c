#include "harness.h"

int	ft_iterative_factorial(int nb);

static int	known_values(void)
{
	static const int	tab[][2] = {{0, 1}, {1, 1}, {2, 2}, {3, 6}, {4, 24},
	{5, 120}, {6, 720}, {7, 5040}, {8, 40320}, {9, 362880}, {10, 3628800},
	{11, 39916800}, {12, 479001600}};
	size_t				i;
	int					got;

	i = 0;
	while (i < sizeof(tab) / sizeof(tab[0]))
	{
		got = ft_iterative_factorial(tab[i][0]);
		if (got != tab[i][1])
			FAIL("ft_iterative_factorial(%d) = %d, esperado %d",
				tab[i][0], got, tab[i][1]);
		i++;
	}
	return (0);
}

static int	zero_is_one(void)
{
	EXPECT_INT(ft_iterative_factorial(0), 1);
	return (0);
}

static int	negatives(void)
{
	EXPECT_INT(ft_iterative_factorial(-1), 0);
	EXPECT_INT(ft_iterative_factorial(-5), 0);
	EXPECT_INT(ft_iterative_factorial(-100), 0);
	EXPECT_INT(ft_iterative_factorial(INT_MIN), 0);
	return (0);
}

static int	overflow_13(void)
{
	EXPECT_INT(ft_iterative_factorial(13), 0);
	return (0);
}

static int	overflow_big(void)
{
	EXPECT_INT(ft_iterative_factorial(20), 0);
	EXPECT_INT(ft_iterative_factorial(INT_MAX), 0);
	return (0);
}

int	main(void)
{
	harness_init();
	T("factoriales de 0 a 12", known_values);
	T("0! = 1", zero_is_one);
	T("negativos -> 0 (error)", negatives);
	TW("13! no cabe en un int -> deberia devolver 0 (error)", overflow_13);
	TW("20! e INT_MAX: 0 y en menos de 2 s", overflow_big);
	return (harness_end());
}
