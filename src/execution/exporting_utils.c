#include "../../includes/minishell.h"

//export_utils
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


