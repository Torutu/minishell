/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 14:53:29 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/25 23:35:34 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * This function first checks if the `env_value` is not `NULL`.
 * it calculates the length of the `env_value` using `ft_strlen()`.
 * Then, it uses `ft_strncpy()` to copy the `env_value` to the `result` string,
 * starting from the current index indicated by `num->j`.
 * Finally, it updates the `num->j` index by
 * adding the length of the `env_value`.
 *
 * If the `env_value` is `NULL`, the function does nothing.
 *
 * Copies the value of the environment variable to the result string,
 * starting from the current index. The function takes three parameters:
 * @param result A pointer to the result string where the value will be copied.
 * @param env_value A pointer to the environment variable value to be copied.
 * @param num A pointer to a `t_index`
 * structure that keeps track of the current index in the result string.
 */
void	copy_env_value(char *result, const char *env_value, t_index *num)
{
	size_t	value_len;

	if (env_value)
	{
		value_len = ft_strlen(env_value);
		ft_strncpy(&result[num->j], env_value, value_len);
		num->j += value_len;
	}
}

/**
 * Toggles the single quote flag and appends the current
 * character from the input string to the result string.
 *
 * @param input The input string being processed.
 * @param data A pointer to the data structure containing the parsing state.
 * @param res The result string where the character will be appended.
 * @param num A pointer to the index of the current position in the input string.
 */
void	setup_env_variables(const char *input, t_data *data)
{
	data->num_of_envs = count_matching_keys(data->envll, input);
	if (data->num_of_envs == 0)
		data->num_of_envs = 1;
	data->s_quote_o = 0;
	data->dbl_q = 0;
}

void	single_q(const char *input, t_data *data, char *res, t_index *num)
{
	data->s_quote_o = !data->s_quote_o;
	res[num->j++] = input[num->i++];
}

/**
 * Toggles the double quote flag and appends the
 * current character from the input string to the result string.
 *
 * @param input The input string being processed.
 * @param data A pointer to the data structure containing the parsing state.
 * @param res The result string where the character will be appended.
 * @param num A pointer to the index of the current position in the input string.
 */
void	double_q(const char *input, t_data *data, char *res, t_index *num)
{
	data->dbl_q = !data->dbl_q;
	res[num->j++] = input[num->i++];
}

/**
 * Handles environment variables in a given string.
 *
 * Retrieves the value of an environment variable from the input string and
 * copies it to the result string.
 *
 * @param str The input string containing the environment variable.
 * @param num A pointer to the index of the current position in the input string.
 * @param data A pointer to the data structure
 * containing the environment variables.
 * @param res The result string where the
 * environment variable value will be copied.
 */
void	handle_env_var(const char *str, t_index *num, t_data *data, char *res)
{
	char	*env_value;

	env_value = fetching_env(str, num, data);
	if (env_value)
		copy_env_value(res, env_value, num);
}
