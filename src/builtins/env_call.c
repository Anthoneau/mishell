# include "minishell.h"

int call_env(t_list *list)
{
    t_node *ptr;

    ptr = list->head;
    while (ptr != NULL)
    {
        printf("%s=%s\n", ptr->key, ptr->value);
        ptr = ptr->next;
    }
    return (0);
}