/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_hlp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/13 15:05:22 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/31 05:40:36 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

char			**alias_expansion_help(char **command, int i, char *value)
{
	char	*temp;
	char	separator;

	separator = (*command)[i];
	(*command)[i] = '\0';
	temp = *command;
	*command = (char *)ft_memalloc(ft_strlen(value) + 1\
	+ ft_strlen((*command) + i + 1) + 1);
	ft_strcpy((*command), value);
	(*command)[ft_strlen(value)] = separator;
	ft_strcat((*command), temp + i + 1);
	(*command)[ft_strlen((*command))] = 0;
	free(temp);
	return (command);
}

void			alias_expansion(char **command)
{
	int		i;
	char	*word;
	char	*temp;
	char	*value;

	word = NULL;
	i = 0;
	while (1)
	{
		if (ft_is_char_in_str((*command)[i], " \t\n&|><;\v\r")\
		|| !(*command)[i])
		{
			if ((value = get_env(word, g_shell->aliases)))
				command = alias_expansion_help(command, i, value);
			break ;
		}
		temp = word;
		word = (char *)malloc(ft_strlen(temp) + 2);
		ft_strcpy(word, temp);
		word[ft_strlen(temp)] = (*command)[i];
		word[ft_strlen(temp) + 1] = '\0';
		free(temp);
		++i;
	}
	free(word);
}

int				creating_ast(t_shell *shell)
{
	if (!(shell->ast = create_background_ast(0,
					shell->lexer->tokens.len - 1, shell)))
	{
		free_lexer(shell->lexer);
		return (-1);
	}
	return (0);
}
