/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 21:42:33 by tjukmong          #+#    #+#             */
/*   Updated: 2023/03/06 03:47:36 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	open_file(char *path, int mode)
{
	if (mode == 1)
	{
		if (access(path, F_OK))
		{
			ft_putstr_fd("PipeX: ", 2);
			ft_putstr_fd(path, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			return (-1);
		}
		return (open(path, O_RDONLY));
	}
	else
		return (open(path, O_CREAT | O_WRONLY | O_TRUNC,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH));
}

int	redirr_in_exec(t_global *g)
{
	int	file;

	dup2(g->proc.pipe[1], 1);
	close(g->proc.pipe[0]);
	file = open_file("infile", 1);
	if (file < 0)
		return (1);
	dup2(file, 0);
	close(file);
	if (execve (
			"/bin/ll",
			(char *[]){"ll", NULL},
		(char *[]){NULL}) < 0)
		return (1);
	return (0);
}

int	redirr_out_exec(t_global *g)
{
	int	out;

	dup2(g->proc.pipe[0], 0);
	close(g->proc.pipe[1]);
	out = open_file("outfile", 0);
	dup2(out, 1);
	if (execve (
			"/bin/ll",
			(char *[]){"ll", NULL},
		(char *[]){NULL}) < 0)
		return (1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_global	g;

	init_precess(&g, argc, argv);
	spawn_child(&g);
	spawn_child(&g);

	if (assign_task(&g, 2, redirr_in_exec))
		close_proc(&g, 2, 1);
	if (assign_task(&g, 1, redirr_out_exec))
		close_proc(&g, 1, 1);
	if (g.proc.indx > 0)
		waitpid(g.proc.pid, &g.status, 0);
	return (g.status);
}
