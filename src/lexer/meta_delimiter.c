/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meta_delimiter.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 16:24:24 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/10 17:57:32 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	shift(char *inpt, int i)
{
	int	j;

	j = i + 1;
	while (inpt[i])
	{
		printf("[%c] -> [%c]\n", inpt[i], inpt[j]);
		inpt[i++] = inpt[j++];
	}
	printf("\n");
	return (1);
}

int	quotes_shifter(char *inpt, int i)
{
	int	s_q;
	int	d_q;
	int	end;
	int	count_char;

	s_q = 0;
	d_q = 0;
	end = 0;
	count_char = 0;
	if (inpt[i] == '\'')
		s_q = 1;
	else if (inpt[i] == '"')
		d_q = 1;
	while (inpt[i])
	{
		if ((s_q && inpt[i] == '\'') || (d_q && inpt[i] == '"'))
			end += shift(inpt, i);
		if (end == 2)
			break ;
		i++;
		count_char++;
	}
	return (count_char - 1);
}

void	check_if_quotes(char *inpt)
{
	int	i;

	i = 0;
	while (inpt[i])
	{
		// printf("[%c] ", inpt[i]);
		if (inpt[i] == '\'' || inpt[i] == '"')
		{
			i += quotes_shifter(inpt, i);
		}
		i++;
	}
	// printf("\n");
}

int	is_delimitation(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '\'' || c == '"' || c == '&' || c == ';')
		return (1);
	return (0);
}

char	*space_or_meta_char_delimitation(char *inpt, long *flag)
{
	int		i;
	int		j;
	char	*new;

	printf("on entre dans word_in_delimitation\n");
	i = 0;
	// check_if_quotes(inpt);
	while (inpt[i] && !is_delimitation(inpt[i]))
	{
		printf("inpt[%d] != c dont on avance\n", i);
		i++;
		if (inpt[i] == ' ')
			break ;
	}
	//printf("on malloc\n");
	new = malloc(i + 1);
	if (!new)
	{
		//printf("malloc foire, on met flag a -10 et on return NULL\n");
		*flag = -10;
		return (NULL);
	}
	i = 0;
	j = 0;
	while (inpt[i] && !is_delimitation(inpt[i]))
	{
		//printf("on copie...\n");
		new[j++] = inpt[i++];
		if (inpt[i] == ' ')
			break ;
	}
	new[j] = '\0';
	printf("new : %s|\n", new);
	//printf("\\0 a %d\n", j);
	return (new);
}
