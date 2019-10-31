/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional_hlp.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/13 15:05:22 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/31 02:26:14 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

void		put_job_in_background(t_job *job, int cont)
{
	if (cont)
		if (kill(-job->pgid, SIGCONT) < 0)
			perror("kill (SIGCONT)");
}

void		put_job_in_foreground(t_job *job, int cont)
{
	extern struct termios		g_tty;

	tcsetpgrp(STDIN_FILENO, job->pgid);
	if (cont)
	{
		tcsetattr(STDIN_FILENO, TCSADRAIN, &job->tmodes);
		if (kill(-job->pgid, SIGCONT) < 0)
			perror("kill (SIGCONT)");
	}
	wait_for_job(job);
	tcsetpgrp(STDIN_FILENO, g_shell->pgid);
	tcgetattr(STDIN_FILENO, &job->tmodes);
	tcsetattr(STDIN_FILENO, TCSADRAIN, &g_tty);
}

static void	update_status(void)
{
	int		status;
	pid_t	pid;

	pid = waitpid(WAIT_ANY, &status, WUNTRACED | WNOHANG);
	while (!mark_process_status(pid, status))
		pid = waitpid(WAIT_ANY, &status, WUNTRACED | WNOHANG);
}

static void	do_job_ntf(t_job *j, t_job *jlast, t_job *jnext)
{
	t_process	*p;
	t_process	*p2;

	if (!j->foreground)
		ft_fprintf(STDERR_FILENO, "%ld: completed\n", j->pgid ?\
		j->pgid : N_TERN(j->first_process, j->first_process->pid, 0));
	if (jlast)
		jlast->next = jnext;
	else
		g_first_job = jnext;
	p = j->first_process;
	p2 = p;
	while (p)
	{
		p2 = p->next;
		free(p);
		p = p2;
	}
	free(j);
}

void		do_job_notification(void)
{
	t_job		*j;
	t_job		*jlast;
	t_job		*jnext;

	jlast = NULL;
	j = g_first_job;
	update_status();
	while (j)
	{
		jnext = j->next;
		if (job_is_completed(j))
			do_job_ntf(j, jlast, jnext);
		else if (job_is_stopped(j) && !j->notified)
		{
			ft_fprintf(STDERR_FILENO, "%ld: stopped\n", j->pgid);
			j->notified = 1;
			jlast = j;
		}
		else
			jlast = j;
		j = jnext;
	}
}
