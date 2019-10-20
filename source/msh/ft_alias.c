/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_alias.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 14:21:04 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/20 16:00:51 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

void			ft_alias(char **args, char **vars, t_shell *shell)
{
	int		i;
	char	*alias;

	if (!args[1] && !vars[0])
	{
		i = 0;
		while (g_shell->aliases[i])
			ft_printf("%s\n", g_shell->aliases[i++]);
		return ;
	}
	i = 1;
	while (args[i])
	{
		alias = get_env(args[i], g_shell->aliases);
		if (alias)
			ft_printf("%s='%s'\n", args[i], alias);
		++i;
	}
	push_variables_into_env(g_shell, vars, &g_shell->aliases, NULL);
}

void			ft_unalias(char **args, char **vars __attribute__ ((unused)), t_shell *shell)
{
	unset_env(args, &g_shell->aliases);
}