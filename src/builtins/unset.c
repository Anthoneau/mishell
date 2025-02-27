#include "minishell.h"

int	unset(t_list *list, char *s)
{
	printf("unset\n");
	t_node  *ptr;
	t_node  *tmp;
	int     cmp;
	
	ptr = list->head;
	while (ptr != NULL)
	{
		cmp = ft_strncmp(s, ptr->key, ft_strlen(s));
		if (cmp == 0)
		{
			/////////////
			printf("\n\nXXXXXXXXXXXXXXXX\n");
			printf("ptr = <%s> ptr->prev = <%s>, ptr->next = %p\n", ptr->key, ptr->prev->key, ptr->next); 
			////////////
			if (ptr->prev != NULL)
			{
				tmp = ptr->prev;
				tmp->next = ptr->next;
				if (ptr->next == NULL)
					list->tail = tmp;
			}
			/////////////
			printf("tmp = <%s> tmp->next = <%s>\n", tmp->key, tmp->next->key);
			// printf("{tail->next = NULL} tmp = <%s> tmp->next = %p\n", tmp->key, tmp->next);
			////////////
			if (ptr->next != NULL)
			{
				tmp = ptr->next;
				tmp->prev = ptr->prev;
				if (ptr->prev == NULL)
					list->head = tmp;
			}
			////////////
			printf("tmp = <%s> tmp->prev = <%s>\n", tmp->key, tmp->prev->key);
			// printf("{head->prev = NULL} tmp = <%s> tmp->prev = %p\n", tmp->key, tmp->prev);
			printf("XXXXXXXXXXXXXXXX\n\n");
			////////////
			free(ptr);
			return (0);
		}
		ptr = ptr->next;
	}
}