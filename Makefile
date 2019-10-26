# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: obamzuro <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/05/11 15:56:03 by obamzuro          #+#    #+#              #
#    Updated: 2019/10/26 19:41:20 by obamzuro         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = 21sh

SRCNAME = main.c	\
		  msh/init_term.c\
		  msh/msh_filler.c\
		  msh/msh_exec.c\
		  msh/msh_free.c\
		  msh/msh_get_printenv.c\
		  msh/msh_cd.c\
		  msh/msh_echo.c\
		  msh/msh_exit.c\
		  msh/msh_print_pwd.c\
		  msh/msh_setenv.c\
		  msh/msh_signal_handler.c\
		  msh/msh_unsetenv.c\
		  msh/msh_replace_env_variable.c\
		  msh/msh_jobs.c\
		  msh/msh_env_commands.c\
		  msh/ft_alias.c\
		  lineeditor/le_input_command.c\
		  lineeditor/le_input_command2.c\
		  lineeditor/le_print.c\
		  lineeditor/le_write.c\
		  lineeditor/le_reading.c\
		  lineeditor/le_free.c\
		  lineeditor/handling_commands/le_left.c\
		  lineeditor/handling_commands/le_right_up_down.c\
		  lineeditor/handling_commands/le_alts_help.c\
		  lineeditor/handling_commands/le_select.c\
		  lineeditor/handling_commands/le_home_end_del.c\
		  lineeditor/handling_commands/le_backspace.c\
		  lexer/lexer.c\
		  lexer/lexer_operator.c\
		  lexer/lexer_quote.c\
		  lexer/lexer_other.c\
		  parser/parser.c\
		  parser/parser_redirection.c\
		  parser/parser_pipe.c\
		  ast_creator/creator_separator.c\
		  ast_creator/creator_redirection.c\
		  ast_creator/creator_command.c\
		  ast_creator/creator_tokenfinders.c\
		  error.c\
		  expansions.c\
		  history.c\
		  additional.c\
		  globbing.c\
		  parameter_expansion_dedicated.c\
		  command_substition.c

FLAGS = -g# -Wall -Wextra -Werror

SRC = $(addprefix source/, $(SRCNAME))

OBJ = $(SRC:.c=.o)

HDRDIR = include	\
		 libft/include		\
		 ft_printf/include

HDR = include/twenty_one_sh.h

all: lib $(NAME)

$(NAME): $(OBJ) libft/libft.a ft_printf/libftprintf.a
	gcc $(FLAGS) $(addprefix -I, $(HDRDIR)) $(OBJ) -L libft -lft -L ft_printf -lftprintf -ltermcap -o $(NAME)

%.o: %.c $(HDR)
	gcc $(FLAGS) $(addprefix -I, $(HDRDIR)) -c $< -o $@

lib:
	make -C libft
	make -C ft_printf

clean:
	make -C libft clean
	make -C ft_printf clean
	find . -name "*.o" -o -name ".*.sw[pon]" -exec rm -rf {} \;
	rm -rf $(OBJ)

fclean: clean
	make -C libft fclean
	make -C ft_printf fclean
	rm -rf $(NAME)

re: fclean all
