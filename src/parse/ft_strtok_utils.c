/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 15:11:11 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/23 13:55:51 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Skips the starting delimiters in a string.
 *
 * This function takes a string, a delimiter string,
 * and a target pointer as input.
 * 
 * It returns a pointer to the first character
 * in the string that is not a delimiter.
 * 
 * If the string is empty or only contains delimiters, it returns NULL.
 *
 * @param str The input string.
 * @param delim The delimiter string.
 * @param target A pointer to a pointer that will be set to
 * the first non-delimiter character in the string.
 *
 * @return A pointer to the first non-delimiter character in the string,
 * or NULL if the string is empty or only contains delimiters.
 */
char	*skip_starting_delim(const char *str, const char *delim, char **target)
{
	if (str)
		*target = (char *)str;
	if (!*target || **target == '\0')
		return (NULL);
	while (**target && ft_charinstr(**target, delim))
	{
		(*target)++;
		if (**target == '\0')
			return (NULL);
	}
	return (*target);
}

/**
 * Removes quotes from the current token and skips any delimiters.
 *
 * @param delim A string of delimiter characters.
 * @param data  A pointer to the tokenization data structure.
 * @param target A pointer to a pointer to the current
 * position in the token string.
 *
 * @return None
 */
void	rm_quotes_and_skil_deli(const char *delim, t_data *data, char **target)
{
	data->ctoken = remove_quotes(data->ctoken, data);
	*target += data->i;
	if (data->fin_tok != NULL)
		free_null(data->fin_tok);
	data->fin_tok = ft_strdup(data->ctoken);
	free_null(data->ctoken);
	while (**target && ft_charinstr(**target, delim))
		(*target)++;
}

/**
 * Process quoting and delimiters in a target string.
 * 
 * This function iterates through the target string,
 * handling quoting and delimiters, it updates the data structure accordingly.
 * and tracks the current index and quote status.
 * 
 * @param target The target string to process.
 * @param delim The delimiter string.
 * @param data The data structure to update.
 * @param cur_tok The current token.
 * 
 * @return None
 */
void	process_quotes_n_deli(const char *target, t_data *data)
{
	data->i = 0;
	while (target[data->i])
	{
		if (data->in_quotes)
		{
			if (target[data->i] == data->quote_char)
			{
				data->in_quotes = 0;
				data->quote_char = '\0';
				data->i++;
				continue ;
			}
		}
		else if (target[data->i] == '"' || target[data->i] == '\'')
		{
			handle_quote(target, data);
			continue ;
		}
		else if (ft_charinstr(target[data->i], data->deli) && !data->in_quotes)
			break ;
		data->i++;
	}
}

/**
 * Checks if there are any unmatched quotes in the input data.
 *
 * @param data The input data structure to check for unmatched quotes.
 *
 * @return 1 if there are unmatched quotes, 0 otherwise.
 */
int	unmatched_quote_check(t_data *data)
{
	if (data->in_quotes)
	{
		printf("syntax error: unmatched quote 😳\n");
		return (1);
	}
	return (0);
}

/**
 * Handles tokenization of a given token string.
 * 
 * This function takes a token string, a token structure, a data structure, 
 * and a target string pointer as parameters. It checks if the token string is 
 * empty and frees the new string in the data structure if so. It then checks 
 * if the first character of the token string is a quote and sets the in_quotes 
 * flag in the token structure accordingly. Finally, it calls the 
 * rm_quotes_and_skil_deli function and returns 1 if successful, 0 otherwise.
 * 
 * @param tok The token string to handle.
 * @param c_t The token structure to update.
 * @param d The data structure containing the new string to free.
 * @param tgt The target string pointer.
 * 
 * @return 1 if successful, 0 if the token string is empty.
 */
int	handle_tok(char *tok, t_token *c_t, t_data *d, char **tgt)
{
	if (!tok)
	{
		free_null(d->new_str);
		return (0);
	}
	if (tok[0] == '\'' || tok[0] == '\"')
		c_t->in_q = true;
	else
		c_t->in_q = false;
	rm_quotes_and_skil_deli(d->deli, d, tgt);
	return (1);
}
