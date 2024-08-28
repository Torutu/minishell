/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_listnew.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 11:48:23 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/28 13:04:18 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Creates a new node for the linked list and returns a pointer to it
 *
 * @return A pointer to the newly created node
 */
t_env	*create_node(void)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	return (node);
}

/**
 * @brief Sets the content field of a linked list node.
 *
 * @details Given a linked list node and a string, this function sets the content
 * field of the node by duplicating the string. If memory allocation fails, the
 * function returns FAILURE.
 *
 * @param node The linked list node to set the content field for.
 * @param content The string to set the content from.
 *
 * @return 0 on success, -1 on failure.
 */
int	set_node_content(t_env *node, void *content)
{
	node->content = ft_strdup(content);
	if (!node->content)
		return (FAILURE);
	return (SUCCESS);
}

/**
 * @brief Sets the key field of a linked list node.
 *
 * @details Given a linked list node and an array of strings, this function sets
 * the key field of the node by duplicating the first string in the array.
 *
 * @param node The linked list node to set the key field for.
 * @param tmp_array An array of strings where the 
 * first element is used as the key.
 *
 * @return 0 on success, -1 on failure.
 */
int	s_node_k(t_env *node, char **tmp_array)
{
	node->key = ft_strdup(tmp_array[0]);
	if (!node->key)
		return (FAILURE);
	return (SUCCESS);
}

/**
 * @brief Sets the value field of a linked list node.
 *
 * @details Given a linked list node and a string, this function sets the value
 * field of the node by duplicating the substring of the content string after the
 * first '=' character. If there is no '=' character in the content string, the
 * value field is set to an empty string.
 *
 * @param node The linked list node to set the value field for.
 * @param content The string to set the value from.
 *
 * @return 0 on success, -1 on failure.
 */
int	s_node_v(t_env *node, void *content)
{
	char	*equal_sign;

	equal_sign = ft_strchr(content, '=');
	if (equal_sign)
		node->value = ft_strdup(equal_sign + 1);
	else
		node->value = ft_strdup("");
	if (!node->value)
		return (FAILURE);
	return (SUCCESS);
}

/**
 * Creates a new node for a linked list of environment variables.
 * 
 * The created node has its content set to the given parameter, and its key and
 * value fields are extracted from the content string using the '=' character as
 * a delimiter.
 * 
 * @param content The content of the new node, which will be copied into the
 * node's content field and used to extract the key and value fields.
 * 
 * @return A pointer to the created node, or NULL if the creation failed due to
 * memory allocation errors or invalid content.
 */
t_env	*ft_listnew(void *content)
{
	t_env	*node;
	char	**tmp_array;

	node = create_node();
	if (!node)
		return (NULL);
	if (set_node_content(node, content) == FAILURE)
	{
		free(node);
		return (NULL);
	}
	tmp_array = ft_split(content, '=');
	if (!tmp_array)
	{
		cleanup_node(node, NULL);
		return (NULL);
	}
	if (s_node_k(node, tmp_array) == 1 || s_node_v(node, content) == 1)
	{
		cleanup_node(node, tmp_array);
		return (NULL);
	}
	node->next = NULL;
	free_array(tmp_array);
	return (node);
}
