/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exporting.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 11:32:21 by walnaimi          #+#    #+#             */
/*   Updated: 2024/09/02 00:21:45 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * If the export command has no arguments, print the current environment
 * variables and return SUCCESS.
 *
 * @param token The token structure containing 
 * the arguments to the export command.
 * @param env_ll The head of the environment linked list.
 *
 * @return 0 if the export command has no arguments, 1 otherwise.
 */
int	no_args(t_token *token, t_env **env_ll)
{
	if (token->next->value == NULL)
	{
		print_export(env_ll);
		return (SUCCESS);
	}
	return (FAILURE);
}

/**
 * Handles special cases for the export built-in command.
 *
 * @param token The token to be handled.
 * @param env_ll The head of the environment linked list.
 *
 * @return 0 if the token is a special case, 1 otherwise.
 *
 * The special cases are:
 *  - If the token is a redirection token, return SUCCESS.
 *  - If the token has no arguments, print the current environment and
 *    return SUCCESS.
 */
int	handle_special_cases(t_token *token, t_env **env_ll)
{
	if (if_redirection(token) == SUCCESS)
		return (SUCCESS);
	if (no_args(token, env_ll) == SUCCESS)
		return (SUCCESS);
	return (FAILURE);
}

/**
 * Checks if a given string is a valid identifier for an environment variable.
 *
 * An identifier is valid if it starts with either an alphabetic character or
 * an underscore. This function does not check if the identifier already exists
 * in the environment.
 *
 * @param value The string to check.
 * @return SUCCESS if the identifier is valid, FAILURE otherwise.
 */
int	is_valid_identifier(char *value)
{
	if (!ft_ischar(value[0]) && !(value[0] == '_'))
	{
		printf(" export: `%s': not a valid identifier\n", value);
		return (FAILURE);
	}
	return (SUCCESS);
}

/**
 * Process a single token from the export command.
 *
 * @param env_ll A pointer to the environment linked list.
 * @param tmp_tok A pointer to the token to be processed.
 *
 * @return 0 if the token is processed successfully, 1 otherwise.
 *
 * A token is processed by first checking if it is a valid identifier, then
 * checking if the identifier is already present in the environment linked list.
 * If it is, the value is updated. If not, a new environment variable is added.
 */
int	process_token(t_env **env_ll, t_token *tmp_tok)
{
	t_env	*tmp_ll;
	int		found;

	tmp_ll = *env_ll;
	found = 0;
	if (is_valid_identifier(tmp_tok->value) == FAILURE)
		return (FAILURE);
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

/**
 * Handles the export built-in command.
 *
 * @param token The token structure containing
 * the arguments to the export command.
 * @param env_ll The head of the environment linked list.
 * @return 0 if the export command is successful, 1 otherwise.
 * The export command sets environment variables. The only argument to
 * the export command is the name of the variable to be exported. The
 * value of the variable is taken from the environment linked list. If
 * the variable is not in the environment linked list, the value is
 * set to an empty string.
 *
 * The export command returns 0 on success and 1 on failure.
 */
int	export(t_token *token, t_env **env_ll)
{
	t_token	*tmp_tok;

	if (handle_special_cases(token, env_ll) == SUCCESS)
		return (SUCCESS);
	tmp_tok = token->next;
	while (tmp_tok != NULL && tmp_tok->value != NULL && tmp_tok->type != PIPE
		&& tmp_tok->type != RED_IN && tmp_tok->type != RED_OUT
		&& tmp_tok->type != HDOC && tmp_tok->type != APPEND)
	{
		if (process_token(env_ll, tmp_tok) == FAILURE)
			return (FAILURE);
		tmp_tok = tmp_tok->next;
	}
	return (SUCCESS);
}
