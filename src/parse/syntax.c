/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 17:18:12 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/27 01:03:24 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * incorrect_syntax() checks for specific operators and checks if they're
 * being repeated by the users.
 * 
 * RETURN VALUES: Upon success, it returns 0. If it fails, it returns 2.
 */
static int	incorrect_syntax(t_token *token, t_type token_type)
{
	t_token	*head;

	head = token;
	while (head)
	{
		if (head->next != NULL)
		{
			if ((head->type == token_type && head->next->type == token_type)
				|| (head->type == token_type && head->next->type == RED_IN)
				|| (head->type == token_type && head->next->type == RED_OUT)
				|| (head->type == token_type && head->next->type == HEREDOC)
				|| (head->type == token_type && head->next->type == APPEND)
				|| (head->type == token_type && head->next->type == FLAG)
				|| (head->type == token_type && head->next->value == NULL))
				return (err_msg(head->next->value, SYNTAX, 1));
		}
		head = head->next;
	}
	head = NULL;
	return (SUCCESS);
}

/**
 * As we run through the tokens (nodes in a linked list) we check
 * if the next token type is valid for the token type that the user
 * inputted previously. Examples below:
 * "%> |||"
 * "%> syntax error near unexpected token `||'"
 */
int	syntax_check(t_token *token)
{
	if (incorrect_syntax(token, PIPE) == FAILURE
		|| incorrect_syntax(token, RED_OUT) == FAILURE
		|| incorrect_syntax(token, RED_IN) == FAILURE
		|| incorrect_syntax(token, HEREDOC) == FAILURE
		|| incorrect_syntax(token, APPEND) == FAILURE)
		return (FAILURE);
	else
		return (SUCCESS);
}
