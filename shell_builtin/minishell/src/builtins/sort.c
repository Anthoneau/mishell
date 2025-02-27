#include "minishell.h"

void	swap(char **s1, char **s2)
{
	char	*tmp;

	tmp = *s1;
	*s1 = *s2;
	*s2 = tmp;
}

void	sort(char **arr, int size)
{
	int	cmp;
	int	i;
	int	j;

	i = size - 1;
	while (i > 0)
	{
		j = 0;
		while (j < i)
		{
			cmp = ft_strncmp(arr[j], arr[j + 1], ft_strlen(arr[j]) + 1);
			if (cmp > 0)
				swap(&arr[j], &arr[j + 1]);
			j++;
		}
		i--;
	}
}