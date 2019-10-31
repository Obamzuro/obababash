/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing_hlp2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/13 15:05:22 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/31 05:27:20 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

int					globbing(char ***args)
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
			ft_fprintf(STDERR_FILENO, "42sh: no matches found: %s\n",\
			(*args)[i]);
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

int					glob_interval(char *filename, char *pattern)
{
	char	**intervals;
	char	is_inverse;
	int		i;

	++pattern;
	is_inverse = 0;
	if (*pattern == '!')
		is_inverse = 1;
	intervals = glob_interval_get_intervals(&pattern);
	if (!intervals)
		return (0);
	i = 0;
	while (intervals[i])
	{
		if (!intervals[i][1] && *filename == intervals[i][0])
			break ;
		else if (intervals[i][1] && (*filename >= intervals[i][0]\
		&& *filename <= intervals[i][1]))
			break ;
		++i;
	}
	if (!is_inverse && !intervals[i])
	{
		free_double_arr(intervals);
		return (0);
	}
	if (is_inverse && intervals[i])
	{
		free_double_arr(intervals);
		return (0);
	}
	free_double_arr(intervals);
	if (!*(filename + 1) && !*(pattern + 1))
		return (1);
	if (!*(pattern + 1))
		return (0);
	return (g_return_glob_func(pattern + 1)(filename + 1, pattern + 1));
}

t_glob_file			**generate_files(void)
{
	t_glob_file		**files;
	DIR				*dir;
	struct dirent	*dp;
	char			*current_dir;
	int				amnt_files[2];

	current_dir = getcwd(0, 0);
	dir = opendir(current_dir);
	if (!dir)
	{
		ft_fprintf(STDERR_FILENO,\
		"42sh: can't access current directory for globbing\n");
		return (NULL);
	}
	amnt_files[0] = 0;
	while ((dp = readdir(dir)) != NULL)
		++amnt_files[0];
	closedir(dir);
	dir = opendir(current_dir);
	free(current_dir);
	if (!dir)
	{
		ft_fprintf(STDERR_FILENO,\
		"42sh: can't access current directory for globbing\n");
		return (NULL);
	}
	files = (t_glob_file **)malloc(sizeof(t_glob_file *) *\
	(amnt_files[0] + 1));
	amnt_files[1] = 0;
	while ((dp = readdir(dir)) != NULL)
	{
		files[amnt_files[1]] = (t_glob_file *)malloc(sizeof(t_glob_file));
		files[amnt_files[1]]->name = ft_strdup(dp->d_name);
		files[amnt_files[1]]->mark = MATCH;
		++amnt_files[1];
	}
	closedir(dir);
	if (amnt_files[1] != amnt_files[0])
	{
		while (amnt_files[1] != amnt_files[0])
		{
			files[amnt_files[1]] = (t_glob_file *)malloc(sizeof(t_glob_file));
			files[amnt_files[1]]->name = NULL;
			files[amnt_files[1]]->mark = WASDELETED;
			++amnt_files[1];
		}
	}
	files[amnt_files[1]] = NULL;
	return (files);
}
