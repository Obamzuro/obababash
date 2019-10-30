/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 13:52:38 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/30 17:10:08 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

void		ft_exit(char **args,\
char **vars __attribute__((unused)), t_shell *shell)
{
	args++;
	shell++;
	exit(0);
}
