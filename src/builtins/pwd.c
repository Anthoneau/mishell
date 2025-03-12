/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 18:48:56 by mel-bout          #+#    #+#             */
/*   Updated: 2025/03/12 18:36:40 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd(t_list *env, int output)
{
	struct stat *buf;

	buf = NULL;
	if (output == -1 || fstat(output, buf) == -1)
		output = 1;
	if (env->pwd)
	{
		ft_putendl_fd(env->pwd, output);
		return (0);
	}
	return (1);
}
