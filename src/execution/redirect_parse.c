/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_parse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 10:41:10 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/28 12:40:14 by walnaimi         ###   ########.fr       */
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
 * function returns NULL
 */
char	**parse_instruction(t_data *data, char **cmd_array)
{
	static int	index;
	static int	len;
	char		**parsed_array;

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
		free_array(cmd_array);
		return (NULL);
	}
	free_array(cmd_array);
	return (parsed_array);
}

static int	is_redirection(char *str)
{
	return (!ft_strcmp(str, "<")
		|| !ft_strcmp(str, ">")
		|| !ft_strcmp(str, ">>")
		|| !ft_strcmp(str, "<<"));
}

static int	is_executable(char *str, t_data *data)
{
	return (check_bin_local(str) == EXECUTABLE
		|| check_bin_path(str, data->binary_paths) == EXECUTABLE);
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
char	**remove_redirect(t_data *data, char **array, int len)
{
	char	**parsed_array;
	int		i_a;
	int		i_b;

	i_a = 0;
	i_b = 0;
	parsed_array = (char **)malloc(sizeof(char *) * (len + 1));
	if (!parsed_array)
		return (NULL);
	while (array[i_a])
	{
		if (is_redirection(array[i_a]))
		{
			i_a += 2;
			continue ;
		}
		if (is_executable(array[i_a], data) || i_a == 0
			|| !is_redirection(array[i_a - 1]))
			parsed_array[i_b++] = ft_strdup(array[i_a]);
		i_a++;
	}
	parsed_array[i_b] = NULL;
	return (parsed_array);
}
