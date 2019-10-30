/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_signal_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 13:53:06 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/30 18:27:19 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

volatile sig_atomic_t	g_sigwinch = 0;

void		int_handler(int sig)
{
	(void)sig;
	if (sig == SIGWINCH)
	{
		g_sigwinch = 1;
	}
}
