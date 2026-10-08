/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_recursive_factorial.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazorin <mazorin@42barcelona.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 21:15:08 by mazorin           #+#    #+#             */
/*   Updated: 2026/10/08 19:35:56 by mazorin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include <limits.h>

int	ft_recursive_factorial(int nb)
{
	if (nb == 0)
		return (1);
	else if (nb < 0 || nb > 12)
		return (0);
	return (nb = nb * ft_recursive_factorial(nb - 1));
}
/*
int	main(void)
{
	assert(ft_recursive_factorial(-1) == 0);
	assert(ft_recursive_factorial(0) == 1);
	assert(ft_recursive_factorial(1) == 1);
	assert(ft_recursive_factorial(12) == 479001600);
	assert(ft_recursive_factorial(13) == 0);
	assert(ft_recursive_factorial(INT_MIN) == 0);
	return (0);
}
*/