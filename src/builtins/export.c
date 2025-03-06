#include "minishell.h"

char	*fill_export(t_node *list)
{
	char	*key;
	char	*str;

	if (!list || !list->key)
		return (NULL);
	if (!list->value)
		return (ft_strdup(list->key));
	key = ft_strjoin(list->key, "=");
	if (!key)
		return (NULL);
	str = ft_strjoin(key, list->value);
	free(key);
	if (!str)
		return (NULL);
	return (str);
}

char	**export_arr(t_list *list)
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
		printf("key = %s\n", ptr->key);
		tab[i] = fill_export(ptr);
		printf("tab = %s i = <%d>\n", tab[i], i);
		if (!tab[i])
			return (free_array(tab), NULL);
		i++;
		ptr = ptr->next;
	}
	tab[i] = NULL;
	return (tab);
}

void	export_order(t_list *list)
{
	char	**export;
	int     i;

	export = export_arr(list);
	sort(export, list->size);
	i = 0;
	while (export[i])
		printf("declare -x %s\n", export[i++]);
	free_array(export);
}

char	*fill_value(char *str, bool button)
{
	if (ft_strchr(str, '='))
		return (get_value(str, button));
	else
	{
		if (button == false)
			return (ft_strdup(str));
		else
			return (NULL);
	}
}



int	export(t_list *list, char **arg)
{
	int		i;
	t_node	*ptr;

	i = 0;
	if (arg[i] == NULL)
	{
		export_order(list);
		return (0);
	}
	else
	{
		while(arg[i])
		{
			get_list(list, arg[i++]);
		}
		ptr = list->head;
		while (ptr != NULL)
		{
			printf("%s\n", ptr->key);
			ptr = ptr->next;
		}
		// printf("%p key = %s and %d\n", list->add_key, list->add_key->key, list_len(&list->add_key));
		printf("list size = %d\n", list->size);
		printf("fin add_node\n");
		return (0);
	}
	return (1);
}
