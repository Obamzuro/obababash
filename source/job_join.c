/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_join.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/18 13:28:24 by obamzuro          #+#    #+#             */
/*   Updated: 2019/08/18 13:48:21 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

void		job_join_child()
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
//	if (!ast->pgid)
//	{
//		ast->pgid = pid;
//		side->pgid = pid;
//	}
//	setpgid(pid, ast->pgid);
//		if (ast->foreground)
//			tcsetpgrp(shell->initfd.fdin, ast->pgid);
	return ;
}

void		job_join_parent()
{
//	if (!ast->pgid)
//	{
//		ast->pgid = pid;
//		side->pgid = pid;
//	}
//	setpgid(pid, ast->pgid);
	return ;
}
