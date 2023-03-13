/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 21:42:33 by tjukmong          #+#    #+#             */
/*   Updated: 2023/03/13 07:04:51 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_path(t_global *g, char *file)
{
	size_t	indx;
	char	*tmp;

	if (!access(file, F_OK))
		return (ft_strdup(file));
	indx = 0;
	while (g->path[indx])
	{
		tmp = ft_strjoin(g->path[indx], file);
		if (!access(tmp, F_OK))
			return (tmp);
		free(tmp);
		indx++;
	}
	return (NULL);
}

void	exec(t_global *g, char **cmd)
{
	char	*err_msg;
	char	*path;

	path = get_path(g, cmd[0]);
	execve(path, cmd, g->envp);
	err_msg = ft_strjoin("\033[91mPipeX:\033[0m ", cmd[0]);
	if (path)
		perror(err_msg);
	else
	{
		ft_putstr_fd(err_msg, 2);
		ft_putendl_fd(": command not found", 2);
	}
	free(err_msg);
	free(path);
	exit(127);
}

void	redirect_infile(t_global *g)
{
	pid_t	file;

	file = open_file(g, g->argv[g->proc.indx], P_READ);
	if (file < 0)
		exit(127);

	close(g->proc.pipe[0]);
	dup2(g->proc.pipe[1], 1);
	close(g->proc.pipe[1]);

	dup2(file, 0);
	close(file);
	exec(g, (char *[]){g->argv[g->proc.indx + 1], NULL});
}

void	pipe_and_execute(t_global *g)
{
	close(g->pipes[g->proc.indx - 2][1]);
	dup2(g->pipes[g->proc.indx - 2][0], 0);
	close(g->pipes[g->proc.indx - 2][0]);

	close(g->pipes[g->proc.indx - 1][0]);
	dup2(g->pipes[g->proc.indx - 1][1], 1);
	close(g->pipes[g->proc.indx - 1][1]);
	exec(g, (char *[]){g->argv[g->proc.indx + 1], NULL});
}

void	redirect_outfile(t_global *g)
{
	pid_t	out;

	out = open_file(g, g->argv[g->proc.indx + 2], P_WRITE);
	if (out < 0)
		exit(127);

	// printf("%s", get_next_line(g->pipes[0][0]));

	close(g->pipes[g->proc.indx - 3][0]);
	close(g->pipes[g->proc.indx - 3][1]);

	close(g->pipes[g->proc.indx - 2][1]);
	dup2(g->pipes[g->proc.indx - 2][0], 0);
	close(g->pipes[g->proc.indx - 2][0]);
	dup2(out, 1);
	close(out);
	exec(g, (char *[]){g->argv[g->proc.indx + 1], NULL});
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

	indx = 1;
	while (indx < g.argc - 4)
	{
		waitpid(g.pids[indx], &exit_stat, 0);
		indx++;
	}
	return (WEXITSTATUS(exit_stat));
}

// printf("%lu:%d\n", g.proc.indx, g.proc.pid);

// waitpid(g.pids[0], &exit_stat, 0);
// waitpid(g.pids[1], &exit_stat, 0);
// waitpid(0, &exit_stat, 0);

// printf("%d > ", g.proc.pid);
// printf("%d ", g.pids[0]);
// printf("%d ", g.pids[1]);
// printf("%d\n", g.pids[3]);