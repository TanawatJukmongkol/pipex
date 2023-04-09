/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 22:00:52 by tjukmong          #+#    #+#             */
/*   Updated: 2023/04/07 16:54:33 by tjukmong         ###   ########.fr       */
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
	char	**split;

	// split = ft_split(cmd[0], '/');
	// path = get_path(envp, split[arr2d_len((void **)split) - 1]);
	// free_2d_arr((void **)split);
	path = get_path(envp, cmd[0]);
	if (path != NULL)
	{
		execve(path, cmd, envp);
		wait(NULL);
	}
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
