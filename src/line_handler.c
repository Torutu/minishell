/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 12:23:49 by walnaimi          #+#    #+#             */
/*   Updated: 2024/09/01 01:30:18 by walnaimi         ###   ########.fr       */
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
	//int	len_found;

	//len_found = 0;
	data->deli = "  \t\n";
	data->id = 0;
	data->tok_res = 0;
	data->ignore_cmd = false;
	data->echoed = false;
	data->echo_flag = false;
	data->redirections = false;
	data->piped = false;
	//if (len_found == 0)
	//{
	data->env_len = total_env_len(data->envll);
	//len_found = 1;
	//}
	if (data->status == 963)
		data->status = 2;
	// if (data->no_cmd_flag == 1 && !data->is_exit)
	// 	data->status = 127;
	if (g_exit_code == 1)
		data->status = 130;
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
		{
			data->no_cmd_flag = 1;
			if (head->id >= 1 && head->prev->type == HEREDOC)
				data->no_cmd_flag = 0;
		}
		else if (head->type == EXEC || head->type == BUILTIN)
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

#include "../../includes/minishell.h"

/**
 * Prints the tokens in the linked list,
 * including their values, types, and other properties.
 * 
 * @param data A pointer to the t_data structure
 * containing the token linked list.
 * 
 * 
 * @return None
 */
void    print_tokens(t_data *data)
{
	t_token    *token;
	t_token    *last_token;

	const char *type_names[]
	= {
		"UNKNOWN",
		"BUILTIN",
		"EXEC",
		"ARGUMENT",
		"PIPE",
		"FLAG",
		"ENVVAR",
		"RED_IN",
		"RED_OUT",
		"HEREDOC",
		"APPEND",
		"TFILE",
	};

	token = data->token;
	last_token = NULL;
	while (token != NULL)
	{
		if (token->value != NULL)
		{
			printf("--------------[%d]--------------\n", token->id);
			printf("token value :[%s]\n", token->value);
			if (token->value[0] == '\0')
				printf("empty string\n");
			printf("token type  :[%s]\n", type_names[token->type]);
			if (token->empty == true)
				printf("empty?      :[%d]\n", token->empty);
			if (token->in_q == true)
				printf("in quotes   :[%d]\n", token->in_q);
            if (token->echo == true)
                printf("echo?       :[%d]\n", token->echo);
            if (token->path != NULL)
                printf("token path  :[%s]\n", token->path);
            printf("\n");
            if (token->next == NULL)
                last_token = token;
        }
        token = token->next;
    }
    printf("#####################################\n");
    token = last_token;
}

// void	ctrl_d_handler(char *str)
// {
// 	if (!str)
// 	{
// 		ft_putendl_fd("exit", 0);
// 		rl_clear_history();
// 		exit(0);
// 	}
// }

// t_token *find_last_tfile(t_token *head)
// {
//     t_token *last_tfile = NULL;

//     // Traverse the linked list
//     while (head && head->value != NULL)
//     {
//         // Update last_tfile if the current token is of type TFILE
//         if (head->type == TFILE)
//         {
//             last_tfile = head;
//         }
        
//         // Move to the next token
//         head = head->next;
//     }

//     return last_tfile;
// }

void swap_tokens(t_token *a, t_token *b)
{

	t_type tmp_type = a->type;
	a->type = b->type;
	b->type = tmp_type;

	char *tmp_value = a->value;
	a->value = b->value;
	b->value = tmp_value;

	char *tmp_path = a->path;
	a->path = b->path;
	b->path = tmp_path;

	bool tmp_echo = a->echo;
	a->echo = b->echo;
	b->echo = tmp_echo;

	bool tmp_in_q = a->in_q;
	a->in_q = b->in_q;
	b->in_q = tmp_in_q;

	bool tmp_empty = a->empty;
	a->empty = b->empty;
	b->empty = tmp_empty;

}

void move_tokens_left(t_token *token)
{
	t_token *current;
	current = token;

	while (current && current->value != NULL && current->type != PIPE)
	{
		if (current->type == TFILE && (current->prev->type == RED_OUT || current->prev->type == APPEND))
		{
			//dprintf(2, "CUR[%s]\n\n", current->value);
			while(current->value != NULL && current->type != ARG)
				current = current->next;
			//dprintf(2, "1CUR[%s]\n\n", current->value);
			while(current->value != NULL && current->type == ARG && (current->prev->type != RED_OUT || current->prev->type != APPEND || current->prev->type != HEREDOC)
			&& current->prev->type != PIPE && current->type != PIPE)
			{
				//dprintf(2, "1-P[%s]<->C[%s]\n", current->prev->value, current->value);
				swap_tokens(current, current->prev);
				//dprintf(2, "2-P[%s]<->C[%s]\n", current->prev->value, current->value);
				current = current->prev;
				//dprintf(2, "WE MOVE THE TO THE PREV <-\n");
				//dprintf(2, "3-P[%s]<->C[%s]\n\n", current->prev->value, current->value);
				if ((current->type == ARG && current->prev->type == ARG) || current->prev->type == BUILTIN)
					break;
			}
		}
		//dprintf(2, "BIG WHILE CUR[%s]\n\n", current->value);
		if(current->value != NULL)
			current = current->next;
	}
}

//[echo] [hello] [world] [>] [file] [arg1] [arg2] [>] [file2] [arg3]
//[echo] [hello] [world] [arg1] [arg2] [arg3] [>] [file] [>] [file2]

//echo hello world > file arg1 arg2 > file2 arg3
//echo hello world arg1 arg2 arg3 > file > file2

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
		return (free_retstatus(data->line_read, 963));
	data->status = 0;
	g_exit_code = 0;
	free(data->line_read);
	if (syntax_check(data->token) == FAILURE)
	{
		data->status = 2;
		return (2);
	}
	//move_tokens_left(data->token);
	//print_tokens(data);
	//token_only_arg(data);
	data->piped = false;
	data->heredoc_exist = false;
	if (count_token(data->token, PIPE) >= 1)
		data->piped = true;
	return (SUCCESS);
}
