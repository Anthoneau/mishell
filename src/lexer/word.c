/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:24:24 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/22 20:35:45 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*word_in_delimitation(char *inpt, char c, long *flag)
{
	int		i;
	int		j;
	char	*new;

	i = 0;
	if (inpt[i] == c)
		i++;
	while (inpt[i] && inpt[i] != c)
		i++;
	new = malloc(i + 1);
	if (!new)
	{
		*flag = -10;
		return (print_error(1, "malloc", "Cannot allocate memory"), NULL);
	}
	i = 0;
	if (inpt[i] == c)
		i++;
	j = 0;
	while (inpt[i] && inpt[i] != c)
		new[j++] = inpt[i++];
	new[j] = '\0';
	return (new);
}

char	*space_or_meta_char_delimitation(char *inpt, long *flag)
{
	int		i;
	char	*new;

	i = 0;
	while (inpt[i] && !is_delimitation(inpt[i]))
	{
		i++;
		if (inpt[i] == ' ')
			break ;
	}
	new = ft_calloc(i + 1, sizeof(char));
	if (!new)
	{
		*flag = -10;
		return (print_error(1, "malloc", "Cannot allocate memory"), NULL);
	}
	i = 0;
	while (inpt[i] && !is_delimitation(inpt[i]))
	{
		new[i] = inpt[i];
		i++;
		if (inpt[i] == ' ')
			break ;
	}
	return (new);
}
