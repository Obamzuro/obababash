/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_replace_env_variable.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/23 12:24:29 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/30 16:28:59 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

static int			replace_env_variable_repl_end(char **args,
		char **env, int i, int *j)
{
	char	*temp;

	(void)env;
	temp = args[i];
	args[i] = ft_strjoin(temp, get_envs(args[i] + *j + 1, 2,\
	g_shell->internal, env));
	free(temp);
	return (1);
}

static void			replace_env_variable_repl_middle(char **str,
		int *j, char *foundstable, char **env)
{
	char	*temp;
	char	*temp2;

	temp = ft_strsub(*str, *j + 1,
			foundstable - *str - *j - 1);
	temp2 = get_envs(temp, 2, g_shell->internal, env);
	*j += ft_strlen(temp2);
	free(temp);
	temp = *str;
	*str = ft_strjoin(temp, temp2);
	temp2 = ft_strdup(foundstable);
	free(temp);
	temp = *str;
	*str = ft_strjoin(temp, temp2);
	free(temp);
	free(temp2);
}

static int			replace_env_variable_repl(char **args, char **env,
		int i, int *j)
{
	char	*foundstable;

	args[i][*j] = 0;
	foundstable = ft_strchr_str(args[i] + *j + 1, "\\$\'\" \t\n");
	if (!foundstable && replace_env_variable_repl_end(args, env, i, j))
		return (1);
	else
		replace_env_variable_repl_middle(&args[i], j, foundstable, env);
	return (0);
}

static int			replace_env_variable_repl_brackets_end(char **args,
		char **env, int i, int *j)
{
	char	*temp;
	char	*key;

	(void)env;
	temp = args[i];
	key = parameter_expansion_dedicated(args[i] + *j + 1, env);
	if (!key)
		return (-1);
	args[i] = ft_strjoin(temp, key);
	free(key);
	free(temp);
	return (0);
}

static int			replace_env_variable_repl_brackets_middle(char **str,
		int *j, char *foundstable, char **env)
{
	char	*temp;
	char	*temp2;

	temp = ft_strsub(*str, *j + 1,
			foundstable - *str - *j - 1);
	temp2 = parameter_expansion_dedicated(temp, env);
	*j += ft_strlen(temp2);
	free(temp);
	if (!temp2)
		return (-1);
	temp = *str;
	*str = ft_strjoin(temp, temp2);
	free(temp2);
	temp2 = ft_strdup(foundstable + 1);
	free(temp);
	temp = *str;
	*str = ft_strjoin(temp, temp2);
	free(temp);
	free(temp2);
	return (0);
}

static int			replace_env_variable_repl_brackets(char **args, char **env,
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
			if (args[i][j + 1] == '{')// IS NEED?
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
