/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 10:19:57 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/09/01 20:19:59 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void    print_args(char **cmd_a)
{
    int    i;

    i = 0;
    if (cmd_a == NULL)
    {
        dprintf(2, "cmd_a is NULL\n");
        return ;
    }
    while (cmd_a[i])
    {
        dprintf(2, "[%s]\n", cmd_a[i]);
        i++;
    }
}
/**
 * At this point we have an instruction that should follow this syntax:
 * 
 * "cmd -flag argument"
 * "cmd -flag"
 * "cmd argument"
 * "redirection infile flag"
 * "cmd redirection outfile"
 * 
 * All these cases will have spaces between them that were set manually in
 * cl_to_array function.
 * 
 * RETURN VALUES: the function returns the binary upon success. If allocation
 * fails at some point, it returns NULL and frees memory used in the function.
 */
// char	*get_binary(char *instruction)
// {
// 	char		**split_instruction;
// 	static char	*binary;

// 	split_instruction = ft_split(instruction, ' ');
// 	if (!split_instruction)
// 		return (NULL);
// 	if (!ft_strcmp(split_instruction[0], "<"))
// 		binary = ft_strdup(split_instruction[2]);
// 	else
// 		binary = ft_strdup(split_instruction[0]);
// 	if (!binary)
// 	{
// 		free_array(split_instruction);
// 		return (NULL);
// 	}
// 	free_array(split_instruction);
// 	return (binary);
// }

// t_token	*find_redtok(t_token *token)
// {
// 	t_token	*tmp;

// 	tmp = token;
// 	while (tmp)
// 	{
// 		if (find_token(token, RED_IN)
// 			|| find_token(token, RED_OUT)
// 			|| find_token(token, APPEND)
// 			|| find_token(token, HEREDOC))
// 			return (tmp);
// 		tmp = tmp->next;
// 	}
// 	tmp = NULL;
// 	return (NULL);
// }

char	**alloc_cmd_array(t_token *token)
{
	int cmd_size = 0;
	char **cmd_array;

	while (token && token->type != PIPE)
	{
		cmd_size++;
		token = token->next;
	}
	cmd_array = (char **)malloc(sizeof(char *) * (cmd_size + 1));
	if (!cmd_array)
		return (NULL);
	return (cmd_array);
}

/**
 * @brief Counts the number of PIPE tokens in a linked list of tokens.
 * @param token the first token in the list
 * @return the number of PIPE tokens in the list
 */
int	count_pipes(t_token *token)//trip
{
	int	count;

	count = 0;
	while (token)
	{
		if (token->type == PIPE)
			count++;
		token = token->next;
	}
	return (count);
}

char	***token_to_array(t_token *token)
{
	t_token	*head;
	char	***cmd_array;
	char	**cmd;
	int		i;
	int		j;

	i = 0;
	head = token;
	cmd_array = (char ***)malloc(sizeof(char **) * (count_pipes(token) + 2));
	if (!cmd_array)
		return (NULL);

	while (head->value)
	{
		cmd = alloc_cmd_array(head);
		if (!cmd)
			return (free_cmd_array(cmd_array));
		j = 0;
		while (head->value && head->type != PIPE)
		{
			cmd[j] = ft_strdup(head->value);
			if (!cmd[j])
				return (free_cmd_array(cmd_array));
			head = head->next;
			j++;
		}
		cmd[j] = NULL;
		cmd_array[i] = cmd;
		i++;

		if (head && head->type == PIPE)
			head = head->next;
	}
	cmd_array[i] = NULL;
	return (cmd_array);
}
