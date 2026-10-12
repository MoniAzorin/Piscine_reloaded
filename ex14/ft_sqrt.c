/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sqrt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazorin <mazorin@student.42barcelona.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/11 22:00:19 by mazorin           #+#    #+#             */
/*   Updated: 2026/10/12 20:03:22 by mazorin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <assert.h>
#include <limits.h>

int	ft_sqrt(int nb)
{
	int	i;

	if (nb < 1)
		return (0);
	i = 1;
	while (i <= nb / i)
	{
		if (i * i == nb)
			return (i);
		i++;
	}
	return (0);
}
/*
int	main(void)
{
	assert(ft_sqrt(1) == 1);
	assert(ft_sqrt(9) == 3);
	assert(ft_sqrt(2147395600) == 46340);

	assert(ft_sqrt(3) == 0);
	assert(ft_sqrt(INT_MAX) == 0);

	assert(ft_sqrt(0) == 0);
	assert(ft_sqrt(-1) == 0);
	assert(ft_sqrt(INT_MIN) == 0);

	printf("Todos los tests de ft_sqrt han pasado\n");
	return (0);
}
*/