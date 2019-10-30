/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creator_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 13:07:28 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/31 01:43:09 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

static int			create_command_getcount_args(t_lexer *lexer,
		int beg, int end, t_tokentype type)
{
	int		i;

	i = 0;
	while (beg <= end)
	{
		if (((t_token *)lexer->tokens.elem[beg])->type == type)
			++i;
		++beg;
	}
	return (i);
}

static t_ast		*ast_filler(char **args, char **vars)
{
	t_ast			*ast;
	t_command_token	*command_token;

	ast = (t_ast *)ft_memalloc(sizeof(t_ast));
	command_token = (t_command_token *)malloc(sizeof(t_command_token));
	command_token->args = args;
	command_token->vars = vars;
	ast->content = (void *)command_token;
	ast->type = COMMAND;
	return (ast);
}

t_ast				*create_command(t_lexer *lexer, int beg, int end)
{
	char			**args;
	char			**vars;
	int				i[2];

	args = (char **)ft_memalloc(sizeof(char *) *
			(create_command_getcount_args(lexer, beg, end, WORD) + 1));
	vars = (char **)ft_memalloc(sizeof(char *) *
			(create_command_getcount_args(lexer, beg, end, VARIABLE) + 1));
	i[0] = 0;
	i[1] = 0;
	while (beg <= end)
	{
		if (((t_token *)lexer->tokens.elem[beg])->type == WORD)
		{
			args[i[0]] = ft_strdup(((t_token *)lexer->tokens.elem[beg])->str);
			++i[0];
		}
		if (((t_token *)lexer->tokens.elem[beg])->type == VARIABLE)
		{
			vars[i[1]] = ft_strdup(((t_token *)lexer->tokens.elem[beg])->str);
			++i[1];
		}
		++beg;
	}
	return (ast_filler(args, vars));
}
