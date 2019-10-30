/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forty_two_sh.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 14:50:56 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/31 00:14:03 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __FORTY_TWO_SH_H
# define __FORTY_TWO_SH_H

# include <curses.h>
# include <term.h>
# include <termcap.h>
# include <unistd.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/ioctl.h>
# include <sys/wait.h>
# include <sys/dir.h>
# include <errno.h>
# include "libft.h"
# include "ft_printf.h"

# define AM_COMMANDS 12
# define AM_OPERATORS 11
# define AM_LEVELS 4
# define AM_SEPARATOROP 3
# define AM_IOFILEOP 7
# define AM_HISTORY 1000
# define AM_ESC 19
# define AM_ASCII_IGNORE 2
# define READING_SIZE 8

# define ESC "\x1B"
# define DOWN "\x1B[B"
# define UP "\x1B[A"
# define LEFT "\x1B[D"
# define RIGHT "\x1B[C"
# define SPACE " "
# define SUSPEND "\x1A"
# define DEL "\x1B[3~"
# define DEFAULT "\x1B[0m"
# define BACKSPACE "\x7F"
# define ENTER "\x0A"
# define ALTLEFT "\x1b\x1b\x5b\x44"
# define ALTRIGHT "\x1b\x1b\x5b\x43"
# define ALTUP "\x1b\x1b\x5b\x41"
# define ALTDOWN "\x1b\x1b\x5b\x42"
# define HOME "\x1b\x5b\x48"
# define END "\x1b\x5b\x46"
# define SHIFTLEFT "\x1b\x5b\x31\x3b\x32\x44"
# define SHIFTRIGHT "\x1b\x5b\x31\x3b\x32\x43"
# define ALTX "\xe2\x89\x88"
# define ALTC "\xc3\xa7"
# define ALTV "\xe2\x88\x9a"
# define ALTQ "\xc5\x93"
# define EOT "\x4"

typedef struct		s_process
{
	struct s_process	*next;
	pid_t				pid;
	char				completed;
	char				stopped;
	int					status;
}					t_process;

typedef struct		s_job
{
	struct s_job	*next;
	char			command[30];
	t_process		*first_process;
	pid_t			pgid;
	char			notified;
	struct termios	tmodes;
	int				foreground;
}					t_job;

extern t_job		*g_first_job;

typedef enum		e_tokentype
{
	UKNOWN = 0,
	WORD,
	NAME,
	OPERATOR,
	IO_NUMBER,
	COMMAND,
	REDIRECTION,
	USED,
	ERROR,
	VARIABLE
}					t_tokentype;

typedef enum		e_reading_mode
{
	BASIC = 0,
	QUOTE,
	HEREDOC,
	PIPE,
	READEND
}					t_reading_mode;

typedef struct		s_history
{
	char			*commands[AM_HISTORY];
	int				last;
	int				current;
}					t_history;

struct s_shell;

typedef struct		s_lineeditor
{
	char			letter[READING_SIZE];
	int				curpos[2];
	int				seek;
	char			*buffer;
	int				is_history_searched;
	int				selected[2];
	int				selectedmode;
	char			*cpbuf;
	struct winsize	ws;
	char			prompt;
	char			trash[READING_SIZE + 1];
	struct s_shell	*shell;
}					t_lineeditor;

typedef struct		s_initfd
{
	int				fdin;
	int				fdout;
	int				fderr;
}					t_initfd;

typedef struct		s_border
{
	int				beg;
	int				end;
}					t_border;

typedef struct		s_token
{
	t_tokentype		type;
	char			*str;
}					t_token;

typedef struct		s_binary_token
{
	char			*left;
	char			*right;
}					t_binary_token;

typedef struct		s_command_token
{
	char			**args;
	char			**vars;
}					t_command_token;

typedef struct		s_ast
{
	void			*content;
	t_tokentype		type;
	pid_t			pgid;
	int				foreground;
	struct s_ast	*left;
	struct s_ast	*right;
}					t_ast;

typedef struct		s_lexer
{
	t_ftvector		tokens;
}					t_lexer;

typedef struct		s_shell
{
	char			**internal;
	char			**env;
	char			**aliases;
	t_initfd		initfd;
	t_history		history;
	t_lexer			*lexer;
	t_lineeditor	lineeditor;
	t_reading_mode	reading_mode;
	t_ast			*ast;
	pid_t			pgid;
}					t_shell;

typedef struct		s_comm_corr
{
	char			*comm;
	void			(*func)(char **, char **, t_shell *);
}					t_comm_corr;

typedef enum		e_file_mark
{
	MATCH = 0,
	WASDELETED,
	NOTMATCH
}					t_file_mark;

typedef struct		s_glob_file
{
	char			*name;
	t_file_mark		mark;
}					t_glob_file;

typedef struct		s_char_glob_corr
{
	char			character;
	int				(*func)(char *, char *);
}					t_char_glob_corr;

int					globbing(char ***args);
void				push_variables_into_env(t_shell *shell, char **args,\
char ***env, char ***dop_env);

void				ft_exit(char **args, char **vars, t_shell *shell);
void				ft_echo(char **args, char **vars, t_shell *shell);
void				change_dir(char **args, char **vars, t_shell *shell);
void				ft_jobs(char **args, char **vars, t_shell *shell);
void				ft_fg(char **args, char **vars, t_shell *shell);
void				ft_bg(char **args, char **vars, t_shell *shell);
void				ft_export_comm(char **args, char **vars, t_shell *shell);
void				ft_unset_comm(char **args, char **vars, t_shell *shell);
void				ft_set_comm(char **args, char **vars, t_shell *shell);
void				print_env(char **args, char ***env);
void				unset_env(char **args, char ***env);
void				unset_env_kernel(char *arg, char ***env);
void				hash_comm(char **args, char **vars, t_shell *shell);

