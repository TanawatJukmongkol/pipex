/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 21:41:44 by tjukmong          #+#    #+#             */
/*   Updated: 2023/02/25 18:58:52 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "../lib/libft/libft.h"
# include "../lib/get_next_line/get_next_line.h"

typedef struct s_process
{
	pid_t	pid;
	int		pipe[2];
	size_t	indx;
}				t_process;

typedef struct s_global
{
	// processes
	t_process	proc;
	size_t		nproc;
	// std out
	int			*main_pipe;
}				t_global;

// Processes utility
void	init_precess(t_global *g);
void	close_proc(t_global *g, ssize_t indx, int status);
void	exit_fail(t_global *g, char *msg);
void	spawn_child(t_global *g);
void	assign_task(t_global *g, ssize_t indx, void task(t_global *g));

#endif