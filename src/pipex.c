/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 21:42:33 by tjukmong          #+#    #+#             */
/*   Updated: 2023/04/05 21:22:43 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

typedef struct s_process
{
	char	**envp;

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

void	create_pipes(t_process *proc, size_t npipe)
{
	size_t	indx;

	while (indx < npipe)
	{
		pipe(proc->pipe[indx]);
		indx++;
	}
}

void	close_pipes(t_process *proc, size_t a, size_t b)
{
	size_t	indx;

	indx = 0;
	while (indx < a)
	{
		close(proc->pipe[indx][0]);
		close(proc->pipe[indx][1]);
		indx++;
	}
	indx = a + b;
	while (indx < proc->nproc)
	{
		close(proc->pipe[indx][0]);
		close(proc->pipe[indx][1]);
		indx++;
	}
}

void	wait_process(t_process *proc)
{
	size_t	indx;

	indx = 0;
	while (indx < proc->nproc)
	{
		waitpid(proc->fork[indx], &proc->exit_stats[indx], 0);
		proc->exit_stats[indx] = WEXITSTATUS(proc->exit_stats[indx]);
		printf("%lu:%d exit stat: %d\n", proc->indx, proc->fork[0], proc->exit_stats[0]);
		indx++;
	}
}

void	infile(t_process *proc)
{
	// close(proc->pipe[1][0]);
	// close(proc->pipe[1][1]);
	close_pipes(proc, 0, 1);

	close(proc->pipe[0][0]);
	dup2(proc->pipe[0][1], 1);
	close(proc->pipe[0][1]);

	redirr_fd("infile", 0, P_READ);
	exec((char *[]){"cat", NULL}, proc->envp);
}

void	command(t_process *proc)
{
	close_pipes(proc, proc->nproc - 1, 1);

	close(proc->pipe[proc->nproc - 1][1]);
	dup2(proc->pipe[proc->nproc - 1][0], 0);
	close(proc->pipe[proc->nproc - 1][0]);

	close(proc->pipe[proc->nproc - 0][0]);
	dup2(proc->pipe[proc->nproc - 0][1], 1);
	close(proc->pipe[proc->nproc - 0][1]);

	exec((char *[]){"cat", NULL}, proc->envp);
}

void	outfile(t_process *proc)
{
	// close(proc->pipe[0][0]);
	// close(proc->pipe[0][1]);
	close_pipes(proc, proc->nproc - 1, 1);

	close(proc->pipe[proc->nproc - 1][1]);
	dup2(proc->pipe[proc->nproc - 1][0], 0);
	close(proc->pipe[proc->nproc - 1][0]);

	redirr_fd("outfile", 1, P_WRITE);
	exec((char *[]){"cat", NULL}, proc->envp);
}

void	spawn_process(t_process *proc, void (*fn)(t_process *p))
{
	proc->fork[proc->nproc] = fork();
	if (proc->fork[proc->nproc] == 0)
		fn(proc);
	proc->nproc++;
}

int	main(int argc, char *argv[], char *envp[])
{
	static t_process	proc;

	if (argc < 5)
		return (1);
	proc.nproc = 0;
	proc.indx = 0;
	proc.envp = envp;
	create_pipes(&proc, argc - 4);

	spawn_process(&proc, infile);
	spawn_process(&proc, command);
	spawn_process(&proc, command);
	spawn_process(&proc, outfile);

	close_pipes(&proc, 0, 0);
	wait_process(&proc);

	return (proc.exit_stats[2]);
}
