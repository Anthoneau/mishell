/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:27:37 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/10 17:58:37 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*word_in_delimitation(char *inpt, char c, long *flag)
{
	int		i;
	int		j;
	char	*new;

	// if (c == ' ')
	// 	return (space_or_meta_char_delimitation(inpt, flag));
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

void	join_content(char *word, t_token **token, long *error)
{
	t_token	*current;
	char	*temp;

	current = *token;
	while (current)
	{
		if (current->next)
			current = current->next;
		else
			break ;
	}
	temp = ft_strdup(current->content);
	if (!temp) //print
	{
		*error = -10;
		return ;
	}
	free(current->content);
	current->content = ft_strjoin(temp, word);
	free(temp);
}

int	is_del_mod(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '&' || c == ';')
		return (1);
	return (0);
}

void	create_word(char *inpt, long *i, t_token **token, int expandable, char **env)
{
	char	*word;
	int		type;
	int		space;

	type = WORD;
	space = 0;
	if (*i > 0 && inpt[*i - 1] && inpt[*i - 1] != ' ' && !is_del_mod(inpt[*i -1]))
	{
		// printf("inpt[%ld] = : %c\n", *i, inpt[*i]);
		space = 1;
	}
	// printf("i dans create word mais avant la creation de word = %ld\n", *i);
	if (inpt[*i] && (inpt[*i] == '\'' || inpt[*i] == '"'))
	{
		printf("on trouve une quote\n");
		if (inpt[*i] == '\'')
		{
			printf("aaaaah\n");
			expandable = 0;
			type = S_QUOTES;
			word = word_in_delimitation(inpt + *i, '\'', i);
		}
		else if (inpt[*i] == '"')
		{
			type = D_QUOTES;
			word = word_in_delimitation(inpt + *i, '"', i);
		}	
		if (*i < 0)
			return ;
		*i += ft_strlen(word) + 2;
		// printf("word apres word in quotes : %s\n\n", word);
	}
	else
	{
		printf("y a pas de quotes\n");
		// word =  word_in_delimitation(inpt + *i, ' ', i);
		word = space_or_meta_char_delimitation(inpt + *i, i);
		if (*i < 0)
			return ;
		// printf("i avant ft_strlen: %ld\nft_strlen en question : %ld\n", *i, ft_strlen(word));
		*i += ft_strlen(word);
		// printf("i apres ft_strlen: %ld\n", *i);
		// printf("word = %s\n\n", word);
	}
	// printf("i dans create word apres creation de word = %ld\n", *i);
	printf("space : %d\n", space);
	printf("expandable : %d\n", expandable);
	if (expandable)
	{
		if (!modify_inpt(&word, env))
		{
			return (print_error_message(1, "malloc", "Cannot allocate memory"));
		}
	}
	if (space)
		join_content(word, token, i);
	else
		new_token(word, type, token, i);
	free(word);
}
