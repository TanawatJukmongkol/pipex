/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 21:42:33 by tjukmong          #+#    #+#             */
/*   Updated: 2023/04/13 11:20:44 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	infile(t_process *proc)
{
	char	**cmd;

	cmd = ft_split(proc->argv[proc->nproc + 2 + proc->heredoc], ' ');
	close_pipes(proc, 0, 1);


	if (!proc->heredoc)
	{
		close(proc->pipe[0][0]);
		dup2(proc->pipe[0][1], 1);
		close(proc->pipe[0][1]);
		redirr_fd(proc->argv[1], 0, P_READ);
		exec(cmd, proc->envp);
	}
	else
	{
		while (1)
		{
			ft_putstr_fd("\033[91mPipeX \033[0mhere_doc > ", 1);
			char *gnl = get_next_line(0);
			if (ft_strlen(gnl) > 1 && ft_strncmp(gnl, proc->argv[2], ft_strlen(gnl) - 1) == 0)
			{
				close(proc->pipe[0][0]);
				close(proc->pipe[0][1]);
				free(gnl);
				exit(0);
			}
			ft_putstr_fd(gnl, proc->pipe[0][1]);
			free(gnl);
		}
	}
}

void	command(t_process *proc)
{
	char	**cmd;

	cmd = ft_split(proc->argv[proc->nproc + 2 + proc->heredoc], ' ');
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

	cmd = ft_split(proc->argv[proc->nproc + 2 + proc->heredoc], ' ');
	close_pipes(proc, proc->nproc - 1, 1);

	close(proc->pipe[proc->nproc - 1][1]);
	dup2(proc->pipe[proc->nproc - 1][0], 0);
	close(proc->pipe[proc->nproc - 1][0]);

	redirr_fd(proc->argv[proc->nproc + 3 + proc->heredoc], 1, P_WRITE);
	exec(cmd, proc->envp);
}

int	main(int argc, char *argv[], char *envp[])
{
	static t_process	proc;
	size_t				indx;

	if (argc > 1)
		proc.heredoc = (ft_strncmp(
					argv[1], "here_doc", ft_strlen(argv[1])) == 0);
	if (argc < (5 + proc.heredoc))
		return (1);

	indx = 0;
	proc.nproc = 0;
	proc.indx = 0;
	proc.argc = argc;
	proc.argv = argv;
	proc.envp = envp;


	create_pipes(&proc, argc - (4 + proc.heredoc));

	spawn_process(&proc, infile);

	while (indx < argc - (5 + proc.heredoc))
	{
		spawn_process(&proc, command);
		indx++;
	}
	spawn_process(&proc, outfile);
	close_pipes(&proc, 0, 0);
	wait_process(&proc);
	return (proc.exit_stats[proc.nproc - 1]);
}
