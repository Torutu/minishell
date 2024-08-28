/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_str_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 21:33:17 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/25 01:35:00 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Counts the number of special characters in a given string.
 *
 * @param str The input string.
 *
 * @return The number of special characters in the input string.
 *
 * @throws None.
 */
int	count_special_characters(const char *str)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '|')
			count += 2;
		else if (str[i] == '>' || str[i] == '<')
		{
			if (str[i + 1] == str[i])
			{
				count += 2;
				i++;
			}
			count += 2;
		}
		i++;
	}
	return (count);
}

/**
 * Copies at most n characters from the source string
 * s2 to the destination string s1.
 *
 * Parameters:
 *  @param s1 (char *): The destination string.
 *  @param s2 (const char *): The source string.
 *  @param n (int): The maximum number of characters to copy.
 *
 * Return Value:
 *  @return (char *): The destination string s1.
 */
char	*ft_strncpy(char *s1, const char *s2, int n)
{
	int	i;

	i = -1;
	while (++i < n && s2[i])
		s1[i] = s2[i];
	s1[i] = '\0';
	return (s1);
}

/**
 * Checks if a character is a whitespace character.
 *
 * Parameters:
 *  @param c (char): The character to check.
 *
 * Return Value:
 *  @return (int): 1 if the character is a whitespace character, 0 otherwise.
 */
int	ft_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r')
		return (1);
	return (0);
}

/**
 * Initializes a t_vars structure with default values.
 *
 * This function returns a t_vars structure where all members are set to 0.
 *
 * Parameters:
 *  @param None
 *
 * Return Value:
 *  @return A t_vars structure with all members initialized to 0.
 */
t_vars	init_vars(void)
{
	t_vars	vars;

	vars.i = 0;
	vars.j = 0;
	vars.single_q = 0;
	vars.double_q = 0;
	return (vars);
}
