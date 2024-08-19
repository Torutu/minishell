/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 14:18:24 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/19 10:58:26 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* This part is taking the tokens and taking the argument just after it. */
int	built_ins(t_data *data, t_token *token, t_env **env_ll)
{
	int	status;
	status = 0;
	data->home_pwd = get_home((*env_ll));
	if (!ft_strncmp(token->value, "env", 3))
		status = print_env((*env_ll));
	else if (!ft_strncmp(token->value, "pwd", 3))
		status = print_pwd();
	else if (!ft_strncmp(token->value, "exit", 4))
		get_the_hell_out(data, token, (*env_ll));
	else if (!ft_strncmp(token->value, "echo", 4))
		status = yodeling(token);
	else if (!ft_strncmp(token->value, "cd", 2))
		status = shell_cd(token, data);
	else if (!ft_strncmp(token->value, "export", 6))
		status = export(token, env_ll); // 1/2 broken
	else if (!ft_strncmp(token->value, "unset", 5))
		status = unset(token, env_ll); // broken
	else
		printf("Unknown command: %s\n", token->value);
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
	return (SUCCESS);
}

/* This is the exit function, it needs to take, if inputted,
an exit code that was manually inserted after exit */
void	get_the_hell_out(t_data *data, t_token *token, t_env *env_ll)
{
	free_ll(env_ll);
	ft_printf("exit\n");
	if (token->next)
		exit(ft_atoi(token->next->value));
	exit(data->status);
}

int	yodeling(t_token *token)
{
	t_token	*head;

	head = token;
	if (head->next == NULL)
		return (printf("\n"), SUCCESS);
	if (head->next->type == FLAG)
	{
		head = head->next;
		while(head->type == FLAG)
		{
			head = head->next;
			if (head == NULL)
				return FAILURE;
		}
		if(head->value[0] == '\0')
			head = head->next;
		while (head != NULL)
		{
			printf("%s", head->value);
			if(head->next != NULL)
				printf(" ");
			head = head->next;
		}
		return (SUCCESS);
	}
	if (head->next->type == ARGUMENT)
	{
		head = head->next;
		if(head->value[0] == '\0')
			head = head->next;
		while (head != NULL)
		{
			if (head->value[0] != '\0')
				printf("%s", head->value);
			head = head->next;
			if(head != NULL && head->value[0] != '\0') 
				printf(" ");
		}
		printf("\n");
		return (SUCCESS);
	}
	return (FAILURE);
}
