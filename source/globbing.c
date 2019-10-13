/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/06 18:19:31 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/13 15:34:34 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

int			(*return_glob_func(char *filename))(char *, char *);

int glob_standard(char *filename, char *pattern)
{
	if (*filename != *pattern)
		return (0);
	if (!*(filename + 1) && !*(pattern + 1))
		return (1);
//	if (!*(filename + 1))
//		return (0);
	if (!*(pattern + 1))
		return (0);
	return (return_glob_func(pattern + 1)(filename + 1, pattern + 1));
}

int	glob_one_char(char *filename, char *pattern)
{
	if (!*(pattern + 1) && !*(filename + 1))
		return (1);
//	if (!*(filename + 1))
//		return (0);
	if (!*(pattern + 1))
		return (0);
	return (return_glob_func(pattern + 1)(filename + 1, pattern + 1));
}

int glob_wildcard(char *filename, char *pattern)
{
//	ft_printf("%15s | %15s\n", filename, pattern);
	if (!*(pattern + 1))
		return (1);
	if (!*filename && *(pattern + 1))
		return (0);
	if (return_glob_func(pattern)(filename + 1, pattern))
		return (1);
	if (return_glob_func(pattern + 1)(filename, pattern + 1))
		return (1);
	if (return_glob_func(pattern + 1)(filename + 1, pattern + 1))
		return (1);
	return (0);
}

int	glob_interval(char *filename, char *pattern)
{
	char	begin;
	char	end;

	if (!(*pattern + 1) || *(pattern + 2) != '-' || !*(pattern + 3) || *(pattern + 4) != ']')
		return (0);
	begin = *(pattern + 1);
	end = *(pattern + 3);
	pattern += 4;
	if (*filename < begin || *filename > end)
		return (0);
	if (!*(filename + 1) && !*(pattern + 1))
		return (1);
//	if (!*(filename + 1))
//		return (0);
	if (!*(pattern + 1))
		return (0);
	return (return_glob_func(pattern + 1)(filename + 1, pattern + 1));
}

t_char_glob_corr globs[] =
{
	{ '*', glob_wildcard },
	{ '?', glob_one_char },
	{ '[', glob_interval },
};

int			(*return_glob_func(char *pattern))(char *, char *)
{
	int		i;

	i = 0;
	while (i < sizeof(globs) / sizeof(*globs))
	{
		if (*pattern == globs[i].character)
			return (globs[i].func);
		++i;
	}
	return (glob_standard);
}

t_glob_file	**generate_files(void)
{
	t_glob_file		**files;
	DIR				*dir;
	struct dirent	*dp;
	char			*current_dir;
	int				amount_of_files;
	int				i;

	current_dir = getcwd(0, 0);
	dir = opendir(current_dir);
	if (!dir)
	{
		ft_fprintf(STDERR_FILENO, "42sh: can't access current directory for globbing\n");
		return (NULL);
	}
	amount_of_files = 0;
	while ((dp = readdir(dir)) != NULL)
		++amount_of_files;
	closedir(dir);
	dir = opendir(current_dir);
	free(current_dir);
	if (!dir)
	{
		ft_fprintf(STDERR_FILENO, "42sh: can't access current directory for globbing\n");
		return (NULL);
	}
	files = (t_glob_file **)malloc(sizeof(t_glob_file *) * (amount_of_files + 1));
	i = 0;
	while ((dp = readdir(dir)) != NULL)
	{
		files[i] = (t_glob_file *)malloc(sizeof(t_glob_file));
		files[i]->name = ft_strdup(dp->d_name);
		files[i]->mark = MATCH;
		++i;
	}
	closedir(dir);
	if (i != amount_of_files)
	{
		while (i != amount_of_files)
		{
			files[i] = (t_glob_file *)malloc(sizeof(t_glob_file));
			files[i]->name = NULL;
			files[i]->mark = WASDELETED;
			++i;
		}
	}
	files[i] = NULL;
	return (files);
}

int			is_globbing_need(char *arg)
{
	int		i;
	int		j;

	i = 0;
	while (arg[i])
	{
		j = 0;
		while (j < sizeof(globs) / sizeof(*globs))
		{
			if (arg[i] == globs[j].character)
				return (1);
			++j;
		}
		++i;
	}
	return (0);
}

char		**globbing_arg(char *arg)
{
	int			i;
	int			j;
	int			k;
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
		if (!return_glob_func(arg)(files[i]->name, arg))
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
	//return_glob_func();
}

void		free_args(t_ftvector *all_args)
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
//		free(args);
		++i;
	}
	free_ftvector(all_args);
}

void		glob_reassemble(char ***args, t_ftvector *all_args)
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

int			globbing(char ***args)
{
	int			i;
	t_ftvector	all_args;
	char		**glob_filenames;

	i = 0;
	init_ftvector(&all_args);
	while ((*args)[i])
	{
		glob_filenames = globbing_arg((*args)[i]);
		if (!glob_filenames)
		{
			free_args(&all_args);
			ft_fprintf(STDERR_FILENO, "42sh: no matches found: %s\n", (*args)[i]);
			return (-1);
		}
		push_ftvector(&all_args, glob_filenames);
		++i;
	}
	i = -1;
	while ((*args)[++i])
		free((*args)[i]);
	free(*args);
	glob_reassemble(args, &all_args);
	return (0);
}