/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hynam <hynam@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/26 14:34:08 by minchoi           #+#    #+#             */
/*   Updated: 2021/10/28 13:26:41 by hynam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	check_bin(t_cmd *cmd)
{
	int			i;
	char		*tmp;
	char		*new_path;
	struct stat	s;

	i = -1;
	if (stat(cmd->word[0], &s) == 0)
		return (1);
	tmp = ft_strjoin("/", cmd->word[0]);
	while (cmd->path[++i])
	{
		new_path = ft_strjoin(cmd->path[i], tmp);
		if (stat(new_path, &s) == 0)
		{
			free(tmp);
			free(new_path);
			return (1);
		}
		free(new_path);
	}
	free(tmp);
	return (0);
}

int	check_builtin(t_cmd *cmd)
{
	char	*cmd_var;

	cmd_var = cmd->word[0];
	if (ft_strcmp(cmd_var, "echo") == 0
		|| ft_strcmp(cmd_var, "cd") == 0
		|| ft_strcmp(cmd_var, "pwd") == 0
		|| ft_strcmp(cmd_var, "env") == 0
		|| ft_strcmp(cmd_var, "exit") == 0
		|| ft_strcmp(cmd_var, "export") == 0
		|| ft_strcmp(cmd_var, "unset") == 0)
		return (0);
	else if (check_bin(cmd))
		return (0);
	else
		return (1);
}
