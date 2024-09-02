/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 21:54:47 by walnaimi          #+#    #+#             */
/*   Updated: 2024/09/01 21:54:47 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	free_all_resources(t_env **env_ll, t_data *data)
{
	free_all_ll(env_ll);
	free_data(data, NULL, NULL);
}

void	free_all_with_cmd(t_env **env_ll, t_data *data, char **cmd_with_args)
{
	free_all_ll(env_ll);
	free_data(data, NULL, cmd_with_args);
}

void	free_before_exit(t_data *data, t_env **env_ll)
{
	free_gang(data);
	free_all_ll(env_ll);
}
