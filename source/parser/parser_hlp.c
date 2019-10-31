/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_hlp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/13 15:05:22 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/31 05:34:31 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

int					parser_states(t_ast *ast, t_shell *shell, int needfork,\
t_job *cur_job)
{
	if (ast->type == OPERATOR && ft_strequ(ast->content, ";"))
	{
		if (parse_ast(ast->left, shell, 1, cur_job))
			;
		if (parse_ast(ast->right, shell, 1, NULL))
			;
	}
	else if (ast->type == OPERATOR && ft_strequ(ast->content, "&&"))
	{
		if (parse_ast(ast->left, shell, 1, cur_job))
			parse_ast(ast->right, shell, 1, NULL);
	}
	else if (ast->type == OPERATOR && ft_strequ(ast->content, "||"))
	{
		if (!parse_ast(ast->left, shell, 1, cur_job))
			parse_ast(ast->right, shell, 1, NULL);
	}
	else if (ast->type == OPERATOR && ft_strequ(ast->content, "|"))
	{
		if (!parse_ast_pipe(ast, shell, cur_job))
			return (-1);
		if (needfork && cur_job->foreground)
			put_job_in_foreground(cur_job, 0);
	}
	return (parser_states_next(ast, shell, needfork, cur_job));
}

int					parse_ast(t_ast *ast, t_shell *shell, int needfork,\
t_job *cur_job)
{
	t_job	*job;

	if (!ast)
		return (-1);
	if (!cur_job)
	{
		cur_job = (t_job *)ft_memalloc(sizeof(t_job));
		ft_strncpy(cur_job->command, g_last_command,\
		sizeof(cur_job->command) - 1);
		cur_job->foreground = 1;
		if (!g_first_job)
			g_first_job = cur_job;
		else
		{
			job = g_first_job;
			while (job->next)
				job = job->next;
			job->next = cur_job;
		}
	}
	return (parser_states(ast, shell, needfork, cur_job));
}
