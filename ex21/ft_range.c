/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_range.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazorin <mazorin@student.42barcelona.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/14 14:30:32 by mazorin           #+#    #+#             */
/*   Updated: 2026/10/14 15:31:32 by mazorin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

int	*ft_range(int min, int max)
{
	int	*range;
	int	i;

	if (min >= max)
		return (NULL);
	range = (int *)malloc(sizeof(int) * (max - min));
	if (!range)
		return (NULL);
	i = 0;
	while (min < max)
	{
		range[i] = min;
		i++;
		min++;
	}
	return (range);
}
/*
static void	test(int min, int max)
{
	int	*r;
	int	i;

	printf("ft_range(%d, %d): ", min, max);
	r = ft_range(min, max);
	if (r == NULL)
	{
		printf("NULL\n");
		return ;
	}
	i = 0;
	while (i < max - min)
	{
		printf("%d ", r[i]);
		i++;
	}
	printf("\n");
	free(r);
}

int	main(void)
{
	test(3, 8);
	test(-2, 3);
	test(0, 1);
	test(5, 5);
	test(8, 3);
	return (0);
}
*/