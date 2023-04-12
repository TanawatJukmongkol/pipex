/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/23 15:42:31 by tjukmong          #+#    #+#             */
/*   Updated: 2023/02/17 17:13:59 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <stddef.h>
# include <unistd.h>
# include <stdlib.h>

# define FD_MAX	1000
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

typedef struct s_files
{
	char	*buf;
	size_t	size;
	size_t	offset;
	int		stop;
	int		eof;
}				t_files;

char	*get_next_line(int fd);

size_t	strlen_chr(const char *str, const char c);
char	*ft_recat(char **dst, char *str);
int		read_next(t_files *file, int fd);

#endif
