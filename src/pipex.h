/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 21:41:44 by tjukmong          #+#    #+#             */
/*   Updated: 2023/04/15 19:19:36 by tjukmong         ###   ########.fr       */
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

//
typedef struct s_process
{
	int		argc;
	char	**argv;
	char	**envp;
	int		heredoc;

	int		pipe[FD_MAX >> 1][2];
	pid_t	fork[FD_MAX >> 2];
	size_t	indx;
	size_t	nproc;
	int		exit_stats[FD_MAX >> 2];
}				t_process;

// Processes
void	wait_process(t_process *proc);
void	spawn_process(t_process *proc, void (*fn)(t_process *p));
void	create_pipes(t_process *proc, size_t npipe);
pid_t	redirr_fd(char *name, int fd, int mode);
void	exec(char **cmd, char **envp);

// Utils
size_t	arr2d_len(void **array);
void	free_2d_arr(void **array);
char	*get_access_path(char *p, char *cmd);
char	*get_path(char **env, char *cmd);
void	close_pipes(t_process *proc, size_t a, size_t b);

// Child process
void	here_doc(t_process *proc);
void	infile(t_process *proc);
void	command(t_process *proc);
void	outfile(t_process *proc);

#endif
