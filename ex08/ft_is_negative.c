/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_negative.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazorin <mazorin@student.42barcelona.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/04 16:04:08 by mazorin           #+#    #+#             */
/*   Updated: 2026/10/04 18:24:40 by mazorin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putchar(char c);

void	ft_is_negative(int n)
{
	if (n < 0)
		write(1, 'N', 1);
	else
		write(1, 'P', 1);
}
/*
void	ft_putchar(char c)
	write(1, &c, 1);
}

int main(void)
{
    ft_is_negative(-5);
    ft_is_negative(0);
    ft_is_negative(5);
    return (0);
}
*/