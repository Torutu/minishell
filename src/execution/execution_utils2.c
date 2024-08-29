/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 10:19:57 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/29 11:22:31 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	fill_instr_loop(char **instruction, t_token **head)
{
	char	*tmp;

	tmp = NULL;
	(*instruction)[0] = '\0';
	while ((*head) && (*head)->type != PIPE)
	{
		tmp = ft_strjoin(*instruction, (*head)->value_us);
		free(*instruction);
		if (!tmp)
			return (FAILURE);
		*instruction = tmp;
		tmp = ft_strjoin(*instruction, " ");
		free(*instruction);
		if (!tmp)
			return (FAILURE);
		*instruction = tmp;
		if (!(*head)->next || !(*head)->next->value)
			break ;
		(*head) = (*head)->next;
	}
	if ((*instruction)[ft_strlen(*instruction) - 1] == ' ')
		(*instruction)[ft_strlen(*instruction) - 1] = '\0';
	return (SUCCESS);
}

// int	alloc_memory(char ***pipe_array, char **instruction, t_token **token)
// {
// 	int	nb_of_instructions;

// 	nb_of_instructions = count_token((*token), PIPE) + 1;
// 	(*pipe_array) = (char **)malloc(sizeof(char *) * (nb_of_instructions + 1));
// 	if (!(*pipe_array))
// 		return (FAILURE);
// 	(*instruction) = ft_strdup("");
// 	if (!instruction)
// 	{
// 		free_null((*pipe_array));
// 		pipe_array = NULL;
// 		return (FAILURE);
// 	}
// 	return (SUCCESS);
// }

/** checking_access() is mainly a last check for general binaries that
 * the original shell uses. If the user inputs a binary of his own making
 * it will fail this check, since it uses the paths that were stablished
 * in the environment variable.
 * 
 * USAGE: pass the struct data and the instruction and it will find the
 * binary by itself and check for existence (F_OK) and executability (X_OK)
 * 
 * RETURN VALUES: checking_access() either returns SUCCESS or FAILURE. If
 * FAILURE is returned, it means that your binary cannot be found in the
 * general concatenated paths in the environment pointers.
 */ // DEPRECATED
// int	checking_access(t_data *data, char *instruction)
// {
// 	int		i;
// 	char	*binary_path;
// 	char	*binary;

// 	i = 0;
// 	binary = get_binary(instruction);
// 	while (data->binary_paths[i])
// 	{
// 		binary_path = ft_strsjoin(data->binary_paths[i++], binary, '/');
// 		if (!access(binary_path, F_OK))
// 		{
// 			if (!access(binary_path, X_OK))
// 			{
// 				free(binary);
// 				return (free_retstatus(binary_path, SUCCESS));
// 			}
// 			ft_putstr_fd(binary, 2);
// 			ft_putstr_fd(": command not found\n", 2);
// 			free(binary);
// 			return (free_retstatus(binary_path, FAILURE));
// 		}
// 		free(binary_path);
// 	}
// 	free(binary);
// 	return (FAILURE);
// }

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
char	*get_binary(char *instruction)
{
	char		**split_instruction;
	static char	*binary;

	split_instruction = ft_split(instruction, ' ');
	if (!split_instruction)
		return (NULL);
	if (!ft_strcmp(split_instruction[0], "<"))
		binary = ft_strdup(split_instruction[2]);
	else
		binary = ft_strdup(split_instruction[0]);
	if (!binary)
	{
		free_array(split_instruction);
		return (NULL);
	}
	free_array(split_instruction);
	return (binary);
}

t_token	*find_redtok(t_token *token)
{
	t_token	*tmp;

	tmp = token;
	while (tmp)
	{
		if (find_token(token, RED_IN)
			|| find_token(token, RED_OUT)
			|| find_token(token, APPEND)
			|| find_token(token, HEREDOC))
			return (tmp);
		tmp = tmp->next;
	}
	tmp = NULL;
	return (NULL);
}

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
