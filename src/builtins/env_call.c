/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_call.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:17:33 by mel-bout          #+#    #+#             */
/*   Updated: 2025/03/12 18:24:31 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	call_env(t_list *list, int output)
{
	t_node	*ptr;
	struct stat *buf;

	buf = NULL;
	if (output == -1 || fstat(output, buf) == -1)
		output = 1;
	ptr = list->head;
	while (ptr != NULL)
	{
		if (ptr->value != NULL)
		{
			ft_putstr_fd(ptr->key, output);
			ft_putchar_fd('=', output);
			ft_putendl_fd(ptr->value, output);
		}
		ptr = ptr->next;
	}
	return (0);
}
