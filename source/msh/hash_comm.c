/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_comm.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 19:08:35 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/27 19:26:00 by obamzuro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

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
