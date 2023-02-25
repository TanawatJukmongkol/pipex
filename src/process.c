/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 22:00:52 by tjukmong          #+#    #+#             */
/*   Updated: 2023/02/25 04:53:44 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	init_precess(t_global *g)
{
	g->nproc = 0;
	g->proc.pid = 0;
}

void	close_proc(t_global *g, ssize_t indx, int status)
{
	g->nproc--;
	if (g->nproc == indx || indx < 0)
		exit(status);
}

void	exit_fail(t_global *g, char *msg)
{
	ft_putstr_fd(msg, 2);
	close_proc(g, -1, 1);
}

void	spawn_child(t_global *g)
{
	if (!g->proc.pid)
	{
		if (pipe(g->proc.pipe) < 0)
			exit_fail(g, "failed to pipe");
		g->proc.pid = fork();
		if (g->proc.pid < 0)
			exit_fail(g, "failed to fork");
		g->nproc++;
		if (g->proc.pid == 0)
			g->proc.indx = g->nproc;
	}
	else
		g->nproc++;
}

void	assign_task(t_global *g, ssize_t indx, void task(t_global *g))
{
	if (g->proc.indx == indx || indx == -1)
		task(g);
}
