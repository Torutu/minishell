/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:18:24 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/27 15:38:28 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* This part is taking the tokens and taking the argument just after it. */
int	built_ins(t_data *data, t_token *token, t_env **env_ll)
{
	int	status;

	status = 0;
	if (token->value == NULL)
		return (status);
	data->home_pwd = get_home((*env_ll));
	if (token->value == NULL)
		return (status);
	if (!ft_strncmp(token->value, "env", 4))
		status = print_env((*env_ll));
	else if (!ft_strncmp(token->value, "pwd", 4))
		status = print_pwd();
	else if (!ft_strncmp(token->value, "exit", 5))
	{
		status = get_the_hell_out(data, token, env_ll);
	}
	else if (!ft_strncmp(token->value, "echo", 5))
		status = yodeling(data->token);
	else if (!ft_strncmp(token->value, "cd", 3))
		status = shell_cd(token, data);
	else if (!ft_strncmp(token->value, "export", 7))
		status = export(token, env_ll);
	else if (!ft_strncmp(token->value, "unset", 6))
		status = unset(token, env_ll);
	else
		return (err_msg(token->value, NO_EXEC, 127));
	return (status);
}

/* The printing of the environment changes in conformity to the use of
export and unset. The command 'env' itself does not take arguments.
e.g. $> env || $> pwd (no white spaces or anything like caps)*/
int	print_env(t_env *env_ll)
{
	t_env	*tmp;

	if (!env_ll)
		return (FAILURE);
	tmp = env_ll;
	while (tmp)
	{
		ft_printf("%s\n", tmp->content);
		tmp = tmp->next;
	}
	env_ll = tmp;
	return (SUCCESS);
}

int	print_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (FAILURE);
	printf("%s\n", pwd);
	free_null(pwd);
	return (SUCCESS);
}

/* This is the exit function, it needs to take, if inputted,
an exit code that was manually inserted after exit */
int	get_the_hell_out(t_data *data, t_token *token, t_env **env_ll)
{
	int	status;

	status = 0;
	free_all_ll(env_ll);
	ft_printf("exit\n");
	if (token->next != NULL && token->next->value != NULL)
	{
		if (ft_isalpha_str(token->next->value))
		{
			status = 2;
			err_msg(token->next->value, SYNTAX_EXIT, status);
			free_gang(data);
			exit(status);
		}
		status = ft_atoi(token->next->value);
		free_gang(data);
		dprintf(2,"exit status is = %d\n", status);
		exit(status);
	}
	free_gang(data);
	exit(data->status);
}

int	handle_flag_type(t_token *head)
{
	head = head->next;
	while (head->type == FLAG)
	{
		head = head->next;
		if (head->value == NULL)
			return (SUCCESS);
	}
	while (head->value != NULL && head->value[0] == '\0')
		head = head->next;
	while (head != NULL)
	{
		if (head->value != NULL && head->value[0] != '\0')
		{
			if (head->type == RED_IN || head->type == RED_OUT
				|| head->type == APPEND || head->type == HEREDOC)
				break ;
			printf("%s", head->value);
		}
		head = head->next;
		if (head != NULL && head->value != NULL && head->value[0] != '\0')
			printf(" ");
	}
	return (SUCCESS);
}

int	handle_arg_type(t_token *head)
{
	head = head->next;
	while (head->value != NULL && head->value[0] == '\0')
		head = head->next;
	while (head != NULL)
	{
		if (head->value != NULL && head->value[0] != '\0')
		{
			if (head->type == RED_IN || head->type == RED_OUT
				|| head->type == APPEND || head->type == HEREDOC)
				break ;
			printf("%s", head->value);
		}
		head = head->next;
		if (head != NULL && head->value != NULL && head->value[0] != '\0')
			printf(" ");
	}
	printf("\n");
	return (SUCCESS);
}

int	yodeling(t_token *token)
{
	t_token	*head;

	head = token;
	while (head->value != NULL)
	{
		if (head->next->value == NULL)
			return (printf("\n"), SUCCESS);
		if (head->next->type == FLAG && head->next->echo == true)
			return (handle_flag_type(head));
		if (head != NULL && head->next != NULL && head->next->type == ARG 
			&& ft_strncmp(head->value, "echo", 5) == 0)
			return (handle_arg_type(head));
		head = head->next;
	}
	return (FAILURE);
}
