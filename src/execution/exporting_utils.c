/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exporting_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 11:32:14 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/28 12:58:53 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	print_export(t_env **env_ll)
{
	char	**env_array;

	if (!env_ll || !*env_ll)
		return (SUCCESS);
	env_array = env_arr_updater(env_ll);
	if (!env_array)
		return (SUCCESS);
	alphabetical_printer(env_array);
	free_array(env_array);
	return (SUCCESS);
}

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

int	set_key_and_value(t_env *env_node, char *token_value)
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
