/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 21:42:33 by tjukmong          #+#    #+#             */
/*   Updated: 2023/02/23 07:42:13 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv)
{
	static t_global	g;
	char			*nl;

	init_precess(&g);
	spawn_child(&g, "ls", "");

	begin_pipe(&g);
	if (g.proc->pid == 0)
	{
		// ft_putstr_fd("Hello, test!", 1);
		execve("/bin/ls", (char **)("ls", NULL), (char **)(NULL));
	}
	end_pipe(&g);

	return (end_process(&g, 0));
}
