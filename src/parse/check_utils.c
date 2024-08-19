/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 17:33:43 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/14 16:20:27 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * loop_path_for_binary function assumes that you have access to the PATH variable
 * inside the environment pointer. In this shell we store the paths inside the env_ll
 * after 'PATH='. Furthermore, the struct data should have binary_paths inside of it
 * and one can use that array extract the same way.
 * 
 * RETURN VALUES: 
 */
char	*loop_path_for_binary(char *binary, char **paths)
{
	char	*token_with_path;
	int		i;

	i = 0;
	while (paths && paths[i])
	{
		token_with_path = ft_strsjoin(paths[i], binary, '/');
		if (!access(token_with_path, F_OK))
		{
			if (!access(token_with_path, X_OK))
				return (token_with_path);
			else
			{
				err_msg(binary, NO_PERMISSION, 1);
				free(token_with_path);
			}
		}
		free(token_with_path);
		i++;
	}
	return (NULL);
}

char	*ft_strndup(const char *s, size_t n)
{
	char			*res;
	unsigned int	i;

	i = 0;
	res = malloc(sizeof(char) * (n + 1)); //1 -> chunky_check 57
	if (res == NULL)
		return (NULL);
	while (i < n)
	{
		res[i] = s[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

int	count_token(t_token *token, t_type type)
{
	t_token	*head;
	int		count;

	count = 0;
	head = token;
	while (head)
	{
		if (head->type == type)
			count++;
		head = head->next;
	}
	head = NULL;
	return (count);
}
