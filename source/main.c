/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/13 15:05:22 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/31 01:03:00 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "forty_two_sh.h"

void			alias_expansion(char **command)
{
	int		i;
	char	*word;
	char	*temp;
	char	separator;
	char	*value;

	word = NULL;
	i = 0;
	while (1)
	{
		if (ft_is_char_in_str((*command)[i], " \t\n&|><;\v\r")\
		|| !(*command)[i])
		{
			if ((value = get_env(word, g_shell->aliases)))
			{
				separator = (*command)[i];
				(*command)[i] = '\0';
				temp = *command;
				*command = (char *)ft_memalloc(ft_strlen(value) + 1\
				+ ft_strlen((*command) + i + 1) + 1);
				ft_strcpy((*command), value);
				(*command)[ft_strlen(value)] = separator;
				ft_strcat((*command), temp + i + 1);
				(*command)[ft_strlen((*command))] = 0;
				free(temp);
			}
			break ;
		}
		temp = word;
		word = (char *)malloc(ft_strlen(temp) + 2);
		ft_strcpy(word, temp);
		word[ft_strlen(temp)] = (*command)[i];
		word[ft_strlen(temp) + 1] = '\0';
		free(temp);
		++i;
	}
	free(word);
}

static char		*lineediting(t_shell *shell)
{
	char		*command;

	if (!(command = input_command(&shell->lineeditor,
					&shell->history, '$', shell)) || !*command)
	{
		le_unselect(&shell->lineeditor, &shell->history, NULL);
		print_buffer(&shell->lineeditor, &shell->history, 0, 0);
		line_editing_end(&shell->lineeditor, &shell->history);
		ft_printf("\n");
		free_lineeditor(&shell->lineeditor);
		return (NULL);
	}
	le_unselect(&shell->lineeditor, &shell->history, NULL);
	print_buffer(&shell->lineeditor, &shell->history, 0, 0);
	line_editing_end(&shell->lineeditor, &shell->history);
	alias_expansion(&command);
	return (command);
}

char			*g_last_command;
t_ftvector		*g_hash;

static int		lexing(t_shell *shell, char *command)
{
	if (g_last_command)
		free(g_last_command);
	g_last_command = ft_strdup(command);
	if (command_substition(&command))
	{
		ft_fprintf(STDERR_FILENO, "42sh: command substitution error!\n");
		return (-1);
	}
	if (lexer_creating(command, shell))
	{
		free_lexer(shell->lexer);
		return (-1);
	}
	write(1, "\n", 1);
	if (!shell->lexer->tokens.len)
	{
		free_lexer(shell->lexer);
		--shell->history.last;
		free(shell->history.commands[shell->history.last]);
		shell->history.commands[shell->history.last] = 0;
		return (-1);
	}
	return (0);
}

int				creating_ast(t_shell *shell)
{
	if (!(shell->ast = create_background_ast(0,
					shell->lexer->tokens.len - 1, shell)))
	{
		free_lexer(shell->lexer);
		return (-1);
	}
	return (0);
}

static void		init_env(void)
{
	char	*value;
	char	*tmp;

	value = get_env("USER", g_shell->env);
	if (value)
	{
		tmp = ft_strjoin("/Users/", value);
		set_env("HOME", tmp, &g_shell->env);
		free(tmp);
	}
	tmp = getcwd(NULL, 0);
	set_env("PWD", tmp, &g_shell->env);
	free(tmp);
}

static void		preparation(t_shell *shell)
{
	struct sigaction		act;
	extern struct termios	g_tty;
	pid_t					shell_pgid;

	g_hash = (t_ftvector *)malloc(sizeof(t_ftvector));
	init_ftvector(g_hash);
	ft_bzero(&act, sizeof(act));
	act.sa_handler = int_handler;
	term_associate();
	if ((shell->initfd.fdin = dup(0)) == -1 ||
		(shell->initfd.fdout = dup(1)) == -1 ||
		(shell->initfd.fderr = dup(2)) == -1)
	{
		ft_fprintf(2, "42sh: dup() error\n");
		exit(EXIT_FAILURE);
	}
	shell->lexer = (t_lexer *)malloc(sizeof(t_lexer));
	ft_bzero(&shell->history, sizeof(shell->history));
	while (tcgetpgrp(shell->initfd.fdin) != (shell_pgid = getpgrp()))
		kill(-shell_pgid, SIGTTIN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	shell_pgid = getpid();
	shell->pgid = shell_pgid;
	if (setpgid(shell_pgid, shell_pgid) < 0)
	{
		ft_fprintf(2, "42sh: Couldn't put shell in its own groupr\n");
		exit(EXIT_FAILURE);
	}
	tcsetpgrp(shell->initfd.fdin, shell_pgid);
	tcgetattr(STDIN_FILENO, &g_tty);
	if (sigaction(SIGINT, &act, 0) == -1 ||
		sigaction(SIGWINCH, &act, 0) == -1)
	{
		ft_fprintf(2, "42sh: sigaction error\n");
		exit(EXIT_FAILURE);
	}
}

t_shell			*g_shell;
t_job			*g_first_job = NULL;

int				main(void)
{
	char		*command;
	t_shell		shell;
	extern char	**environ;

	g_shell = &shell;
	preparation(&shell);
	shell.aliases = (char **)malloc(sizeof(char *));
	shell.aliases[0] = NULL;
	shell.env = fill_env(environ);
	shell.internal = (char **)malloc(sizeof(char *));
	shell.internal[0] = NULL;
	init_env();
	while (1)
	{
		change_termios(&shell.initfd, 0);
		ft_bzero(shell.lexer, sizeof(t_lexer));
		do_job_notification();
		ft_printf("\e[31m[%s]\x1b[0m\n", get_env("PWD", shell.env));
		ft_printf("$> ");
		shell.reading_mode = BASIC;
		if (!(command = lineediting(&shell)) ||
			lexing(&shell, command) ||
			creating_ast(&shell))
			continue;
		change_termios(&shell.initfd, 1);
		parse_ast(shell.ast, &shell, 1, NULL);
		free_lexer(shell.lexer);
		free_ast(shell.ast);
	}
	free_double_arr(shell.env);
	free(shell.lexer);
	return (0);
}
