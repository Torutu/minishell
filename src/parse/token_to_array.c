/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_to_array.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 13:20:01 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/05 12:07:01 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*ft_strcpy(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i])
	{
		s1[i] = s2[i];
		i++;
	}
	s1[i] = '\0';
	return (s1);
}

/**
 * ttda - token to array w/ delimiter
 * 
 * USAGE: give the token and the delimiter. Delimiters: types of tokens.
 * Use '0' if you don't want to have a delimiter.
 * 
 * Types of tokens: COMMANDS, FLAGS, ARGUMENTS, REDIRECTS.
 */
char **ttad(t_token *token_list, t_type delimiter)
{
    int 		count;
    t_token 	*current;
	char		**array;
	int			length;
	int			index;
	char		*command;
	t_token 	*segment_start;
	int			i;
	int			pos;

	i = 0;
	pos = 0;
	length = 0;
	index = 0;
	count = 0;
	current = token_list;
    segment_start = current;
    while (current != NULL)
    {
        if (current->type == delimiter)
            count++;
        current = current->next;
    }
    count++;
    array = (char **)malloc(sizeof(char *) * (count + 1));
    if (array == NULL)
        return NULL;
    current = token_list;
    while (current != NULL)
    {
        while (current != NULL && current->type != delimiter)
        {
            if (current->type == COMMAND || current->type == FLAG || current->type == ARGUMENT)
            {
                length += ft_strlen(current->value) + 1;
            }
            current = current->next;
        }
        command = (char *)malloc(length);
        if (command == NULL)
        {
            i = 0;
			while (i < index)
			{
				free(array[i]);
				i++;
			}
			free(array);
            return (NULL);
        }
        command[0] = '\0';
        while (segment_start != current)
        {
            if (segment_start->type == COMMAND || segment_start->type == FLAG || segment_start->type == ARGUMENT)
            {
                ft_strcpy(command + pos, segment_start->value);
                pos += ft_strlen(segment_start->value);
                command[pos++] = ' ';
            }
            segment_start = segment_start->next;
        }
        if (pos > 0 && command[pos - 1] == ' ')
            command[pos - 1] = '\0';
        array[index++] = command;
        if (current != NULL)
            current = current->next;
    }
    array[index] = NULL;
    return array;
}

// char	**ttad(t_token *token_list, t_type delimiter)
// {
// 	int		count;
// 	int		index;
// 	char	**array;
// 	t_token	*current;

// 	count = 0;
// 	index = 0;
// 	current = token_list;
// 	while (current != NULL)
// 	{
// 		if (current->type == delimiter)
// 			break ;
// 		if (current->type == COMMAND || current->type == FLAG
// 			|| current->type == ARGUMENT)
// 			count++;
// 		current = current->next;
// 	}
// 	array = (char **)malloc(sizeof(char *) * (count + 1));
//     if (array == NULL)
// 		return NULL;
// 	current = token_list;
// 	while (current != NULL && current->type != delimiter)
// 	{
// 		if (current->type == COMMAND || current->type == FLAG
// 			|| current->type == ARGUMENT)
// 			array[index++] = current->value;
// 		current = current->next;
// 	}
// 	array[index] = NULL;
// 	return (array);
// }
