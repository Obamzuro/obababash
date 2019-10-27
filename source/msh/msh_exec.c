/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_exec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 13:52:33 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/27 19:32:38 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

static char		*ft_exec_path_find_comm(char **args, char **paths)
{
	int			i;
	struct stat	mystat;
	char		*temp;

	i = 0;
	while (i < g_hash->len)
	{
		temp = g_hash->elem[i];
		if (ft_strequ(args[0], ft_strrchr(temp, '/') + 1))
		{
			ft_printf("ALLO\n");
			if (lstat(temp, &mystat) != -1)
			{
				if (access(temp, X_OK) == -1)
				{
					ft_fprintf(2, "21sh: Permission denied: %s\n", args[0]);
					return (0);
				}
				else
					return (ft_strdup(temp));
			}
		}
		++i;
	}
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin_char(paths[i], args[0], '/');
		if (lstat(temp, &mystat) != -1)
		{
			if (access(temp, X_OK) == -1)
			{
				ft_fprintf(2, "21sh: Permission denied: %s\n", args[0]);
				free(temp);
				return (0);
			}
			else
			{
				push_ftvector(g_hash, ft_strdup(temp));
				return (temp);
			}
		}
		free(temp);
		++i;
	}
	ft_fprintf(2, "21sh: command not found: %s\n", args[0]);
	return (0);
}

static char		*ft_exec_path(char **args, char ***env)
{
	char		**paths;
	int			i;
	char		*ret;

	paths = ft_strsplit(get_env("PATH", *env), ':');
	i = 0;
	if (!paths)
	{
		ft_fprintf(2, "21sh: command not found: %s\n", args[0]);
		return (0);
	}
	ret = ft_exec_path_find_comm(args, paths);
	free_double_arr(paths);
	return (ret);
}

static int		ft_exec_check_err(char **args, char *comm)
{
	struct stat	tempstat;

	if (!comm)
		return (-1);
	if (lstat(comm, &tempstat) == -1)
	{
		ft_fprintf(2, "21sh: no such file or directory: %s\n", comm);
		if (comm != args[0])
			free(comm);
		return (-1);
	}
	if (!S_ISREG(tempstat.st_mode) || access(comm, X_OK) == -1)
	{
		if (S_ISDIR(tempstat.st_mode))
			ft_fprintf(2, "21sh: %s: is a directory\n", comm);
		else
			ft_fprintf(2, "21sh: Permission denied: %s\n", comm);
		if (comm != args[0])
			free(comm);
		return (-1);
	}
	return (0);
}

static int		ft_exec_fork(char **args, char ***env, char *comm, t_job *cur_job)
{
	pid_t		pid;

	pid = fork();
	if (pid == 0)
	{
		pid = getpid();
		add_process_to_job(cur_job, pid);
		if (!cur_job->pgid)
			cur_job->pgid = pid;
		setpgid(pid, cur_job->pgid);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGTSTP, SIG_DFL);
		signal(SIGTTIN, SIG_DFL);
		signal(SIGTTOU, SIG_DFL);
		signal(SIGCHLD, SIG_DFL);
		if (execve(comm, args, *env) == -1)
		{
			ft_fprintf(2, "21sh: File execution error: %s\n", comm);
			return (-1);
		}
	}
	else if (pid < 0)
	{
		ft_fprintf(2, "21sh: Error creating a child thread\n");
		return (-1);
	}
	add_process_to_job(cur_job, pid);
	if (!cur_job->pgid)
		cur_job->pgid = pid;
	setpgid(pid, cur_job->pgid);
	if (cur_job->foreground)
		put_job_in_foreground(cur_job, 0);
	return (0);
}

int				ft_exec(char **args, char ***env, int forkneed, t_job *cur_job)
{
	char		*comm;
	int			ret;

	if (!ft_strchr(args[0], '/') &&
			!(ft_is_str_in_args(args[0], 2, "..", ".")))
		comm = ft_exec_path(args, env);
	else
		comm = args[0];
	if (ft_exec_check_err(args, comm) == -1)
		return (-1);
	if (forkneed)
		ret = ft_exec_fork(args, env, comm, cur_job);
	else if ((ret = execve(comm, args, *env) == -1))
		ft_fprintf(2, "21sh: File execution error: %s\n", comm);
	if (comm != args[0])
		free(comm);
	return (ret);
}
