/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_term.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/09 14:25:20 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/31 00:08:03 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

void	term_associate(void)
{
	char	*termtype;
	int		success;

	termtype = getenv("TERM");
	if (!termtype)
	{
		ft_fprintf(2, "42sh: Can't find an env TERM\n");
		exit(EXIT_FAILURE);
	}
	success = tgetent(0, termtype);
	if (success < 0)
	{
		ft_fprintf(2, "42sh: Can't access the termcap data base\n");
		exit(EXIT_FAILURE);
	}
	if (!success)
	{
		ft_fprintf(2, "42sh: Terminal type '%s' is not defined\n", termtype);
		exit(EXIT_FAILURE);
	}
}

struct termios		g_tty;

void	change_termios(t_initfd *initfd, int canon)
{
	struct termios		tty;

	(void)initfd;
	if (!canon)
	{
		dup2(initfd->fdin, STDIN_FILENO);
		dup2(initfd->fdout, STDOUT_FILENO);
		dup2(initfd->fderr, STDERR_FILENO);
		tty = g_tty;
		tty.c_lflag &= ~(ICANON | ECHO);
		tty.c_cc[VMIN] = 1;
		tty.c_cc[VTIME] = 0;
		tcsetattr(0, TCSANOW, &tty);
	}
	else
		tcsetattr(0, TCSANOW, &g_tty);
}
