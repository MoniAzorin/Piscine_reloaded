/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_if.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazorin <mazorin@student.42barcelona.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 12:20:12 by mazorin           #+#    #+#             */
/*   Updated: 2026/10/15 13:05:51 by mazorin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

/*int ft_comparison(char *str)
{
    while (*str)
    {
        if (*str == 'a')
            return (1);
        str++;
    }
	return (0);
} */

int	ft_count_if(char **tab, int (*f)(char*))
{
	int	count;

	count = 0;
	while (*tab)
	{
		if (f(*tab) == 1)
			count++;
		tab++;
	}
	return (count);
}

/*int	main(void)
{
	char	*tab[]= {"Hola", "prova", NULL};
	printf("%d", ft_count_if(tab,&ft_comparison));
	return (0);
}*/