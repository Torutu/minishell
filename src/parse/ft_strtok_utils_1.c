/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok_utils_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 00:32:24 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/25 01:22:00 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * Checks if a character exists in a given string.
 *
 * @param c The character to search for.
 * @param str The string to search in.
 *
 * @return 1 if the character is found, 0 otherwise.
 */
int	ft_charinstr(char c, const char *str)
{
	while (*str)
	{
		if (c == *str)
			return (1);
		str++;
	}
	return (0);
}

/**
 * Handles a quote character in the target string,
 * updating the data structure accordingly.
 * It checks if the current character is a single quote and sets the
 * in_quotes flag and quote_char variable in the data structure.
 * The function also increments the index in the data structure.
 *
 * @param target The string being processed.
 * @param data The data structure containing the current index and quote state.
 *
 * @return None
 */
void	handle_quote(const char *target, t_data *data)
{
	if (target[data->i] == '\'')
		data->in_quotes = 1;
	else
		data->in_quotes = 2;
	data->quote_char = target[data->i];
	data->i++;
}

/**
 * Extracts a substring from the target string based on the data structure's
 * current index and token start position, and then expands any environment
 * variables within the substring.
 *
 * @param target The string from which to extract the substring.
 * @param data   The data structure containing the current index and token
 * start position, as well as the environment variables to expand.
 *
 * @return The expanded substring, or NULL if an error occurs or no token is
 * found.
 */
char	*substr_and_expand(const char *target, t_data *data)
{
	if (data->in_quotes)
	{
		data->status = 963;
		return (NULL);
	}
	if (data->i == 0 && !data->in_quotes)
		return (NULL);
	data->ctoken = ft_substr(target, data->tok_srt, data->i - data->tok_srt);
	if (!data->ctoken)
		return (NULL);
	data->cnew_token = expand_env_variables(data->ctoken, data);
	if (data->cnew_token)
	{
		free_null(data->ctoken);
		data->ctoken = data->cnew_token;
	}
	else
	{
		free_null(data->ctoken);
		data->ctoken = NULL;
	}
	return (data->ctoken);
}

/**
 * Handles a quoted segment within a string, extracting the characters between
 * quotes.
 *
 * @param str The original string containing the quoted segment.
 * @param n_str The new string to store the extracted characters.
 * @param data A pointer to the data structure containing relevant information.
 * @param qt_c The quote character (' or ").
 */
void	quoted_segment(const char *str, char *n_str, t_data *data, char qt_c)
{
	data->indexx.i++;
	while (str[data->indexx.i] && str[data->indexx.i] != qt_c)
		n_str[data->indexx.j++] = str[data->indexx.i++];
	if (str[data->indexx.i] == qt_c)
		data->indexx.i++;
}

/**
 * Removes quotes from a given string, handling both single and double quotes.
 *
 * @param str The original string containing quotes to be removed.
 * @param data A pointer to the data structure containing relevant information.
 * @return A new string with quotes removed, or NULL if memory allocation fails.
 */
char	*remove_quotes(const char *str, t_data *data)
{
	char	*new_str;

	data->indexx.i = 0;
	data->indexx.j = 0;
	new_str = (char *)malloc(ft_strlen(str) + 1);
	if (!new_str)
		return (NULL);
	while (str[data->indexx.i])
	{
		if (str[data->indexx.i] == '"' || str[data->indexx.i] == '\'')
			quoted_segment(str, new_str, data, str[data->indexx.i]);
		else
			new_str[data->indexx.j++] = str[data->indexx.i++];
	}
	new_str[data->indexx.j] = '\0';
	return (new_str);
}
