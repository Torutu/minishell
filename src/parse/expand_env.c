/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 21:55:38 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/25 19:44:52 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Retrieves the value of an environment variable from the given input string.
 * 
 * This function takes a pointer to the input string, the start position of
 *  the variable name, 
 * the length of the variable name, and a pointer to the data structure 
 * containing the environment variables.
 * 
 * It returns a pointer to the value of the environment variable, 
 * or NULL if the variable is not found.
 * 
 * @param input The input string containing the environment variable to retrieve.
 * @param var_start The start position of the variable name in the input string.
 * @param var_len The length of the variable name.
 * @param data A pointer to the data structure containing
 *  the environment variables.
 * @return A pointer to the value of the environment variable,
 *  or NULL if the variable is not found.
 */
char	*get_env_value(const char *input, size_t v_s, size_t v_l, t_data *data)
{
	data->var_name = malloc(sizeof(char) * (v_l + 1));
	ft_strncpy(data->var_name, &input[v_s], v_l);
	data->var_name[v_l] = '\0';
	return (ft_getenv(data->var_name, data->envll));
}

/**
 * Retrieves the value of an environment variable from the given input string.
 *
 * @param str The input string containing the environment variable to expand.
 * @param num A pointer to the index of the current position in the input string.
 * @param data A pointer to the data structure 
 * containing the environment variables.
 * @return A pointer to the expanded environment variable,
 * or NULL if the variable is not found.
 */
char	*fetching_env(const char *str, t_index *num, t_data *data)
{
	size_t	var_start;
	size_t	var_len;
	char	*env_value;

	var_start = ++(num->i);
	while (str[num->i] && (ft_isalnum(str[num->i]) || str[num->i] == '_'))
		num->i++;
	var_len = num->i - var_start;
	if (var_len > 0)
	{
		env_value = get_env_value(str, var_start, var_len, data);
		if (env_value)
			return (env_value);
		else
			return (NULL);
	}
	return (NULL);
}

/**
 * Handles the status variable and appends it to the result string.
 *
 * @param data Pointer to the t_data structure.
 * @param result Pointer to the result string.
 * @param j Pointer to the current index in the result string.
 *
 * @return void
 *
 * @throws None
 */
void	handle_status_variable(t_data *data, char *result, t_index *num)
{
	size_t	k;
	char	*status_str;

	status_str = ft_itoa(data->status);
	if (status_str)
	{
		k = 0;
		while (status_str[k])
			result[num->j++] = status_str[k++];
		free_null(status_str);
	}
}

/**
 * Handles the dollar sign character in a string,
 * determining whether it's a special variable or a literal character.
 *
 * @param str The input string to process.
 * @param num A pointer to the t_index structure,
 * containing the current index in the string.
 * @param data A pointer to the t_data structure, containing the program's data.
 * @param result A pointer to the result string,
 * where the processed character will be appended.
 *
 * @return void
 */
void	dollar_sign(const char *str, t_index *num, t_data *data, char *result)
{
	if (str[num->i + 1] == ' ' || str[num->i + 1] == '\0')
		result[num->j++] = str[num->i++];
	else if (data->dbl_q && (str[num->i + 1] == '"' || str[num->i + 1] == '\''))
		result[num->j++] = str[num->i++];
	else if (str[num->i + 1] == '$')
	{
		num->i += 2;
		result[num->j++] = '$';
	}
	else if (str[num->i + 1] >= '0' && str[num->i + 1] <= '9')
		num->i += 2;
	else if (str[num->i + 1] == '?')
	{
		num->i += 2;
		handle_status_variable(data, result, num);
	}
	else
		handle_env_var(str, num, data, result);
}

/**
 * Expands environment variables in a given input string.
 *
 * This function takes a constant input string
 * and a t_data structure as parameters.
 * It allocates memory for the expanded result,
 * then iterates over the input string,
 * handling single and double quotes, and expanding environment variables.
 * If memory allocation fails, it returns NULL.
 *
 * @param input The input string to expand environment variables in.
 * @param data The t_data structure containing environment variable information.
 *
 * @return A dynamically allocated string containing the expanded input string,
 *         or NULL if memory allocation fails.
 */
char	*expand_env_variables(const char *input, t_data *data)
{
	char	*result;
	t_index	num;

	setup_env_variables(input, data);
	result = (char *)malloc(data->env_len * data->num_of_envs * sizeof(char));
	if (!result)
		return (NULL);
	num.i = 0;
	num.j = 0;
	while (input[num.i])
	{
		if (input[num.i] == '\'' && !data->dbl_q)
			single_q(input, data, result, &num);
		else if (input[num.i] == '"' && !data->s_quote_o)
			double_q(input, data, result, &num);
		else if (input[num.i] == '$' && !data->s_quote_o)
			dollar_sign(input, &num, data, result);
		else
			result[num.j++] = input[num.i++];
	}
	result[num.j] = '\0';
	return (result);
}
