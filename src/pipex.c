/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 21:42:33 by tjukmong          #+#    #+#             */
/*   Updated: 2023/04/09 09:28:22 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	infile(t_process *proc)
{
	char	**cmd;

	cmd = ft_split(proc->argv[proc->nproc + 2], ' ');
	close_pipes(proc, 0, 1);

	close(proc->pipe[0][0]);
	dup2(proc->pipe[0][1], 1);
	close(proc->pipe[0][1]);

	redirr_fd(proc->argv[1], 0, P_READ);
	exec(cmd, proc->envp);
}

void	command(t_process *proc)
{
	char	**cmd;

	cmd = ft_split(proc->argv[proc->nproc + 2], ' ');
	close_pipes(proc, proc->nproc - 1, 2);

	close(proc->pipe[proc->nproc - 1][1]);
	dup2(proc->pipe[proc->nproc - 1][0], 0);
	close(proc->pipe[proc->nproc - 1][0]);

	close(proc->pipe[proc->nproc - 0][0]);
	dup2(proc->pipe[proc->nproc - 0][1], 1);
	close(proc->pipe[proc->nproc - 0][1]);

	exec(cmd, proc->envp);
}

void	outfile(t_process *proc)
{
	char	**cmd;

	cmd = ft_split(proc->argv[proc->nproc + 2], ' ');
	close_pipes(proc, proc->nproc - 1, 1);

	close(proc->pipe[proc->nproc - 1][1]);
	dup2(proc->pipe[proc->nproc - 1][0], 0);
	close(proc->pipe[proc->nproc - 1][0]);

	redirr_fd(proc->argv[proc->nproc + 3], 1, P_WRITE);
	exec(cmd, proc->envp);
}

int	main(int argc, char *argv[], char *envp[])
{
	static t_process	proc;
	size_t				indx;

	if (argc < 5)
		return (1);
	indx = 0;
	proc.nproc = 0;
	proc.indx = 0;
	proc.argc = argc;
	proc.argv = argv;
	proc.envp = envp;
	proc.has_heredoc = ft_memcmp(
			argv[1], "here_doc", ft_strlen(argv[1])) > 0;

	create_pipes(&proc, argc - (5 + proc.has_heredoc));

	spawn_process(&proc, infile);
	while (indx < argc - (5 + proc.has_heredoc))
	{
		spawn_process(&proc, command);
		indx++;
	}
	spawn_process(&proc, outfile);

	close_pipes(&proc, 0, 0);
	wait_process(&proc);

	return (proc.exit_stats[2]);
}
