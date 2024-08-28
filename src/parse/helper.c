/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 15:08:47 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/27 13:20:10 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Searches for a specific token type within a linked list of tokens.
 *
 * @param token The head of the linked list of tokens.
 * @param type The type of token to search for.
 *
 * @return A pointer to the first token of 
 * the specified type if found, NULL otherwise.
 */
t_token	*find_token(t_token *token, t_type type)
{
	t_token	*head;

	head = token;
	while (head != NULL)
	{
		if (type == head->type)
			return (head);
		head = head->next;
	}
	head = NULL;
	return (NULL);
}

/**
 * Searches for a specific token type within a linked list of tokens.
 *
 * @param token The head of the linked list of tokens.
 * @param type The type of token to search for.
 *
 * @return 1 if the token type is found, 0 otherwise.
 */
int	search_token_type(t_token *token, t_type type)
{
	t_token	*head;

	head = token;
	while (head != NULL)
	{
		if (head->type == type)
			return (1);
		head = head->next;
	}
	return (0);
}

/**
 * Checks if a string contains only alphabetic characters.
 *
 * @param str The string to check.
 *
 * @return 1 if the string contains only alphabetic characters, 0 otherwise.
 */
int	ft_isalpha_str(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_isalpha(str[i]))
			return (1);
		i++;
	}
	return (0);
}
