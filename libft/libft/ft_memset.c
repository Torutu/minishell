/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 21:08:23 by walnaimi          #+#    #+#             */
/*   Updated: 2024/09/02 21:10:56 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t len)
{
	unsigned char	*p;
	unsigned char	value;

	value = (unsigned char)c;
	p = s;
	while (len > 0)
	{
		*p = value;
		p++;
		len--;
	}
	return (s);
}
