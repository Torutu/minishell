/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exporting_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 11:32:10 by walnaimi          #+#    #+#             */
/*   Updated: 2024/09/01 22:14:21 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Updates the content field of the given environment node with the given
 * token_value. The content field is duplicated from the token_value.
 *
 * @param env_node The environment node to update the content field for.
 * @param token_value The string to copy into the content field.
 *
 * @return SUCCESS if the content was updated successfully, FAILURE otherwise.
 */
int	update_content(t_env *env_node, char *token_value)
{
	env_node->content = ft_strdup(token_value);
	if (!env_node->content)
		return (FAILURE);
	return (SUCCESS);
}

/**
 * Extracts the value from a given token_value, which is a string of the format
 * "key=value". If the token_value does not contain a '=', the function
 * returns an empty string.
 *
 * @param token_value The token value from which to extract the value.
 * @return The value part of the token value, or an empty string if the
 * token_value does not contain a '='.
 */
char	*extract_value(char *token_value)
{
	char	*value;

	if (ft_strchr(token_value, '='))
		value = ft_strdup(ft_strchr(token_value, '=') + 1);
	else
		value = ft_strdup("");
	return (value);
}

/**
 * Compares the given key with the keys in the environment linked list.
 *
 * @param env_ll The head of the environment linked list.
 * @param key The key to search for in the environment linked list.
 *
 * @return 1 if the key is found, 0 otherwise.
 */
int	compare_keys(t_env *env_ll, char *key)
{
	t_env	*current;

	current = env_ll;
	while (current != NULL)
	{
		if (ft_strncmp(current->key, key, ft_strlen(current->key)) == 0)
			return (1);
		current = current->next;
	}
	return (0);
}
