/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 17:17:25 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/31 05:34:20 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

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

void				push_variables_into_env(t_shell *shell, char **args,\
char ***env, char ***dop_env)
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
	(void)shell;
}

int					parse_ast_command(t_ast *ast, t_shell *shell,
		int needfork, t_job *cur_job)
{
	t_command_token		*command_token;
	char				**args;
	char				**vars;

	command_token = (t_command_token *)ast->content;
	vars = command_token->vars;
	globbing(&(((t_command_token *)ast->content)->args));
	args = command_token->args;
	tilde_expansion(shell, args);
	if (env_expansion(shell, args))
		return (0);
	backslash_handling(&command_token->args, 0);
	args = command_token->args;
	tilde_expansion(shell, vars);
	if (env_expansion(shell, vars))
		return (0);
	backslash_handling(&vars, 0);
	if (!args[0])
	{
		push_variables_into_env(shell, vars, &shell->internal, &shell->env);
		return (0);
	}
	if (!handle_commands(args, vars, shell))
	{
		push_variables_into_env(shell, shell->env, &vars, NULL);
		((t_command_token *)ast->content)->vars = vars;
		if (needfork)
		{
			if (ft_exec(args, &vars, 1, cur_job) == -1)
				return (-1);
		}
		else if (ft_exec(args, &vars, 0, cur_job) == -1)
			return (-1);
	}
	return (0);
}

int					parser_states_next(t_ast *ast, t_shell *shell,\
int needfork, t_job *cur_job)
{
	if (ast->type == OPERATOR && ft_strequ(ast->content, "&"))
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
