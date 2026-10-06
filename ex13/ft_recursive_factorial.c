/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_recursive_factorial.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazorin <mazorin@student.42barcelona.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 21:15:08 by mazorin           #+#    #+#             */
/*   Updated: 2026/10/05 22:05:24 by mazorin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include <limits.h>

int	ft_recursive_factorial(int nb)
{
	if (nb == 0)
		return (1);
	if (nb < 0)
		return (0);
	nb = nb * ft_recursive_factorial(nb - 1);
}

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