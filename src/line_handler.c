/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 12:23:49 by walnaimi          #+#    #+#             */
/*   Updated: 2024/08/28 15:00:45 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	total_env_len(t_env *head)
{
	int		total_length;
	t_env	*current;

	total_length = 0;
	current = head;
	while (current != NULL)
	{
		if (current->value != NULL)
			total_length += strlen(current->value);
		current = current->next;
	}
	return (total_length);
}

void	setup(t_data *data)
{
	int	len_found;

	len_found = 0;
	data->deli = "  \t\n";
	data->id = 0;
	data->tok_res = 0;
	data->ignore_cmd = false;
	data->echoed = false;
	data->echo_flag = false;
	data->redirections = false;
	data->piped = false;
	if (len_found == 0)
	{
		data->env_len = total_env_len(data->envll);
		len_found = 1;
	}
	if (data->status == 963)
		data->status = 2;
	if (data->no_cmd_flag == 1 && !data->is_exit)
		data->status = 127;
}

int	token_only_arg(t_data *data)
{
	t_token	*head;
	int		expect_command;

	head = data->token;
	expect_command = 1;
	data->no_cmd_flag = 0;
	while (head)
	{
		if (head->type == PIPE)
		{
			expect_command = 1;
			data->no_cmd_flag = 1;
		}
		else if (expect_command && head->type == ARG)
			data->no_cmd_flag = 1;
		else if (head->type == COMMAND || head->type == BUILTIN)
		{
			expect_command = 0;
			data->no_cmd_flag = 0;
			if (ft_strncmp(head->value, "exit", 5) == 0)
				data->is_exit = 1;
		}
		head = head->next;
	}
	return (SUCCESS);
}

/**
 * Here we are prompting the user to give input with the readline() and
 * tokenizing afterwards. After tokenizing, we are using the tokens to check
 * for invalid inputs. More information in closed issue #19 in the repository.
 */
int	sniff_line(t_data *data)
{
	data->line_read = readline("[ft_putchar]> ");
	if (!data->line_read)
		return (NULL_LINE);
	if (*data->line_read)
		add_history(data->line_read);
	setup(data);
	line_tokenization(data);
	if (data->status == 963)
		return (free_retstatus(data->line_read, 963));
	data->status = 0;
	free(data->line_read);
	if (syntax_check(data->token) == FAILURE)
	{
		data->status = 2;
		return (2);
	}
	token_only_arg(data);
	data->piped = false;
	data->heredoc_exist = false;
	if (count_token(data->token, PIPE) >= 1)
		data->piped = true;
	return (SUCCESS);
}
