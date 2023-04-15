/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tested.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 21:42:33 by tjukmong          #+#    #+#             */
/*   Updated: 2023/04/05 17:06:39 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

typedef struct s_process
{
	int		pipe[FD_MAX >> 1][2];
	pid_t	fork[FD_MAX >> 2];
	size_t	indx;
	size_t	nproc;
	int		exit_stats[FD_MAX >> 2];
}				t_process;

char	*get_path(char **envp, char *cmd)
{
	char	*cmd_path;
	char	**paths;
	size_t	i;

	i = 0;
	while (envp[++i])
	{
		if (ft_memcmp(envp[i], "PATH=", 5))
			continue ;
		paths = ft_split(ft_strchr(envp[i], '=') + 1, ':');
		break ;
	}
	i = 0;
	while (paths[i++])
	{
		cmd_path = ft_strjoin(ft_strjoin(paths[i], "/"), cmd);
		if (!access(cmd_path, F_OK))
		{
			while (paths[++i])
				free(paths[i]);
			free(paths);
			return (cmd_path);
		}
		free(cmd_path);
	}
	free(paths);
	return (cmd_path);
}

pid_t	redirr_fd(char *name, int fd, int mode)
{
	int	file;

	if (mode == P_READ)
		file = open(name, O_RDONLY);
	else if (mode == P_WRITE)
		file = open(name, O_CREAT | O_WRONLY | O_TRUNC,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	else
		return (-1);
	dup2(file, fd);
	return (file);
}

void	exec(char **cmd, char **envp)
{
	char	*err_msg;
	char	*path;

	path = get_path(envp, cmd[0]);
	execve(path, cmd, envp);

	wait(NULL);
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

int	main(int argc, char *argv[], char *envp[])
{
	static t_process	proc;

	pipe(proc.pipe[0]);
	pipe(proc.pipe[1]);

	proc.nproc = 0;

	proc.fork[0] = fork();

	if (proc.fork[0] == 0)
	{
		close(proc.pipe[1][0]);
		close(proc.pipe[1][1]);

		close(proc.pipe[0][0]);
		dup2(proc.pipe[0][1], 1);
		close(proc.pipe[0][1]);

		redirr_fd("infile", 0, P_READ);
		exec((char *[]){"sleep", "3", NULL}, envp);
	}

	proc.fork[1] = fork();

	if (proc.fork[1] == 0)
	{
		close(proc.pipe[0][1]);
		dup2(proc.pipe[0][0], 0);
		close(proc.pipe[0][0]);

		close(proc.pipe[1][0]);
		dup2(proc.pipe[1][1], 1);
		close(proc.pipe[1][1]);

		exec((char *[]){"ll", NULL}, envp);
	}

	proc.fork[2] = fork();

	if (proc.fork[2] == 0)
	{
		close(proc.pipe[0][0]);
		close(proc.pipe[0][1]);

		close(proc.pipe[1][1]);
		dup2(proc.pipe[1][0], 0);
		close(proc.pipe[1][0]);

		redirr_fd("outfile", 1, P_WRITE);
		exec((char *[]){"sleep", "1", NULL}, envp);
	}

	close(proc.pipe[0][0]);
	close(proc.pipe[0][1]);
	close(proc.pipe[1][0]);
	close(proc.pipe[1][1]);

	waitpid(proc.fork[0], &proc.exit_stats[0], 0);
	proc.exit_stats[0] = WEXITSTATUS(proc.exit_stats[0]);

	waitpid(proc.fork[1], &proc.exit_stats[1], 0);
	proc.exit_stats[1] = WEXITSTATUS(proc.exit_stats[1]);

	waitpid(proc.fork[2], &proc.exit_stats[2], 0);
	proc.exit_stats[2] = WEXITSTATUS(proc.exit_stats[2]);

	printf("%lu:%d exit stat: %d\n", proc.indx, proc.fork[0], proc.exit_stats[0]);
	printf("%lu:%d exit stat: %d\n", proc.indx, proc.fork[1], proc.exit_stats[1]);
	printf("%lu:%d exit stat: %d\n", proc.indx, proc.fork[2], proc.exit_stats[2]);

	printf("nproc: %lu\n", proc.nproc);

	return (proc.exit_stats[2]);
}
