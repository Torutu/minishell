/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exporting_utils_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 11:32:01 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/28 12:30:21 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	add_new_env_variable(t_env **env_ll, char *token_value)
{
	t_env	*new_env;

	new_env = ft_listnew(token_value);
	if (!new_env)
		return (FAILURE);
	ft_listadd_back(env_ll, new_env);
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
