/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/10/09 18:02:43 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "libft.h"

char	*ft_strjoin(char *line, char *buf)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!line || !buf)
		return (NULL);
	str = (char *)malloc(ft_strlen(line) + ft_strlen(buf) + 1);
	if (!str)
		return (NULL);
	join[0] = '\0';
	if (line)
		ft_strlcat(join, line, ft_strlen(line) + 1);
	if (buf)
		ft_strlcat(join, buf, ft_strlen(buf) + ft_strlen(line) + 1);
	if (!join[0])
		return (free(join), NULL);
	return (join);
}
