/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameter_expansion_dedicated.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 17:12:01 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/30 18:27:19 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

char		*parameter_expansion_plus(char *key, char *additional, char **env)
{
	char	*value;
	char	*ret;

	value = get_envs(key, 2, g_shell->internal, env);
	if (!value)
		return (ft_strdup(""));
	else
		return (ft_strdup(additional + 1));
}

char		*parameter_expansion_equal(char *key, char *additional, char **env)
{
	char	*value;
	char	*ret;

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
	char	*ret;

	value = get_envs(key, 2, g_shell->internal, env);
	if (!value)
		return (ft_strdup(additional + 1));
	else
		return (ft_strdup(value));
}

char		*parameter_expansion_error(char *key, char *additional, char **env)
{
	char	*value;
	char	*ret;

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

char		*parameter_expansion_sharp_small_character(char *str, char *pattern);
char		*parameter_expansion_sharp_big_character(char *str, char *pattern);

char		*parameter_expansion_sharp_small_wildchard(char *str, char *pattern)
{
	char	*temp;

	if (!*(pattern + 1))
		return (str);
	if (!*str && *(pattern + 1))
		return (NULL);
	if (*(pattern + 1) == '*')
		temp = parameter_expansion_sharp_small_wildchard(str, pattern + 1);
	else
		temp = parameter_expansion_sharp_small_character(str, pattern + 1);
	if (!temp)
		temp = parameter_expansion_sharp_small_wildchard(str + 1, pattern);
	return (temp);
}

char		*parameter_expansion_sharp_big_wildchard(char *str, char *pattern)
{
	char	*temp;

	if (!*str)
	{
		if (*(pattern + 1))
			return (NULL);
		else
			return (str);
	}
	temp = parameter_expansion_sharp_big_wildchard(str + 1, pattern);
	if (temp)
		return (temp);
	if (!*(pattern + 1))
		return (str);
	else if (*(pattern + 1) == '*')
		temp = parameter_expansion_sharp_big_wildchard(str, pattern + 1);
	else
		temp = parameter_expansion_sharp_big_character(str, pattern + 1);
	return (temp);
}

char		*parameter_expansion_sharp_small_character(char *str,\
char *pattern)
{
	char	*temp;

//	if (!*(pattern + 1))
//		return (str);
	if (*str != *pattern)
		return (NULL);
	if (!*(pattern + 1))
		return (str + 1);
	temp = NULL;
	if (*(pattern + 1) == '*')
		temp = parameter_expansion_sharp_small_wildchard(str + 1, pattern + 1);
	else
		temp = parameter_expansion_sharp_small_character(str + 1, pattern + 1);
	return (temp);
}

char		*parameter_expansion_sharp_big_character(char *str, char *pattern)
{
	char	*temp;

//	if (!*(pattern + 1))
//		return (str);
	if (*str != *pattern)
		return (NULL);
	if (!*(pattern + 1))
		return (str + 1);
	temp = NULL;
	if (*(pattern + 1) == '*')
		temp = parameter_expansion_sharp_big_wildchard(str + 1, pattern + 1);
	else
		temp = parameter_expansion_sharp_big_character(str + 1, pattern + 1);
	return (temp);
}

char		*parameter_expansion_sharp_small(char *key, char *additional,\
char **env)
{
	char		*value;
	char		*last;

	value = get_envs(key, 2, g_shell->internal, env);
	if (!additional[0])
		return (ft_strdup(value));
	else if (additional[0] == '*')
		last = parameter_expansion_sharp_small_wildchard(value, additional);
	else
		last = parameter_expansion_sharp_small_character(value, additional);
	if (last)
		return (ft_strdup(last));
	else
		return (ft_strdup(value));
}

char		*parameter_expansion_sharp_big(char *key, char *additional,\
char **env)
{
	char		*value;
	char		*last;

	value = get_envs(key, 2, g_shell->internal, env);
	if (!additional[0])
		return (ft_strdup(value));
	else if (additional[0] == '*')
		last = parameter_expansion_sharp_big_wildchard(value, additional);
	else
		last = parameter_expansion_sharp_big_character(value, additional);
	if (last)
		return (ft_strdup(last));
	else
		return (ft_strdup(value));
}

char		*parameter_expansion_procent_small(char *key, char *additional,\
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
		last = parameter_expansion_sharp_small_wildchard(temp_value,\
		additional);
	else
		last = parameter_expansion_sharp_small_character(temp_value,\
		additional);
	if (last)
	{
		free(temp_value);
		return (ft_strdup(last));
	}
	else
		return (temp_value);
}

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
