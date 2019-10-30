/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_unsetenv.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 13:53:18 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/30 18:27:19 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

static void			unset_env_cpenv(char *arg, char ***env, char **newenv)
{
	int		i;
	int		flag;

	i = 0;
	flag = 0;
	while ((*env)[i])
	{
		if (!flag && ft_strcmp((*env)[i], arg) == '=')
		{
			free((*env)[i]);
			++i;
			flag = 1;
			continue ;
		}
		newenv[i - flag] = ft_strdup((*env)[i]);
		free((*env)[i]);
		++i;
	}
	newenv[i - flag] = 0;
	free(*env);
	*env = newenv;
}

void				unset_env_kernel(char *arg, char ***env)
{
	char	**newenv;
	int		i;

	if (!get_env(arg, *env))
		return ;
	i = 0;
	while ((*env)[i])
		++i;
	newenv = (char **)malloc(sizeof(char *) * (i));
	unset_env_cpenv(arg, env, newenv);
}

void				unset_env(char **args, char ***env)
{
	int		i;

	i = 1;
	while (args[i])
	{
		unset_env_kernel(args[i], env);
		++i;
	}
}
