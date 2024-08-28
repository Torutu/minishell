/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ll_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 09:30:58 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/28 02:36:56 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*create_node(void)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	return (node);
}

int	set_node_content(t_env *node, void *content)
{
	node->content = ft_strdup(content);
	if (!node->content)
		return (FAILURE);
	return (SUCCESS);
}

int	s_node_k(t_env *node, char **tmp_array)
{
	node->key = ft_strdup(tmp_array[0]);
	if (!node->key)
		return (FAILURE);
	return (SUCCESS);
}

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

void	cleanup_node(t_env *node, char **tmp_array)
{
	if (tmp_array)
		free_array(tmp_array);
	if (node)
	{
		free_null(node->key);
		free_null(node->value);
		free_null(node->content);
		free(node);
	}
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

// t_env	*ft_listnew(void *content)
// {
// 	t_env	*node;
// 	char	**tmp_array;

// 	node = (t_env *)malloc(sizeof(t_env));
// 	if (!node)
// 		return (NULL);
// 	node->content = ft_strdup(content);
// 	// add a malloc check
// 	tmp_array = ft_split(content, '=');
// 	// add a malloc check
// 	node->key = ft_strdup(tmp_array[0]);
// 	// add a malloc check
// 	node->value = ft_strdup(ft_strchr(content, '=') + 1);
// 	// add a malloc check
// 	node->next = NULL;
// 	free_array(tmp_array);
// 	return (node);
// }

char	*get_home(t_env *env_ll)
{
	t_env	*tmp;

	tmp = env_ll;
	while (env_ll->next != NULL)
	{
		if (!ft_strncmp(env_ll->content, "HOME=", 5))
			return (env_ll->content + 5);
		env_ll = env_ll->next;
	}
	env_ll = tmp;
	return (NULL);
}

void	ft_listadd_back(t_env **lst, t_env *new)
{
	t_env	*last;

	if (lst)
	{
		if (*lst)
		{
			last = ft_list_last(*lst);
			last->next = new;
		}
		else
			*lst = new;
	}
}

t_env	*ft_list_last(t_env *lst)
{
	while (lst)
	{
		if (!lst->next)
			return (lst);
		lst = lst->next;
	}
	return (lst);
}

void	free_ll(t_env *env_ll)
{
	t_env	*tmp;

	while (env_ll != NULL)
	{
		tmp = env_ll;
		free(env_ll->key);
		free(env_ll->value);
		free(env_ll->content);
		env_ll = env_ll->next;
		free(tmp);
		tmp = NULL;
	}
}
