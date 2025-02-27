/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_call.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bout <mel-bout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 19:17:33 by mel-bout          #+#    #+#             */
/*   Updated: 2025/02/27 19:17:35 by mel-bout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int call_env(t_list *list)
{
    t_node *ptr;

    ptr = list->head;
    while (ptr != NULL)
    {
        printf("%s=%s\n", ptr->key, ptr->value);
        ptr = ptr->next;
    }
    return (0);
}
