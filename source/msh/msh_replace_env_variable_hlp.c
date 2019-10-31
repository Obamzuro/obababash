/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_replace_env_variable_hlp.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/13 15:05:22 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/31 05:58:15 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

int					replace_env_variable_repl_brackets(char **args, char **env,
		int i, int *j)
{
	char	*foundstable;

	args[i][*j] = 0;
	*j += 1;
	foundstable = ft_strchr_str(args[i] + *j + 1, "}");
	if (!foundstable)
		return (replace_env_variable_repl_brackets_end(args, env, i, j));
	else
		return (replace_env_variable_repl_brackets_middle(&args[i], j,\
		foundstable, env));
}

int					env_expansion_kernel(t_shell *shell,
		int i, char **args)
{
	int		squotemode;
	int		dquotemode;
	int		j;

	j = 0;
	squotemode = 0;
	dquotemode = 0;
	while (args[i][j])
		if (args[i][j] == '\"')
			++j && (dquotemode = !dquotemode);
		else if (args[i][j] == '\'' && !dquotemode)
			++j && (squotemode = !squotemode);
		else if (!squotemode && args[i][j] == '$' && !(j > 0\
		&& args[i][j - 1] == '\\'))
		{
			if (args[i][j + 1] == '{')
			{
				if (replace_env_variable_repl_brackets(args, shell->env, i, &j))
					return (-1);
				continue ;
			}
			else if (ft_is_char_in_str(args[i][j + 1], "\\$\'\" \t\n")
						|| !args[i][j + 1])
			{
				++j;
				continue ;
			}
			else if (replace_env_variable_repl(args, shell->env, i, &j))
				break ;
		}
		else
			++j;
	return (0);
}

int					env_expansion(t_shell *shell, char **args)
{
	int		i;

	i = -1;
	while (args[++i])
		if (env_expansion_kernel(shell, i, args))
			return (-1);
	return (0);
}
