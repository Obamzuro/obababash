/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_print_pwd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 13:52:58 by obamzuro          #+#    #+#             */
/*   Updated: 2019/09/26 14:31:06 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

void			print_pwd(char **args, char **vars __attribute__ ((unused)), char ***env)
{
	char	*line;

	if (args[1])
	{
		ft_fprintf(2, "pwd: too many arguments\n");
		return ;
	}
	line = getcwd(0, 0);
	env = 0;
	ft_printf("%s\n", line);
	free(line);
	return ;
}
