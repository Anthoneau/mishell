/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 18:48:56 by mel-bout          #+#    #+#             */
/*   Updated: 2025/03/03 13:55:46 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd(t_list *env)
{
	if (env->pwd->value)
	{
		ft_putendl_fd(env->pwd->value, 1);
		return (0);
	}
	return (1);
}
