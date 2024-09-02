/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exporting_utils_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 11:32:01 by walnaimi          #+#    #+#             */
/*   Updated: 2024/09/01 22:59:14 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Adds a new environment variable to the linked list.
 *
 * The function takes a double pointer to the environment linked list
 * and a string containing the key-value pair of the new variable.
 * It creates a new node with the given key-value pair and adds it
 * to the end of the linked list. If the allocation for the new node
 * fails, it returns FAILURE. Otherwise, it returns SUCCESS.
 *
 * @param env_ll A double pointer to the environment linked list.
 * @param token_value A string containing the key-value pair
 * of the new variable.
 *
 * @return SUCCESS if the new node was successfully added,
 * FAILURE otherwise.
 */
int	add_new_env_variable(t_env **env_ll, char *token_value)
{
	t_env	*new_env;

	new_env = ft_listnew(token_value);
	if (!new_env)
		return (FAILURE);
	ft_listadd_back(env_ll, new_env);
	return (SUCCESS);
}

/**
 * Updates the content, key and value fields of an existing
 * environment node given a token_value string.
 *
 * @param env_node The environment node to update.
 * @param token_value The string to update the content, key and value
 * with.
 *
 * @return SUCCESS if the update was successful, FAILURE otherwise.
 */
int	update_existing_env(t_env *env_node, char *token_value)
{
	free_null(env_node->key);
	free_null(env_node->value);
	free_null(env_node->content);
	if (update_content(env_node, token_value) == FAILURE)
		return (FAILURE);
	if (set_key_and_value(env_node, token_value) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

/**
 * Checks if the given token is a redirection operator.
 *
 * @param token The token to check.
 *
 * @return 0 if the token is a redirection operator, non-zero otherwise.
 */
int	if_redirection(t_token *token)
{
	if (find_token(token, APPEND) || find_token(token, HDOC)
		|| find_token(token, RED_IN) || find_token(token, RED_OUT)
		|| find_token(token, PIPE))
		return (SUCCESS);
	return (FAILURE);
}

/**
 * Checks if a character is an alphabetical character.
 *
 * @param c The character to check.
 *
 * @return 1 if the character is an alphabetical character, 0 otherwise.
 */
int	ft_ischar(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

char	**split_and_validate_token(char *token_value)
{
	char	**array;

	array = ft_split(token_value, '=');
	if (array == NULL || array[0] == NULL)
	{
		free_array(array);
		return (NULL);
	}
	return (array);
}
