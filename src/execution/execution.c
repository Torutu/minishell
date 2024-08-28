/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:58:07 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/28 12:16:20 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// this function turns space into underscore,
// better used after echo and before cmd
void	replace_spaces_with_underscores(t_token *token_list)
{
	t_token	*current_token;
	int		i;

	current_token = token_list;
	while (current_token != NULL && current_token->value != NULL)
	{
		current_token->value_us = ft_strdup(current_token->value);
		if (!current_token->value_us)
			exit(err_msg(NULL, "Memory allocation failed", 1));
		i = 0;
		while (current_token->value_us[i] != '\0')
		{
			if (current_token->value_us[i] == ' ')
				current_token->value_us[i] = '_';
			i++;
		}
		current_token = current_token->next;
	}
}

/**
 * Execution and execution prepping are just the same function broke
 * into two parts for norminetting reasons. These functions mainly deal
 * with the general execution logic: separating built-ins execution from
 * all the other executions (piped, piped with redirections, just
 * redirection...). In here we fork and make the parent wait for execution
 * of the children. Another important part of it is the cl_to_array() that
 * turns our tokens into a 
 */
int	execution(t_data *data, t_env **env_ll)
{
	t_token	*token;

	token = data->token;
	if (token->value == NULL)
		return (0);
	data->nb_cmds = count_token(token, PIPE) + 1;
	if (data->nb_cmds == 0)
		data->nb_cmds = 1;
	if (!ft_strncmp(token->value, "cd", 3)
		|| !ft_strncmp(token->value, "export", 7)
		|| !ft_strncmp(token->value, "unset", 6)
		|| !ft_strncmp(token->value, "exit", 5))
		data->status = built_ins(data, token, env_ll);
	else
	{
		replace_spaces_with_underscores(token);
		data->status = execution_prepping(data, token, env_ll);
	}
	return (data->status);
}

int	execution_prepping(t_data *data, t_token *token, t_env **env_ll)
{
	static pid_t	pids;
	static char		**cmd_a;

	cmd_a = cl_to_array(token);
	if (!cmd_a)
		return (0);
	if (pipe(data->sync_pipe) == -1)
		return (err_msg(NULL, "Broken pipe\n", 141));
	data->status = forking(data, env_ll, cmd_a, pids);
	close_fds(data);
	pids = wait(&data->status);
	while (pids > 0)
		pids = wait(&data->status);
	g_exit_code = 0;
	free_array(cmd_a);
	return (WEXITSTATUS(data->status));
}
