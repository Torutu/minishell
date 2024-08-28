/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 14:42:40 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/25 00:33:57 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Adds spaces around a single character in a string if necessary.
 *
 *  It checks the character before and after the current character in the string.
 * 
 *  If the previous character is not a space
 *  and the current character is not the first,
 *  it adds a space before the character.
 * 
 *  If the next character is not a space
 *  and the current character is not the last,
 *  it adds a space after the character.
 *
 * Parameters:
 *  @param str (char *): The original string.
 *  @param n_str (char *): The new string with added spaces.
 *  @param i (int *): The index of the current character in the string.
 *  @param j (int *): The index of the current character in the new string.
 */
void	add_spaces_s_c(char *str, char *n_str, int *i, int *j)
{
	int	len;

	len = ft_strlen(str);
	if (*i > 0 && !ft_isspace(str[*i - 1]))
		n_str[(*j)++] = ' ';
	n_str[(*j)++] = str[*i];
	if (*i < len - 1 && !ft_isspace(str[*i + 1]))
		n_str[(*j)++] = ' ';
}

/**
 * Adds spaces around a double character in a string if necessary.
 *
 * It checks the character before and after the current character in the string.
 * 
 * If the previous character is not a space and the
 * current character is not the first,
 * it adds a space before the double character.
 * 
 * If the next character is not a space and the
 * current character is not the last,
 * it adds a space after the double character.
 *
 * Parameters:
 *  @param str (char *): The original string.
 *  @param n_str (char *): The new string with added spaces.
 *  @param i (int *): The index of the current character in the string.
 *  @param j (int *): The index of the current character in the new string.
 *
 * Returns:
 *  None
 */
void	add_spaces_d_c(char *str, char *n_str, int *i, int *j)
{
	int	len;

	len = ft_strlen(str);
	if (*i > 0 && !ft_isspace(str[*i - 1]))
		n_str[(*j)++] = ' ';
	n_str[(*j)++] = str[*i];
	n_str[(*j)++] = str[*i];
	if (*i < len - 2 && !ft_isspace(str[*i + 2]))
		n_str[(*j)++] = ' ';
	(*i)++;
}

/**
 * Processes a character in a string, handling quote states and adding 
 * spaces around certain characters.
 *
 * Parameters:
 *  @param str (char *): The original string.
 *  @param n_str (char *): The new string with added spaces.
 *  @param pos (t_vars *): A structure containing the current position 
 *                        in the string and the quote states.
 */
void	process_chars(char *str, char *n_str, t_vars *pos)
{
	if (str[pos->i] == '\'' && !(pos->double_q))
		pos->single_q = !(pos->single_q);
	else if (str[pos->i] == '"' && !(pos->single_q))
		pos->double_q = !(pos->double_q);
	if (!(pos->single_q) && !(pos->double_q))
	{
		if (str[pos->i] == '|')
			add_spaces_s_c(str, n_str, &pos->i, &pos->j);
		else if (str[pos->i] == '>' && str[pos->i + 1] == '>')
			add_spaces_d_c(str, n_str, &pos->i, &pos->j);
		else if (str[pos->i] == '<' && str[pos->i + 1] == '<')
			add_spaces_d_c(str, n_str, &pos->i, &pos->j);
		else if (str[pos->i] == '>')
			add_spaces_s_c(str, n_str, &pos->i, &pos->j);
		else if (str[pos->i] == '<')
			add_spaces_s_c(str, n_str, &pos->i, &pos->j);
		else
			n_str[pos->j++] = str[pos->i];
	}
	else
		n_str[pos->j++] = str[pos->i];
}

/**
 * Iterates over the input string `str` and processes special characters
 * until the end of the string is reached.
 * 
 * @param str The input string to process.
 * @param n_str The new string to store the processed characters.
 * @param len The length of the input string.
 * @param pos A pointer to the struct containing the current indices
 * and quote flags.
 * 
 * @return None
 */
void	loop(char *str, char *n_str, int len, t_vars *pos)
{
	while (pos->i < len && str[pos->i] != '\0')
	{
		process_chars(str, n_str, pos);
		pos->i++;
	}
}

/**
 * Modifies the input string by processing special characters and returns
 * the modified string.
 * 
 * @param str The input string to be modified.
 * 
 * @return A newly allocated string with the modifications applied.
 */
char	*modify_str(char *str)
{
	int		len;
	int		special_count;
	int		new_len;
	char	*new_str;
	t_vars	vars;

	len = ft_strlen(str);
	special_count = count_special_characters(str);
	new_len = 2 * special_count + 2 * len;
	new_str = malloc(new_len + 1 * sizeof(char));
	if (!new_str)
		exit(EXIT_FAILURE);
	vars = init_vars();
	loop(str, new_str, len, &vars);
	new_str[vars.j] = '\0';
	return (new_str);
}
