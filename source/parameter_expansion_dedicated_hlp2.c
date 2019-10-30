/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _hlp2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/13 15:05:22 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/31 00:32:28 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

char		*parameter_expansion_procent_big(char *key, char *additional,\
char **env)
{
	char		*value;
	char		*last;
	char		*temp_value;

	value = get_envs(key, 2, g_shell->internal, env);
	temp_value = ft_strdup(value);
	ft_strreverse(temp_value, ft_strlen(temp_value));
	ft_strreverse(additional, ft_strlen(additional));
	if (!additional[0])
		return (temp_value);
	else if (additional[0] == '*')
		last = parameter_expansion_sharp_big_wildchard(temp_value, additional);
	else
		last = parameter_expansion_sharp_big_character(temp_value, additional);
	if (last)
	{
		free(temp_value);
		return (ft_strdup(last));
	}
	else
		return (temp_value);
}

char		*parameter_expansion_sharp(char *key, char **env)
{
	char	*sharp;
	char	*additional;

	sharp = ft_strchr(key, '#');
	*sharp = '\0';
	additional = sharp + 1;
	if (additional[0] == '#')
		return (parameter_expansion_sharp_big(key, additional + 1, env));
	return (parameter_expansion_sharp_small(key, additional, env));
}

char		*parameter_expansion_procent(char *key, char **env)
{
	char	*sharp;
	char	*additional;
	char	*ret;

	sharp = ft_strchr(key, '%');
	*sharp = '\0';
	additional = sharp + 1;
	if (additional[0] == '%')
		ret = parameter_expansion_procent_big(key, additional + 1, env);
	else
		ret = parameter_expansion_procent_small(key, additional, env);
	ft_strreverse(ret, ft_strlen(ret));
	return (ret);
}

char		*parameter_expansion_length(char *key, char **env)
{
	char	*value;
	char	*ret;

	value = get_envs(key + 1, 2, g_shell->internal, env);
	ret = ft_itoa(ft_strlen(value));
	return (ret);
}

char		*parameter_expansion_dedicated(char *key, char **env)
{
	if (key[0] == '#')
		return (parameter_expansion_length(key, env));
	else if (ft_strchr(key, ':'))
		return (parameter_expansion_colon(key, env));
	else if (ft_strchr(key, '#'))
		return (parameter_expansion_sharp(key, env));
	else if (ft_strchr(key, '%'))
		return (parameter_expansion_procent(key, env));
	return (ft_strdup(get_envs(key, 2, g_shell->internal, env)));
}
