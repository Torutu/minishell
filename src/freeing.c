/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 22:21:18 by bposa             #+#    #+#             */
/*   Updated: 2024/08/25 10:00:31 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_my_boi(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
	{
		free_null(paths[i]);
		i++;
	}
	free_null(paths);
}

void	free_gang(t_data *data)
{
	if (data->tok_res != NULL)
		free_null(data->tok_res);
	if (data->temp_str != NULL)
		free_null(data->temp_str);
	if (data->new_str != NULL)
		free_null(data->new_str);
	free_tokens(data->token);
}

void	free_tokens(t_token *head)
{
	t_token	*tmp;

	tmp = NULL;
	while (head != NULL)
	{
		tmp = head;
		if (tmp->value)
		{
			free_null(tmp->value);
			tmp->value = NULL;
		}
		if (tmp->path)
			free_null(tmp->path);
		head = head->next;
		free_null(tmp);
	}
}

void	*free_arr_retnull(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
	array = NULL;
	return (NULL);
}

int	free_retstatus(char *array, int status)
{
	free(array);
	array = NULL;
	return (status);
}
