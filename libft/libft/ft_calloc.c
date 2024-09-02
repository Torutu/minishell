/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 20:58:49 by walnaimi          #+#    #+#             */
/*   Updated: 2024/09/02 20:58:52 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;
	size_t	a;

	a = count * size;
	if (a < count && a < size)
		return (NULL);
	ptr = malloc(a);
	if (!ptr)
		return (NULL);
	if (ptr)
		ft_memset(ptr, 0, a);
	return (ptr);
}
