/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazorin <mazorin@student.42barcelona.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 10:34:42 by mazorin           #+#    #+#             */
/*   Updated: 2026/10/12 23:29:45 by mazorin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <assert.h>
#include <string.h>

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 || *s2)
	{
		if ((unsigned char)*s1 > (unsigned char)*s2)
			return (1);
		else if ((unsigned char)*s1 < (unsigned char)*s2)
			return (-1);
		s1++;
		s2++;
	}
	return (0);
}

/*
int	main(void)
{
	assert(ft_strcmp("", "") == strcmp("", ""));
	assert(ft_strcmp("a", "") == strcmp("a", ""));
	assert(ft_strcmp("", "a") == strcmp("", "a"));
	assert(ft_strcmp("abc", "abc") == strcmp("abc", "abc"));
	assert(ft_strcmp("abc", "abd") == strcmp("abc", "abd"));
	assert(ft_strcmp("abd", "abc") == strcmp("abd", "abc"));
	assert(ft_strcmp("abc", "ab") == strcmp("abc", "ab"));
	assert(ft_strcmp("ab", "abc") == strcmp("ab", "abc"));
	assert(ft_strcmp("Hello", "hello") == strcmp("Hello", "hello"));
	assert(ft_strcmp("\xff", "a") == strcmp("\xff", "a"));
	assert(ft_strcmp("a", "\xff") == strcmp("a", "\xff"));
	assert(ft_strcmp("\x80", "\x7f") == strcmp("\x80", "\x7f"));
	assert(ft_strcmp("abc\xff", "abc\x01") == strcmp("abc\xff", "abc\x01"));
	return (0);
}
*/