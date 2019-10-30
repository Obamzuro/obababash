/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 12:46:18 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/30 18:27:19 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

static int			parse_ast_pipe_child(t_ast *ast, t_shell *shell,
		int fdpipe[2], int is_out, t_job *cur_job)
{
	pid_t	pid;
	int		a;
	int		b;
	t_ast	*side;

	a = is_out ? 0 : 1;
	b = is_out ? 1 : 0;
	side = is_out ? ast->left : ast->right;
	pid = fork();
	if (pid < 0)
	{
		ft_fprintf(2, "21sh: Error creating subprocess");
		return (-1);
	}
	if (!pid)
	{
		pid = getpid();
		add_process_to_job(cur_job, pid);
		if (!cur_job->pgid)
			cur_job->pgid = pid;
		setpgid(pid, cur_job->pgid);
		///////////////
//		if (cur_job->foreground)
//			tcsetpgrp(shell->initfd.fdin, cur_job->pgid);
//			//////////////////
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGTSTP, SIG_DFL);
		signal(SIGTTIN, SIG_DFL);
		signal(SIGTTOU, SIG_DFL);
		signal(SIGCHLD, SIG_DFL);
		close(fdpipe[a]);
		dup2(fdpipe[b], b);
		close(fdpipe[b]);
		parse_ast(side, shell, 0, cur_job);
		exit(0);
	}
	else
	{
		add_process_to_job(cur_job, pid);
		if (!cur_job->pgid)
			cur_job->pgid = pid;
		setpgid(pid, cur_job->pgid);
	}
	return (pid);
}

// TODO: Add foreground parameter
int					parse_ast_pipe(t_ast *ast, t_shell *shell, t_job *cur_job)
{
	int			fdpipe[2];
	pid_t		pid[2];

	if (pipe(fdpipe) == -1)
		return ((int64_t)print_error_zero("21sh: pipe creating error"));
	if ((pid[0] = parse_ast_pipe_child(ast, shell, fdpipe, 0, cur_job)) == -1)
	{
		close(fdpipe[0]);
		close(fdpipe[1]);
		return (0);
	}
	if ((pid[1] = parse_ast_pipe_child(ast, shell, fdpipe, 1, cur_job)) == -1)
	{
		close(fdpipe[0]);
		close(fdpipe[1]);
		kill(pid[0], SIGINT);
		return (0);
	}
	close(fdpipe[0]);
	close(fdpipe[1]);
//	while (waitpid(pid[0], 0, 0) == -1)
//		;
//	while (waitpid(pid[1], 0, 0) == -1)
//		;
	return (1);
}
