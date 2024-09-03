/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 12:23:49 by walnaimi          #+#    #+#             */
/*   Updated: 2024/09/03 02:54:20 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	total_env_len(t_env *head)
{
	int		total_length;
	t_env	*cur;

	total_length = 0;
	cur = head;
	while (cur != NULL)
	{
		if (cur->value != NULL)
			total_length += strlen(cur->value);
		cur = cur->next;
	}
	return (total_length);
}

void	setup(t_data *data)
{
	data->deli = "  \t\n";
	data->id = 0;
	data->tok_res = 0;
	data->ignore_cmd = false;
	data->echoed = false;
	data->echo_flag = false;
	data->redirections = false;
	data->piped = false;
	data->env_len = total_env_len(data->envll);
	if (data->status == 963)
		data->status = 2;
	if (g_mod == 1)
		data->status = 130;
}

/**
 * Reads a line from the user and sets up the data structure.
 * 
 * This function will read a line from the user using readline(),
 * add it to the history list if it is not empty, and set up the
 * tokenization process. If the line is empty, it will return
 * NULL_LINE. If the line is not empty, it will tokenize the line
 * and check for syntax errors. If there are any syntax errors,
 * it will return 2. If there are no syntax errors, it will set up
 * the tokenization process and return SUCCESS.
 * 
 * @param data A pointer to a t_data structure containing
 * the line to be tokenized.
 * 
 * @return 0 on successful tokenization, 2 otherwise.
 */
int	sniff_line(t_data *data)
{
	data->line_read = readline("[ft_putchar] ");
	if (!data->line_read)
		return (NULL_LINE);
	if (*data->line_read)
		add_history(data->line_read);
	setup(data);
	line_tokenization(data);
	if (data->status == 963)
		return (963);
	data->status = 0;
	g_mod = 0;
	if (syntax_check(data->token) == FAILURE)
	{
		data->status = 2;
		return (2);
	}
	data->piped = false;
	data->heredoc_exist = false;
	if (count_token(data->token, PIPE) >= 1)
		data->piped = true;
	return (SUCCESS);
}
