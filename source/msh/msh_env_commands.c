/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_set_comm.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/26 14:17:10 by obamzuro          #+#    #+#             */
/*   Updated: 2019/09/26 15:31:03 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

void				ft_set_comm(char **args, char **vars __attribute__ ((unused)), t_shell *shell)
{
	print_env(args, &shell->internal);
	ft_printf("/-------------------------------\\\n");
	print_env(args, &shell->env);
}

void				ft_export_comm(char **args, char **vars, t_shell *shell)
{
	int		i;
	char	*value;

	i = 1;
	while (args[i])
	{
		value = get_env(args[i], shell->internal);
		if (value)
		{
			set_env(args[i], value, &shell->env);
			unset_env_kernel(args[i], &shell->internal);
		}
		++i;
	}
	i = 0;
	push_variables_into_env(shell, vars, &shell->env, NULL);
}

void				ft_unset_comm(char **args, char **vars __attribute__ ((unused)), t_shell *shell)
{
	unset_env(args, &shell->internal);
	unset_env(args, &shell->env);
}
