/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmonserr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/30 12:43:55 by rmonserr          #+#    #+#             */
/*   Updated: 2019/09/30 12:43:56 by rmonserr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

t_gnl		*new_list(const int fd)
{
	t_gnl	*new;

	if (!(new = (t_gnl *)malloc(sizeof(t_gnl))))
		return (NULL);
	new->fd = fd;
	new->rem = ft_strnew(BUFF_SIZE);
	new->next = NULL;
	return (new);
}

char		*check_rem(char **p_nl, char *rem)
{
	char *str;

	if ((*p_nl = ft_strchr(rem, '\n')) != NULL)
	{
		str = ft_strsub(rem, 0, *p_nl - rem);
		ft_strcpy(rem, ++(*p_nl));
	}
	else
	{
		str = ft_strnew(ft_strlen(rem) + 1);
		ft_strcat(str, rem);
		ft_strclr(rem);
	}
	return (str);
}

int			get_line(const int fd, char **line, char *rem)
{
	char			buf[BUFF_SIZE + 1];
	char			*p_nl;
	char			*tmp;
	int				read_result;

	p_nl = NULL;
	read_result = 1;
	*line = check_rem(&p_nl, rem);
	while (p_nl == 0 && ((read_result = read(fd, buf, BUFF_SIZE)) != 0))
	{
		buf[read_result] = '\0';
		if ((p_nl = ft_strchr(buf, '\n')) != NULL)
		{
			ft_strcpy(rem, ++p_nl);
			ft_strclr(--p_nl);
		}
		tmp = *line;
		if (!(*line = ft_strjoin(tmp, buf)) || read_result < 0)
			return (-1);
		ft_strdel(&tmp);
	}
	if (ft_strlen(*line) || ft_strlen(rem) || read_result)
		return (1);
	else
		return (0);
}

int			get_next_line(const int fd, char **line)
{
	static t_gnl	*list;
	t_gnl			*tmp;
	int				ret;

	if (fd < 0 || line == 0)
		return (-1);
	if (!list)
		list = new_list(fd);
	tmp = list;
	while (tmp->fd != fd)
	{
		if (tmp->next == NULL)
			tmp->next = new_list(fd);
		tmp = tmp->next;
	}
	ret = get_line(fd, line, tmp->rem);
	return (ret);
}