/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exporting.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 11:32:21 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/28 11:45:06 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_null_next_token(t_token *token, t_env **env_ll)
{
	if (token->next->value == NULL)
	{
		print_export(env_ll);
		return (1);
	}
	return (0);
}

int	handle_special_cases(t_token *token, t_env **env_ll)
{
	if (if_redirection(token))
		return (SUCCESS);
	if (handle_null_next_token(token, env_ll))
		return (SUCCESS);
	return (FAILURE);
}

int	is_valid_identifier(char *value)
{
	if (!ft_ischar(value[0]) && !(value[0] == '_'))
	{
		printf("minishell: export: '%s': not a valid identifier\n", value);
		return (FAILURE);
	}
	return (SUCCESS);
}

int	process_token(t_env **env_ll, t_token *tmp_tok)
{
	t_env	*tmp_ll;
	int		found;

	tmp_ll = *env_ll;
	found = 0;
	if (is_valid_identifier(tmp_tok->value) == FAILURE)
		return (SUCCESS);
	while (tmp_ll != NULL)
	{
		if (find_key_in_env(tmp_ll, tmp_tok->value, NULL) == SUCCESS)
		{
			found = 1;
			if (update_existing_env(tmp_ll, tmp_tok->value) == FAILURE)
				return (FAILURE);
			break ;
		}
		tmp_ll = tmp_ll->next;
	}
	if (!found)
	{
		if (add_new_env_variable(env_ll, tmp_tok->value) == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}

int	export(t_token *token, t_env **env_ll)
{
	t_token	*tmp_tok;

	if (handle_special_cases(token, env_ll) == SUCCESS)
		return (SUCCESS);
	tmp_tok = token->next;
	while (tmp_tok != NULL && tmp_tok->value != NULL)
	{
		if (process_token(env_ll, tmp_tok) == FAILURE)
			return (FAILURE);
		tmp_tok = tmp_tok->next;
	}
	return (SUCCESS);
}
