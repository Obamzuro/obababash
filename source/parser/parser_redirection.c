/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyrychu <akyrychu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 12:46:57 by obamzuro          #+#    #+#             */
/*   Updated: 2019/10/30 17:41:00 by akyrychu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "twenty_one_sh.h"

static int					parse_ast_redirection_right(t_ast *ast)
{
	int				rightfd;
	t_binary_token	*binary_token;

	rightfd = 0;
	binary_token = (t_binary_token *)ast->right->content;
	if (ft_strequ((char *)ast->content, ">"))
		rightfd = open(binary_token->right, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (ft_strequ((char *)ast->content, ">>"))
		rightfd = open(binary_token->right, O_WRONLY |
				O_CREAT | O_APPEND, 0644);
	else if (ft_strequ((char *)ast->content, "<"))
		rightfd = open(binary_token->right, O_RDONLY, 0644);
	else if (ft_strequ((char *)ast->content, ">&")
			|| ft_strequ((char *)ast->content, "<&"))
		rightfd = ft_atoi(binary_token->right);
	if (rightfd == -1)
	{
		ft_fprintf(2, "21sh: parse_redirection error\n", binary_token->right);
		return (-1);
	}
	return (rightfd);
}

static int					parse_ast_redirection_kernel_heredoc(t_ast *ast,
		int fd)
{
	int		pipefd[2];

	if (pipe(pipefd) == -1)
	{
		ft_fprintf(2, "21sh: pipe creating error");
		return (0);
	}
	dup2(pipefd[0], fd);
	close(pipefd[0]);
	write(pipefd[1], ((t_binary_token *)(ast->right->content))->right,
		ft_strlen(((t_binary_token *)(ast->right->content))->right));
	close(pipefd[1]);
	return (1);
}

static int					parse_ast_redirection_kernel(t_ast *ast,
		t_shell *shell, int fd[], t_job *cur_job)
{
	int		oldfd;
	int		devnull;

	oldfd = dup(fd[0]);
	if (ft_strequ(((t_binary_token *)(ast->right->content))->right, "-"))
	{
		devnull = open("/dev/null", O_RDWR);
		dup2(devnull, fd[0]);
		close(devnull);
	}
	else if (ft_strequ(ast->content, "<<"))
	{
		if (!parse_ast_redirection_kernel_heredoc(ast, fd[0]))
			return (0);
	}
	else
		dup2(fd[1], fd[0]);
	if (!parse_ast(ast->left, shell, 0, cur_job))
		return (0);
	dup2(oldfd, fd[0]);
	close(oldfd);
	return (1);
}

/*
** fd[1] --> rightfd
*/

static int					parse_ast_redirection_child(t_ast *ast,
		t_shell *shell, t_job *cur_job)
{
	int			fd[2];

	if (((t_binary_token *)(ast->right->content))->left)
		fd[0] = ft_atoi(((t_binary_token *)(ast->right->content))->left);
	else if (((char *)ast->content)[0] == '>')
		fd[0] = 1;
	else if (((char *)ast->content)[0] == '<')
		fd[0] = 0;
	if ((fd[1] = parse_ast_redirection_right(ast)) == -1)
		return (0);
	if (!parse_ast_redirection_kernel(ast, shell, fd, cur_job))
		return (0);
	return (1);
}

int							parse_ast_redirection(t_ast *ast, t_shell *shell,
		int needfork, t_job *cur_job)
{
	pid_t		pid;

	if (needfork)
	{
		pid = fork();
		if (pid < 0)
		{
			ft_fprintf(2, "21sh: fork error\n");
			return (0);
		}
		if (!pid)
		{
			if (!parse_ast_redirection_child(ast, shell, cur_job))
				exit(1);
		}
		else if (needfork)
			while (wait(0) == -1)
				;
		return (1);
	}
	if (!parse_ast_redirection_child(ast, shell, cur_job))
		return (0);
	return (1);
}
