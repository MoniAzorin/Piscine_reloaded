/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazorin <mazorin@student.42barcelona.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 10:19:33 by mazorin           #+#    #+#             */
/*   Updated: 2026/10/06 10:28:41 by mazorin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>

int	ft_strlen(char *str)
{
	int	len;

	len = 0;
	while (*str)
	{
		len++;
		str++;
	}
	return (len);
}

int	main(void)
{
	assert(ft_strlen("") == 0);
	assert(ft_strlen("a") == 1);
	assert(ft_strlen("Hola") == 4);
	assert(ft_strlen("  x ") == 4);
	return (0);
}
