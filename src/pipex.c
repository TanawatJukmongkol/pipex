/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 21:42:33 by tjukmong          #+#    #+#             */
/*   Updated: 2023/04/15 19:08:32 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
	while (indx < (size_t)argc - (5 + proc.heredoc))
	{
		spawn_process(&proc, command);
		indx++;
	}
	spawn_process(&proc, outfile);
	close_pipes(&proc, 0, 0);
	wait_process(&proc);
	return (proc.exit_stats[proc.nproc - 1]);
}
