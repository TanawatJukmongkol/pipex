/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 21:41:44 by tjukmong          #+#    #+#             */
/*   Updated: 2023/03/11 14:53:55 by tjukmong         ###   ########.fr       */
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

# define FD_MAX	1000
# define P_READ 1
# define P_WRITE 0

typedef struct s_process
{
	pid_t	pid;
	int		pipe[2];
	size_t	indx;
	int		status;
}				t_process;

typedef struct s_global
{
	int			argc;
	char		**argv;
	char		**envp;
	char		**path;
	t_process	proc;
	pid_t		pids[FD_MAX];
	size_t		nproc;
}				t_global;

//
void	init_global(t_global *g, int argc, char **argv, char **envp);
void	free_global(t_global *g);
void	clear_pipes(t_global *g, size_t indx);
int		pipex_error(t_global *g, char *msg);
int		open_file(t_global *g, char *path, int mode);

//
void	spawn_child(t_global *g);
void	assign_task(t_global *g, ssize_t indx, void task(t_global *));

#endif