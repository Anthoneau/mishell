/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber < agoldber@student.s19.be >       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 15:10:55 by agoldber          #+#    #+#             */
/*   Updated: 2025/03/26 15:20:15 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_token_free(t_token **t, t_list **e, int free_t)
{
	free_array((*e)->env_c);
	if (free_t)
		free_token(t);
}

int	error_checker(t_token *cur)
{
	if (cur->next && ((cur->type != WORD && cur->type != PIPE
				&& cur->next->type != WORD) || (cur->type == PIPE
				&& cur->next->type == PIPE)))
		return (error_h(cur), 0);
	if (cur->type != WORD && cur->type != PIPE && !cur->next)
		return (solo_h(cur), 0);
	return (1);
}

int	check_token(t_token **t, char **inpt, t_list **e)
{
	t_token	*cur;

	cur = *t;
	if (cur->type != WORD && !cur->next)
		return (solo_h(cur), check_token_free(t, e, 1), 0);
	if (cur->type == PIPE && cur->next && cur->next->type != PIPE)
		return (print_syntax_error("|"), check_token_free(t, e, 1), 0);
	while (cur)
	{
		if (!error_checker(cur))
			return (check_token_free(t, e, 1), 0);
		if (cur->type == R_HEREDOC && !do_heredoc(cur, (*e)->env_c))
			return (check_token_free(t, e, 1), 0);
		if (cur->next)
			cur = cur->next;
		else if (cur->type == PIPE && !end_pipe(&cur, inpt, e, t))
			return (check_token_free(t, e, 1), print_e(1, "malloc", 1, ""), 0);
		else
			break ;
	}
	if (forbidden_token(t))
		return (check_token_free(t, e, 1), 0);
	return (check_token_free(t, e, 0), 1);
}