typedef struct		s_esc_corr
{
	char			*str;
	void			(*func)(t_lineeditor *, t_history *);
	int				is_printing : 1;
	int				is_selecting : 1;
}					t_esc_corr;

extern char			*g_last_command;
extern t_shell		*g_shell;
extern t_ftvector	*g_hash;

char				**fill_env(char **environ);
void				fill_commands(t_comm_corr *commands);

int					ft_exec(char **args, char ***env, int forkneed,\
t_job *cur_job);
void				set_env(char *key, char *value, char ***env);
char				*get_env(char *key, char **env);
void				int_handler(int sig);

void				free_double_arr(char **args);

void				term_associate(void);
void				change_termios(t_initfd *initfd, int canon);
char				**init_operators(void);

void				history_append(char *command, t_history *history,
		int appendmode);

void				line_editing_left(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_right(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_up(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_down(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_altup(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_altdown(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_begword(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_endword(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_backspace(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_home(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_end(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_shiftleft(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_shiftright(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_altc(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_altv(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_altx(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_help(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_del(t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_eot(t_lineeditor *lineeditor,
		t_history *history);

void				*print_error_zero(char *str);

int					tilde_expansion(t_shell *shell, char **args);
int					env_expansion(t_shell *shell, char **args);
int					quote_removing(t_shell *shell, char **args);

int					free_ast(t_ast *ast);
int					parse_ast(t_ast *ast, t_shell *shell, int needfork,\
t_job *cur_job);
int					parse_ast_redirection(t_ast *ast, t_shell *shell,
		int needfork, t_job *cur_job);
int					parse_ast_pipe(t_ast *ast, t_shell *shell, t_job *cur_job);

void				free_lexer(t_lexer *lexer);
int					lexer_creating(char *command, t_shell *shell);
t_token				*lexing_handling_initword(t_token *token, char buf);
int					lexing_handling_appword(t_token *token, char buf);
t_token				*lexing_handling_separator(t_lexer *lexer, t_token *token);
int					lexing_handling_end(t_shell *shell, t_token *token,
		char **last, char **command);
t_token				*lexing_handling_quotes(t_shell *shell,
		t_token *token, char **last, char **command);
int					lexing_is_operator_begin(char buf);

t_token				*lexing_init_operator_token(char buf,
		t_token *token, t_lexer *lexer);
int					lexing_try_append_operator(char buf, char **tokenstr);
t_token				*lexing_divide_operator(t_lexer *lexer, t_token *token);

int					first_token_pos(t_lexer *lexer, int beg, int end,
		char **findname);
int					last_token_pos(t_lexer *lexer, int beg, int end,
		char **findname);
t_ast				*create_separator_ast(int beg, int end,
		t_shell *shell);
t_ast				*create_redirection_ast(t_lexer *lexer, int beg,
		int end, t_shell *shell);
t_ast				*create_command(t_lexer *lexer, int beg, int end);

char				*input_command(t_lineeditor *lineeditor, t_history *history,
		char prompt, t_shell *shell);
void				free_lineeditor(t_lineeditor *lineeditor);
char				*input_command(t_lineeditor *lineeditor,
		t_history *history, char prompt, t_shell *shell);
void				get_cursor_position(int cursorpos[2],
		t_lineeditor *lineeditor);
int					le_reading(t_lineeditor *lineeditor, int offset, int size);
int					le_ignore_special_chars(t_lineeditor *lineeditor);
int					le_line_editing(t_lineeditor *lineeditor,
		t_history *history);
void				le_zero_hist_research(t_lineeditor *lineeditor,
		t_history *history);
void				le_unselect(t_lineeditor *lineeditor,
		t_history *history, t_esc_corr *esc);
void				left_shift_cursor(int amount, t_lineeditor *lineeditor,
		t_history *history);
void				line_editing_left_notmove(t_lineeditor *lineeditor,
		t_history *history);
void				input_command_print_buffer(t_lineeditor *lineeditor,
		t_history *history);
void				print_buffer(t_lineeditor *lineeditor, t_history *history,
		char *newbuf, int offset);
int					ft_putc(int c);
void				write_line(t_lineeditor *lineeditor);

void				put_job_in_foreground(t_job *job, int cont);
void				put_job_in_background(t_job *job, int cont);
void				wait_for_job(t_job *job);
void				add_process_to_job(t_job *job, pid_t pid);
void				do_job_notification(void);
t_ast				*create_background_ast(int beg, int end,
		t_shell *shell);
int					job_is_completed(t_job *j);
int					job_is_stopped(t_job *j);

int					set_env_check_existing(char *key, char *value, char **env);

char				*get_envs(char *key, int amount_envs, ...);

int					lexing_handling_baskslash(t_token **token,
		char **last, char **command);
void				backslash_handling(char ***args);

char				*parameter_expansion_dedicated(char *key, char **env);

void				ft_alias(char **args, char **vars, t_shell *shell);
void				ft_unalias(char **args, char **vars, t_shell *shell);

int					creating_ast(t_shell *shell);
int					command_substition(char **command);

char				*parameter_expansion_sharp_small_character(char *str,\
char *pattern);
char				*parameter_expansion_sharp_big_character(char *str,\
char *pattern);

#endif
