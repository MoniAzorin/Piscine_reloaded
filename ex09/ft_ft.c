
#include <limits.h>
#include <stdio.h>

void	ft_ft(int *nbr)
{
	*nbr = 42;
}

int	main(void)
{
	int	cases[] = {0, 1, -1, INT_MIN, INT_MAX};
	int	i;

	i = 0;
	while (i < 5)
	{
		ft_ft(&cases[i]);
		printf("%d\n", cases[i]);
		i++;
	}
	return (0);
}