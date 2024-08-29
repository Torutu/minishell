/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 10:31:53 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/29 15:11:09 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Changes the current directory to the user's home directory.
 * @param data A pointer to the t_data structure containing the home directory
 * @return 0 on success, 1 on failure
 */
static int	change_to_home_directory(t_data *data)
{
	if (chdir(data->home_pwd) < 0)
		return (err_msg(data->home_pwd, FILE_ERROR, 1));
	return (SUCCESS);
}


/**
 * @brief Changes the current directory to the one given by the token's value,
 * which is expected to be a relative path.
 * @param token A pointer to the t_token structure containing the value to be
 * used as the new directory.
 * @return 0 on success, 1 on failure
 */
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

/**
 * @brief Changes the current directory to the one given by the token's value,
 * which is expected to be an absolute path.
 * @param token A pointer to the t_token structure containing the value to be
 * used as the new directory.
 * @return 0 on success, 1 on failure
 */
static int	change_to_absolute_directory(t_token *token)
{
	if (chdir(token->value) < 0)
		return (err_msg(token->value, FILE_ERROR, 1));
	return (SUCCESS);
}

/**
 * @brief Executes the cd command.
 *
 * If the user does not provide an argument, this function changes the current
 * directory to the home directory. If the user provides a relative path, this
 * function changes the current directory to the given path relative to the
 * current directory. If the user provides an absolute path, this function
 * changes the current directory to the given path.
 *
 * @param token A pointer to the t_token structure containing the value to be
 * used as the new directory.
 * @param data A pointer to the t_data structure containing the execution
 * context.
 *
 * @return 0 on success, 1 on failure
 */
int	shell_cd(t_token *token, t_data *data)
{
	t_token *arg_checker = token->next;
	int arg_count = 0;
	int result;
	if (!token->next || !token->next->value)
		return(result = change_to_home_directory(data));
	while (arg_checker->value && arg_checker->type != PIPE)
	{
		arg_count++;
		arg_checker = arg_checker->next;
	}
	if (arg_count > 1)
	{
		data->cd_executed = true;
		return (err_msg(NULL, CD_ERR, 1));
	}
	token = token->next;
	if (ft_strchr(token->value, '/') == NULL)
		result = change_to_relative_directory(token);
	else
		result = change_to_absolute_directory(token);
	if (result == SUCCESS)
	{
		data->cd_executed = true;
		return (SUCCESS);
	}
	else
	{
		data->cd_executed = true;
		return (result);
	}
}

// int shell_cd(t_token *token, t_data *data)
// {
//     t_token *arg_checker = token->next;
//     int arg_count = 0;
//     int result;

//     // Check if the next token is a redirection
//     if ((token->next->type == RED_IN ||
//         token->next->type == RED_OUT ||
//         token->next->type == APPEND ||
//         token->next->type == HEREDOC) &&
// 		data->ignore_redirections == false)
//     {
//         data->cd_executed = false;
// 		data->ignore_redirections = true;
//         return 0;
//     }

// 	if (!token->next || !token->next->value || data->ignore_redirections == true)
//     {
//         // If no argument or redirection, change to home directory
// 		dprintf(2, "dude\n");
// 		data->ignore_redirections = false;
//         result = change_to_home_directory(data);
//         if (result == SUCCESS)
//             data->cd_executed = true;
//         return result;
//     }

//     // Count the number of arguments
//     while (arg_checker->value && arg_checker->type != PIPE && 
//           (arg_checker->type != RED_IN && 
//            arg_checker->type != RED_OUT &&
//            arg_checker->type != APPEND &&
//            arg_checker->type != HEREDOC))
//     {
//         arg_count++;
//         arg_checker = arg_checker->next;
//     }
//     // Check if there are too many arguments
//     if (arg_count > 1)
//     {
//         data->cd_executed = true;
//         return err_msg(NULL, CD_ERR, 1);
//     }
// 	data->ignore_redirections = false;
//     // Perform the directory change
//     token = token->next;
//     if (ft_strchr(token->value, '/') == NULL)
//         result = change_to_relative_directory(token);
//     else
//         result = change_to_absolute_directory(token);

//     if (result == SUCCESS)
//         data->cd_executed = true;

//     return result;
// }
