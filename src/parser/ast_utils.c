/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 11:32:10 by agoldber          #+#    #+#             */
/*   Updated: 2025/02/11 11:33:39 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*find_first_word_to_the_left(t_token *current)
{
	while (current->explored != 1)
	{
		if (current->prev && current->prev->type == 0
			&& current->explored == 0)
			current = current->prev;
		else
			break ;
	}
	return (current);
}

t_token	*good_cur(t_token *current, int side, int type)
{
	if (side == 0 || type == 1)
		return (current);
	else
		return (find_first_word_to_the_left(current));
}

void	put_error_to_one(int *error)
{
	*error = 1;
}
