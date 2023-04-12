/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/23 15:42:45 by tjukmong          #+#    #+#             */
/*   Updated: 2023/02/17 19:12:50 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

char	*get_next_line(int fd)
{
	static t_files	files[FD_MAX];
	size_t			indx;
	size_t			slen;
	char			*str;

	if (fd < 0 || files[fd].stop)
		return (NULL);
	if (!files[fd].buf)
	{
		files[fd].buf = NULL;
		files[fd].size = 0;
		files[fd].offset = 0;
	}
	if (read_next(&files[fd], fd))
	{
		files[fd].stop = 1;
		free(files[fd].buf);
		return (NULL);
	}
	if (files[fd].offset >= files[fd].size)
	{
		files[fd].stop = 1;
		free(files[fd].buf);
		return (NULL);
	}
	indx = 0;
	slen = strlen_chr(files[fd].buf + files[fd].offset, '\n');
	if (slen == BUFFER_SIZE && !files[fd].eof)
		read_next(&files[fd], fd);
	if (files[fd].buf[slen] != '\0')
		slen++;
	if (slen == 0)
	{
		files[fd].stop = 1;
		free(files[fd].buf);
		return (NULL);
	}
	str = malloc(slen + 1);
	if (!str)
	{
		files[fd].stop = 1;
		free(files[fd].buf);
		return (NULL);
	}
	str[slen] = '\0';
	while (indx < slen)
	{
		str[indx] = *(files[fd].buf + files[fd].offset);
		indx++;
		files[fd].offset++;
	}
	if (files[fd].eof && files[fd].offset >= files[fd].size)
	{
		files[fd].stop = 1;
		free(files[fd].buf);
	}
	return (str);
}
