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

void	export_order(t_list *list)
{
	char	**export;
	int     i;

	export = get_env(list);
	sort(export, list->size);
	i = 0;
	while (export[i])
		printf("declare -x %s\"\n", export[i++]);
	free_array(export);
}

void	add_node(t_list2 **add_key, char *str)
{
	t_list2	*new_node;
	t_list2 *ptr;

	new_node = malloc(sizeof(t_list2));
	if (!new_node)
		return ;
	new_node->key = ft_strdup(str);
	new_node->value = NULL;
	new_node->next = NULL;
	ptr = (*add_key);
	if ((*add_key) != NULL)
	{
		printf("list = %p ptr = %p node++\n", (*add_key), ptr);
		while(ptr->next != NULL)
			ptr = ptr->next;
		printf("%p\n", ptr);
		ptr->next = new_node;
	}
	if ((*add_key) == NULL)
	{
		printf("first node\n");
		(*add_key) = new_node;
	}
}

int	export(t_list *list, char **arg)
{
	int		i;
	t_list2	*ptr;

	i = 0;
	if (arg[i] == NULL)
	{
		export_order(list);
		// if (list->add_key)
		// 	export_order(list->add_key);
		return (0);
	}
	else
	{
		while(arg[i])
		{
			add_node(&list->add_key, arg[i++]);
		}
		ptr = list->add_key;
		while (ptr != NULL)
		{
			printf("%s\n", ptr->key);
			ptr = ptr->next;
		}
		printf("%p key = %s and %d\n", list->add_key, list->add_key->key, list_len(&list->add_key));
		printf("fin add_node\n");
		return (0);
	}
	return (1);
}
