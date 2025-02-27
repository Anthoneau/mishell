#include "minishell.h"

int	strllen(char *s, char c)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	while(s[i] && s[i] != c)
		i++;
	return (i);
}

char	*ft_strldup(const char *s1, int len)
{
	//printf("<%s> len=<%d>\n", s1, len);
	int		i;
	char	*str;

	i = 0;
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	while (i != len)
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = '\0';
	//printf("final str<%s>\n", str);
	return (str);
}

void	free_list(t_list *env)
{
	t_node	*ptr;
	t_node	*next;

	ptr = env->head;
	next = ptr->next;
	while (ptr != NULL)
	{
		free(ptr->key);
		free(ptr->value);
		free(ptr);
		ptr = next;
		if (ptr != NULL)
			next = ptr->next;
	}
}

// void	ft_free(char **arr)
// {
// 	int	i;

// 	i = 0;
// 	while (arr[i])
// 	{
// 		free(arr[i]);
// 		i++;
// 	}
// 	free(arr);
// }
