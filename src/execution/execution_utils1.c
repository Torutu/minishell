/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 15:29:42 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/18 15:41:24 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*access_path(char **path, char *cmd)
{
	int		i;
	char	*curr_path;

	i = 0;
	while (path[i])
	{
		curr_path = ft_strsjoin(path[i], cmd, '/');
		if (!access(curr_path, F_OK))
		{
			if (!access(curr_path, X_OK))
			{
				free_array(path);
				return (curr_path);
			}
			ft_putstr_fd("Command not found: ", 2);
			ft_putendl_fd(cmd, 2);
		}
		free(curr_path);
		i++;
	}
	return (NULL);
}

// int	how_many_children(t_token *token) DEPRECATED
// {
// 	int		cmds;
// 	t_token	*tmp;

// 	cmds = 0;
// 	tmp = token;
// 	while (tmp != NULL)
// 	{
// 		if (tmp->type == COMMAND)
// 			cmds++;
// 		tmp = tmp->next;
// 	}
// 	tmp = NULL;
// 	return (cmds);
// }

/**
* This is our standard error printer.
* NEVER NULL THE FIRST PARAMETER!
*/
int	err_msg(char *obj, char *msg, int err_code)
{
	if (*obj && *msg)
		printf("%s: %s\n", obj, msg);
	else if (!obj && *msg)
		printf("%s\n", msg);
	return (err_code);
}

void	close_fds(t_data *data)
{
    if (data->pipe_fd[0] != 0)
        close(data->pipe_fd[0]);
    if (data->pipe_fd[1] != 0)
        close(data->pipe_fd[1]);
    if (data->fd_in != 0)
        close(data->fd_in);
    if (data->fd_out != 0)
        close(data->fd_out);
    if (data->read_end != 0)
        close(data->read_end);
}
