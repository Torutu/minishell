/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunky_check_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 04:35:52 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/23 15:49:45 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Checks if the given token is a pipe and updates
 * the current token and data accordingly.
 *
 * @param token The token to be checked.
 * @param current_token The current token to be updated.
 * @param data The data structure to be updated.
 *
 * @return 0 if the token is a pipe, 1
 */
int	check_pipe(char *token, t_token *current_token, t_data *data)
{
	if (ft_pipe_check(token, current_token) == 0)
	{
		data->ignore_cmd = false;
		current_token->echo = false;
		data->echoed = false;
		return (0);
	}
	return (1);
}

/**
 * Checks if the given token is a redirect operator.
 *
 * @param token The token to check.
 * @param current_token A pointer to the current token.
 * @param data A pointer to the data structure.
 * @return 0 if the token is a redirect operator, 1 otherwise.
 */
int	check_redirect(char *token, t_token *current_token, t_data *data)
{
	if (ft_redirect_op_check(token, current_token) == 0)
	{
		current_token->echo = false;
		data->echoed = false;
		return (0);
	}
	return (1);
}

/**
 * Checks if a command token is valid and should be executed.
 * 
 * @param token The token to be checked.
 * @param current_token The current token being processed.
 * @param data The data structure containing the execution context.
 * 
 * @return 0 if the command is valid and should be executed, 1 otherwise.
 */
int	check_command(char *token, t_token *current_token, t_data *data)
{
	if (!data->echoed && !data->ignore_cmd
		&& (cmd_check(token, current_token, data) == 0))
	{
		data->ignore_cmd = true;
		return (0);
	}
	return (1);
}

/**
 * Checks if the given token is a valid argument.
 * 
 * @param token The token to be checked.
 * @param current_token The current token in the token list.
 * @param data The data structure containing
 * information about the current command.
 * 
 * @return 0 if the token is a valid argument, 1 otherwise.
 */
int	check_argument(char *token, t_token *current_token, t_data *data)
{
	if (ft_argument_check(token, current_token) == 0)
	{
		if (current_token->id == 1 && current_token->prev->type == ARG)
			data->ignore_cmd = true;
		if (data->echoed == true)
			current_token->echo = true;
		return (0);
	}
	return (1);
}
