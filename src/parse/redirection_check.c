/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_check.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 03:58:57 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/23 22:43:09 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Handles the output redirection token.
 *
 * @param token The token to be handled.
 * @param current_token The current token being processed.
 *
 * @return 0 if the token is an output redirection token, 1 otherwise.
 */
int	handle_redirect_out(char *token, t_token *current_token)
{
	if (ft_strncmp(token, ">", 2) == 0 && current_token->in_q == false)
	{
		current_token->value = ft_strdup(token);
		current_token->type = RED_OUT;
		free_null(token);
		return (0);
	}
	return (1);
}

/**
 * Handles the append redirection token.
 *
 * @param token The token to be handled.
 * @param current_token The current token being processed.
 *
 * @return 0 if the token is an append redirection token, 1 otherwise.
 */
int	handle_append(char *token, t_token *current_token)
{
	if (ft_strncmp(token, ">>", 3) == 0 && current_token->in_q == false)
	{
		current_token->value = ft_strdup(token);
		current_token->type = APPEND;
		free_null(token);
		return (0);
	}
	return (1);
}

/**
 * Handles the input redirection token.
 *
 * @param token The token to be handled.
 * @param current_token The current token being processed.
 *
 * @return 0 if the token is an input redirection token, 1 otherwise.
 */
int	handle_redirect_in(char *token, t_token *current_token)
{
	if (ft_strncmp(token, "<", 2) == 0 && current_token->in_q == false)
	{
		current_token->value = ft_strdup(token);
		current_token->type = RED_IN;
		free_null(token);
		return (0);
	}
	return (1);
}

/**
 * Handles the heredoc redirection token.
 *
 * @param token The token to be handled.
 * @param current_token The current token being processed.
 *
 * @return 0 if the token is a heredoc redirection token, 1 otherwise.
 */
int	handle_heredoc(char *token, t_token *current_token)
{
	if (ft_strncmp(token, "<<", 3) == 0 && current_token->in_q == false)
	{
		current_token->value = ft_strdup(token);
		current_token->type = HEREDOC;
		free_null(token);
		return (0);
	}
	return (1);
}
