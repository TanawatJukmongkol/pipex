/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 21:41:44 by tjukmong          #+#    #+#             */
/*   Updated: 2023/02/22 23:42:06 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <sys/resource.h>
# include "../lib/libft/libft.h"

typedef struct s_process
{
	pid_t	pid;
	int		pipe[2];
}				t_process;

typedef struct s_global
{
	t_process	*proc;
	size_t		nproc;
}				t_global;

// Processes utility
pid_t	spawn_child(t_global *g);

#endif