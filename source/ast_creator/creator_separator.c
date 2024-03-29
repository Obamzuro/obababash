/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creator_separator.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 16:35:41 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/31 00:08:03 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

char				*(g_separator_op[AM_LEVELS][AM_SEPARATOROP]) =
{
	{ ";", NULL, NULL },
	{ "|", NULL, NULL },
	{ "&&", "||", NULL },
	{ "&", NULL, NULL }
};

static t_ast		*create_separator_ast_pipe(int beg,
		int end, t_shell *shell)
{
	int		pos;
	t_ast	*ast;
	t_lexer	*lexer;

	lexer = shell->lexer;
	ast = 0;
	if ((pos = last_token_pos(lexer, beg, end, g_separator_op[1])) == -1)
		return (create_redirection_ast(lexer, beg, end, shell));
	if (pos == beg || pos + 1 > end)
		return (print_error_zero("42sh: parse operator error\n"));
	ast = (t_ast *)ft_memalloc(sizeof(t_ast));
	ast->content = ft_strdup(((t_token *)lexer->tokens.elem[pos])->str);
	ast->type = OPERATOR;
	if (!(ast->left = create_separator_ast_pipe(beg,
			pos - 1, shell)) && free_ast(ast))
		return (0);
	if (!(ast->right = create_redirection_ast(lexer, pos + 1,
			end, shell)) && free_ast(ast))
		return (0);
	return (ast);
}

static t_ast		*create_separator_ast_logical(int beg,
		int end, t_shell *shell)
{
	int		pos;
	t_ast	*ast;
	t_lexer	*lexer;

	lexer = shell->lexer;
	ast = 0;
	if ((pos = first_token_pos(lexer, beg, end, g_separator_op[2])) == -1)
		return (create_separator_ast_pipe(beg, end, shell));
	if (pos == beg || pos + 1 > end)
		return (print_error_zero("42sh: parse operator [&&  ||] error\n"));
	ast = (t_ast *)ft_memalloc(sizeof(t_ast));
	ast->content = ft_strdup(((t_token *)lexer->tokens.elem[pos])->str);
	ast->type = OPERATOR;
	if (!(ast->left = create_separator_ast_pipe(beg, pos - 1,
			shell)) && free_ast(ast))
		return (0);
	if (!(ast->right = create_separator_ast_logical(pos + 1,
			end, shell)) && free_ast(ast))
		return (0);
	return (ast);
}

t_ast				*create_separator_ast(int beg, int end,
		t_shell *shell)
{
	int		pos;
	t_ast	*ast;
	t_lexer	*lexer;

	lexer = shell->lexer;
	ast = 0;
	if ((pos = last_token_pos(lexer, beg, end, g_separator_op[0])) == -1)
		return (create_separator_ast_logical(beg, end, shell));
	ast = (t_ast *)ft_memalloc(sizeof(t_ast));
	ast->content = ft_strdup(((t_token *)lexer->tokens.elem[pos])->str);
	ast->type = OPERATOR;
	if (pos != beg && !(ast->left = create_separator_ast(beg,
					pos - 1, shell)) && free_ast(ast))
		return (0);
	if (pos + 1 <= end && !(ast->right = create_separator_ast_logical(pos + 1,
					end, shell)) && free_ast(ast))
		return (0);
	return (ast);
}

t_ast				*create_background_ast(int beg, int end,
		t_shell *shell)
{
	int		pos;
	t_ast	*ast;
	t_lexer	*lexer;

	lexer = shell->lexer;
	ast = 0;
	pos = last_token_pos(lexer, beg, end, g_separator_op[3]);
	if (pos == -1)
		return (create_separator_ast(beg, end, shell));
	else if (pos != end || end == 0)
	{
		ft_fprintf(STDERR_FILENO, "Wrong location of [&] operator!\n");
		return (0);
	}
	ast = (t_ast *)ft_memalloc(sizeof(t_ast));
	ast->content = ft_strdup(((t_token *)lexer->tokens.elem[pos])->str);
	ast->type = OPERATOR;
	ast->left = create_separator_ast(beg, end, shell);
	return (ast);
}
