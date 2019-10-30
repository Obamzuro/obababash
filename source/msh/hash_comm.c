/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_comm.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 19:08:35 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/30 18:27:19 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

void				hash_comm(char **args, char **vars, t_shell *shell)
{
	int		i;

	i = 0;
	while (i < g_hash->len)
	{
		ft_printf("%s\n", g_hash->elem[i]);
		++i;
	}
}
