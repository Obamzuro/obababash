/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 17:22:38 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/30 18:27:19 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

void		add_process_to_job(t_job *job, pid_t pid)
{
	int			i;
	t_process	*process;

	process = job->first_process;
	if (!process)
	{
		job->first_process = (t_process *)ft_memalloc(sizeof(t_process));
		job->first_process->pid = pid;
//		if (!job->pgid)
//		{
//			job->pgid = pid;
//			setpgid(pid, job->pgid);
//			tcsetpgrp(STDIN_FILENO, job->pgid);
//		}
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
	///////////////
//	ft_printf("RAKDO %d", job->pgid);
//	if (!job->pgid)
//	{
//		ft_printf("RAK");
//		job->pgid = pid;
//		setpgid(pid, job->pgid);
//		tcsetpgrp(STDIN_FILENO, job->pgid);
//	}
//	//////////////////
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
				{
					p->status = status;
					if (WIFSTOPPED(status))
						p->stopped = 1;
					else
					{
						p->completed = 1;
						if (WIFSIGNALED(status))
							ft_fprintf(STDERR_FILENO,\
							"%d: Terminated by signal %d.\n", (int)pid, WTERMSIG(p->status));
					}
					return (0);
				}
				p = p->next;
			}
			j = j->next;
		}
	}
//	else if (!pid)
//		return (-1);
//		????????????????
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

void		print_processes(void)
{
	t_job		*job;
	t_process	*process;

	job = g_first_job;
	while (job)
	{
		process = job->first_process;
		ft_printf("job #%d\n", job->pgid);
		while (process)
		{
			ft_printf("process #%d\n", process->pid);
			process = process->next;
		}
		job = job->next;
	}
}

void		wait_for_job(t_job *job)
{
	int		status;
	pid_t	pid;

//	print_processes();
	pid = waitpid(WAIT_ANY, &status, WUNTRACED);
	if (pid == -1)
		perror("waitpid error: ");
	while (!mark_process_status(pid, status) &&
				!job_is_stopped(job) &&
				!job_is_completed(job))
	{
		pid = waitpid(WAIT_ANY, &status, WUNTRACED);
		if (pid == -1)
			perror("waitpid error: ");
	}
}

void		put_job_in_background(t_job *job, int cont)
{
	if (cont)
		if (kill(-job->pgid, SIGCONT) < 0)
			perror("kill (SIGCONT)");
}

void		put_job_in_foreground(t_job *job, int cont)
{
	extern struct termios		g_tty;

	// shell->initfd.in ???
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

void		do_job_notification(void)
{
	t_job		*j;
	t_job		*jlast;
	t_job		*jnext;
	t_process	*p;
	t_process	*p2;

	jlast = NULL;
	j = g_first_job;
//	print_processes();
	update_status();
	while (j)
	{
		jnext = j->next;
		if (job_is_completed(j))
		{
			if (!j->foreground)
				ft_fprintf(STDERR_FILENO, "%ld: completed\n",\
				j->pgid ? j->pgid : (j->first_process ? j->first_process->pid : 0));
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
