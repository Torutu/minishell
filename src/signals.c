/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 10:37:35 by lstorey           #+#    #+#             */
/*   Updated: 2024/08/29 19:56:25 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void	normal_state(int sig)
{
	g_exit_code = 1;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	(void)sig;
}

void	running_exe(int sig)
{
	g_exit_code = 1;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	(void)sig;
}

void	heredocing(int sig)
{
	g_exit_code = 1;
	printf("\n");
	clear_history();
	signal(sig, SIG_DFL);
	kill(getpid(), sig);
}
void	signals(int sig)
{
	if (sig == 1)
	{
		signal(SIGINT, normal_state);
		signal(SIGQUIT, SIG_IGN);
	}
	if (sig == 2)
	{
		signal(SIGINT, running_exe);
		signal(SIGQUIT, SIG_IGN);
	}
	if (sig == 3)
	{
		signal(SIGINT, heredocing);
		signal(SIGQUIT, SIG_IGN);
	}
}
