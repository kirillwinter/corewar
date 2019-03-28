/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_program_structure.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vrestles <vrestles@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 18:09:28 by vrestles          #+#    #+#             */
/*   Updated: 2019/03/28 20:41:36 by vrestles         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/asm.h"

static int 		count_command_definitions(t_tokens *tokens, char *str)
{
	int			i;
	int 		count;
	t_lex_list	*tmp;

	i = 0;
	count = 0;
	while (i < tokens->count)
	{
		tmp = tokens->tokenlst[i];
		while (tmp) {
			if (tmp->lexem->type == COMMAND &&
				(ft_strcmp(tmp->lexem->value, str) == 0))
				count++;
			tmp = tmp->next;
		}
		i++;
	}
	return (count);
}

static t_lexem	*find_redef_command(t_tokens *tokens, char *str)
{
	int			i;
	int 		count;
	t_lex_list	*tmp;
	t_lexem		*lexem;

	i = 0;
	count = 0;
	lexem = NULL;
	while (i < tokens->count)
	{
		tmp = tokens->tokenlst[i];
		while (tmp) {
			if (tmp->lexem->type == COMMAND &&
				(ft_strcmp(tmp->lexem->value, str) == 0))
			{
				if (count == 1)
				{
					lexem = tmp->lexem;
					break ;
				}
				count++;
			}
			tmp = tmp->next;
		}
		i++;
	}
	return (lexem);
}

void			not_valid_count_commands(t_tokens *tokens, t_errors **err, int name_count, int comment_count)
{
	t_lexem	*lexem;

	if (name_count == 0)
		push_back_errors_list(err, SEMANTIC, COMMAND_NAME_NOT_FOUND, NULL);
	else if (name_count > 1)
	{
		lexem = find_redef_command(tokens, "name");
		push_back_errors_list(err, SEMANTIC, COMMAND_REDEFINITION, lexem);
	}
	if (comment_count == 0)
		push_back_errors_list(err, SEMANTIC, COMMAND_COMMENT_NOT_FOUND, NULL);
	else if (comment_count > 1)
	{
		lexem = find_redef_command(tokens, "comment");
		push_back_errors_list(err, SEMANTIC, COMMAND_REDEFINITION, lexem);
	}
}

void			check_command_size(t_lexem *lexem, t_errors **error)
{
	if (ft_strcmp(lexem->value, "name"))
		if (ft_strlen(lexem->value) >= PROG_NAME_LENGTH)
			push_back_errors_list(error, SEMANTIC, EXCESS_SIZE_COMMAND, lexem);
	if (ft_strcmp(lexem->value, "comment"))
		if (ft_strlen(lexem->value) >= COMMENT_LENGTH)
			push_back_errors_list(error, SEMANTIC, EXCESS_SIZE_COMMAND, lexem);
}

void		find_command_line_errors(t_lex_list *lexem_lst, t_errors **error)
{
	t_lex_list	*head;

	head = lexem_lst;
	if (!lexem_lst->next)
		return (push_back_errors_list(error, SEMANTIC, NO_ARGUMENTS, lexem_lst->lexem));
	lexem_lst = lexem_lst->next;
	if (lexem_lst->lexem->type != STRING)
		push_back_errors_list(error, SEMANTIC, INVALID_TYPE_OF_ARGUMENT, lexem_lst->lexem);
	else
		check_command_size(lexem_lst->lexem, error);
	if (lexem_lst->next)
		return (push_back_errors_list(error, SEMANTIC, INVALID_NUMBER_OF_ARGUMENT, head->lexem));
}

void			check_commands(t_tokens *tokens, t_errors **err)
{
	int 		name_count;
	int 		comment_count;
	int 		i;
	t_lex_list	*tmp;

	name_count = count_command_definitions(tokens, "name");
	comment_count = count_command_definitions(tokens, "comment");
	i = 0;
	if (name_count == 0 || name_count > 1 || comment_count == 0 || comment_count > 1)
	{
		not_valid_count_commands(tokens, err, name_count, comment_count);
		return ;
	}
	while (i < tokens->count)
	{
		if ((tmp = tokens->tokenlst[i]) != NULL)
		{
			if (tmp->lexem->type == COMMAND)
				find_command_line_errors(tmp, err);
		}
		i++;
	}
}
