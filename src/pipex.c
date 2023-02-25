/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 21:42:33 by tjukmong          #+#    #+#             */
/*   Updated: 2023/02/25 13:02:26 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	open_file(char *path, int mode)
{
	if (mode == 1)
	{
		if (access(path, F_OK))
		{
			ft_putstr_fd("pipex: ", 2);
			ft_putstr_fd(path, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			return (1);
		}
		return (open(path, O_RDONLY));
	}
	else
		return (open(path, O_CREAT | O_WRONLY | O_TRUNC,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH));
}

void	exec_task(t_global *g)
{
	dup2(g->proc.pipe[0], 0);
	close(g->proc.pipe[0]);
	dup2(g->proc.pipe[1], 1);
	close(g->proc.pipe[1]);
	execve("/bin/ls", (char *[]){"ls", NULL}, (char *[]){NULL});
}

void	exec_task2(t_global *g)
{
	// close(g->proc.pipe[1]);
	dup2(g->proc.pipe[0], 0);
	close(g->proc.pipe[0]);
	dup2(g->proc.pipe[1], 1);
	close(g->proc.pipe[1]);
	execve("/bin/cat", (char *[]){"cat", "-e", NULL}, (char *[]){NULL});
}

void	exec_task3(t_global *g)
{
	// close(g->proc.pipe[1]);
	// dup2(g->proc.pipe[0], 0);
	// close(g->proc.pipe[0]);
	// // dup2(g->proc.pipe[1], 1);
	// execve("/bin/grep", (char *[]){"grep", "dSYM", NULL}, (char *[]){NULL});
	// close(g->proc.pipe[1]);
}

int	main(int argc, char **argv)
{
	t_global	g;

	init_precess(&g);

	spawn_child(&g);
	// if (g.proc.indx == 0)
	// 	g.main_pipe = g.proc.pipe;
	spawn_child(&g);

	assign_task(&g, 2, exec_task);
	assign_task(&g, 1, exec_task2);

	close_proc(&g, 2, 0);
	close_proc(&g, 1, 0);

	// char *str = get_next_line(g.main_pipe[0]);
	// printf("%s\n", str);
	// free(str);

	wait(NULL);
	close_proc(&g, -1, 0);
	return (0);
}
