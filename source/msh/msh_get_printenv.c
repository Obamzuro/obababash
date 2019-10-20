/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_get_printenv.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 13:52:51 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/20 14:41:19 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

char		*get_env(char *key, char **env)
{
	int		i;
	char	*envvalue;

	i = 0;
	while (env[i])
	{
		if (!(envvalue = ft_strchr(env[i], '=')))
			continue ;
		if (ft_strcmp(env[i], key) == '=')
			return (envvalue + 1);
		++i;
	}
	return (0);
}

char		*get_envs(char *key, int amount_envs, ...)
{
	int			i;
	va_list		ap;
	char		**env;
	char		*value;

	va_start(ap, amount_envs);
	i = 0;
	while (i < amount_envs)
	{
		env = va_arg(ap, char **);
		if ((value = get_env(key, env)))
		{
			va_end(ap);
			return (value);
		}
		++i;
	}
	va_end(ap);
	return (NULL);
}

void		print_env(char **args __attribute__ ((unused)), char ***env)
{
	int		i;

	i = 0;
	while ((*env)[i])
		ft_printf("%s\n", (*env)[i++]);
	return ;
}
