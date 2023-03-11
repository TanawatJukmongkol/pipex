/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 22:00:52 by tjukmong          #+#    #+#             */
/*   Updated: 2023/03/11 18:49:12 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	close_proc(t_global *g, ssize_t indx, int status)
{
	g->nproc--;
	if (g->nproc == indx || indx < 0)
		exit(status);
}

void	spawn_child(t_global *g)
{
	if (!g->nproc || g->proc.pid > 0)
	// if ( !g->proc.pid)
	{
		if (pipe(g->proc.pipe) < 0)
			pipex_error(g, "Broken pipe.");
		g->proc.pid = fork();
		if (g->proc.pid < 0)
			pipex_error(g, "fork failed: Resource temporarily unavailable");
		g->nproc++;
		if (g->proc.pid == 0)
			g->proc.indx = g->nproc;
	}
	else
		g->nproc++;
	g->pids[g->nproc - 1] = g->proc.pid;
}

void	assign_task(t_global *g, ssize_t indx, void task(t_global *g))
{
	if (g->proc.indx == indx && g->proc.pid == 0)
	// if (g->proc.indx == indx && g->proc.pid > 0)
		task(g);
}
