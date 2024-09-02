/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 10:37:35 by lstorey           #+#    #+#             */
/*   Updated: 2024/09/02 18:01:01 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
*	stasis_mode:
*	When the program receives a SIGINT, it
*		writes a newline to the console,
*		replaces the current line with an empty string,
*		moves to a new line, and
*	redraws the prompt.
*	The argument "sig" is not used.
*/
void	stasis_mode(int sig)
{
	g_mod = 1;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	(void)sig;
}

/**
 * @brief Handle SIGINT signal in execution mode.
 *
 * If the user inputs SIGINT (Ctrl-C) in execution mode, the function
 * terminates the current command execution and returns to the main loop.
 *
 * @param[in] sig The received signal.
 */
void	exec_stream(int sig)
{
	g_mod = 1;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	(void)sig;
}

/**
 * @brief Handle SIGINT signal in holodoc mode.
 *
 * In this mode, we clear the history list, restore default signal handlers,
 * and send the signal again to ourselves.
 *
 * @param sig The signal number that triggered this function.
 */
void	holodoc(int sig)
{
	g_mod = 1;
	write(1, "\n", 1);
	clear_history();
	signal(sig, SIG_DFL);
	kill(getpid(), sig);
}

/**
 * @brief Set signal handlers based on the mode.
 *
 * This function sets signal handlers based on the mode passed as argument.
 * The mode can be one of the following values:
 * 1: Stasis mode, where SIGINT is handled by stasis_mode and SIGQUIT is ignored.
 * 2: Execution mode, where SIGINT is 
 *    handled by exec_stream and SIGQUIT is ignored.
 * 3: Holodoc mode, where SIGINT is handled by holodoc and SIGQUIT is ignored.
 *
 * @param[in] sig The mode to set the signal handlers for.
 */
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
