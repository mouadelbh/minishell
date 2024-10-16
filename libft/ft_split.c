/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zelbassa <zelbassa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 20:49:16 by mel-bouh          #+#    #+#             */
/*   Updated: 2024/10/04 13:17:03 by zelbassa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static void	ft_free(char **arr, int i)
{
	while (i >= 0)
	{
		free(arr[i]);
		i--;
	}
	free(arr);
}

static int	count_words(const char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (*(s + i))
	{
		while (*(s + i) == c && *(s + i))
			i++;
		if (*(s + i) != '\0')
		{
			count++;
			while (*(s + i) != c && *(s + i))
				i++;
		}
	}
	return (count);
}

static char	*cat_words(const char *s, char c)
{
	char	*word;
	int		i;
	int		k;

	i = 0;
	k = 0;
	while (*(s + i) != c && *(s + i))
		i++;
	word = (char *)malloc(sizeof(char) * i + 1);
	if (word == NULL)
		return (NULL);
	while (k < i)
	{
		*(word + k) = *(s + k);
		k++;
	}
	*(word + i) = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	int		counter;
	int		i;
	char	**split;

	if (!s)
		return (NULL);
	i = 0;
	counter = count_words(s, c);
	split = (char **)malloc(sizeof(char *) * (counter + 1));
	if (split == NULL)
		return (NULL);
	while (i < counter)
	{
		while (*s == c && *s)
			s++;
		if (*s != '\0')
		{
			split[i] = cat_words(s, c);
			if (!split[i])
				return (ft_free(split, i), NULL);
			i++;
		}
		while (*s != c && *s)
			s++;
	}
	split[i] = NULL;
	return (split);
}
