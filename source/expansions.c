/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 14:47:04 by obamzuro          #+#    #+#             */
/*   Updated: 2019/09/27 15:01:32 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

int					tilde_expansion(t_shell *shell, char **args)
{
	int			i;
	char		*temp;

	i = -1;
	while (args[++i])
	{
		if (args[i][0] == '~')
		{
			temp = ft_strjoin(get_env("HOME", shell->env), args[i] + 1);
			free(args[i]);
			args[i] = temp;
		}
	}
	return (0);
}

void				quote_removing_str(char **str)
{
	char		*temp;
	char		*temp2;
	char		*quote;
	char		*quote2;
	int			nextsearch;

	nextsearch = 0;
	while (*str && **str)
	{
		if (!(quote = ft_strchr_str(*str + nextsearch, "\'\"")) ||
			!(quote2 = ft_strchr(quote + 1, *quote)))
			return ;
		nextsearch = quote2 - *str - 1;
		*quote = 0;
		*quote2 = 0;
		temp = *str;
		temp2 = ft_strjoin(*str, quote + 1);
		*str = ft_strjoin(temp2, quote2 + 1);
		free(temp);
		free(temp2);
		if (!*(*str + nextsearch))
			break ;
	}
}

int					quote_removing(t_shell *shell, char **args)
{
	int			i;

	(void)shell;
	i = -1;
	while (args[++i])
		quote_removing_str(&args[i]);
	return (0);
}

void				reassemble_args(char ***args, int elem_to_delete)
{
	int		i;
	char	**new_args;
	char	is_deleted;

	i = 0;
	while ((*args)[i])
		++i;
	new_args = (char **)malloc(sizeof(char *) * i);
	i = 0;
	is_deleted = 0;
	while ((*args)[i])
	{
		if (i == elem_to_delete)
			elem_to_delete = 1;
		else if (!is_deleted)
			new_args[i] = (*args)[i];
		else if (is_deleted)
			new_args[i] = (*args)[i + 1];
		++i;
	}
	*args = new_args;
	free(*args);
}

void				backslash_handling(char ***args)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while ((*args)[i])
	{
		j = 0;
		while ((*args)[i][j])
		{
			if ((*args)[i][j] == '\\')
			{
				if ((*args)[i][j + 1] == '\n')
				{
					if (!(*args)[i][j + 2])
						reassemble_args(args, i);
					else
					{
						(*args)[i][j] = 0;
						temp = ft_strjoin((*args)[i], (*args)[i] + j + 2);
						free((*args)[i]);
						(*args)[i] = temp;
					}
				}
				else
				{
					(*args)[i][j] = 0;
					temp = ft_strjoin((*args)[i], (*args)[i] + j + 1);
					free((*args)[i]);
					(*args)[i] = temp;
				}
			}
			++j;
		}
		++i;
	}
}

//void				push_internals_variables(t_shell *shell, char **args)
//{
//	int		i;
//	char	*key;
//	char	*value;
//
//	i = 0;
//	while (args[i])
//	{
//		key = ft_strsub(args[i], 0, ft_strchr(args[i], '=') - args[i]);
//		value = ft_strdup(ft_strchr(args[i], '=') + 1);
//		set_env(key, value, &shell->internal);
//		++i;
//	}
//}
//
//char				**internal_variables_handler(t_shell *shell, char ***args)
//{
//	int		i;
//	int		internal_variables;
//	char	**variables;
//
//	i = 0;
//	internal_variables = 0;
//	while ((*args)[i])
//	{
//		if (ft_strchr((*args)[i], '='))
//			++internal_variables;
//		++i;
//	}
//	if (i == internal_variables)
//	{
//		push_internals_variables(shell, *args);
//		return (NULL);
//	}
//	variables = fill_env(shell->env);
//	i = 0;
//	while ((*args)[i])
//	{
//		++i;
//	}
//	return (variables);
//}
