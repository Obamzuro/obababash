/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_jobs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 18:38:29 by obamzuro          #+#    #+#             */
/*   Updated: 2019/09/22 19:36:06 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

void			ft_jobs(char **args, char ***env)
{
	t_job		*j;
	t_process	*p;
	int			i;

	j = first_job;
	i = 1;
	while (j && j->next)
	{
		ft_printf("%-4d", i);
		if (job_is_stopped(j))
			ft_printf("%-20s", "Stopped");
		ft_printf("%-8d %s\n", j->pgid, j->command);
		j = j->next;
		++i;
	}
}

void			ft_fg(char **args, char ***env)
{
	t_process	*p;
	t_job		*j;
	int			num_process;
	int			i;

	if (!args[1])
		num_process = 1;
	else
		num_process = ft_atoi(args[1]);
	if (num_process < 1)
	{
		ft_fprintf(STDERR_FILENO, "42sh: fg: [%d] wrong argument number!\n", num_process);
		return ;
	}
	j = first_job;
	i = num_process;
	--i;
	while (i && j && j->next)
	{
		j = j->next;
		--i;
	}
	if (!j || !j->next)
	{
		ft_fprintf(STDERR_FILENO, "42sh: fg: [%d] wrong argument number!\n", num_process);
		return ;
	}
	p = j->first_process;
	while (p)
	{
		p->stopped = 0;
		p = p->next;
	}
	j->notified = 0;
	put_job_in_foreground(j, 1);
}

void			ft_bg(char **args, char ***env)
{
	t_process	*p;
	t_job		*j;
	int			num_process;
	int			i;

	if (!args[1])
		num_process = 1;
	else
		num_process = ft_atoi(args[1]);
	if (num_process < 1)
	{
		ft_fprintf(STDERR_FILENO, "42sh: bg: [%d] wrong argument number!\n", num_process);
		return ;
	}
	j = first_job;
	i = num_process;
	--i;
	while (i && j && j->next)
	{
		j = j->next;
		--i;
	}
	if (!j || !j->next)
	{
		ft_fprintf(STDERR_FILENO, "42sh: bg: [%d] wrong argument number!\n", num_process);
		return ;
	}
	p = j->first_process;
	while (p)
	{
		p->stopped = 0;
		p = p->next;
	}
	j->notified = 0;
	put_job_in_background(j, 1);
}
