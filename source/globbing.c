/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/06 18:19:31 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/31 05:26:57 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

int					(*g_return_glob_func(char *filename))(char *, char *);

int					glob_standard(char *filename, char *pattern)
{
	if (*filename != *pattern)
		return (0);
	if (!*(filename + 1) && !*(pattern + 1))
		return (1);
	if (!*(pattern + 1))
		return (0);
	return (g_return_glob_func(pattern + 1)(filename + 1, pattern + 1));
}

int					glob_one_char(char *filename, char *pattern)
{
	if (!*(pattern + 1) && !*(filename + 1))
		return (1);
	if (!*(pattern + 1))
		return (0);
	return (g_return_glob_func(pattern + 1)(filename + 1, pattern + 1));
}

int					glob_wildcard(char *filename, char *pattern)
{
	if (!*(pattern + 1))
		return (1);
	if (!*filename && *(pattern + 1))
		return (0);
	if (g_return_glob_func(pattern)(filename + 1, pattern))
		return (1);
	if (g_return_glob_func(pattern + 1)(filename, pattern + 1))
		return (1);
	if (g_return_glob_func(pattern + 1)(filename + 1, pattern + 1))
		return (1);
	return (0);
}

char				**glob_interval_reassemble(t_ftvector *intervals)
{
	char		**ret;
	int			i;

	ret = (char **)malloc(sizeof(char *) * (intervals->len + 1));
	i = 0;
	while (i < intervals->len)
	{
		ret[i] = intervals->elem[i];
		++i;
	}
	ret[i] = NULL;
	free(intervals->elem);
	return (ret);
}

char				**glob_interval_get_intervals(char **pattern)
{
	t_ftvector	intervals;
	char		*interval;
	char		beg;
	char		end;

	init_ftvector(&intervals);
	while (1)
	{
		if (**pattern == '\\')
			++(*pattern);
		else if (**pattern == ']')
			break ;
		if (!**pattern)
		{
			free_ftvector(&intervals);
			return (NULL);
		}
		beg = **pattern;
		++(*pattern);
		if (**pattern == '-')
		{
			++(*pattern);
			if (**pattern == '\\')
				++(*pattern);
			if (!**pattern || **pattern == ']')
			{
				free_ftvector(&intervals);
				return (NULL);
			}
			end = **pattern;
			++(*pattern);
			interval = (char *)malloc(sizeof(char) * 3);
			interval[0] = beg;
			interval[1] = end;
			interval[2] = '\0';
		}
		else
		{
			interval = (char *)malloc(sizeof(char) * 2);
			interval[0] = beg;
			interval[1] = '\0';
		}
		push_ftvector(&intervals, interval);
	}
	return (glob_interval_reassemble(&intervals));
}
