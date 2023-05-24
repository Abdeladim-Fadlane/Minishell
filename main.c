/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afadlane <afadlane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 12:01:15 by afadlane          #+#    #+#             */
/*   Updated: 2023/05/24 21:58:16 by afadlane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	strim_her_doc(char *s, char *buff)
{
	if (buff == NULL)
	{
		free(buff);
		exit(0);
	}
	if (ft_strcmp(s, buff) == 0)
		exit(0);
}

void	her_doc(char *s, t_env *p)
{
	int		pid;
	char	*buff;

	sig->signal = 1;
	pipe(p->fd2);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		while (1)
		{
			buff = readline(">");
			strim_her_doc(s, buff);
			if (buff[0] == '\0')
			{
				free(buff);
				continue ;
			}
			ft_putstr_fd(buff, p->fd2[1]);
			ft_putstr_fd("\n", p->fd2[1]);
		}
	}
	close(p->fd2[1]);
	wait(NULL);
	sig->signal = 0;
}

void	__main__(t_env *lst, t_minishell *list)
{
	t_minishell	*p;
	t_object	*obj;

	p = list;
	obj = NULL;
	while (list)
	{
		while (list->redirct)
		{
			if (list->redirct->type == INF)
				open_infile(list->redirct->in, list);
			if (list->redirct->type == OUT)
				open_outfile(list->redirct->out, list);
			if (list->redirct->type == APE)
				open_append(list->redirct->out, list);
			if (list->redirct->type == DEL)
			{
				t_lstadd_back(&obj, t_lstnew(list->redirct->limiter));
				lst->n = 1;
			}
			list->redirct = list->redirct->next;
		}
		list = list->next;
	}
	pipex(p, lst, obj);
}

void	trim_pipex(t_minishell *lst, t_env *p, char **ptr)
{
	pipe(p->fd);
	first_proccess(p, ptr, lst->all_cmds, lst);
	p->flag_fd = p->fd[0];
	close(p->fd[1]);
	lst = lst->next;
	while (2 <= lstsize(lst))
	{
		if (lst->pipe == 1)
		{
			pipe(p->fd);
			midlle_proccess(p, ptr, lst->all_cmds, lst);
			close(p->fd[1]);
			close(p->flag_fd);
			p->flag_fd = p->fd[0];
		}
		lst = lst->next;
	}
	last_proccess(p, ptr, lst->all_cmds, lst);
	close(p->flag_fd);
}

void	pipex(t_minishell *lst, t_env *p, t_object *obj)
{
	char	**ptr;

	ptr = ft_split(check_env(p), ':');
	if (p->n >= 1)
	{
		main_herdoc(p, obj);
		if (!lst->all_cmds[0])
			return ;
	}
	if (lstsize(lst) == 1)
		one_command(ptr, lst->all_cmds, lst, p);
	else
		trim_pipex(lst, p, ptr);
	exitstatus();
}
