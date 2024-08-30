/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 15:34:10 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/30 02:24:48 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Initializes the string tokenization data structure.
 *
 * @param data Pointer to the tokenization data structure.
 *
 * @return void
 *
 */
void	init_strtok(t_data *data)
{
	data->i = 0;
	data->tok_srt = 0;
	data->quote = 0;
	data->in_quotes = 0;
	data->quote_char = '\0';
}

/**
 * Tokenizes a string based on delimiters and quotes.
 *
 * @param str The input string to be tokenized.
 * @param data A pointer to the tokenization data structure.
 * @param cur_tok A pointer to the current token.
 *
 * @return A pointer to the tokenized string or NULL on failure.
 */
char	*ft_strtok(char *str, t_data *data, t_token *cur_tok)
{
	static char	*target;
	char		*token;

	token = NULL;
	if (str)
	{
		init_strtok(data);
		data->new_str = modify_str(str);
		target = data->new_str;
	}
	if (!target || skip_starting_delim(target, data->deli, &target) == NULL)
		return (NULL);
	process_quotes_n_deli(target, data);
	if (unmatched_quote_check(data) == 1)
	{
		data->status = 963;
		return (NULL);
	}
	token = substr_and_expand(target, data);
	if (!handle_tok(token, cur_tok, data, &target))
		return (NULL);
	free_null(token);
	return (data->fin_tok);
}
