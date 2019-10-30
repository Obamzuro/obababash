/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _hlp4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/13 15:05:22 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/31 00:35:14 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

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
