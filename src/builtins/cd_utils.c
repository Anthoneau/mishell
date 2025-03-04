/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:55:06 by agoldber          #+#    #+#             */
/*   Updated: 2025/03/04 17:56:34 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error_cd(char *name)
{
	print_error(1, "cd", 0, NULL);
	if (errno == EACCES)
		print_error(0, name, 0, "Permission denied");
	else if (errno == EIO)
		print_error(0, name, 0, "Input/output error");
	else if (errno == ELOOP)
		print_error(0, name, 0, "Too many levelsof symbolic links");
	else if (errno == ENAMETOOLONG)
		print_error(0, name, 0, "File name too long");
	else if (errno == ENOENT)
		print_error(0, name, 0, "No such file or directory");
	else if (errno == ENOMEM)
		print_error(0, name, 0, "Cannot allocate memory");
	else if (errno == ENOTDIR)
		print_error(0, name, 0, "Not a directory");
}

int	ft_strsrch(char	*str, char	*search)
{
	int	i;

	i = 0;
	if (!str || !search)
		return (0);
	while (str[i] && search[i])
	{
		if (str[i] != search[i])
			return (0);
		i++;
	}
	if (str[i] != '\0')
		return (0);
	return (1);
}
