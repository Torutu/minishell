/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freedom.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdessoy- <fdessoy-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 13:19:49 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/07/22 13:19:50 by fdessoy-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void free_my_boi(char **paths)
{
	int i;
	i = 0;
	while(paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

/*
let's assume that paths is inside a struct t_data
paths is a char *

we malloc (1 * sizeof (t_data));
value of paths is ??????

instead we calloc(1, sizeof(t_data));
paths is 0x0;

if (paths) if we malloc, we don't know address of paths, might be trying to free random mem
	free(paths);
if we calloc we guarantee that paths == NULL, so it will not go into this condition if we did not assign something to paths;
*/ 