/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 10:37:35 by lstorey           #+#    #+#             */
/*   Updated: 2024/08/30 02:19:19 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void	stasis_mode(int sig)
{
	g_exit_code = 1;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	(void)sig;
}

void	exec_stream(int sig)
{
	g_exit_code = 1;
	rl_on_new_line();
	rl_replace_line("", 0);
	(void)sig;
}

void	holodoc(int sig)
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
		signal(SIGINT, stasis_mode);
		signal(SIGQUIT, SIG_IGN);
	}
	if (sig == 2)
	{
		signal(SIGINT, exec_stream);
		signal(SIGQUIT, SIG_IGN);
	}
	if (sig == 3)
	{
		signal(SIGINT, holodoc);
		signal(SIGQUIT, SIG_IGN);
	}
}
