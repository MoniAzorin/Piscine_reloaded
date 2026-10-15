/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mazorin <mazorin@student.42barcelona.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 13:08:39 by mazorin           #+#    #+#             */
/*   Updated: 2026/10/15 14:18:35 by mazorin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>

int	ft_printerror(char *str)
{
	while (*str)
	{
		write(2, str, 1);
		str++;
	}
	return (1);
}

int	ft_display_file(char *filename)
{
	int	fd;
	int		n;
	char	buf[4096];

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (ft_printerror("Cannot read file.\n"));
	n = read(fd, buf, sizeof(buf));
	if(n == -1)
		return (ft_printerror("Cannot read file.\n"));
	write(1, &buf, n);
	close(fd);
	return(0);
}

int	main(int argc, char **argv)
{
	if (argc < 2)
		return (ft_printerror("File name missing.\n"));
	else if (argc > 2)
		return (ft_putstr("Too many arguments.\n"));
	else
		return(ft_display_file(argv[1]));
	return (0);
}
