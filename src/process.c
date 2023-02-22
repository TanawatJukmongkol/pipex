/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 22:00:52 by tjukmong          #+#    #+#             */
/*   Updated: 2023/02/22 23:43:33 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	exit_err(t_global *g, char	*msg, int stat)
{
	ft_putstr_fd(msg, 2);
	exit(stat);
}

void	init_precess(t_global *g)
{
	g->nproc = 0;
}

pid_t	spawn_child(t_global *g)
{
	if (!g->proc->pid)
		g->proc->pid = fork();
	
	return (g->proc->pid);
}
