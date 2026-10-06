/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazorin <mazorin@student.42barcelona.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 10:34:42 by mazorin           #+#    #+#             */
/*   Updated: 2026/10/06 11:20:45 by mazorin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}


int	main(void)
{
	printf("Iguales: %d\n", ft_strcmp("Hola", "Hola"));
	printf("Primera menor: %d\n", ft_strcmp("abc", "abd"));
	printf("Primera mayor: %d\n", ft_strcmp("abd", "abc"));
	printf("Prefijo mas corto primero: %d\n", ft_strcmp("ab", "abc"));
	printf("Cadenas vacias: %d\n", ft_strcmp("", ""));
	printf("sizeof(\"Hola\") = %zu bytes\n", sizeof("Hola"));
	return (0);
}

