/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 13:21:28 by tjukmong          #+#    #+#             */
/*   Updated: 2023/03/11 15:57:50 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	pipex_error(t_global *g, char *msg)
{
	ft_putstr_fd("\033[91mPipeX:\033[0m: ", 2);
	ft_putendl_fd(msg, 2);
	free_global(g);
	exit(EXIT_FAILURE);
}

int	open_file(t_global *g, char *path, int mode)
{
	char	*cmd;

	cmd = ft_strrchr(path, '/');
	if (!cmd)
		cmd = "-";
	else
		cmd++;
	if (mode == P_READ)
	{
		if (access(path, F_OK))
			pipex_error(g, "No such file or directory");
		return (open(path, O_RDONLY));
	}
	else if (mode == P_WRITE)
		return (open(path, O_CREAT | O_WRONLY | O_TRUNC,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH));
	return (-1);
}

void	init_global(t_global *g, int argc, char **argv, char **envp)
{
	size_t	indx;
	char	*tmp;

	indx = 0;
	g->nproc = 0;
	g->proc.pid = 0;
	g->proc.status = 0;
	g->proc.indx = 0;
	g->argc = argc;
	g->argv = argv;
	while (envp[indx] && ft_memcmp(envp[indx], "PATH=", 5))
		indx++;
	g->path = ft_split(envp[indx] + 5, ':');
	indx = 0;
	while (g->path[indx])
	{
		tmp = ft_strjoin(g->path[indx], "/");
		free(g->path[indx]);
		g->path[indx] = tmp;
		indx++;
	}
}

void	free_global(t_global *g)
{
	size_t	indx;

	indx = 0;
	while (g->path[indx])
	{
		free(g->path[indx]);
		indx++;
	}
	free(g->path);
}
