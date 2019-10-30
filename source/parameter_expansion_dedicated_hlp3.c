/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameter_expansion_dedicated_hlp3.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/13 15:05:22 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/31 00:34:22 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

char		*parameter_expansion_sharp_small_character(char *str,\
char *pattern)
{
	char	*temp;

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
