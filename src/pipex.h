/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 21:41:44 by tjukmong          #+#    #+#             */
/*   Updated: 2023/03/04 01:30:02 by tjukmong         ###   ########.fr       */
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
	int		status;
}				t_process;

typedef struct s_global
{
	// main
	int			argc;
	char		**argv;
	// processes
	t_process	proc;
	size_t		nproc;
}				t_global;

// Processes utility
void	init_precess(t_global *g, int argc, char **argv);
void	close_proc(t_global *g, ssize_t indx, int status);
void	exit_fail(t_global *g, char *msg);
void	spawn_child(t_global *g);
void	assign_task(t_global *g, ssize_t indx, void task(t_global *g));

#endif