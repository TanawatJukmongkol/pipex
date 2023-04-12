/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 22:00:52 by tjukmong          #+#    #+#             */
/*   Updated: 2023/04/13 05:00:36 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	wait_process(t_process *proc)
{
	size_t	indx;

	indx = 0;
	while (indx < proc->nproc)
	{
		waitpid(proc->fork[indx], &proc->exit_stats[indx], 0);
		proc->exit_stats[indx] = WEXITSTATUS(proc->exit_stats[indx]);
		// printf("%lu:%d exit stat: %d\n", proc->indx, proc->fork[0], proc->exit_stats[0]);
		indx++;
	}
}

void	spawn_process(t_process *proc, void (*fn)(t_process *p))
{
	proc->fork[proc->nproc] = fork();
	if (proc->fork[proc->nproc] == 0)
		fn(proc);
	proc->nproc++;
}

void	create_pipes(t_process *proc, size_t npipe)
{
	size_t	indx;

	indx = 0;
	while (indx < npipe)
	{
		pipe(proc->pipe[indx]);
		indx++;
	}
}

pid_t	redirr_fd(char *name, int fd, int mode)
{
	int		file;
	char	*err_msg;

	if (mode == P_READ)
		file = open(name, O_RDONLY);
	else if (mode == P_WRITE)
		file = open(name, O_CREAT | O_WRONLY | O_TRUNC,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	else
		return (-1);
	if (access(name, F_OK) || file < 0)
	{
		close(file);
		err_msg = ft_strjoin("\033[91mPipeX:\033[0m ", name);
		ft_putstr_fd(err_msg, 2);
		ft_putendl_fd(": No such file or directory", 2);
		free(err_msg);
		exit(1);
	}
	dup2(file, fd);
	return (file);
}

void	exec(char **cmd, char **envp)
{
	char	*err_msg;
	char	*path;

	err_msg = ft_strjoin("\033[91mPipeX:\033[0m ", cmd[0]);
	if (access(cmd[0], F_OK) == 0)
		path = ft_strdup(cmd[0]);
	else
		path = get_path(envp, cmd[0]);
	if (path)
	{
		execve(path, cmd, envp);
		perror(err_msg);
		free(path);
		free(err_msg);
	}
	else
	{
		ft_putstr_fd(err_msg, 2);
		ft_putendl_fd(": command not found", 2);
		free(path);
		free(err_msg);
		exit(127);
	}
}
