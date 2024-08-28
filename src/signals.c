/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 10:37:35 by lstorey           #+#    #+#             */
/*   Updated: 2024/08/28 11:52:07 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handler(int sig)
{
	if (sig == SIGINT)
	{
		if (g_exit_code == HEREDOC_SIG)
			handler(SIGQUIT);
		else if (g_exit_code == EXEC_SIG)
		{
			printf("\n");
			rl_on_new_line();
			rl_replace_line("", 0);
		}
		else
		{
			printf("\n");
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
	}
	else if (sig == SIGQUIT)
	{
		printf("\n");
		clear_history();
		signal(sig, SIG_DFL);
		kill(getpid(), sig);
	}
}
