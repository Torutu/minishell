/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 03:25:26 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/23 22:42:05 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Compares two strings character by character until a mismatch is found.
 * 
 * @param s1 The first string to compare.
 * @param s2 The second string to compare.
 * 
 * @return A negative integer if s1 is less than s2, 
 *         zero if s1 is equal to s2, and a positive integer if s1 is 
 *         greater than s2.
 */
int	ft_strccmp(char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

/**
 * Checks if a given key exists in a linked list of environment variables.
 * 
 * @param head A pointer to the head of the linked list of environment variables.
 * @param word The key to be searched for in the linked list.
 * 
 * @return 1 if the key is found, 0 otherwise.
 */
int	is_key_in_list(t_env *head, const char *word)
{
	while (head != NULL)
	{
		if (head->key != NULL && ft_strccmp(head->key, word) == 0)
		{
			return (1);
		}
		head = head->next;
	}
	return (0);
}

/**
 * Extracts a word from a given string pointer.
 *
 * @param ptr Pointer to the string from which to extract the word.
 * @param max_word_size Maximum size of the word to be extracted.
 *
 * @return A dynamically allocated string containing the extracted word.
 *
 * @throws None.
 */
char	*extract_word(const char **ptr, int max_word_size)
{
	int		index;
	char	*word;

	word = (char *)malloc((max_word_size + 1) * sizeof(char));
	index = 0;
	if (!word)
		return (NULL);
	while (**ptr != '\0' && !isspace(**ptr) && **ptr != '\'' && **ptr != '"')
	{
		if (index < max_word_size)
			word[index++] = **ptr;
		(*ptr)++;
	}
	word[index] = '\0';
	return (word);
}

void	skip_non_word_characters(const char **ptr)
{
	while (**ptr != '\0'
		&& (ft_isspace(**ptr) || **ptr == '\'' || **ptr == '"'))
		(*ptr)++;
}

/**
 * Counts the number of keys in the environment
 * list that match the words in the input string.
 *
 * @param env_list The list of environment variables to search in.
 * @param input    The input string containing words
 * to match against the environment list.
 *
 * @return The number of matching keys found in the environment list.
 */
int	count_matching_keys(t_env *env_list, const char *input)
{
	int			count;
	int			max_word_size;
	char		*word;
	const char	*ptr;

	count = 0;
	max_word_size = ft_strlen(input);
	ptr = input;
	while (*ptr != '\0')
	{
		skip_non_word_characters(&ptr);
		if (*ptr == '$')
			ptr++;
		word = extract_word(&ptr, max_word_size);
		if (word)
		{
			if (ft_strlen(word) > 0 && is_key_in_list(env_list, word))
				count++;
			free_null(word);
		}
	}
	return (count);
}
