/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 10:53:41 by walnaimi          #+#    #+#             */
/*   Updated: 2024/09/02 21:13:29 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * Checks if the command is a built-in command and
 * executes it directly. Only one command can be
 * executed at a time.
 *
 * @param data		minishell data
 * @param token		token to be processed
 * @param env_ll	environment linked list
 * @return			status of the built-in command
 */
int	built_ins(t_data *data, t_token *token, t_env **env_ll, int child)
{
	int	status;

	status = 0;
	if (token->value == NULL)
		return (status);
	data->home_pwd = get_home((*env_ll));
	if (!ft_strncmp(token->value, "env", 4))
		status = print_env((*env_ll));
	else if (!ft_strncmp(token->value, "pwd", 4))
		status = print_pwd();
	else if (!ft_strncmp(token->value, "exit", 5))
		status = shut_down(data, token, env_ll, child);
	else if (!ft_strncmp(token->value, "echo", 5))
		status = yodeling(token);
	else if (!ft_strncmp(token->value, "cd", 3))
		status = shell_cd(token, data);
	else if (!ft_strncmp(token->value, "export", 7))
		status = export(token, env_ll);
	else if (!ft_strncmp(token->value, "unset", 6))
		status = unset(token, env_ll, data);
	else
		status = handle_non_builtin(token);
	data->status = status;
	return (status);
}

/**
 * Prints out the environment variables in the format
 * "VARIABLE_NAME=variable_value"
 *
 * @param env_ll	environment linked list
 * @return		status of the operation
 */
int	print_env(t_env *env_ll)
{
	t_env	*tmp;

	if (!env_ll)
		return (FAILURE);
	tmp = env_ll;
	while (tmp)
	{
		if (ft_strchr(tmp->content, '='))
			ft_putendl_fd(tmp->content, 1);
		tmp = tmp->next;
	}
	return (SUCCESS);
}

/**
 * Prints the current working directory.
 *
 * @return		status of the operation
 */
int	print_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (FAILURE);
	ft_putendl_fd(pwd, 1);
	free_null(pwd);
	return (SUCCESS);
}

/**
 * Converts the exit status from a string to an integer, performs cleanup,
 * and exits with the specified status.
 *
 * @param data		data structure holding the program state
 * @param env_ll	environment linked list
 * @param status_str	string containing the status to exit with
 */
int	exit_with_status(t_data *data, t_env **env_ll, const char *status_str)
{
	int	status;

	status = ft_atoi(status_str);
	free_before_exit(data, env_ll);
	return (status);
}

/**
 * Handles the exit built-in command. If the command is not provided with any
 * arguments, it just exits the shell.
 * If the command is provided with an argument,
 * it will exit the shell with the provided value as the exit status.
 *
 * @param data		data structure holding the program state
 * @param token		token list
 * @param env_ll	environment linked list
 * @return			status of the operation
 */
int	shut_down(t_data *data, t_token *token, t_env **env_ll, int child)
{
	int	status;

	if (token->next != NULL && token->next->value != NULL)
	{
		if (token->next->type == PIPE || child > 0)
			return (0);
		if (!ft_isnum_str(token->next->value))
		{
			printf("%s", SYNTAX_EXIT);
			free_before_exit(data, env_ll);
			exit(2);
		}
		else if (token->next->next->value != NULL)
		{
			printf("%s", EXIT_ERR);
			return (1);
		}
		printf("bye bye👋!\n");
		status = exit_with_status(data, env_ll, token->next->value);
		exit(status);
	}
	if (child == 0)
		printf("bye bye👋!\n");
	free_before_exit(data, env_ll);
	exit(data->status);
}
