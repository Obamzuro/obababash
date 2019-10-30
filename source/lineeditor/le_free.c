/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   le_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 14:06:28 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/30 18:27:19 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

void				free_lineeditor(t_lineeditor *lineeditor)
{
	free(lineeditor->buffer);
	free(lineeditor->cpbuf);
}
