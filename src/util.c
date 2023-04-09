/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 13:21:28 by tjukmong          #+#    #+#             */
/*   Updated: 2023/04/07 16:52:16 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

size_t	arr2d_len(void **array)
{
	size_t	len;

	len = 0;
	while (array[len])
		len++;
	return (len);
}

void	free_2d_arr(void **array)
{
	size_t	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

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
			free_2d_arr((void **)paths);
			return (cmd_path);
		}
		free(cmd_path);
	}
	free(paths);
	return (NULL);
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
