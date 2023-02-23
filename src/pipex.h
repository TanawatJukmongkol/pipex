/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 21:41:44 by tjukmong          #+#    #+#             */
/*   Updated: 2023/02/23 08:03:34 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <unistd.h>
# include <sys/wait.h>
# include "../lib/libft/libft.h"
# include "../lib/get_next_line/get_next_line.h"

typedef struct s_process
{
	pid_t	pid;
	int		pipe[2];
	char	*cmd;
}				t_process;

typedef struct s_global
{
	t_process	*proc;
	size_t		nproc;
	char		*out;
	int			pout;
}				t_global;

// Processes utility
void	init_precess(t_global *g);
int		end_process(t_global *g, int stat);
int		spawn_child(t_global *g, char *cmd, char *arg);
void	begin_pipe(t_global *g);
void	end_pipe(t_global *g);

#endif