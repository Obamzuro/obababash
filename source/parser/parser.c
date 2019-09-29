/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 17:17:25 by obamzuro          #+#    #+#             */
/*   Updated: 2019/09/29 19:20:06 by obamzuro         ###   ########.fr       */
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
	{
		free_double_arr(((t_command_token *)ast->content)->args);
		free_double_arr(((t_command_token *)ast->content)->vars);
		free(ast->content);
	}
	else
		free(ast->content);
	free(ast);
	return (1);
}

static int			handle_commands(char **args, char **vars,
		t_shell *shell)
{
	int					i;
	extern t_comm_corr	g_commands[AM_COMMANDS];

	i = -1;
	while (++i < AM_COMMANDS)
	{
		if (!ft_strncmp(g_commands[i].comm, args[0],
					ft_strlen(g_commands[i].comm) + 1))
		{
			g_commands[i].func(args, vars, shell);
			break ;
		}
	}
	if (i == AM_COMMANDS)
		return (0);
	return (1);
}

void				push_variables_into_env(t_shell *shell, char **args, char ***env, char ***dop_env)
{
	int		i;
	char	*key;
	char	*value;

	i = 0;
	while (args[i])
	{
		key = ft_strsub(args[i], 0, ft_strchr(args[i], '=') - args[i]);
		value = ft_strdup(ft_strchr(args[i], '=') + 1);
		if (dop_env && set_env_check_existing(key, value, *dop_env))
			;
		else
			set_env(key, value, env);
		free(key);
		free(value);
		++i;
	}
}

int					parse_ast_command(t_ast *ast, t_shell *shell,
		int needfork, t_job *cur_job)
{
	int					ret;
	char				**variables;
	t_command_token		*command_token;
	char				**args;
	char				**vars;

	command_token = (t_command_token *)ast->content;
	args = command_token->args;
	vars = command_token->vars;
	tilde_expansion(shell, args);
	env_expansion(shell, args);
	backslash_handling(&command_token->args);
//	quote_removing(shell, args);
	args = command_token->args;

	tilde_expansion(shell, vars);
	env_expansion(shell, vars);
	backslash_handling(&vars);
//	quote_removing(shell, vars);

	if (!args[0])
	{
		push_variables_into_env(shell, vars, &shell->internal, &shell->env);
		return (0);
	}
	push_variables_into_env(shell, shell->env, &vars, NULL);
	((t_command_token *)ast->content)->vars = vars;
//	get_child_variables(shell, &ast->content);
	if (!handle_commands(args, vars, shell))
	{
		if (needfork)
		{
			if (ft_exec(args, &vars, 1, cur_job) == -1)
				return (-1);
//			if (cur_job->foreground)
//				while (wait(&ret) == -1)
//					;
//			else
//			{
//				cur_job->pgid = 
//			}
		}
		else if (ft_exec(args, &vars, 0, cur_job) == -1)
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
		ft_strncpy(cur_job->command, last_command, sizeof(cur_job->command) - 1);
		cur_job->foreground = 1;
		if (!first_job)
			first_job = cur_job;
		else
		{
			t_job *job = first_job;
			while (job->next)
				job = job->next;
			job->next = cur_job;
		}
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
		if (needfork && cur_job->foreground)
			put_job_in_foreground(cur_job, 0);
	}
	else if (ast->type == OPERATOR && ft_strequ(ast->content, "&"))
	{
		cur_job->foreground = 0;
		if (!parse_ast(ast->left, shell, needfork, cur_job))
			return (-1);
	}
	else if (ast->type == REDIRECTION &&
			!parse_ast_redirection(ast, shell, needfork, cur_job))
		return (-1);
	else if (ast->type == COMMAND)
		return (parse_ast_command(ast, shell, needfork, cur_job));
	return (0);
}
