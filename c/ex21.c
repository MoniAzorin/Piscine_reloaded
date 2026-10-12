#include "harness.h"

int	*ft_range(int min, int max);

static int	check(int min, int max)
{
	int			*r;
	long long	len;
	long long	i;

	r = ft_range(min, max);
	if (!r)
		FAIL("ft_range(%d, %d) devolvio NULL", min, max);
	len = (long long)max - (long long)min;
	i = 0;
	while (i < len)
	{
		if ((long long)r[i] != (long long)min + i)
			FAIL("ft_range(%d, %d)[%lld] = %d, esperado %lld",
				min, max, i, r[i], (long long)min + i);
		i++;
	}
	free(r);
	return (0);
}

static int	check_null(int min, int max)
{
	int	*r;

	r = ft_range(min, max);
	if (r != NULL)
	{
		free(r);
		FAIL("ft_range(%d, %d) deberia devolver NULL (min >= max)", min, max);
	}
	return (0);
}

static int	positives(void)
{
	TRY(check(0, 5));
	TRY(check(1, 2));
	TRY(check(10, 20));
	TRY(check(42, 43));
	return (0);
}

static int	negatives(void)
{
	TRY(check(-3, 3));
	TRY(check(-1, 0));
	TRY(check(-5, -2));
	TRY(check(-100, -99));
	return (0);
}

static int	max_excluded(void)
{
	int	*r;

	r = ft_range(0, 3);
	EXPECT_TRUE(r != NULL, "devolvio NULL");
	EXPECT_INT(r[0], 0);
	EXPECT_INT(r[1], 1);
	EXPECT_INT(r[2], 2);
	free(r);
	return (0);
}

static int	null_cases(void)
{
	TRY(check_null(5, 5));
	TRY(check_null(0, 0));
	TRY(check_null(5, 0));
	TRY(check_null(1, -1));
	TRY(check_null(-1, -5));
	TRY(check_null(INT_MAX, INT_MIN));
	TRY(check_null(INT_MAX, INT_MAX));
	TRY(check_null(INT_MIN, INT_MIN));
	return (0);
}

static int	limits(void)
{
	TRY(check(INT_MAX - 3, INT_MAX));
	TRY(check(INT_MIN, INT_MIN + 3));
	TRY(check(-2, 2));
	return (0);
}

static int	big_range(void)
{
	return (check(-50000, 50000));
}

int	main(void)
{
	harness_init();
	T("rangos positivos", positives);
	T("rangos negativos y que cruzan el cero", negatives);
	T("min incluido, max excluido", max_excluded);
	T("min >= max -> NULL", null_cases);
	T("limites INT_MAX / INT_MIN sin overflow", limits);
	T("rango grande (100000 elementos)", big_range);
	return (harness_end());
}
