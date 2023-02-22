/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 22:00:52 by tjukmong          #+#    #+#             */
/*   Updated: 2023/02/23 04:45:24 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	init_precess(t_global *g)
{
	g->nproc = 0;
	g->proc = malloc(sizeof(t_process));
}

int	end_process(t_global *g, int stat)
{
	close(g->proc->pipe[1]);
	free(g->proc);
	exit(stat);
	return (0);
}

void	exit_err(t_global *g, char	*msg)
{
	ft_putstr_fd(msg, 2);
	end_process(g, 1);
}

int	spawn_child(t_global *g, char *cmd, char *arg)
{
	if (!g->proc->pid)
	{
		if (pipe(g->proc->pipe) < 0)
			return (-1);
		g->proc->pid = fork();
		if (g->proc->pid < 0)
			return (-1);
	}
	g->nproc++;
	return (1);
}

void	write_pipe(t_global *g)
{
	if (g->proc->pid == 0)
	{
		close(g->proc->pipe[0]);
		dup2(g->proc->pipe[1], 0);
		close(g->proc->pipe[1]);
	}
}

void	read_pipe(t_global *g)
{
	if (g->proc->pid > 0)
	{
		dup2(g->proc->pipe[1], 1);
		close(g->proc->pipe[0]);
		close(g->proc->pipe[1]);
	}
}
