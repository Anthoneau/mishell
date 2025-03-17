/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_call.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:17:33 by mel-bout          #+#    #+#             */
/*   Updated: 2025/03/13 18:31:53 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	call_env(t_list *list, int output)
{
	t_node		*ptr;

	output = get_output(output);
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
