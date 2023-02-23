/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 22:00:52 by tjukmong          #+#    #+#             */
/*   Updated: 2023/02/23 08:58:10 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	init_precess(t_global *g)
{
	g->proc = malloc(sizeof(t_process));
	g->nproc = 0;
}

int	end_process(t_global *g, int stat)
{
	close(g->proc->pipe[1]);
	free(g->proc);
	if (g->out)
		free(g->out);
	g->nproc--;
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

void	begin_pipe(t_global *g)
{
	if (g->proc->pid == 0)
	{
		close(g->proc->pipe[0]);
		dup2(g->proc->pipe[1], 1);
		close(g->proc->pipe[1]);
	}
}

void	end_pipe(t_global *g)
{
	char	*next;

	if (g->proc->pid > 0)
	{
		close(g->proc->pipe[1]);
		next = get_next_line(g->proc->pipe[0]);
		if (!g->out)
			g->out = NULL;
		while (next)
		{
			ft_recat(&g->out, next);
			next = get_next_line(g->proc->pipe[0]);
		}
		close(g->proc->pipe[0]);
	}
}
