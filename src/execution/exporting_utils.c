/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exporting_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 11:32:14 by walnaimi          #+#    #+#             */
/*   Updated: 2024/09/02 00:17:12 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Prints the environment variables in a sorted manner.
 *
 * @param env_ll A pointer to the linked list containing the environment
 * variables.
 *
 * @return SUCCESS if the function executes successfully, FAILURE otherwise.
 *
 * This function first checks if the environment linked list is empty. If it is,
 * it returns SUCCESS. Then, it allocates an array to store the environment
 * variables. If the allocation fails, it returns FAILURE. Finally, it prints
 * the environment variables in a sorted manner using alphabetical_printer()
 * and frees the array.
 */
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

/**
 * Prints the environment variables in alphabetical order.
 *
 * The printing is done by first looping through the array of environment
 * variables, and then looping through the characters in the ASCII table
 * (from 32 to 126, which are the printable characters). For each character,
 * it checks if the variable starts with that character, and if so, it
 * prints it. This way, the variables are printed in alphabetical order.
 *
 * @param env_array An array of environment variables.
 */
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
				ft_putstr_fd("declare -x ", 1);
				ft_putendl_fd(env_array[i], 1);
			}
			i++;
		}
		i = 0;
		c++;
	}
}

/**
 * Checks if a key already exists in the environment linked list.
 *
 * @param env_ll The head of the environment linked list.
 * @param token_value The token value to search for in the environment.
 * @param out_key A pointer to a pointer to the environment node that matches
 * the key. If NULL, the function does not set the pointer.
 *
 * @return SUCCESS if a match was found, FAILURE otherwise.
 */
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

/**
 * Splits the given token_value into a key-value pair and assigns them
 * to the given environment node.
 *
 * @param env_node The environment node to set the key-value pair.
 * @param token_value The token value to split, which should be in the format
 * "key=value".
 *
 * @return SUCCESS if the key-value pair was successfully set, FAILURE otherwise.
 */
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

/**
 * Searches the environment linked list to find a matching key for the given
 * token_value. If a match is found, sets the found_env pointer to the matching
 * node.
 *
 * @param env_ll The head of the environment linked list.
 * @param token_value The token value to search for in the environment.
 * @param found_env A pointer to a pointer to the environment node that matches
 * the key. If NULL, the function does not set the pointer.
 *
 * @return SUCCESS if a match was found, FAILURE otherwise.
 */
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
