/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echoing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 12:19:17 by walnaimi          #+#    #+#             */
/*   Updated: 2024/09/02 10:52:00 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Handles the flag type tokens in the echo command.
 * 
 * @param head The token to start processing from.
 * 
 * @return 0 if the tokens were processed successfully, 1 otherwise.
 */
int	handle_flag_type(t_token *head)
{
	head = head->next;
	while (head->type == FLAG)
	{
		head = head->next;
		if (head->value == NULL)
			return (SUCCESS);
	}
	while (head->value != NULL && head->value[0] == '\0')
		head = head->next;
	while (head != NULL)
	{
		if (head->value != NULL && head->value[0] != '\0')
		{
			if (head->redirect == true || head->type == PIPE)
				break ;
			ft_putstr_fd(head->value, 1);
		}
		head = head->next;
		if (head != NULL && head->value != NULL && head->value[0] != '\0'
			&& head->redirect == false && head->type != PIPE)
			write(1, " ", 1);
	}
	return (SUCCESS);
}

/**
 * Handles the argument type tokens in the echo command.
 * 
 * @param head The token to start processing from.
 * 
 * @return 0 if the tokens were processed successfully, 1 otherwise.
 */
int	handle_arg_type(t_token *head)
{
	head = head->next;
	while (head->value != NULL && head->value[0] == '\0')
		head = head->next;
	while (head != NULL)
	{
		if (head->value != NULL && head->value[0] != '\0')
		{
			if (head->redirect == true || head->type == PIPE)
				break ;
			ft_putstr_fd(head->value, 1);
		}
		head = head->next;
		if (head != NULL && head->value != NULL && head->value[0] != '\0'
			&& head->redirect == false && head->type != PIPE)
			write(1, " ", 1);
	}
	write(1, "\n", 1);
	return (SUCCESS);
}

/**
 * Handles the echo command.
 * 
 * @param token The token to start processing from.
 * 
 * @return 0 if the tokens were processed successfully, 1 otherwise.
 */
int	yodeling(t_token *token)
{
	t_token	*head;

	head = token;
	while (head->value != NULL)
	{
		if (head->next->value == NULL)
			return (write(1, "\n", 1), SUCCESS);
		if (head->next->type == FLAG && head->next->echo == true)
			return (handle_flag_type(head));
		if (head != NULL && head->next != NULL && head->next->type == ARG
			&& ft_strncmp(head->value, "echo", 5) == 0)
			return (handle_arg_type(head));
		head = head->next;
	}
	return (FAILURE);
}
