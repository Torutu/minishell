/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_parse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 10:41:10 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/14 09:59:34 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Its necessary to know which redirection we have here and give back the
 * array organized in the usual fashion of "cmd -flag" for execution. We
 * assume that the index should start at 2 because of the syntax:
 * %> "< infile cat"
 * 
 * For redirect for output, we already have at the index 0 our commands, and
 * the flags will follow as far as we hit the redirect itself. 
 * 
 * The first and second elements of the instruction, inside array_instruction,
 * MUST be the redirect and file by consequence of our parsing. At this point
 * we are working with purely validated inputs.
 * 
 * Return values: upon success, this function will return an array with only
 * the commands that will be used in execve(). In case of any failures, the
 * function returns NULL.dprintf(2, "we got here\n");
 */
char	**parse_instruction(t_data *data, char **cmd_array)
{
	int	index;
	int	len;
	char **parsed_array;
	
	index = 0;
	len = 0;
	while (cmd_array[index])
	{
		if (!ft_strcmp(cmd_array[index], ">") 
			|| !ft_strcmp(cmd_array[index], "<")
			|| !ft_strcmp(cmd_array[index], "<<")
			|| !ft_strcmp(cmd_array[index], ">>"))
		{
			if (cmd_array[index + 2])
				index += 2;
		}
		len++;
		index++;
	}
	parsed_array = remove_redirect(data, cmd_array, len);
	if (!parsed_array)
	{
		dprintf(2, "\n\n received null parsed_array in parse_instruction \n\n");
		free_array(cmd_array);
		return (NULL);
	}
	free_array(cmd_array);
	return (parsed_array);
}

/**
 * This function is responsible for taking out the redirection character
 * of the whole array, leaving just command, flags and arguments.
 * 
 * we are getting the whole array and we need to take out the 
 * redirection and change order of stuff if it is an output redirection.
 * Otherwhise we will feed the wrong arguments.
 * 
 * The first iteration (first while loop) will try to grab the executable
 * from the line given, if any if there.
 */
char	**remove_redirect(t_data *data, char **array, int len) {
    static char *cwd;
    char **parsed_array;
    int index_a;
    int index_b;

    parsed_array = (char **)malloc(sizeof(char *) * (len + 1));
    if (!parsed_array)
        return NULL;
    
    cwd = getcwd(NULL, 0);
    index_b = 0;
    index_a = 0;

    while (array[index_a])
	{
		if (!strcmp(array[index_a], "<") || !strcmp(array[index_a], ">")
			|| !ft_strcmp(array[index_a], ">>") || !ft_strcmp(array[index_a], "<<"))
		{
			index_a += 2;
			continue ;
		}
		if (check_bin_local(array[index_a]) == EXECUTABLE || check_bin_path(array[index_a], data->binary_paths) == EXECUTABLE)
		{
			parsed_array[index_b] = ft_strdup(array[index_a]);
			index_b++;
		}
		else if (index_a == 0 || (ft_strcmp(array[index_a - 1], "<") != 0 && ft_strcmp(array[index_a - 1], ">") != 0 && ft_strcmp(array[index_a - 1], ">>") != 0))
		{
			parsed_array[index_b] = ft_strdup(array[index_a]);
			index_b++;
		}
        index_a++;
    }
    parsed_array[index_b] = NULL;
    free(cwd);
    return parsed_array;
}

/*************************************************************
 ************************* DUMP ******************************
 *************************************************************/

// char	**parse_instruction(t_data *data, char **cmd_array)
// {
// 	int	index;
// 	int	len;
// 	char **parsed_array;
	
// 	index = 0;
// 	len = 0;
// 	while (cmd_array[index])
// 	{
// 		if (!ft_strcmp(cmd_array[index], ">") || !ft_strcmp(cmd_array[index], "<"))
// 		{
// 			if (cmd_array[index + 2])
// 				index += 2;
// 		}
// 		len++;
// 		index++;
// 	}
// 	parsed_array = remove_redirect(data, cmd_array, len);
// 	if (!parsed_array)
// 	{
// 		dprintf(2, "\n\n received null parsed_array in parse_instruction \n\n");
// 		free_array(cmd_array);
// 		return (NULL);
// 	}
// 	free_array(cmd_array);
// 	return (parsed_array);
// }

// char	**remove_redirect(t_data *data, char **array, int len)
// {
// 	static char	*cwd;
// 	char		**parsed_array;
// 	int			index_a;
// 	int			index_b;

// 	parsed_array = (char **)malloc(sizeof(char *) * (len + 1));
// 	if (!parsed_array)
// 		return (NULL);
// 	cwd = getcwd(NULL, 0);
// 	index_b = 0;
// 	index_a = 0;
// 	while (array[index_a])
// 	{
// 		if (check_bin_local(array[index_a]) == EXECUTABLE
// 			|| check_bin_path(array[index_a], data->binary_paths) == EXECUTABLE) // need to check absolute path later
// 		{
// 			parsed_array[index_b] = ft_strdup(array[index_a]);
// 			index_b++;
// 		}
// 		index_a++;
// 	}
// 	parsed_array[index_b] = NULL;
// 	free(cwd);
// 	return (parsed_array);
// }