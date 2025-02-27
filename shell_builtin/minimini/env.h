/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-bout <mel-bout@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 00:02:34 by dell              #+#    #+#             */
/*   Updated: 2025/02/24 20:36:17 by mel-bout         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <limits.h>
# include <stdbool.h>

typedef struct s_node
{
	char			*key;
	char			*value;
	struct s_node 	*next;
	struct s_node 	*prev;
}	t_node;

typedef struct s_list
{
	t_node			*head;
	t_node			*tail;
	t_node			*oldpwd;
	int				size;
}	t_list;

char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char *s);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strdup(const char *s1);
void	sort(char **arr, int size);
void    unset(t_list *list, char *s);
void	export_order(t_list *list);
void	ft_free(char **arr);

#endif