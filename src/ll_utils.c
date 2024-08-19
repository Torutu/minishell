/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ll_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 09:30:58 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/19 10:52:11 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*ft_listnew(void *content)
{
	t_env	*node;
	char	**tmp_array;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->content = content;
	tmp_array = ft_split(content, '=');
	node->key = ft_strdup(tmp_array[0]);
	node->value = ft_strdup(ft_strchr(content, '=') + 1);
	node->next = NULL;
	free_array(tmp_array);
	return (node);
}

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
		env_ll = env_ll->next;
		free(tmp);
		tmp = NULL;
	}
}
