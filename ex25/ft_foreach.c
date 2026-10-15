/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_foreach.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazorin <mazorin@student.42barcelona.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 10:54:33 by mazorin           #+#    #+#             */
/*   Updated: 2026/10/15 12:18:48 by mazorin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <unistd.h>

/*void	ft_putnbr(int nb)
{
	unsigned int	num;
	char	c;

	if (nb < 0)
	{
		write(1, "-", 1);
		num = -(unsigned int)nb;
	}
	else
		num = nb;
	if (num > 9)
		ft_putnbr(num / 10);
	c = (num % 10) + '0';
	write(1, &c, 1);
}
*/
void	ft_foreach(int *tab, int lenght, void (*f)(int))
{
	int	i;

	i = 0;
	while (i < lenght)
	{
		f(tab[i]);
		i++;
	}
}

/*int	main(void)
{
	int	tests[] = {0, -1, 9, 10, 42, -42, INT_MAX, INT_MIN};

	ft_foreach(tests, 8, &ft_putnbr);

	return (0);
}*/