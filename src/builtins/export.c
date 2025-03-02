#include "minishell.h"

char	*fill_tab(t_node *list)
{
	char	*key;
	char	*str;

	if (!list || !list->value || !list->key)
		return (NULL);
	key = ft_strjoin(list->key, "=");
	if (!key)
		return (NULL);
	str = ft_strjoin(key, list->value);
	free(key);
	if (!str)
		return (NULL);
	return (str);
}

char	**get_env(t_list *list)
{
	t_node	*ptr;
	char	**tab;
	int		i;

	if (!list || !list->head)
		return (NULL);
	ptr = list->head;
	i = 0;
	tab = malloc(sizeof(char *) * (list->size + 1));
	if (!tab)
		return (NULL);
	while (ptr)
	{
		tab[i] = fill_tab(ptr);
		if (!tab[i])
			return (free_array(tab), NULL);
		i++;
		ptr = ptr->next;
	}
	tab[i] = NULL;
	return (tab);
}

int	export_order(t_list *list)
{
	char	**export;
	int     i;

	export = get_env(list);
	sort(export, list->size);
	i = 0;
	while (export[i])
		printf("declare -x %s\n", export[i++]);
	free_array(export);
	return (0);
}