/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 10:19:57 by walnaimi          #+#    #+#             */
/*   Updated: 2024/09/03 02:29:56 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	**alloc_cmd_array(t_token *token)
{
	int		cmd_size;
	char	**cmd_array;

	cmd_size = 0;
	while (token && token->type != PIPE)
	{
		cmd_size++;
		token = token->next;
	}
	cmd_array = (char **)malloc(sizeof(char *) * (cmd_size + 1));
	if (!cmd_array)
		return (NULL);
	return (cmd_array);
}

/**
 * @brief Counts the number of PIPE tokens in a linked list of tokens.
 * @param token the first token in the list
 * @return the number of PIPE tokens in the list
 */
int	count_pipes(t_token *token)
{
	int	count;

	count = 0;
	while (token)
	{
		if (token->type == PIPE)
			count++;
		token = token->next;
	}
	return (count);
}

char	**build_command(t_token **head)
{
	char	**cmd;
	int		j;

	cmd = alloc_cmd_array(*head);
	if (!cmd)
		return (NULL);
	j = 0;
	while ((*head)->value && (*head)->type != PIPE)
	{
		cmd[j] = ft_strdup((*head)->value);
		if (!cmd[j])
			return (NULL);
		*head = (*head)->next;
		j++;
	}
	cmd[j] = NULL;
	return (cmd);
}

char	***token_to_array(t_token *token)
{
	t_token	*head;
	char	***cmd_array;
	int		i;

	i = 0;
	head = token;
	cmd_array = (char ***)malloc(sizeof(char **) * (count_pipes(token) + 2));
	if (!cmd_array)
		return (NULL);
	while (head && head->value)
	{
		cmd_array[i] = build_command(&head);
		if (!cmd_array[i])
			return (free_cmd_array(cmd_array));
		i++;
		if (head && head->type == PIPE)
			head = head->next;
	}
	cmd_array[i] = NULL;
	return (cmd_array);
}
