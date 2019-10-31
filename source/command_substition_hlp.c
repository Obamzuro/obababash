/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_substition_hlp.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/13 15:05:22 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/31 03:55:28 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

int				command_substition(char **command)
{
	int		i;

	i = 0;
	while ((*command)[i])
	{
		if ((*command)[i] == '$' && (*command)[i + 1] == '(')
			if (substitute(command, &i) == -1)
				return (-1);
		++i;
	}
	return (0);
}
