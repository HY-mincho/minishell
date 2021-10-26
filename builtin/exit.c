/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hynam <hynam@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/30 15:53:16 by minchoi           #+#    #+#             */
/*   Updated: 2021/10/26 14:39:59 by hynam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(t_cmd *cmd)
{
	int	n;

	n = 0;
	if (cmd->cmd_num != 1)
	{
		printf("%s: too many arguments\n", cmd->word[0]);
		g_status = 2;
	}
	printf("Minishell will be over\n");
	return (1);
}
