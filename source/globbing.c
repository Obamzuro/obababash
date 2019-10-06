/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/06 18:19:31 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/06 19:51:03 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

char		*glob_wildcard(t_glob_file *file)
{
	return (arg);
}

t_char_glob_corr globs[] =
{
	{ '*', glob_wildcard },
};

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

char		*globbing_arg(char *arg)
{
	int			i;
	int			j;
	char		*temp;
	t_glob_file	**files;

	i = 0;
	while (arg[i])
	{
		j = 0;
		while (j < sizeof(globs) / sizeof(*globs))
		{
			if (arg[i] == globs[j].character)
			{
				files = generate_files();
				if (!files)
					return (NULL);
				temp = globs[j].func(files);
			}
			++j;
		}
	}
	return (arg);
}

void		globbing(char **args)
{
	int		i;
	char	*res;

	i = 0;
	while (args[i])
	{
		res = globbing_arg(args[i]);

	}
}
