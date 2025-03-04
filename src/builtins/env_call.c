/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_call.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:17:33 by mel-bout          #+#    #+#             */
/*   Updated: 2025/03/04 18:01:01 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	call_env(t_list *list)
{
	t_node	*ptr;

	ptr = list->head;
	while (ptr != NULL)
	{
		printf("%s=%s\n", ptr->key, ptr->value);
		ptr = ptr->next;
	}
	return (0);
}
