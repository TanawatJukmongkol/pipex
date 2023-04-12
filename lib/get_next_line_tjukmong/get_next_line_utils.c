/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/29 12:58:11 by tjukmong          #+#    #+#             */
/*   Updated: 2023/02/17 17:20:04 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

size_t	strlen_chr(const char *str, const char c)
{
	size_t	len;

	if (!str)
		return (0);
	len = 0;
	while (str[len] != '\0' && str[len] != c)
		len++;
	return (len);
}

char	*ft_recat(char **dst, char *str)
{
	size_t	dlen;
	size_t	slen;
	char	*tmp;

	dlen = 0;
	slen = 0;
	if (!str)
		return (*dst);
	if (!dst || *dst == NULL)
	{
		*dst = str;
		return (*dst);
	}
	dlen = strlen_chr(*dst, '\0');
	slen = strlen_chr(str, '\0');
	tmp = malloc(dlen + slen + 1);
	tmp[dlen + slen] = '\0';
	while (slen-- > 0)
		tmp[dlen + slen] = str[slen];
	free(str);
	while (dlen-- > 0)
		tmp[dlen] = (*dst)[dlen];
	free(*dst);
	*dst = tmp;
	return (*dst);
}

int	read_next(t_files *file, int fd)
{
	ssize_t	read_len;
	char	*tmp;

	read_len = 1;
	while (read_len)
	{
		tmp = malloc(BUFFER_SIZE + 1);
		read_len = read(fd, tmp, BUFFER_SIZE);
		if (read_len < 0)
		{
			free(tmp);
			return (1);
		}
		tmp[read_len] = '\0';
		file->size += read_len;
		ft_recat(&file->buf, tmp);
		if ((ssize_t)strlen_chr(file->buf, '\n') < read_len)
			return (0);
	}
	file->eof = 1;
	return (0);
}