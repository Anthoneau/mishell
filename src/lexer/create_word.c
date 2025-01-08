/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:27:37 by agoldber          #+#    #+#             */
/*   Updated: 2025/01/08 14:37:41 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*word_in_delimitation(char *inpt, char c, long *flag)
{
	int		i;
	int		j;
	char	*new;

	i = 0;
	if (inpt[i] == c)
		i++;
	while (inpt[i] != c)
		i++;
	new = malloc(i);
	if (!new)
	{
		*flag = -1;
		return (NULL);
	}
	i = 0;
	if (inpt[i] == c)
		i++;
	j = 0;
	while (inpt[i] != c)
		new[j++] = inpt[i++];
	new[j] = '\0';
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
		if (*i == -1)
			return ;
		*i += ft_strlen(word) + 2;
		// printf("word apres word in quotes : %s\n\n", word);
	}
	else
	{
		// printf("y a pas de quotes\n");
		word =  word_in_delimitation(inpt + *i, ' ', i);
		if (*i == -1)
			return ;
		// printf("i avant ft_strlen: %ld\nft_strlen en question : %ld\n", *i, ft_strlen(word));
		*i += ft_strlen(word) + 1;
		// printf("i apres ft_strlen: %ld\n", *i);
		// printf("word = %s\n\n", word);
	}
	// printf("i dans create word apres creation de word = %ld\n", *i);
	new_token(word, WORD, token, i);
	free(word);
}
