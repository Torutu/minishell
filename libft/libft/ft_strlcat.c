/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 21:09:12 by walnaimi          #+#    #+#             */
/*   Updated: 2024/09/02 21:10:39 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t n)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	total_len;
	size_t	chars_copy;

	src_len = 0;
	dst_len = 0;
	if (src != NULL && dst != NULL)
	{
		src_len = ft_strlen(src);
		dst_len = ft_strlen(dst);
	}
	total_len = dst_len + src_len;
	chars_copy = 0;
	if (n > dst_len)
		chars_copy = n - dst_len - 1;
	else
		chars_copy = 0;
	if (chars_copy > 0)
		ft_strncat(dst, src, chars_copy);
	if (n >= dst_len)
		total_len = dst_len + src_len;
	else
		total_len = n + src_len;
	return (total_len);
}
