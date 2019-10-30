/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_join.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/18 13:28:24 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/31 01:04:58 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

void		job_join_child(void)
{
	pid_t		pid;
	t_job		*current_job;

	pid = getpid();
	if (!current_job)
	{
		current_job = (t_job *)ft_memalloc(sizeof(t_job));
		current_job->pgid = pid;
	}
	setpgid(pid, current_job->pgid);
	return ;
}
