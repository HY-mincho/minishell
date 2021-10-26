/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hynam <hynam@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/04 20:06:10 by hynam             #+#    #+#             */
/*   Updated: 2021/10/26 19:07:58 by hynam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_flag(t_cmd **cmd, int *fd)
{
	int	flag;

	flag = 0;
	*fd = set_redir_fd(cmd);
	if (*fd == -1)
	{
		g_status = 1;
		return (-1);
	}
	if ((*cmd)->p_type == 1 || (*cmd)->p_type == 2)
	{
		flag = 1;
		dup2(*fd, 0);
	}
	else
		dup2(*fd, 1);
	close(*fd);
	return (flag);
}

int	here_document(t_cmd **cmd, int fd)
{
	char	*str;

	while (1)
	{
		str = readline("heredoc> ");
		if (strcmp(str, (*cmd)->next->word[0]) == 0)
		{
			if ((*cmd)->next->word[1] == NULL)
			{
				free(str);
				break ;
			}
		}
		write(fd, str, ft_strlen(str));
		write(fd, "\n", 1);
		free(str);
	}
	(*cmd)->word = ft_arrjoinstr((*cmd)->word, ".tmp");
	*cmd = (*cmd)->next;
	return (0);
}

int	redir_process(t_cmd **cmd, char **envp)
{
	int		fd;
	int		flag;
	t_cmd	*head;

	head = *cmd;
	flag = 0;
	if ((*cmd)->p_type == 2)
		here_document(cmd, set_redir_fd(cmd));
	while ((*cmd)->next && (*cmd)->p_type != 0)
	{
		flag = set_flag(cmd, &fd);
		if (!flag && head->p_type != 2 && !check_builtin(head))
			exec_builtin(head, envp);
		*cmd = (*cmd)->next;
	}
	if ((flag > 0 || head->p_type == 2) && !check_builtin(head))
		exec_builtin(head, envp);
	if (flag == -1)
		print_exec_err((*cmd)->word[0], NULL, 5);
	unlink(".tmp");
	return (g_status);
}

int	pipe_redir(t_cmd **cmd, char **envp, int *fd, int n)
{
	int		i;

	i = 0;
	while (*cmd)
	{
		if (!check_builtin(*cmd))
		{
			if (fork() == 0)
			{
				set_pipe_fd(cmd, fd, n, &i);
				if ((*cmd)->p_type > 0)
					g_status = redir_process(cmd, envp);
				else
					g_status = exec_builtin(*cmd, envp);
				exit(g_status);
			}
		}
		else
			print_exec_err((*cmd)->word[0], NULL, 5);
		while ((*cmd)->p_type > 0)
			*cmd = (*cmd)->next;
		if ((*cmd)->p_type == 0)
			++i;
		if ((*cmd)->next == NULL)
			break ;
		*cmd = (*cmd)->next;
	}
	return (0);
}

int	exec_pipe(t_cmd **cmd, char **envp)
{
	int		i;
	int		n;
	int		*fd;
	t_cmd	*tmp;

	i = -1;
	n = create_fd(cmd, &fd);
	while (++i < n / 2)
		pipe(fd + i * 2);
	tmp = *cmd;
	pipe_redir(cmd, envp, fd, n);
	while (tmp)
	{
		wait(NULL);
		close_all(fd, n);
		tmp = tmp->next;
	}
	if (n)
		free(fd);
	return (0);
}
