/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echoing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 12:19:17 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/28 12:56:21 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
			if (head->type == RED_IN || head->type == RED_OUT
				|| head->type == APPEND || head->type == HEREDOC)
				break ;
			printf("%s", head->value);
		}
		head = head->next;
		if (head != NULL && head->value != NULL && head->value[0] != '\0')
			printf(" ");
	}
	return (SUCCESS);
}

int	handle_arg_type(t_token *head)
{
	head = head->next;
	while (head->value != NULL && head->value[0] == '\0')
		head = head->next;
	while (head != NULL)
	{
		if (head->value != NULL && head->value[0] != '\0')
		{
			if (head->type == RED_IN || head->type == RED_OUT
				|| head->type == APPEND || head->type == HEREDOC)
				break ;
			printf("%s", head->value);
		}
		head = head->next;
		if (head != NULL && head->value != NULL && head->value[0] != '\0')
			printf(" ");
	}
	printf("\n");
	return (SUCCESS);
}

int	yodeling(t_token *token)
{
	t_token	*head;

	head = token;
	while (head->value != NULL)
	{
		if (head->next->value == NULL)
			return (printf("\n"), SUCCESS);
		if (head->next->type == FLAG && head->next->echo == true)
			return (handle_flag_type(head));
		if (head != NULL && head->next != NULL && head->next->type == ARG
			&& ft_strncmp(head->value, "echo", 5) == 0)
			return (handle_arg_type(head));
		head = head->next;
	}
	return (FAILURE);
}
