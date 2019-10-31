/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_substition.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 17:22:38 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/31 03:51:58 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

static void		sub_help_three(char **command, int *iter, char arr[],\
char *end)
{
	char	*tmp;
	char	*tmp2;

	*((*command) + *iter) = '\0';
	tmp = (*command);
	(*command) = ft_strjoin((*command), arr);
	tmp2 = (*command);
	(*command) = ft_strjoin((*command), end + 1);
	free(tmp);
	free(tmp2);
}

static void		sub_help_two(int old_stdout, int fdpipe[], char arr[])
{
	parse_ast(g_shell->ast, g_shell, 1, NULL);
	free_lexer(g_shell->lexer);
	free_ast(g_shell->ast);
	ft_bzero(g_shell->lexer, sizeof(t_lexer));
	ft_bzero(arr, 10000);
	read(fdpipe[0], arr, 10000 - 1);
	dup2(old_stdout, STDOUT_FILENO);
	close(fdpipe[0]);
	close(old_stdout);
}

static int		sub_help(int old_stdout, int fdpipe[], char **command,\
int *iter)
{
	char	*tmp;
	char	*end;

	end = ft_strchr((*command) + *iter + 2, ')');
	free_lexer(g_shell->lexer);
	dup2(old_stdout, STDOUT_FILENO);
	close(fdpipe[0]);
	close(old_stdout);
	*((*command) + *iter) = '\0';
	tmp = (*command);
	(*command) = ft_strjoin((*command), end + 1);
	free(tmp);
	return (-2);
}

int				substitute(char **command, int *iter)
{
	char	*new_command;
	char	*end;
	int		old_stdout;
	int		fdpipe[2];
	char	arr[10000];

	end = ft_strchr((*command) + *iter + 2, ')');
	if (!end)
		return (-1);
	new_command = ft_strsub((*command) + *iter + 2, 0, end -\
	(*command) - *iter - 2);
	if (pipe(fdpipe) == -1)
		return (-1);
	old_stdout = dup(STDOUT_FILENO);
	dup2(fdpipe[1], STDOUT_FILENO);
	close(fdpipe[1]);
	if (lexer_creating(new_command, g_shell))
	{
		free_lexer(g_shell->lexer);
		dup2(old_stdout, STDOUT_FILENO);
		close(fdpipe[0]);
		close(old_stdout);
		return (-1);
	}
	if (!g_shell->lexer->tokens.len)
		return (sub_help(old_stdout, fdpipe, command, iter));
	if (creating_ast(g_shell))
	{
		free_lexer(g_shell->lexer);
		dup2(old_stdout, STDOUT_FILENO);
		close(fdpipe[0]);
		close(old_stdout);
		return (-1);
	}
	sub_help_two(old_stdout, fdpipe, arr);
	sub_help_three(command, iter, arr, end);
	return (0);
}
