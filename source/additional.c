/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 17:22:38 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/31 02:28:30 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

void		add_process_to_job(t_job *job, pid_t pid)
{
	t_process	*process;

	process = job->first_process;
	if (!process)
	{
		job->first_process = (t_process *)ft_memalloc(sizeof(t_process));
		job->first_process->pid = pid;
		return ;
	}
	if (process->pid == pid)
		return ;
	while (process->next)
	{
		if (process->pid == pid)
			return ;
		process = process->next;
	}
	process->next = (t_process *)ft_memalloc(sizeof(t_process));
	process->next->pid = pid;
}

static int	mrk_prc_sts(t_process *p, pid_t pid, int status)
{
	p->status = status;
	if (WIFSTOPPED(status))
		p->stopped = 1;
	else
	{
		p->completed = 1;
		if (WIFSIGNALED(status))
			ft_fprintf(STDERR_FILENO,\
			"%d: Terminated by signal %d.\n",\
			(int)pid, WTERMSIG(p->status));
	}
	return (0);
}

int			mark_process_status(pid_t pid, int status)
{
	t_job		*j;
	t_process	*p;

	if (pid > 0)
	{
		j = g_first_job;
		while (j)
		{
			p = j->first_process;
			while (p)
			{
				if (p->pid == pid)
					return (mrk_prc_sts(p, pid, status));
				p = p->next;
			}
			j = j->next;
		}
	}
	if (pid > 0)
		ft_fprintf(STDERR_FILENO, "No child process %d.\n", pid);
	return (-1);
}

int			job_is_stopped(t_job *j)
{
	t_process		*p;

	p = j->first_process;
	while (p)
	{
		if (!p->completed && !p->stopped)
			return (0);
		p = p->next;
	}
	return (1);
}

int			job_is_completed(t_job *j)
{
	t_process		*p;

	p = j->first_process;
	while (p)
	{
		if (!p->completed)
			return (0);
		p = p->next;
	}
	return (1);
}
