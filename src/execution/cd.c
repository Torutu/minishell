/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 10:31:53 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/28 12:56:08 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	change_to_home_directory(t_data *data)
{
	if (chdir(data->home_pwd) < 0)
		return (err_msg(data->home_pwd, FILE_ERROR, 1));
	return (SUCCESS);
}

static int	change_to_relative_directory(t_token *token)
{
	static char	*curr_pwd;
	static char	*new_pwd;
	int			result;

	curr_pwd = getcwd(NULL, 0);
	if (!curr_pwd)
		return (err_msg(token->value, "Failed to get current directory\n", 1));
	new_pwd = ft_strsjoin(curr_pwd, token->value, '/');
	free(curr_pwd);
	if (!new_pwd)
		return (err_msg(NULL, MALLOC, -1));
	result = chdir(new_pwd);
	free(new_pwd);
	if (result < 0)
		return (err_msg(token->value, FILE_ERROR, 1));
	return (SUCCESS);
}

static int	change_to_absolute_directory(t_token *token)
{
	if (chdir(token->value) < 0)
		return (err_msg(token->value, FILE_ERROR, 1));
	return (SUCCESS);
}

int	shell_cd(t_token *token, t_data *data)
{
	if (!token->next || !token->next->value)
		return (change_to_home_directory(data));
	token = token->next;
	if (ft_strchr(token->value, '/') == NULL)
		return (change_to_relative_directory(token));
	else
		return (change_to_absolute_directory(token));
}
