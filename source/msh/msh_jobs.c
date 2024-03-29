/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_jobs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 18:38:29 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/31 00:47:58 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

void			ft_jobs(char **args, char **vars __attribute__((unused)),\
t_shell *shell)
{
	t_job		*j;
	int			i;

	j = g_first_job;
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
	(void)args;
	(void)shell;
}

void			ft_fg(char **args, char **vars __attribute__((unused)),\
t_shell *shell)
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
		ft_fprintf(STDERR_FILENO, "42sh: fg: [%d] wrong argument number!\n",\
		num_process);
		return ;
	}
	j = g_first_job;
	i = num_process;
	--i;
	while (i && j && j->next)
	{
		j = j->next;
		--i;
	}
	if (!j || !j->next)
	{
		ft_fprintf(STDERR_FILENO, "42sh: fg: [%d] wrong argument number!\n",\
		num_process);
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
	(void)shell;
}

void			ft_bg(char **args, char **vars __attribute__((unused)),\
t_shell *shell)
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
		ft_fprintf(STDERR_FILENO, "42sh: bg: [%d] wrong argument number!\n",\
		num_process);
		return ;
	}
	j = g_first_job;
	i = num_process;
	--i;
	while (i && j && j->next)
	{
		j = j->next;
		--i;
	}
	if (!j || !j->next)
	{
		ft_fprintf(STDERR_FILENO, "42sh: bg: [%d] wrong argument number!\n",\
		num_process);
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
	(void)shell;
}
