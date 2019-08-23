# include "twenty_one_sh.h"

void		add_process_to_job(t_job *job, pid_t pid)
{
	int			i;
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

int			mark_process_status(pid_t pid, int status)
{
	t_job		*j;
	t_process	*p;

	if (pid > 0)
	{
		j = first_job;
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
							ft_fprintf(STDERR_FILENO, "%d: Terminated by signal %d.\n", (int)pid, WTERMSIG(p->status));
					}
					return (0);
				}
				p = p->next;
			}
			j = j->next;
		}
	}
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

void		print_processes()
{
	t_job		*job;
	t_process	*process;

	job = first_job;
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

	print_processes();
	pid = waitpid(WAIT_ANY, &status, WUNTRACED);
	while (!mark_process_status(pid, status) &&
				!job_is_stopped(job) &&
				!job_is_completed(job))
			pid = waitpid(WAIT_ANY, &status, WUNTRACED);
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