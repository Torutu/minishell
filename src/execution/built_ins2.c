/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 15:26:27 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/27 17:40:26 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void alphabetical_printer(char **env_array)
{
    char	c;
	int		i;
	
	i = 0;
    c = 32;
    while (c < 127)
    {
        while (env_array[i])
        {
			//name = tmp->key;
            if (env_array[i][0] == c)
            {
                printf("declare -x ");
				printf("%s\n", env_array[i]);
            }
			i++;
        }
		i = 0;
        c++;
    }
}

/* export puts variables declared by user in the env */
int	export(t_token *token, t_env **env_ll)
{
	t_token *tmp_tok;
	t_env	*tmp_ll;
	char	**array;
	int		found;

	if (find_token(token, APPEND)
	|| find_token(token, HEREDOC)
	|| find_token(token, RED_IN)
	|| find_token(token, RED_OUT))
		return (SUCCESS);
	if (token->next->value == NULL)
	{
		print_export(env_ll);
		return (SUCCESS);
	}
	token = token->next;
	tmp_tok = token;
	while (tmp_tok->next != NULL)
	{
		found = 0;
		if (!ft_strchr(tmp_tok->value, '='))//tmp fix
			tmp_tok = tmp_tok->next;
		tmp_ll = (*env_ll);
		while (tmp_ll != NULL)
		{
			array = ft_split(tmp_tok->value, '=');
			if (!array)
				return (FAILURE);
			if (!ft_strncmp(tmp_ll->key, array[0], ft_strlen(tmp_ll->key)))
			{
				found = 1;
				free_array(array);
				break ;
			}
			free_array(array);
			tmp_ll = tmp_ll->next;
		}
		if (found)
		{
			free_null(tmp_ll->value);
			free_null(tmp_ll->key);
			free_null(tmp_ll->content);
			tmp_ll->content = ft_strdup(tmp_tok->value);
			array = ft_split(tmp_tok->value, '=');
			tmp_ll->key = ft_strdup(array[0]);
			if (!tmp_ll)
			{
				free_array(array);
				return (FAILURE);
			}
			tmp_ll->value = ft_strdup(ft_strchr(tmp_tok->value, '='));
			if (!tmp_ll->value)
			{
				free_array(array);
				free_null(tmp_ll->key);
				return (FAILURE);
			}
			free_array(array);
		}
		else
		{
			ft_listadd_back(env_ll, ft_listnew(tmp_tok->value));
		}
		tmp_tok = tmp_tok->next;
	}
	return (SUCCESS);
}

// when someone types EXPORT only, it prints all env variables
// IN ALPHABETICAL ORDER!!! <- still needs to be implemented (not really necessary)
int	print_export(t_env **env_ll)
{
	char	**env_array;

	if (!env_ll || !*env_ll)
		return (SUCCESS);
	env_array = env_arr_updater(env_ll);
	if (!env_array)
		return (SUCCESS);
	alphabetical_printer(env_array);
	free_array(env_array);
	return (SUCCESS);
}

/* this function unsets whatever argument given after unset in the command line */
// int	unset(t_token *token, t_env **env_ll)
// {
// 	t_env	*tmp;
// 	t_env	*del;
// 	t_token	*head;

// 	head = token;
// 	if (!head->next->value || !*env_ll || !env_ll)
// 		return (SUCCESS);
// 	tmp = *env_ll;
// 	head = head->next;

// 	while (!ft_strncmp(head->value, tmp->content, ft_strlen(head->value)))
// 	{
// 		tmp = tmp->next;
// 		free_null(tmp->key);
// 		free_null(tmp->value);
// 		free_null(tmp->content);
// 		free(tmp);
// 		tmp = NULL;
// 		return (SUCCESS);
// 	}
// 	tmp = *env_ll;
// 	while (tmp->next != NULL)
// 	{
// 		if (!ft_strncmp(head->value, tmp->next->content,
// 				ft_strlen(head->value)))
// 		{
// 			del = tmp->next;
// 			tmp->next = tmp->next->next;
// 			free_null(del->key);
// 			free_null(del->value);
// 			free_null(del->content);
// 			free(del);
// 			del = NULL;
// 			return (SUCCESS);
// 		}
// 		tmp = tmp->next;
// 	}
// 	//*env_ll = tmp;
// 	tmp = NULL;
// 	head = NULL;
// 	return (SUCCESS);
// }

int	unset(t_token *token, t_env **env_ll)
{
	t_env	*tmp;
	t_env	*del;
	t_token	*head;

	head = token->next;
	while (head && head->value)
	{
		tmp = *env_ll;

		// Handling the case where the first node is removed
		if (tmp && !ft_strncmp(head->value, tmp->content, ft_strlen(head->value)))
		{
			*env_ll = tmp->next;  // Update head of the list
			free_null(tmp->key);
			free_null(tmp->value);
			free_null(tmp->content);
			free(tmp);
			tmp = NULL;  // Avoid dangling pointer
			head = head->next;
			continue;
		}

		// Traverse and remove any matching node in the middle or end
		while (tmp && tmp->next != NULL)
		{
			if (!ft_strncmp(head->value, tmp->next->content, ft_strlen(head->value)))
			{
				del = tmp->next;
				tmp->next = tmp->next->next;
				free_null(del->key);
				free_null(del->value);
				free_null(del->content);
				free(del);
				del = NULL;  // Avoid dangling pointer
				break;
			}
			tmp = tmp->next;
		}

		head = head->next;  // Move to the next token
	}
	//*env_ll = tmp;
	return (SUCCESS);
}


// {
// 	t_env	*tmp;
// 	char	**exp_list;
// 	int		count;
// 	t_token	*head;

// 	head = token;
// 	count = 0;
// 	if (!head->next->value)
// 	{
// 		print_export(env_ll);
// 		return (SUCCESS);
// 	}
// 	head = token->next;
// 	if(head->value[0] >= '0' && head->value[0] <= '9')
// 		return(err_msg(head->value,ERR_EXP,FAILURE));
// 	tmp = (*env_ll);
// 	while (head != NULL)
// 	{
// 		while (tmp)
// 		{
// 			if (ft_strncmp(tmp->key, head->value, ft_strlen(tmp->key)) == 0)
// 			{
// 				free(tmp->content);
// 				free(tmp->value);
// 				tmp->content = ft_strdup(head->value);
// 				tmp->value = ft_substr(head->value, ft_strlen(tmp->key) + 1, ft_strlen(head->value) - ft_strlen(tmp->key)); 
// 				return (SUCCESS);
// 			}
// 			tmp = tmp->next;
// 		}
		
// 		head = head->next;
// 		count++;
// 	}
// 	exp_list = (char **)malloc(sizeof(char *) * (count + 1));
// 	if (!exp_list)
// 		return (FAILURE);
// 	head = token->next;
// 	i = 0;
// 	while (head != NULL)
// 	{
// 		if (head->value != NULL)
// 			exp_list[i++] = ft_strdup(head->value);
// 		head = head->next;
// 	}
// 	exp_list[i] = NULL;
// 	i = 0;
// 	tmp = (*env_ll);
// 	if (!*env_ll)
// 		(*env_ll) = ft_listnew(exp_list[i++]);
// 	i = 0;
// 	while (exp_list[i])
// 		ft_listadd_back(env_ll, ft_listnew(exp_list[i++]));
// 	(*env_ll) = tmp;
// 	tmp = NULL;
// 	free_array(exp_list);
// 	return (SUCCESS);
// }

