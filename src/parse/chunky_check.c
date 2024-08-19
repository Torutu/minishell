/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunky_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:33:52 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/18 15:42:36 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_builtin_check(char *token, t_token *current_token)
{
	if(ft_strncmp(token, "echo", 5) == SUCCESS
	|| ft_strncmp(token, "exit", 5) == SUCCESS
	|| ft_strncmp(token, "pwd", 4) == SUCCESS
	|| ft_strncmp(token, "cd", 3) == SUCCESS
	|| ft_strncmp(token, "export", 7) == SUCCESS
	|| ft_strncmp(token, "unset", 6) == SUCCESS
	|| ft_strncmp(token, "env", 3) == SUCCESS)
		{
			current_token->value = ft_strdup(token);
			current_token->type = BUILTIN;
			return (SUCCESS);
		}
		return (FAILURE);
}

void print_binary_paths(t_data *data)
{
    for (int i = 0; data->binary_paths[i] != NULL; i++)
    {
        printf("Path %d: %s\n", i, data->binary_paths[i]);
    }
	if (!data->binary_paths)
    {
        printf("Binary paths are NULL\n");
        return;
    }
}

int ft_command_check(char *token, t_token *current_token, t_data *data)
{
    char **paths = ft_split(data->bin, ':');

    char *executable_path = loop_path_for_binary(token, paths);
    if (executable_path != NULL)
    {
        char *last_slash = ft_strrchr(executable_path, '/');// Find the last '/' character to separate the path and name
        if (last_slash)
        {
            int path_len = last_slash - executable_path + 1;
            current_token->path = ft_strndup(executable_path, path_len); // 2 -> check_command 28
            current_token->value = ft_strdup(last_slash + 1);
        }
        else
        {
            current_token->path = NULL;
            current_token->value = ft_strdup(executable_path);
        }
        current_token->type = COMMAND;
		free_my_boi(paths);
        free(executable_path);  // Free the allocated path
        return SUCCESS;
    }
	free_my_boi(paths);
    return FAILURE;
}

int ft_pipe_check(char *token, t_token *current_token)
{
		if (ft_strcmp(token,"|") == 0)
		{
			current_token->value = token;
			current_token->type = PIPE;
			return(SUCCESS);
		}
	return(FAILURE);
}

int	ft_redirect_op_check(char *token,t_token *current_token)
{
	if(ft_strncmp(token,">",2) == SUCCESS)
	{
		current_token->value = ft_strdup(token);
		current_token->type = RED_OUT;// > output
		return(SUCCESS);
	}
	else if (ft_strncmp(token,">>",3) == SUCCESS)
	{
		current_token->value = ft_strdup(token);
		current_token->type = APPEND;// >> output
		return(SUCCESS);
	}
	else if (ft_strncmp(token,"<",2) == SUCCESS)
	{
		current_token->value = ft_strdup(token);
		current_token->type = RED_IN;// < input
		return(SUCCESS);
	}
	else if (ft_strncmp(token,"<<",3) == SUCCESS)
	{
		current_token->value = ft_strdup(token);
		current_token->type = HEREDOC;// <<input
		return(SUCCESS);
	}
	return(FAILURE);
}

int	ft_argument_check(char *token, t_token *current_token)
{
	current_token->value = token;
	current_token->type = ARGUMENT;
	return SUCCESS;
}
