/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 16:11:18 by agoldber          #+#    #+#             */
/*   Updated: 2024/12/04 18:24:13 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_exit(char *str, int exit_code)
{
	if (str)
		ft_putstr_fd(str, 2);
	exit(exit_code);
}

void	free_token(t_token **token)
{
	t_token	*current;
	t_token	*to_free;

	current = *token;
	while (current)
	{
		to_free = current;
		if (current->next)
			current = current->next;
		else
		{
			free(to_free);
			break ;
		}
		if (to_free)
			free(to_free);
	}
	token = NULL;
}

int	count_lst(t_token *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}
