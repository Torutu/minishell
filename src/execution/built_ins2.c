/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 15:26:27 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/28 11:18:35 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
/********************************************************************* */
void	alphabetical_printer(char **env_array)
{
	char	c;
	int		i;

	i = 0;
	c = 32;
	while (c < 127)
	{
		while (env_array[i])
		{
			if (env_array[i][0] == c)
			{
				printf("declare -x ");
				printf("%s\n", env_array[i]);
			}
			i++;
		}
		i = 0;
		c++;
	}
}

// Helper function to check if the token is a redirection or append type
int	if_redirection(t_token *token)
{
	if (find_token(token, APPEND) || find_token(token, HEREDOC)
		|| find_token(token, RED_IN) || find_token(token, RED_OUT))
		return (1);
	return (0);
}


int	ft_ischar(char c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	return (0);
}

//export_utils
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

//export_utils
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
//export_utils
int	check_existing_key(t_env *env_ll, char *token_value, char **out_key)
{
	char	**array;
	int		found;

	found = 0;
	array = split_and_validate_token(token_value);
	if (array == NULL)
		return (FAILURE);

	found = compare_keys(env_ll, array[0]);

	if (out_key != NULL)
		*out_key = array[0];
	else
	{
		free_array(array);
		return (FAILURE);
	}

	free_array(array);

	if (found)
		return (SUCCESS);
	else
		return (FAILURE);
}

//export_utils
int		update_content(t_env *env_node, char *token_value)
{
	env_node->content = ft_strdup(token_value);
	if (!env_node->content)
		return (FAILURE);
	return (SUCCESS);
}

char	*extract_value(char *token_value)
{
	char	*value;

	if (ft_strchr(token_value, '='))
		value = ft_strdup(ft_strchr(token_value, '=') + 1);
	else
		value = ft_strdup("");

	return (value);
}

int set_key_and_value(t_env *env_node, char *token_value)
{
	char	**array;

	array = ft_split(token_value, '=');
	if (!array)
		return (FAILURE);
	if (array[0] == NULL)
	{
		free_array(array);
		return (FAILURE);
	}
	env_node->key = ft_strdup(array[0]);
	if (!env_node->key)
	{
		free_array(array);
		return (FAILURE);
	}
	env_node->value = extract_value(token_value);
	if (!env_node->value)
	{
		free_array(array);
		free_null(env_node->key);
		return (FAILURE);
	}
	free_array(array);
	return (SUCCESS);
}

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

int	find_key_in_env(t_env *env_ll, char *token_value, t_env **found_env)
{
	char	**array;
	int		found;

	found = 0;
	array = ft_split(token_value, '=');
	if (array == NULL)
		return (FAILURE);
	if (array[0] == NULL)
	{
		free_array(array);
		return (FAILURE);
	}
	if (ft_strncmp(env_ll->key, array[0], ft_strlen(env_ll->key)) == 0)
	{
		found = 1;
		if (found_env != NULL)
			*found_env = env_ll;
	}
	free_array(array);
	if (found)
		return (SUCCESS);
	else
		return (FAILURE);
}

int	add_new_env_variable(t_env **env_ll, char *token_value)
{
	t_env	*new_env;

	new_env = ft_listnew(token_value);
	if (!new_env)
		return (FAILURE);

	ft_listadd_back(env_ll, new_env);
	return (SUCCESS);
}
/********************************************************** */
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
