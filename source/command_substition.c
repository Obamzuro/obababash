/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_substition.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/26 17:22:38 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/30 17:51:35 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

static int		substitute(char **command, int *iter)
{
	char	*new_command;
	char	*end;
	char	*tmp;
	char	*tmp2;
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
//	if (!new_command)
//		return (-1);
	if (lexer_creating(new_command, g_shell))
	{
		free_lexer(g_shell->lexer);
		dup2(old_stdout, STDOUT_FILENO);
		close(fdpipe[0]);
		close(old_stdout);
		return (-1);
	}
	if (!g_shell->lexer->tokens.len)
	{
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
	if (creating_ast(g_shell))
	{
		free_lexer(g_shell->lexer);
		dup2(old_stdout, STDOUT_FILENO);
		close(fdpipe[0]);
		close(old_stdout);
		return (-1);
	}
	/// ?????????????????????????/
	parse_ast(g_shell->ast, g_shell, 1, NULL);
	free_lexer(g_shell->lexer);
	free_ast(g_shell->ast);
	ft_bzero(g_shell->lexer, sizeof(t_lexer));
	ft_bzero(arr, sizeof(arr));
	read(fdpipe[0], arr, sizeof(arr) - 1);
//	while (read(fdpipe[0], arr, sizeof(arr) - 1) > 0)
//	{
//		ft_fprintf(STDERR_FILENO, "%s: %s", "kek", arr);
//		ft_bzero(arr, sizeof(arr));
//	}
	dup2(old_stdout, STDOUT_FILENO);
	close(fdpipe[0]);
	close(old_stdout);
	*((*command) + *iter) = '\0';
	tmp = (*command);
	(*command) = ft_strjoin((*command), arr);
	tmp2 = (*command);
	(*command) = ft_strjoin((*command), end + 1);
	free(tmp);
	free(tmp2);
	return (0);
}

int				command_substition(char **command)
{
	int		i;

	i = 0;
	while ((*command)[i])
	{
		if ((*command)[i] == '$' && (*command)[i + 1] == '(')
			if (substitute(command, &i) == -1)
				return (-1);
		++i;
	}
	return (0);
}
