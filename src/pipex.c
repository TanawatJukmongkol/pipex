/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 21:42:33 by tjukmong          #+#    #+#             */
/*   Updated: 2023/03/07 07:35:41 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
/*
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
			"/bin/sleep",
			(char *[]){"sleep", "5", NULL},
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
			"/bin/sleep",
			(char *[]){"sleep", "2", NULL},
		(char *[]){NULL}) < 0)
		return (1);
	return (0);
}
*/

void	exec(char **cmd, char **env)
{
	char	*path;

	path = ft_strjoin("/bin/", cmd[0]);
	if (open_file(path, P_EXE) < 0)
	{
		free(path);
		exit(1);
	}
	execve(path, cmd, env);
	free(path);
	perror("\033[91mPipeX:\033[0m ");
}

int	main(int argc, char **argv)
{
	int		exit_statatus;
	int		file;
	int		out;
	int		p[2];
	pid_t	f;
	pid_t	f2;

	pipe(p);
	f = fork();
	f2 = 0;
	if (f > 0)
		f2 = fork();
	if (f == 0 && f2 == 0)
	{
		file = open_file("infile", P_READ);
		if (file < 0)
			exit(1);
		close(p[0]);
		dup2(p[1], 1);
		close(p[1]);
		dup2(file, 0);
		close(file);
		exec(
			(char *[]){"lss", NULL},
			(char *[]){NULL});
	}
	else if (f > 0 && f2 == 0)
	{
		out = open_file("outfile", P_WRITE);
		if (out < 0)
			exit(1);
		close(p[1]);
		dup2(p[0], 0);
		close(p[0]);
		dup2(out, 1);
		close(out);
		exec(
			(char *[]){"sleep 5", NULL},
			(char *[]){NULL});
	}
	else
	{
		close(p[0]);
		close(p[1]);
	}
	waitpid(f, &exit_statatus, 0);
	waitpid(f2, &exit_statatus, 0);

	return (0);
}
