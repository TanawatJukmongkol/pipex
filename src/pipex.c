/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 21:42:33 by tjukmong          #+#    #+#             */
/*   Updated: 2023/03/11 18:56:00 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	redirect_infile(t_global *g)
{
	printf("< infile cmd%lu | ", g->proc.indx);
	// execve(
	// 	"/bin/sleep",
	// 	(char *[]){"sleep", "2", NULL},
	// 	(char *[]){NULL});
}

void	pipe_and_execute(t_global *g)
{
	printf("cmd%lu | ", g->proc.indx);
	// execve(
	// 	"/bin/sleep",
	// 	(char *[]){"sleep", "5", NULL},
	// 	(char *[]){NULL});
}

void	redirect_outfile(t_global *g)
{
	printf("cmd%lu > outfile ", g->proc.indx);
	// execve(
	// 	"/bin/sleep",
	// 	(char *[]){"sleep", "5", NULL},
	// 	(char *[]){NULL});
}

int	main(int argc, char **argv, char **envp)
{
	t_global	g;
	size_t		indx;
	int			exit_stat;

	if (argc < 5)
		return (1);
	indx = 0;
	init_global(&g, argc, argv, envp);
	while (indx < argc - 3)
	{
		spawn_child(&g);
		indx++;
	}
	indx = 1;
	assign_task(&g, indx, redirect_infile);
	while (++indx < argc - 3)
		assign_task(&g, indx, pipe_and_execute);
	assign_task(&g, indx, redirect_outfile);

	// printf("%lu:%d\n", g.proc.indx, g.proc.pid);

	indx = 0;
	while (indx < g.argc - 3)
	{
		waitpid(g.pids[indx], &exit_stat, 0);
		indx++;
	}

	// waitpid(g.pids[0], &exit_stat, 0);
	// waitpid(g.pids[1], &exit_stat, 0);
	// waitpid(0, &exit_stat, 0);

	// printf("%d > ", g.proc.pid);
	// printf("%d ", g.pids[0]);
	// printf("%d ", g.pids[1]);
	// printf("%d\n", g.pids[3]);

	wait(NULL);
	return (WEXITSTATUS(exit_stat));
}
