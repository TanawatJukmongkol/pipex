/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 21:42:33 by tjukmong          #+#    #+#             */
/*   Updated: 2023/02/23 09:39:36 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv)
{
	t_process	proc;
	t_global	g;
	char			*nl;

	init_precess(&g);
	spawn_child(&g, "ls", "arg1"); // TODO: Implement feature.
	//  Note to self: Stop procrastinating!

	begin_pipe(&g);
	if (g.proc->pid == 0)
	{
		execve("/bin/ls", (char **)("ls", NULL), (char **)(NULL));
		end_process(&g, 0);
	}
	end_pipe(&g);

	if (g.proc->pid > 0)
	{
		ft_putstr_fd(g.out, 1);
		wait(NULL);
	}

	return (end_process(&g, 0));
}
