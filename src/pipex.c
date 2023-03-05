/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjukmong <tjukmong@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 21:42:33 by tjukmong          #+#    #+#             */
/*   Updated: 2023/03/06 00:10:39 by tjukmong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	open_file(char *path, int mode)
{
	if (mode == 1)
	{
		if (access(path, F_OK))
		{
			ft_putstr_fd("pipex: ", 2);
			ft_putstr_fd(path, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			return (-1);
		}
		return (open(path, O_RDONLY));
	}
	else
		return (open(path, O_CREAT | O_WRONLY | O_TRUNC,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH));
}

int	main(int argc, char **argv)
{
	t_global	g;

	// init_precess(&g);
	int exit_status;
	int p[2];
	pipe(p);
	int pid = fork();

	if (pid == 0)
	{
		int file = open_file("infile", 1);
		dup2(p[1], 1);
		close(p[0]);
		dup2(file, 0);
		close(file);
		execve (
			"/bin/sleep",
			(char *[]){"sleep", "5", NULL},
			(char *[]){NULL}
		);
	}
	else
	{
		dup2(p[0], 0);
		close(p[1]);
		execve (
			"/bin/sleep",
			(char *[]){"sleep", "3", NULL},
			(char *[]){NULL}
		);
	}

	int out = open_file("outfile", 0);
	dup2(out, 1);

	waitpid(pid, &exit_status, 0);

	// wait(NULL);
	// close_proc(&g, -1, 0);
	return (0);
}
