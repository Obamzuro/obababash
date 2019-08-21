/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 17:17:25 by obamzuro          #+#    #+#             */
/*   Updated: 2019/08/14 19:23:10 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

/*
** void				print_ast(t_ast *ast)
** {
** 	int		i;
**
** 	i = 0;
** 	while (1)
** 	{
** 		if (!ast)
** 			break ;
** 		if (ast->type == COMMAND)
** 			ft_printf("AST #%d - %s %d\n", i,
** ((char **)ast->content)[0], ast->type);
** 		else
** 			ft_printf("AST #%d - %s %d\n", i, ast->content, ast->type);
** 		if (ast->right)
** 		{
** 			if (ast->type == REDIRECTION)
** 				ft_printf("AST #%d - |%s  %s| %d\n", i,
** 					((t_binary_token *)ast->right->content)->left,
** 					((t_binary_token *)ast->right->content)->right,
** 					ast->right->type);
** 			else if (ast->type == COMMAND)
** 				ft_printf("AST #%d - %s %d\n", i,
** ((char **)ast->content)[0], ast->type);
** 			else
** 				ft_printf("AST #%d - %s %d\n", i, ast->right->content,
** 						ast->right->type);
** 		}
** 		else
** 			ft_printf("AST #%d right - NULL\n", i);
** 		ast = ast->left;
** 		++i;
** 	}
** }
*/

int					free_ast(t_ast *ast)
{
	if (!ast)
		return (1);
	if (ast->type == REDIRECTION && ft_strequ((char *)ast->content, "<<"))
		free(((t_binary_token *)(ast->right->content))->right);
	free_ast(ast->left);
	free_ast(ast->right);
	if (ast->type == COMMAND)
		free_double_arr(ast->content);
	else
		free(ast->content);
	free(ast);
	return (1);
}

static int			handle_commands(char **args,
		char ***env)
{
	int					i;
	extern t_comm_corr	g_commands[AM_COMMANDS];

	i = -1;
	while (++i < AM_COMMANDS)
	{
		if (!ft_strncmp(g_commands[i].comm, args[0],
					ft_strlen(g_commands[i].comm) + 1))
		{
			g_commands[i].func(args, env);
			break ;
		}
	}
	if (i == AM_COMMANDS)
		return (0);
	return (1);
}

int					parse_ast_command(t_ast *ast, t_shell *shell,
		int needfork, t_job *cur_job)
{
	int		ret;

	tilde_expansion(shell, ast->content);
	env_expansion(shell, ast->content);
	quote_removing(shell, ast->content);
	if (!handle_commands(ast->content, &shell->env))
	{
		if (needfork)
		{
			if (ft_exec(ast->content, &shell->env, 1, cur_job) == -1)
				return (-1);
			while (wait(&ret) == -1)
				;
		}
		else if (ft_exec(ast->content, &shell->env, 0, cur_job) == -1)
			return (-1);
	}
	return (ret);
}

int					parse_ast(t_ast *ast, t_shell *shell, int needfork, t_job *cur_job)
{
	if (!ast)
		return (-1);
	if (!cur_job)
	{
		cur_job = (t_job *)ft_memalloc(sizeof(t_job));
	}
	if (ast->type == OPERATOR && ft_strequ(ast->content, ";"))
	{
		if (parse_ast(ast->left, shell, 1, cur_job))
			;
		if (parse_ast(ast->right, shell, 1, NULL))
			;
	}
	else if (ast->type == OPERATOR && ft_strequ(ast->content, "&&"))
	{
		if (!parse_ast(ast->left, shell, 1, cur_job))
			parse_ast(ast->right, shell, 1, NULL);
	}
	else if (ast->type == OPERATOR && ft_strequ(ast->content, "||"))
	{
		if (parse_ast(ast->left, shell, 1, cur_job))
			parse_ast(ast->right, shell, 1, NULL);
	}
	else if (ast->type == OPERATOR && ft_strequ(ast->content, "|"))
	{
		if (!parse_ast_pipe(ast, shell, cur_job))
			return (-1);
		put_job_in_foreground(cur_job, 0)
	}
	else if (ast->type == REDIRECTION &&
			!parse_ast_redirection(ast, shell, needfork, cur_job))
		return (-1);
	else if (ast->type == COMMAND)
		return (parse_ast_command(ast, shell, needfork, cur_job));
	return (0);
}
