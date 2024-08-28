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