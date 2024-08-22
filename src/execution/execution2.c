/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 13:30:29 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/22 14:03:14 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
At first, the user will give us a command such as:

shell %> ls -la | echo test > outfile | cat outfile

In our parsing we will take care of stripping everything that is unnecessary
and take care of redirections accordingly. Every command between pipes will
be sent down to the children processes that will execute them depending if 
they are built-ins or other commands. In the previous example, echo is a 
built-in and I need to execute it in a different manner. For the execution
of built-ins I have done everything with a linked list type t_token that
parses out empty spaces and takes mostly strings and has a pointer to the 
value of the string called value in the node.

Therefore, at the doorstep of execution, the tokens will contain the full
instruction "ls -la | echo test > outfile | cat outfile" where the nodes 
will contain "ls, -la, |, echo, test, >, outfile, |, cat, and outfile". 
My parsed array will have only what the child is supposed to execute.
So, in the second child, which contains a builtin, we will see:

arr[0] = echo 
arr[1] = test
 */
bool	builtin_filter(t_token *token, char *command)
{
	t_token	*tmp;

	tmp = token;
	while (tmp->next != NULL)
	{
		if (!ft_strncmp(command, tmp->value, ft_strlen(command))
			&& tmp->type == BUILTIN)
				return (true);
		tmp = tmp->next;
	}
	tmp = NULL;
	return (false);
}

t_token *find_token_exec(t_token *token, char **array)
{
	int		i;
	t_token	*tmp;

	i = 0;
	while (array[i])
	{
		tmp = token;
		while (tmp != NULL && tmp->next != NULL)
		{
			if (!ft_strncmp(array[i], tmp->value, ft_strlen(array[i]))
				&& tmp->type == BUILTIN)
			{
				if (tmp->next && array[i + 1]
					&&!ft_strncmp(array[i + 1], tmp->next->value, ft_strlen(array[i])))
					return (tmp);
			}
			tmp = tmp->next;
		}
		i++;
	}
	tmp = NULL;
	return (NULL);
}

void	ft_builtin_exec(t_data *data, t_token *token, t_env **env_ll)
{
	int status;

	status = 0;
	if (token == NULL)
		exit(status);
	status = built_ins(data, token, env_ll);
	exit(status);
}