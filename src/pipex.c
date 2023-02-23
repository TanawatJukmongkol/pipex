/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 21:42:33 by tjukmong          #+#    #+#             */
/*   Updated: 2023/02/23 13:32:29 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv)
{
	t_global	g;
	char		*args[] = {"ls", NULL};
	char		*env[] = {"", NULL};

	init_precess(&g);
	spawn_child(&g, "ls", "arg1"); // TODO: Implement feature.
	//  Note to self: Stop procrastinating!

	begin_pipe(&g);
	if (g.proc.pid == 0)
	{
		execve("/bin/ls", args, env);
		end_process(&g, 0);
	}
	end_pipe(&g);

	if (g.proc.pid > 0)
	{
		ft_putstr_fd(g.out, 1);
		wait(NULL);
	}

	return (end_process(&g, 0));
}
