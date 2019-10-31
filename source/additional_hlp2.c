/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional_hlp2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/13 15:05:22 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/31 02:28:34 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

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
