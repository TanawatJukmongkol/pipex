/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 22:00:52 by tjukmong          #+#    #+#             */
/*   Updated: 2023/03/07 03:38:26 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	pipex_error(char *path, char *msg)
{
	ft_putstr_fd("\033[91mPipeX:\033[0m ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(msg, 2);
	return (-1);
}

int	open_file(char *path, int mode)
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
			return (pipex_error(path, "No such file or directory"));
		return (open(path, O_RDONLY));
	}
	else if (mode == P_WRITE)
		return (open(path, O_CREAT | O_WRONLY | O_TRUNC,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH));
	else if (mode == P_EXE)
	{
		if (access(path, F_OK | X_OK))
			return (pipex_error(cmd, "command not found"));
		return (1);
	}
	return (-1);
}
