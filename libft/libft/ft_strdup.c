/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 12:12:08 by fdessoy-          #+#    #+#             */
/*   Updated: 2024/08/21 18:48:34 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

char	*ft_strdup(const char *src)
{
	int		size;
	int		i;
	char	*cpy;

	i = 0;
	size = 0;
	while (src[size])
		size++;
	cpy = malloc(sizeof(char) * (size + 1));
	if (cpy == NULL)
		return (NULL);
	while (src[i])
	{
		cpy[i] = src[i];
		i++;
	}
	cpy[i] = '\0';
	return (cpy);
}
