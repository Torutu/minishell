/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 10:19:57 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/14 14:18:53 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * COMMAND LINE TO ARRAY
 * This function takes all tokens and organize them into elements of an array
 * so that we can distribute instructions to each child process individually.
 * 
 * E.g.:
 * *************** IN COMMAND LINE ********************
 * %> ls -la | grep Makefile | cat > outfile
 * ****************************************************
 * The array will take the instructions and organize them as follows:
 * pipe_array[0] = "ls -la"
 * pipe_array[1] = "grep Makefile"
 * pipe_array[2] = "cat > outfile"
 * 
 * ***NOTE: I dont think we need malloc checks after strdup, as its taken care of inside the function
 * 			that should save a few lines at least***
 */
char	**cl_to_array(t_token *token)
{
	t_token	*head;
	char	**pipe_array;
	char	*instruction;
	char	*tmp;
	int		i;
	int		nb_of_instructions;
	
	nb_of_instructions = count_token(token, PIPE) + 1;
	pipe_array = (char **)malloc(sizeof(char *) * (nb_of_instructions + 1));
	if (!pipe_array)
		return (NULL);
	i = 0;
	head = token;
	instruction = ft_strdup("");
	if (!instruction)
		return (NULL);
	while (head)
	{
		instruction[0] = '\0';
		while (head && head->type != PIPE)
		{
			tmp = ft_strjoin(instruction, head->value);
			if (!tmp)
				return (NULL);
			free(instruction);
			instruction = tmp;
 			tmp = ft_strjoin(instruction, " ");
			if (!tmp)
				return (NULL);
            free(instruction);
            instruction = tmp;
            head = head->next;
        }
        if (instruction[ft_strlen(instruction) - 1] == ' ')
            instruction[ft_strlen(instruction) - 1] = '\0';
        pipe_array[i++] = ft_strdup(instruction);
        if (!pipe_array[i - 1])
            return (NULL);
        if (head && head->type == PIPE)
            head = head->next;
    }
    free(instruction);
	instruction = NULL;
    pipe_array[i] = NULL;
    return (pipe_array);
}

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
 */
int	checking_access(t_data *data, char *instruction, int child)
{
	int		i;
	char	*binary_path;
	char	*binary;
	
	i = 0;
	binary = get_binary(instruction);
	(void)child;
	//dprintf(2, "\nThe binary in child %i is: %s\n\n", child, binary);//debug
	while (data->binary_paths[i++])
	{
		binary_path = ft_strsjoin(data->binary_paths[i], binary, '/');
		if (!access(binary_path, F_OK))
		{
			if(!access(binary_path, X_OK))
			{
				free(binary);
				free(binary_path);
				return (SUCCESS);
			}
			else
			{
				ft_putstr_fd(binary, 2);
				ft_putstr_fd(": ", 2);
				ft_putstr_fd("command not found\n", 2);
				free(binary);
				free(binary_path);
				return (FAILURE);
			}
		}
		else
			free(binary_path);
	}
	free(binary);
	return (FAILURE);
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
		dprintf(2, "\nwe got an empty binary at get_binary\n\n");
		free_array(split_instruction);
		return (NULL);
	}
	free_array(split_instruction);
	return (binary);
}
