/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_div_mod.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazorin <mazorin@42barcelona.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/08 18:08:42 by mazorin           #+#    #+#             */
/*   Updated: 2026/10/08 18:08:49 by mazorin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <assert.h>

void	ft_div_mod(int a, int b, int *div, int *mod)
{
	*div = a / b;
	*mod = a % b;
}
/*
int	main(void)
{
	int div;
	int mod;

	ft_div_mod(0, 5, &div, &mod);
	assert(div == 0 && mod == 0);
	ft_div_mod(0, -5, &div, &mod);
	assert(div == 0 && mod == 0);
	return (0);
}
*/