/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 13:21:28 by tjukmong          #+#    #+#             */
/*   Updated: 2023/04/13 05:00:20 by tjukmong         ###   ########.fr       */
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

char	*get_env(char *env, char **envp)
{
	size_t	i;
	size_t	len;
	char	*match;

	i = 0;
	len = ft_strlen(env) + 2;
	match = ft_strjoin(env, "=");
	while (envp[i] && ft_strncmp(envp[i], match, len) != 0)
		i++;
	free(match);
	return (envp[i]);
}

// char	*get_path(char *env, char *cmd)
// {
// 	char	**paths;
// 	char	*dir;
// 	char	*path;
// 	size_t	i;

// 	if (env)
// 	{
// 		paths = ft_split(env, ':');
// 		i = 0;
// 		while (paths[i])
// 		{
// 			dir = ft_strjoin(paths[i - 1], "/");
// 			path = ft_strjoin(dir, cmd);
// 			free(dir);
// 			if (access(path, F_OK) == 0)
// 			{
// 				free_2d_arr((void **)paths);
// 				return (path);
// 			}
// 			free(path);
// 			i++;
// 		}
// 		free_2d_arr((void **)paths);
// 	}
// 	return (NULL);
// }

char	*get_path(char **envp, char *cmd)
{
	char	**paths;
	char	*dir;
	char	*path;
	size_t	i;

	i = 0;
	while ( envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (envp[i])
	{
		paths = ft_split(envp[i], ':');
		i = 0;
		while (paths[i])
		{
			dir = ft_strjoin(paths[i], "/");
			path = ft_strjoin(dir, cmd);
			free(dir);
			if (access(path, F_OK) == 0)
			{
				free_2d_arr((void **)paths);
				return (path);
			}
			free(path);
			i++;
		}
		free_2d_arr((void **)paths);
	}
	return (NULL);
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
