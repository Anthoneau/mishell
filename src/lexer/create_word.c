/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:27:37 by agoldber          #+#    #+#             */
/*   Updated: 2025/01/13 10:22:35 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_delimitation(char c)
{
	if (c == ' ' || c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

char	*space_or_meta_char_delimitation(char *inpt, long *flag)
{
	int		i;
	int		j;
	char	*new;

	//printf("on entre dans word_in_delimitation\n");
	i = 0;
	while (inpt[i] && !is_delimitation(inpt[i]))
	{
		//printf("inpt[%d] != c dont on avance\n", i);
		i++;
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
	}
	new[j] = '\0';
	//printf("\\0 a %d\n", j);
	return (new);
}

char	*word_in_delimitation(char *inpt, char c, long *flag)
{
	int		i;
	int		j;
	char	*new;

	if (c == ' ')
		return (space_or_meta_char_delimitation(inpt, flag));
	//printf("on entre dans word_in_delimitation\n");
	i = 0;
	if (inpt[i] == c)
	{
		//printf("inpt[i] == c donc on avance de 1\n");
		i++;
	}
	while (inpt[i] && inpt[i] != c)
	{
		//printf("inpt[%d] != c dont on avance\n", i);
		i++;
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
	if (inpt[i] == c)
	{
		//printf("inpt[i] == c donc on avance de 1\n");
		i++;
	}
	j = 0;
	while (inpt[i] && inpt[i] != c)
	{
		//printf("on copie...\n");
		new[j++] = inpt[i++];
	}
	new[j] = '\0';
	//printf("\\0 a %d\n", j);
	return (new);
}

void	create_word(char *inpt, long *i, t_token **token)
{
	char	*word;

	// printf("i dans create word mais avant la creation de word = %ld\n", *i);
	if (inpt[*i] == '\'' || inpt[*i] == '"')
	{
		// printf("on trouve une quote\n");
		if (inpt[*i] == '\'')
			word = word_in_delimitation(inpt + *i, '\'', i);
		else if (inpt[*i] == '"')
			word = word_in_delimitation(inpt + *i, '"', i);
		if (*i < 0)
			return ;
		*i += ft_strlen(word) + 2;
		// printf("word apres word in quotes : %s\n\n", word);
	}
	else
	{
		// printf("y a pas de quotes\n");
		word =  word_in_delimitation(inpt + *i, ' ', i);
		if (*i < 0)
			return ;
		// printf("i avant ft_strlen: %ld\nft_strlen en question : %ld\n", *i, ft_strlen(word));
		*i += ft_strlen(word);
		// printf("i apres ft_strlen: %ld\n", *i);
		// printf("word = %s\n\n", word);
	}
	// printf("i dans create word apres creation de word = %ld\n", *i);
	new_token(word, WORD, token, i);
	free(word);
}
