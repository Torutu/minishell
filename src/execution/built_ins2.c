/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 15:26:27 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/28 11:44:40 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	delete_first_node(t_env **env_ll, t_token *head, t_data *data)
{
	t_env	*tmp;

	tmp = *env_ll;
	if (tmp && !ft_strncmp(head->value, tmp->content, ft_strlen(head->value)))
	{
		*env_ll = tmp->next;
		data->envll = *env_ll;
		free_null(tmp->key);
		free_null(tmp->value);
		free_null(tmp->content);
		free(tmp);
		return (1);
	}
	return (0);
}

void	delete_subsequent_nodes(t_env *env_ll, t_token *head)
{
	t_env	*tmp;
	t_env	*del;

	tmp = env_ll;
	while (tmp && tmp->next != NULL)
	{
		if (!ft_strncmp(head->value, tmp->next->content,
				ft_strlen(head->value)))
		{
			del = tmp->next;
			tmp->next = tmp->next->next;
			free_null(del->key);
			free_null(del->value);
			free_null(del->content);
			free(del);
			break ;
		}
		tmp = tmp->next;
	}
}

int	unset(t_token *token, t_env **env_ll, t_data *data)
{
	t_token	*head;

	head = token->next;
	while (head && head->value)
	{
		if (delete_first_node(env_ll, head, data))
		{
			head = head->next;
			continue ;
		}
		delete_subsequent_nodes(*env_ll, head);
		head = head->next;
	}
	return (SUCCESS);
}
