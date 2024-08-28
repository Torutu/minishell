/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunky_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:33:52 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/25 00:31:50 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Checks if the given token is a built-in command.
 *
 * @param token The token to check.
 * @param current_token The token to set the type for.
 * @return 0 if the token is a built-in command, non-zero otherwise.
 */
int	ft_builtin_check(char *token, t_token *current_token)
{
	if (ft_strncmp(token, "echo", 5) == 0
		|| ft_strncmp(token, "exit", 5) == 0
		|| ft_strncmp(token, "pwd", 4) == 0
		|| ft_strncmp(token, "cd", 3) == 0
		|| ft_strncmp(token, "export", 7) == 0
		|| ft_strncmp(token, "unset", 6) == 0
		|| ft_strncmp(token, "env", 3) == 0)
	{
		current_token->value = ft_strdup(token);
		current_token->type = BUILTIN;
		free_null(token);
		return (0);
	}
	return (1);
}

/**
 * Checks if the given token is a valid command.
 *
 * @param token The token to check.
 * @param current_token The token to set the type for.
 * @param data The data structure containing information about the command.
 * @return 0 if the token is a valid command, non-zero otherwise.
 */
int	cmd_check(char *token, t_token *current_token, t_data *data)
{
	if (handle_absolute_path(token, current_token) == 0)
		return (0);
	if (handle_cmd_exe(token, current_token, data) == 0)
		return (0);
	return (1);
}

/**
 * Checks if the given token is a pipe symbol
 * and sets the corresponding token type.
 *
 * @param token The token to check.
 * @param current_token The token to set the type for.
 * @return 0 if the token is a pipe symbol, non-zero otherwise.
 */
int	ft_pipe_check(char *token, t_token *current_token)
{
	if (ft_strcmp(token, "|") == 0)
	{
		current_token->value = ft_strdup(token);
		current_token->type = PIPE;
		free_null(token);
		return (0);
	}
	return (1);
}

/**
 * Checks if the given token is a redirection operator
 * and sets the corresponding token type.
 * 
 * @param token The token to check.
 * @param current_token The token to set the type for.
 * @return 0 if the token is a redirection operator, non-zero otherwise.
 */
int	ft_redirect_op_check(char *token, t_token *current_token)
{
	if (handle_redirect_out(token, current_token) == 0)
		return (0);
	if (handle_append(token, current_token) == 0)
		return (0);
	if (handle_redirect_in(token, current_token) == 0)
		return (0);
	if (handle_heredoc(token, current_token) == 0)
		return (0);
	return (1);
}

/**
 * Checks if the given token is an argument and
 * sets the corresponding token type.
 *
 * @param token The token to check.
 * @param current_token The token to set the type for.
 * @return 0 on success, non-zero on failure.
 */
int	ft_argument_check(char *token, t_token *current_token)
{
	current_token->value = ft_strdup(token);
	current_token->type = ARG;
	free_null(token);
	return (0);
}
