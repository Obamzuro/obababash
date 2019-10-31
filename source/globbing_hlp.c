/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing_hlp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/13 15:05:22 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/31 05:23:27 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

t_char_glob_corr	g_globs[] =
{
	{ '*', glob_wildcard },
	{ '?', glob_one_char },
	{ '[', glob_interval },
};

int					(*g_return_glob_func(char *pattern))(char *, char *)
{
	int		i;

	i = 0;
	while ((unsigned long)i < sizeof(g_globs) / sizeof(*g_globs))
	{
		if (*pattern == g_globs[i].character)
			return (g_globs[i].func);
		++i;
	}
	return (glob_standard);
}

int					is_globbing_need(char *arg)
{
	int		i;
	int		j;
	int		teoretical;

	i = 0;
	teoretical = 0;
	while (arg[i])
	{
		j = 0;
		if (arg[i] == '$')
			return (0);
		while ((unsigned long)j < sizeof(g_globs) / sizeof(*g_globs))
		{
			if (arg[i] == g_globs[j].character)
				teoretical = 1;
			++j;
		}
		++i;
	}
	if (teoretical)
		return (1);
	else
		return (0);
}

char				**globbing_arg(char *arg)
{
	int			i;
	int			j;
	int			amount_of_matches;
	t_glob_file	**files;
	char		**ret;

	i = 0;
	if (!is_globbing_need(arg))
	{
		ret = (char **)(malloc(sizeof(char *) * 2));
		ret[0] = ft_strdup(arg);
		ret[1] = NULL;
		return (ret);
	}
	files = generate_files();
	while (files[i])
	{
		if (!g_return_glob_func(arg)(files[i]->name, arg))
			files[i]->mark = NOTMATCH;
		++i;
	}
	i = -1;
	amount_of_matches = 0;
	while (files[++i])
		if (files[i]->mark == MATCH)
			++amount_of_matches;
	if (!amount_of_matches)
	{
		i = -1;
		while (files[++i])
		{
			free(files[i]->name);
			free(files[i]);
		}
		free(files);
		return (NULL);
	}
	ret = (char **)(malloc(sizeof(char *) * (amount_of_matches + 1)));
	i = -1;
	j = 0;
	while (files[++i])
	{
		if (files[i]->mark == MATCH)
		{
			ret[j] = ft_strdup(files[i]->name);
			++j;
		}
		free(files[i]->name);
		free(files[i]);
	}
	ret[j] = NULL;
	free(files);
	return (ret);
}

void				free_args(t_ftvector *all_args)
{
	int		i;
	int		j;
	char	**args;

	i = 0;
	while (i < all_args->len)
	{
		args = (char **)((all_args->elem)[i]);
		j = -1;
		while (args[++j])
			free(args[j]);
		++i;
	}
	free_ftvector(all_args);
}

void				glob_reassemble(char ***args, t_ftvector *all_args)
{
	int		i;
	int		j;
	int		args_amount;
	char	**args_piece;

	i = 0;
	args_amount = 0;
	while (i < all_args->len)
	{
		j = 0;
		args_piece = (char **)(all_args->elem[i]);
		while (args_piece[j])
		{
			++args_amount;
			++j;
		}
		++i;
	}
	*args = (char **)(malloc(sizeof(char *) * (args_amount + 1)));
	i = 0;
	args_amount = 0;
	while (i < all_args->len)
	{
		j = 0;
		args_piece = (char **)(all_args->elem[i]);
		while (args_piece[j])
		{
			(*args)[args_amount] = ft_strdup(args_piece[j]);
			free(args_piece[j]);
			++args_amount;
			++j;
		}
		++i;
	}
	(*args)[args_amount] = NULL;
	free_ftvector(all_args);
}
