/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 18:48:56 by mel-bout          #+#    #+#             */
/*   Updated: 2025/03/13 18:33:06 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd(t_list *env, int output)
{
	output = get_output(output);
	if (env->pwd)
	{
		ft_putendl_fd(env->pwd, output);
		return (0);
	}
	return (1);
}
