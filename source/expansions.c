/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 14:47:04 by obamzuro          #+#    #+#             */
/*   Updated: 2019/09/29 19:40:05 by obamzuro         ###   ########.fr       */
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
		while (1)
		{
			quote = ft_strchr_str(*str + nextsearch, "\'\"");
			if (!quote || !(quote != *str && *(quote - 1) == '\\' && *quote == '\"'))
				break ;
		}
		if (!quote)
			return ;
		while (1)
		{
			quote2 = ft_strchr(quote + 1, *quote);
			if (!quote2 || !(*(quote2 - 1) == '\\' && *quote == '\"'))
				break ;
		}
		if (!quote2)
			return ;
//		if (!(quote = ft_strchr_str(*str + nextsearch, "\'\"")) ||
//			!(quote2 = ft_strchr(quote + 1, *quote)))
//			return ;
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
			is_deleted = 1;
		else if (!is_deleted)
			new_args[i] = (*args)[i];
		else if (is_deleted)
			new_args[i - 1] = (*args)[i];
		++i;
	}
	new_args[i - 1] = NULL;
	free((*args)[elem_to_delete]);
	free(*args);
	*args = new_args;
}

void				backslash_handling(char ***args)
{
	int		i;
	int		j;
	int		squote;
	int		dquote;
	char	*temp;
	char	*temp2;

	i = 0;
	while ((*args)[i])
	{
		j = 0;
		squote = -1;
		dquote = -1;
		while ((*args)[i][j])
		{
			if ((*args)[i][j] == '\'' && dquote == -1)
			{
				if (squote == -1)
					squote = j;
				else
				{
					temp = (*args)[i];
					(*args)[i][squote] = '\0';
					(*args)[i][j] = '\0';
					temp2 = ft_strjoin((*args)[i], (*args)[i] + squote + 1);
					(*args)[i] = ft_strjoin(temp2, (*args)[i] + j + 1);
					free(temp);
					free(temp2);
					j -= 2;
					squote = -1;
				}
			}
			else if ((*args)[i][j] == '\"' && squote == -1)
			{
				if (dquote == -1)
					dquote = j;
				else
				{
					temp = (*args)[i];
					(*args)[i][dquote] = '\0';
					(*args)[i][j] = '\0';
					temp2 = ft_strjoin((*args)[i], (*args)[i] + dquote + 1);
					(*args)[i] = ft_strjoin(temp2, (*args)[i] + j + 1);
					free(temp);
					free(temp2);
					j -= 2;
					dquote = -1;
				}
			}
			else if ((*args)[i][j] == '\\' && squote == -1)
			{
				if ((*args)[i][j + 1] == '\n')
				{
					if (!(*args)[i][j + 2])
					{
						reassemble_args(args, i);
						--i;
						break ;
					}
					else
					{
						(*args)[i][j] = 0;
						temp = ft_strjoin((*args)[i], (*args)[i] + j + 2);
						free((*args)[i]);
						(*args)[i] = temp;
						--j;
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
