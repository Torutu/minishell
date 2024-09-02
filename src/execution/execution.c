/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 10:58:07 by walnaimi          #+#    #+#             */
/*   Updated: 2024/09/03 02:30:01 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_only_builtins(t_token *tokens)
{
	while (tokens->value != NULL)
	{
		if (tokens->type == EXEC || tokens->type == PIPE
			|| tokens->type == RED_IN || tokens->type == RED_OUT
			|| tokens->type == HDOC || tokens->type == APPEND)
			return (false);
		tokens = tokens->next;
	}
	return (true);
}

int	execution(t_data *data, t_env **env_ll)
{
	t_token	*token;
	int		status;

	status = 0;
	token = data->token;
	if (token == NULL || token->value == NULL)
		return (status);
	data->nb_cmds = count_token(token, PIPE) + 1;
	if (data->nb_cmds == 0)
		data->nb_cmds = 1;
	if (is_only_builtins(token) == true)
		data->status = built_ins(data, token, env_ll, 0);
	else if (data->nb_cmds >= 1)
		data->status = trip_execution_prepping(data, data->token, env_ll);
	return (data->status);
}

int	trip_execution_prepping(t_data *data, t_token *token, t_env **env_ll)
{
	static pid_t	pids;
	static char		***cmd_a;

	cmd_a = token_to_array(token);
	if (!cmd_a)
		return (0);
	if (pipe(data->sync_pipe) == -1)
		return (err_msg(NULL, "Broken pipe\n", 141));
	data->status = tri_forking(data, env_ll, cmd_a, pids);
	close_fds(data);
	pids = wait(&data->status);
	while (pids > 0)
		pids = wait(&data->status);
	g_mod = 0;
	free_cmd_array(cmd_a);
	return (WEXITSTATUS(data->status));
}

t_token	*search_backwards_for_builtin(t_token *start, const char *cmd)
{
	t_token	*end;

	end = start;
	while (end && end->next != NULL)
		end = end->next;
	while (end != start)
	{
		if (end->type == BUILTIN
			&& !ft_strncmp(cmd, end->value, ft_strlen(cmd)))
			return (end);
		end = end->prev;
	}
	return (start);
}
