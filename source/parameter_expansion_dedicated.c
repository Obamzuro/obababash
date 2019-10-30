/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameter_expansion_dedicated.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 17:12:01 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/31 00:56:42 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

char		*parameter_expansion_plus(char *key, char *additional, char **env)
{
	char	*value;

	value = get_envs(key, 2, g_shell->internal, env);
	if (!value)
		return (ft_strdup(""));
	else
		return (ft_strdup(additional + 1));
}

char		*parameter_expansion_equal(char *key, char *additional, char **env)
{
	char	*value;

	value = get_envs(key, 2, g_shell->internal, env);
	if (!value)
	{
		set_env(key, additional + 1, &g_shell->internal);
		return (ft_strdup(additional + 1));
	}
	else
		return (ft_strdup(value));
}

char		*parameter_expansion_minus(char *key, char *additional, char **env)
{
	char	*value;

	value = get_envs(key, 2, g_shell->internal, env);
	if (!value)
		return (ft_strdup(additional + 1));
	else
		return (ft_strdup(value));
}

char		*parameter_expansion_error(char *key, char *additional, char **env)
{
	char	*value;

	value = get_envs(key, 2, g_shell->internal, env);
	if (!value)
	{
		ft_fprintf(STDERR_FILENO, "42sh: %s: %s\n", key, additional + 1);
		return (NULL);
	}
	else
		return (ft_strdup(value));
}

char		*parameter_expansion_colon(char *key, char **env)
{
	char	*colon;
	char	*additional;

	colon = ft_strchr(key, ':');
	*colon = '\0';
	additional = colon + 1;
	if (additional[0] == '?')
		return (parameter_expansion_error(key, additional, env));
	else if (additional[0] == '-')
		return (parameter_expansion_minus(key, additional, env));
	else if (additional[0] == '=')
		return (parameter_expansion_equal(key, additional, env));
	else if (additional[0] == '+')
		return (parameter_expansion_plus(key, additional, env));
	return (0);
}
