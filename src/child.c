/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 19:05:55 by tjukmong          #+#    #+#             */
/*   Updated: 2023/04/18 21:00:26 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	here_doc(t_process *proc)
{
	char	*gnl;

	while (1)
	{
		ft_putstr_fd("\033[91mPipeX \033[0mhere_doc > ", 1);
		gnl = get_next_line(0);
		if (ft_strlen(gnl) > 1 && ft_strncmp(gnl,
				proc->argv[2], ft_strlen(gnl) - 1) == 0)
		{
			close(proc->pipe[0][0]);
			close(proc->pipe[0][1]);
			free(gnl);
			exit(0);
		}
		ft_putstr_fd(gnl, proc->pipe[0][1]);
		free(gnl);
	}
}

void	infile(t_process *proc)
{
	char	**cmd;

	cmd = ft_split(proc->argv[proc->nproc + 2], ' ');
	close_pipes(proc, 0, 1);
	if (!proc->heredoc)
	{
		close(proc->pipe[0][0]);
		dup2(proc->pipe[0][1], 1);
		close(proc->pipe[0][1]);
		redirr_fd(proc->argv[1], 0, P_READ);
		exec(cmd, proc->envp);
	}
	else
		here_doc(proc);
}

void	command(t_process *proc)
{
	char	**cmd;

	cmd = ft_split(proc->argv[proc->nproc + 2], ' ');
	close_pipes(proc, proc->nproc - 1, 2);
	close(proc->pipe[proc->nproc - 1][1]);
	dup2(proc->pipe[proc->nproc - 1][0], 0);
	close(proc->pipe[proc->nproc - 1][0]);
	close(proc->pipe[proc->nproc - 0][0]);
	dup2(proc->pipe[proc->nproc - 0][1], 1);
	close(proc->pipe[proc->nproc - 0][1]);
	exec(cmd, proc->envp);
}

void	outfile(t_process *proc)
{
	char	**cmd;

	cmd = ft_split(proc->argv[proc->nproc + 2], ' ');
	close_pipes(proc, proc->nproc - 1, 1);
	close(proc->pipe[proc->nproc - 1][1]);
	dup2(proc->pipe[proc->nproc - 1][0], 0);
	close(proc->pipe[proc->nproc - 1][0]);
	redirr_fd(proc->argv[proc->nproc + 3], 1, P_WRITE);
	exec(cmd, proc->envp);
}
