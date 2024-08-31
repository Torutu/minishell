/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 10:53:41 by walnaimi          #+#    #+#             */
/*   Updated: 2024/09/01 00:05:15 by walnaimi         ###   ########.fr       */
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
int	built_ins(t_data *data, t_token *token, t_env **env_ll)
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
		status = get_the_hell_out(data, token, env_ll);
	else if (!ft_strncmp(token->value, "echo", 5))
		status = yodeling(data->token);
	else if (!ft_strncmp(token->value, "cd", 3))
		status = shell_cd(token, data);
	else if (!ft_strncmp(token->value, "export", 7))
		status = export(token, env_ll);
	else if (!ft_strncmp(token->value, "unset", 6))
		status = unset(token, env_ll, data);
	else
		return (err_msg(token->value, NO_EXEC, 127));
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
			ft_putendl_fd(tmp->content, 1);//ft_printf("%s\n", tmp->content);
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
	ft_putendl_fd(pwd, 1);//printf("%s\n", pwd);
	free_null(pwd);
	return (SUCCESS);
}

void	free_before_exit(t_data *data, t_env **env_ll)
{
	free_gang(data);
	free_all_ll(env_ll);
}

/**
 * Handles the exit built-in command. If the command is not provided with any
 * arguments, it just exits the shell. If the command is provided with an argument,
 * it will exit the shell with the provided value as the exit status.
 *
 * @param data		data structure holding the program state
 * @param token		token list
 * @param env_ll	environment linked list
 * @return			status of the operation
 */
int	get_the_hell_out(t_data *data, t_token *token, t_env **env_ll)
{
	int	status;

	if (token->next != NULL && token->next->value != NULL)
	{
		if(token->next->type == PIPE)
			return(0);
		if (!ft_isnum_str(token->next->value))
		{
			printf("%s" ,SYNTAX_EXIT);
			free_before_exit(data, env_ll);
			exit(2);
		}
		else if (token->next->next->value != NULL)
		{
			printf("%s" ,EXIT_ERR);
			return (1);
		}
		printf("bye bye👋!\n");
		status = ft_atoi(token->next->value);
		free_before_exit(data, env_ll);
		exit(status);
	}
	printf("bye bye👋!\n");
	free_before_exit(data, env_ll);
	exit(data->status);
}
