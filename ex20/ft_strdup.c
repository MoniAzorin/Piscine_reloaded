/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazorin <mazorin@student.42barcelona.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/14 12:46:41 by mazorin           #+#    #+#             */
/*   Updated: 2026/10/14 18:26:52 by mazorin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <assert.h>
#include <stdlib.h>
#include <string.h>

int	ft_strlen(char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

char	*ft_strcpy(char *dest, char *src)
{
	int	i;

	i = 0;
	while (*src)
	{
		dest[i] = *src;
		src++;
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strdup(char *src)
{
	char	*dup;

	dup = malloc(sizeof(char) * (ft_strlen(src) + 1));
	if (!dup)
		return (NULL);
	return (ft_strcpy(dup, src));
}

/*int	main(void)
{
	char	*a;
	char	*b;

	a = ft_strdup("hello");
	b = strdup("hello");
	assert(strcmp(a, b) == 0);
	free(a);
	free(b);

	a = ft_strdup("");
	b = strdup("");
	assert(strcmp(a, b) == 0);
	free(a);
	free(b);

	return (0);
}*/