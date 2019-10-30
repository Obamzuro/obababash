/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 12:58:07 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/31 01:33:11 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

static int					lexing_handling_quotes_ifnull(t_shell *shell,
		t_token *token, char **last, char **command)
{
	char	*temp;

	temp = ft_chrjoin(token->str, '\n');
	line_editing_end(&shell->lineeditor, &shell->history);
	free(token->str);
	token->str = temp;
	free(*command);
	ft_putstr("\nq> ");
	shell->reading_mode = QUOTE;
	if ((!(*command = input_command(&shell->lineeditor,
					&shell->history, 'q', shell)) && ft_printf("\n"))
		|| shell->reading_mode == READEND)
	{
		if (shell->reading_mode == READEND)
			ft_printf("\n42sh: syntax error: unexpected eof\n");
		free(token->str);
		free(token);
		free_lineeditor(&shell->lineeditor);
		return (-1);
	}
	history_append("\n", &shell->history, 1);
	history_append(*command, &shell->history, 1);
	*last = *command - 1;
	return (0);
}

t_token						*lexing_handling_quotes(t_shell *shell,
		t_token *token, char **last, char **command)
{
	char	delim;
	char	*temp;

	delim = **last;
	if (!token)
		token = (t_token *)ft_memalloc(sizeof(t_token));
	lexing_handling_appword(token, delim);
	while (1)
	{
		++(*last);
		if (**last == delim && (*(*last - 1) != '\\' || delim != '\"'))
		{
			lexing_handling_appword(token, delim);
			break ;
		}
		if (!**last)
		{
			if (lexing_handling_quotes_ifnull(shell,
							token, last, command))
				return (NULL);
			else
				continue;
		}
		temp = ft_chrjoin(token->str, **last);
		free(token->str);
		token->str = temp;
	}
	return (token);
}

int							lexing_handling_baskslash(t_token **token,
		char **last, char **command)
{
	char	*temp;

	if (*token && (*token)->type == UKNOWN && (*last + 1))
		lexing_handling_appword(*token, **last);
	else if ((*last + 1))
		*token = lexing_handling_initword(*token, **last);
	++(*last);
	if (!**last)
	{
		temp = ft_chrjoin((*token)->str, '\n');
		line_editing_end(&g_shell->lineeditor, &g_shell->history);
		free((*token)->str);
		(*token)->str = temp;
		free(*command);
		ft_putstr("\n\\> ");
		g_shell->reading_mode = QUOTE;
		if ((!(*command = input_command(&g_shell->lineeditor,
						&g_shell->history, '\\', g_shell)) && ft_printf("\n"))
			|| g_shell->reading_mode == READEND)
		{
			if (g_shell->reading_mode == READEND)
				ft_printf("\n42sh: syntax error: unexpected eof\n");
			free((*token)->str);
			free(token);
			free_lineeditor(&g_shell->lineeditor);
			return (-1);
		}
		*last = *command - 1;
	}
	else if (*token && (*token)->type == UKNOWN)
		lexing_handling_appword(*token, **last);
	else
		*token = lexing_handling_initword(*token, **last);
	return (0);
}
