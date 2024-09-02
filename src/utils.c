/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 11:23:58 by walnaimi          #+#    #+#             */
/*   Updated: 2024/09/03 02:33:24 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Frees the shell most used data allocators.
 * USAGE: pass the function the arguments it asks, otherwhise use NULL.
 * data: main data struct for the shell;
 * path: defined and allocated in the execution part;
 * env: our linked list that works as our environment pointer;
 * command_array: variable linked to the execution of single/multiple processes.
 */
void	free_data(t_data *data, char *path, char **command_array)
{
	if (data->binary_paths)
		free_array(data->binary_paths);
	if (data->env)
		free_array(data->env);
	if (command_array)
		free_array(command_array);
	if (path)
	{
		free(path);
		path = NULL;
	}
}

// void	free_token(t_token *token)
// {
// 	t_token	*tmp;

// 	while (token != NULL)
// 	{
// 		tmp = token;
// 		token = token->next;
// 		free(tmp);
// 		tmp = NULL;
// 	}
// 	free(token);
// 	token = NULL;
// }

/**
 * check_bin_local() searches for a binary in the current working
 * directory. 
 * 
 * RETURN VALUES: Upon completion, it returns either EXECUTABLE, FILE or,
 * FAILURE. FILE means that your binary is actually a file; EXECUTABLE
 * means that it is a binary; and FAILURE means that the current directory
 * holds no such file.
 */
int	check_bin_local(char *binary)
{
	char		*binary_with_path;
	static char	*cwd;

	cwd = getcwd(NULL, 0);
	binary_with_path = ft_strsjoin(cwd, binary, '/');
	if (!access(binary_with_path, F_OK))
	{
		if (!access(binary_with_path, X_OK))
		{
			free_null(binary_with_path);
			free_null(cwd);
			return (EXECUTABLE);
		}
		free_null(cwd);
		free_null(binary_with_path);
		return (FILE);
	}
	free_null(cwd);
	free_null(binary_with_path);
	return (FAILURE);
}

/** check_bin_path() does the same work as check_bin_local()
 * but it goes throught the variable PATH= from env for that. One can
 * access this variable in the environment pointer (env_ll) or in the
 * data struct as BINARY_PATHS.
 * 
 * RETURN VALUES: Upon completion, it returns either EXECUTABLE, FILE or,
 * FAILURE. FILE means that your binary is actually a file; EXECUTABLE
 * means that it is a binary; and FAILURE means that the current directory
 * holds no such file.
 */
int	check_bin_path(char *binary, char **paths)
{
	char	*token_with_path;
	int		i;

	i = 0;
	while (paths && paths[i])
	{
		token_with_path = ft_strsjoin(paths[i], binary, '/');
		if (!access(token_with_path, F_OK))
		{
			if (!access(token_with_path, X_OK))
			{
				free_null(token_with_path);
				return (EXECUTABLE);
			}
			free_null(token_with_path);
			return (FILE);
		}
		free_null(token_with_path);
		i++;
	}
	return (FAILURE);
}
