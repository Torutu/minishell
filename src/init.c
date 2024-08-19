/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 12:38:16 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/07 00:53:19 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// node is getting a copy of env in a linked list
void	ll_env(t_env **env_ll, char **env)
{
	t_env	*tmp;
	int		i;

	i = 0;
	if (*env_ll == NULL)
		*env_ll = ft_listnew(env[i++]);
	tmp = (*env_ll);
	while (env[i])
		ft_listadd_back(env_ll, ft_listnew(env[i++]));
	(*env_ll) = tmp;
}

void	find_bin(t_env **env_ll, t_data *data)
{
	t_env	*tmp;

	tmp = (*env_ll);
	while ((*env_ll)->next != NULL)
	{
		if (!ft_strncmp((*env_ll)->content, "PATH=", 5))
			data->bin = bin_extract((*env_ll)->content);
		(*env_ll) = (*env_ll)->next;
	}
	(*env_ll) = tmp;
}

char	*bin_extract(char *path)
{
	int	i;

	i = 0;
	if (!path)
		return (NULL);
	while (path)
	{
		if (!ft_strncmp(path, "PATH=", 5))
			return (path + 5);
		i++;
	}
	return (NULL);
}

void	initializer(t_data *data, t_env **env_ll, char **env)
{
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
	ft_bzero(data, sizeof(t_data));
	ll_env(env_ll, env);
	find_bin(env_ll, data);
	data->binary_paths = ft_split(data->bin, ':');
	data->envll = *env_ll; //to make sure the data->envll points to the initialized env_ll
	data->in_quotes = 0;
	data->echoed = false;
	//print_binary_paths(data);
	data->echoed = false;
	data->cmd_ignore = false;
}
