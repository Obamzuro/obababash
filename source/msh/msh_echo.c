/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 13:52:29 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/30 18:27:19 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

void			ft_echo(char **args,\
char **vars __attribute__((unused)), t_shell *shell)
{
	int		i;
	int		first;

	i = 1;
	first = 0;
	shell++;
	if (ft_strequ(args[1], "-n"))
		++i;
	while (args[i])
	{
		if (!first)
		{
			ft_printf("%s", args[i]);
			first = 1;
		}
		else
			ft_printf(" %s", args[i]);
		++i;
	}
	if (!ft_strequ(args[1], "-n"))
		ft_printf("\n");
	return ;
}
