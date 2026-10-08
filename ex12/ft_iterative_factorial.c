/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_iterative_factorial.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazorin <mazorin@student.42barcelona.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 12:28:03 by mazorin           #+#    #+#             */
/*   Updated: 2026/10/08 17:49:39 by mazorin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include <limits.h>

int	ft_iterative_factorial(int nb)
{
	int	result;

	result = 1;
	if (nb < 0)
		return (0);
	while (nb > 0)
	{
		if (result > INT_MAX / nb)
			return (0);
		result *= nb;
		nb--;
	}
	return (result);
}
/*
int	main(void)
{
	assert(ft_iterative_factorial(-1) == 0);
	assert(ft_iterative_factorial(0) == 1);
	assert(ft_iterative_factorial(1) == 1);
	assert(ft_iterative_factorial(5) == 120);
	assert(ft_iterative_factorial(13) == 0);
	assert(ft_iterative_factorial(INT_MIN) == 0);
	return (0);
}
*/
