/*
#define FD_MAX 1000

typedef struct s_process
{
	int		pipe[FD_MAX][2];
	pid_t	fork[FD_MAX];
	size_t	indx;
	size_t	nproc;
	int		exit_stat;
}				t_process;

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
			while (paths[++i])
				free(paths[i]);
			free(paths);
			return (cmd_path);
		}
		free(cmd_path);
	}
	free(paths);
	return (cmd_path);
}

pid_t	redirr_fd(char *name, int fd, int mode)
{
	int	file;

	if (mode == P_READ)
		file = open(name, O_RDONLY);
	else if (mode == P_WRITE)
		file = open(name, O_CREAT | O_WRONLY | O_TRUNC,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	else
		return (-1);
	dup2(file, fd);
	return (file);
}

int	is_parent_fork(t_process *proc)
{
	size_t	indx;

	indx = 0;
	while (indx < proc->nproc)
	{
		if (proc->fork[indx] == 0)
			return (0);
		indx++;
	}
	return (1);
}

void	close_unused_pipes(t_process *proc)
{
	size_t	indx;

	indx = 0;
	while (indx < proc->nproc)
	{
		if (indx == proc->indx || indx + 1 == proc->indx)
		{
			indx++;
			continue ;
		}
		close(proc->pipe[indx][0]);
		close(proc->pipe[indx][1]);
		indx++;
	}
}

void	spawn_child(t_process *proc, size_t nchild)
{
	size_t	indx;

	if (proc->nproc > 10)
		exit(69);
	indx = 0;
	while (indx < nchild)
	{
		pipe(proc->pipe[indx]);
		indx++;
	}
	while (proc->nproc < nchild)
	{
		if (!proc->indx || is_parent_fork(proc))
		{
			proc->fork[proc->indx] = fork();
			proc->nproc++;
			if (!proc->fork[proc->indx])
				proc->indx = proc->nproc;
		}
		else
			proc->nproc++;
	}
}

int	main(int argc, char **argv, char **envp)
{
	static t_process	proc;
	size_t				indx;

	if (argc < 5)
		exit(1);

	proc.indx = 0;
	proc.nproc = 0;
	spawn_child(&proc, argc - 4);

	indx = 0;
	while (indx < proc.nproc + 1)
	{
		if (proc.indx != indx++)
			continue ;
		char	**args = ft_split(argv[proc.indx + 2], ' ');
		printf("[%lu:%lu]: %d %s\n", indx, proc.indx, proc.fork[proc.indx], args[0]);
		if (proc.indx == 0)
		{
			int		p_read = proc.pipe[proc.indx][0];
			int		p_write = proc.pipe[proc.indx][1];
			char	**args = ft_split(argv[proc.indx + 2], ' ');
			close_unused_pipes(&proc);
			close(proc.pipe[proc.indx - 1][0]);
			close(proc.pipe[proc.indx - 1][1]);
			close(p_read);
			dup2(p_write, 1);
			close(p_write);
			redirr_fd(argv[1], 0, P_READ);
			execve(get_path(envp, args[0]), args, envp);
			free(args);
			perror("cmd in not found");
			exit(127);
		}
		else if (proc.indx != proc.nproc) {
			int		p_read = proc.pipe[proc.indx - 1][0];
			int		p_write = proc.pipe[proc.indx - 1][1];
			int		p2_read = proc.pipe[proc.indx][0];
			int		p2_write = proc.pipe[proc.indx][1];
			char	**args = ft_split(argv[proc.indx + 2], ' ');
			close_unused_pipes(&proc);
			close(p_write);
			dup2(p_read, 0);
			close(p_read);
			close(p2_read);
			dup2(p2_write, 1);
			close(p2_write);
			execve(get_path(envp, args[0]), args, envp);
			perror("cmd not found");
			exit(127);
		}
		else
		{
			int		p_read = proc.pipe[proc.indx - 1][0];
			int		p_write = proc.pipe[proc.indx - 1][1];
			char	**args = ft_split(argv[proc.indx + 2], ' ');
			close_unused_pipes(&proc);
			close(proc.pipe[proc.indx][0]);
			close(proc.pipe[proc.indx][1]);
			close(p_write);
			dup2(p_read, 0);
			close(p_read);
			redirr_fd(argv[proc.indx + 3], 1, P_WRITE);
			execve(get_path(envp, args[0]), args, envp);
			free(args);
			perror("cmd out not found");
			exit(127);
		}
	}

	indx = 0;
	while (indx < proc.nproc)
	{
		waitpid(proc.fork[indx], &proc.exit_stat, 0);
		indx++;
	}
	close(proc.pipe[proc.indx][0]);
	close(proc.pipe[proc.indx][1]);
	// printf("%lu: %d\n", proc.indx, proc.fork[proc.indx]);
	return (WEXITSTATUS(proc.exit_stat));
}
*/