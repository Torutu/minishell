/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 04:08:46 by walnaimi          #+#    #+#             */
/*   Updated: 2024/09/03 01:29:06 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Handles relative path token.
 *
 * @param token The token to be handled.
 * @param current_token The current token being processed.
 *
 * @return 0 if the token is a relative path token, 1 otherwise.
 */
int	handle_relative_path(char *token, t_token *current_token)
{
	if (ft_strncmp(token, "./", 2) == 0)
	{
		current_token->path = NULL;
		current_token->value = ft_strdup(token);
		free_null(token);
		current_token->type = EXEC;
		return (0);
	}
	return (1);
}

/**
 * Handles absolute path token.
 *
 * @param token The token to be handled.
 * @param current_token The current token being processed.
 *
 * @return 0 if the token is an absolute path token, 1 otherwise.
 */
int	handle_absolute_path(char *token, t_token *current_token)
{
	char	*last_slash;
	int		path_len;

	if (token[0] == '/')
	{
		last_slash = ft_strrchr(token, '/');
		if (last_slash)
		{
			path_len = last_slash - token + 1;
			current_token->path = ft_strndup(token, path_len);
			current_token->value = ft_strdup(token);
			free_null(token);
		}
		else
		{
			current_token->path = NULL;
			current_token->value = ft_strdup(token);
			free_null(token);
		}
		current_token->type = EXEC;
		return (0);
	}
	else if (handle_relative_path(token, current_token) == 0)
		return (0);
	return (1);
}

/**
 * Handles the case where the executable path contains a slash.
 *
 * @param exe_path The executable path.
 * @param token The token to be handled.
 * @param current_token The current token being processed.
 *
 * This function will split the executable path into a directory path and
 * a file name. The directory path will be assigned to the path field of the
 * current token, and the file name will be assigned to the value field. The
 * original token will be freed.
 */
void	handle_slash(char *exe_path, char *token, t_token *current_token)
{
	char	*last_slash;
	int		path_len;

	last_slash = ft_strrchr(exe_path, '/');
	if (last_slash)
	{
		path_len = last_slash - exe_path + 1;
		current_token->path = ft_strndup(exe_path, path_len);
		current_token->value = ft_strdup(last_slash + 1);
		free_null(token);
	}
}

/**
 * Handles the case where the executable path does not contain a slash.
 *
 * @param exe_path The executable path.
 * @param token The token to be handled.
 * @param current_token The current token being processed.
 *
 * This function will assign the executable path to the value field of the
 * current token, and set the path field to NULL. The original token will be
 * freed.
 */
void	handle_no_slash(char *exe_path, char *token, t_token *current_token)
{
	current_token->path = NULL;
	current_token->value = ft_strdup(exe_path);
	free_null(token);
}

/**
 * Handles the case where the executable path is an executable that can be
 * found in the environment's PATH.
 *
 * @param token The token to be handled.
 * @param current_token The current token being processed.
 * @param data The data structure containing the environment state.
 *
 * This function will split the executable path into a directory path and
 * a file name. The directory path will be assigned to the path field of the
 * current token, and the file name will be assigned to the value field. The
 * original token will be freed.
 *
 * If the executable path cannot be found in the environment's PATH, this
 * function will return 1 and the token will not be modified.
 *
 * @return 0 if the token is a valid executable path, 1 otherwise.
 */
int	handle_cmd_exe(char *token, t_token *current_token, t_data *data)
{
	char	**paths;
	char	*executable_path;
	char	*last_slash;

	paths = ft_split(data->bin, ':');
	executable_path = loop_path_for_binary(token, paths);
	if (executable_path != NULL)
	{
		last_slash = ft_strrchr(executable_path, '/');
		if (last_slash)
			handle_slash(executable_path, token, current_token);
		else
			handle_no_slash(executable_path, token, current_token);
		current_token->type = EXEC;
		free_my_boi(paths);
		free_null(executable_path);
		return (0);
	}
	free_my_boi(paths);
	return (1);
}
