/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pointer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 21:11:49 by walnaimi          #+#    #+#             */
/*   Updated: 2024/09/02 21:12:02 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_pointer(void *ptr, int *check)
{
	int	count;

	count = 0;
	count += ft_str("0x", check);
	if (count == -1)
		return (-1);
	count += ft_hex((unsigned long)ptr, 0, check);
	return (count);
}
