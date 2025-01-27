/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_ast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agoldber <agoldber@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 12:18:52 by agoldber          #+#    #+#             */
/*   Updated: 2025/01/27 11:00:22 by agoldber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*find_first_word_to_the_left(t_token *current)
{
	/*
		Fonction pour chercher le premier mot a retourner quand on cherche a gauche
		Le fonctionnement est explique avec l'exemple, et en gros les points "." sont la pour
			representer la ou pointe current (ou plutot ou pointe les checks)
		exemple :
		cmd arg | cmd
				.
			.
		.
		imaginons que current pointe sur le pipe (le premier ".") et qu'on cherche un mot a gauche,
			on vient sur "arg", on voit que explored == 0 donc trop bien on a trouve le mot !
		Non, parce que cmd est toujours derriere et c'est lui qu'on veut, donc on va en arriere
			et on check si on peut trouver un mot qui a pas encore ete explore
		On le fait jusqu'a, soit tomber sur le debut de la liste, soit tomber sur un token deja explore

		exemple 2 :
		cmd1 arg1 | cmd2 arg2 | cmd
							  .
						 .
					.
				  .
				  	.
		La on a current sur le deuxieme pipe, on va en arriere jusqu'a trouver
			un autre pipe (donc un token deja explore ou pas du meme type),
			donc on revient en avant
	*/
	while (current->explored != 1)
	{
		if (current->prev && current->prev->type == 0 && current->explored == 0)
			current = current->prev;
		else
			break ;
	}
	return (current);	
}

t_token	*find_redir(t_token *current, int side)
{
	/*
		Fonction pour trouver la bonne redirection a envoyer
		Elle fonctionne comme find_first_word_to_the_left() (la fonction au dessus) mais pour les redirs
	*/
	t_token	*check;

	check = current;
	if (side == 1)
	{
		while (check->explored != 1)
		{
			if (check->type >= 2)
				current = check;
			if (check->prev)
				check = check->prev;
			else
				break ;
		}
	}
	return (current);
}

t_token	*find_heredoc(t_token *current, int side)
{
	/*
		Fonction pour trouver la bonne redirection a envoyer
		Elle fonctionne comme find_first_word_to_the_left() (la fonction au dessus) mais pour les redirs
	*/
	t_token	*check;

	check = current;
	if (side == 1)
	{
		while (check->explored != 1)
		{
			if (check->type == 6)
				current = check;
			if (check->prev)
				check = check->prev;
			else
				break ;
		}
	}
	return (current);
}

t_token	*search_type(t_token **list, t_token *current, int type, int side)
{
	/*
		Fonction pour chercher un type particulier

		La fonction recoit la liste des tokens, un token current, le type qu'on cherche et le cote ou chercher
		Si current est NULL (c'est toujours le cas au premier appel mais jamais apres)
			on le fait pointer sur la tete de la liste de tokens
		On boucle sur la valeur "explored" de current et on vient check si le type correspond avec ce qu'on cherche
		
		Si le type cherche est soit un mot soit un pipe (0 ou 1) (et que le token current n'a pas encore ete vu) :
			- si c'est un pipe qu'on trouve, on retourne directement
			- si c'est un mot et qu'on cherche a droite, on retourne directement
			- si c'est un mot et qu'on cherche a gauche, on retourne le premier mot avec find_first_word_to_the_left
		Sinon si c'est une redir (type 2-5) (+token pas encore vu) :
			- le noeud est retourne

		Si aucune de ces conditions sont vraies, alors on fait avancer ou reculer current par rapport au side
		Si on peut plus reculer ou avancer, on break et on retourne NULL

		En gros : c'est juste une serie de check pour trouver le bon token
	*/
	if (!list)
		return (NULL);
	if (!current)
		current = *list;
	while (current->explored != 1)
	{
		if (type < 2 && current->type == type && current->explored == 0)
		{
			if (side == 0 || type == 1)
				return (current);
			else
				return(find_first_word_to_the_left(current));
		}
		else if (type == R_HEREDOC && current->type == R_HEREDOC && current->explored == 0)
			return (find_heredoc(current, side));
		else if ((type >= 2 && type != R_HEREDOC) && current->type >= 2 && current->explored == 0)
			return (find_redir(current, side));
		if (side == 0 && current->next)
			current = current->next;
		else if (side == 1 && current->prev)
			current = current->prev;
		else
			break ;
	}
	return (NULL);
}

t_ast	*pipe_node(t_token **tokens, t_token *current, int *error)
{
	/*
		Fonction pour creer un pipe
		On initialise simplement les valeurs de t_ast et on donne la fonction create_ast aux "branches"
	*/
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
	{
		*error = 1;
		return (NULL);
	}
	current->explored = 1;
	node->type = PIPE;
	node->content = NULL;
	node->top = NULL;
	node->left = NULL;
	node->right = NULL;
	node->left = create_ast(tokens, current->prev, 1, error);
	if (*error == 1)
		return (free_error_node(node), NULL);
	if (node->left)
		node->left->top = node;
	node->right = create_ast(tokens, current->next, 0, error);
	if (*error == 1)
		return (free_error_node(node), NULL);
	if (node->right)
		node->right->top = node;
	return (node);
}

t_ast	*create_ast(t_token **tokens, t_token *current, int after_explored, int *error)
{
	/*
		Fonction principale de l'ast
		La fonction est recursive a un certain point parce qu'elle s'appelle
			elle meme dans les fonctions suivantes
		t_token se presente comme suit :
				char			*content;
				int				expand;
				int				type;
				int				explored;
				int				error;
				struct s_token	*next;
				struct s_token	*prev;

		La fonction prend en argument :
			- la liste des tokens
			- un token "current",
			- un flag pour savoir si on check avant ou apres le token current (utile uniquement pour search_type())
			- un flag error
		
		main envoie une liste de token viable, un current NULL, un after_explored = 0 et un error = 0
		Fonctionnement en gros :
			on a un token check et un noeud de l'ast "node",
			on initialise node a NULL puis check va recevoir le resultat de search_type; si il existe on rempli node
			on check a la fin si le pointer sur error = 1, si oui on free ce qu'il faut et on retourne NULL,
				si non on retourne le noeud
		
		En gros on verifie le type d'un token et on en fait un noeud
	*/
	// printf("on parse\n");
	t_token	*check;
	t_ast	*node;

	//printf("error == %d\n", *error);
	//sleep(1);
	node = NULL;
	if ((check = search_type(tokens, current, PIPE, after_explored)) && *error == 0)
	{
		//printf("on trouve un pipe\n");
		//sleep(1);
		//printf("-------------------------------\n\n");
		node = pipe_node(tokens, check, error);
	}
	else if ((check = search_type(tokens, current, R_HEREDOC, after_explored)) && *error == 0)
	{
		//printf("on trouve une redir\n");
		//sleep(1);
		//printf("-------------------------------\n\n");
		node = redir_node(tokens, check, error);
	}
	else if ((check = search_type(tokens, current, 2, after_explored)) && *error == 0)
	{
		//printf("on trouve une redir\n");
		//sleep(1);
		//printf("-------------------------------\n\n");
		node = redir_node(tokens, check, error);
	}
	else if ((check = search_type(tokens, current, WORD, after_explored)) && *error == 0)
	{
		//printf("on trouve un mot\n");
		//sleep(1);
		//printf("-------------------------------\n\n");
		node = word_node(check, error);
	}
	//printf("on check error\n");
	if (*error == 1)
	{
		//printf("error == 1 donc on free ce qu'il faut");
		return (free_error_node(node), NULL);
	}
	//printf("error == 0\n");
	return (node);
}
