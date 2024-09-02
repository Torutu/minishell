/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsjoin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: walnaimi <walnaimi@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 21:09:42 by walnaimi          #+#    #+#             */
/*   Updated: 2024/09/02 21:10:33 by walnaimi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strsjoin(const char *s1, const char *s2, const char sep)
{
	char	*str;
	size_t	i;
	size_t	j;

	if (s1 != NULL && s2 != NULL)
	{
		str = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + (sep != 0) + 1);
		if (!str)
			return (NULL);
		i = 0;
		j = 0;
		while (s1[i])
			str[j++] = s1[i++];
		i = 0;
		if (sep)
			str[j++] = sep;
		while (s2[i])
			str[j++] = s2[i++];
		str[j] = '\0';
		return (str);
	}
	return (NULL);
}
