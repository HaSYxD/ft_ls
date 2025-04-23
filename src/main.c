/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasyxd <aliaudet@student.42lehavre.fr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:11:55 by hasyxd            #+#    #+#             */
/*   Updated: 2025/04/23 15:56:49 by hasyxd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <config.h>

uint32_t	g_term_width = 0;

int	get_termw(const char **env, env_t *data)
{
	// Get the position of the TERM environement variable
	int	i = 0;
	while (ft_strncmp(env[i], "TERM=", 5) != 0)
		i++;
	if (!env[i]) {
		ft_fprintf(2, "Error: \"TERM\" environement variable was not found\n");
		return (-1);
	}

	// Load terminfo database
	char	term_buff[1024] = {0};
	int	status = tgetent(term_buff, env[i] + 5);
	if (status == -1)
		return (-1);

	// recover the number of column
	data->_term_width = tgetnum("co");
	return (0);
}

static int	get_envcolor(const char **env, env_t *data, t_garb *gc)
{
	// Get the position of the LS_COLORS environement variable
	int	i = 0;
	while (ft_strncmp(env[i], "LS_COLORS=", 10) != 0)
		i++;
	if (!env[i]) {
		ft_fprintf(2, "Error: \"LS_COLORS=\" environement variable was not found\n");
		return (-1);
	}

	// Parse each value of the LS_COLORS variable to assign to the environement data colors
	char	w[16] = {0};
	size_t	off = ft_strlenc(env[i], '=');
	size_t	s = ft_strlenc(env[i] + off, ':');
	ft_strlcpy(w, env[i] + off + 1, s);
	while (s) {
		off += s;
		s = ft_strlenc(env[i] + off + 1, ':');
		ft_strlcpy(w, env[i] + off + 1, s + 1);
		off += 1;
		for (int i = 0; i < LS_COLORSCOUNT; i++) {
			if (ft_strncmp(w, LS_COLORSENT[i], 3) == 0) {
				if (data->_colors[i])
					break ;
				data->_colors[i] = ft_strjoin("\e[", w + ft_strlenc(w, '=') + 1, gc);
				data->_colors[i] = ft_strjoin(data->_colors[i], "m", gc);
			}
		}
	}

	// Set missing color values to the default color
	for (int i = 0; i < LS_COLORSCOUNT; i++)
		if (!data->_colors[i])
			data->_colors[i] = ft_strdup("\e[0m", gc);

	return (0);
}

int	main(const int argc, const char **argv, const char **env)
{
	bool		flags[FLAG_COUNT] = {false};
	t_garb		gc = init_gc();
	t_list *	fileArgs = NULL;
	env_t		data = {0, {NULL}};

	if (get_termw(env, &data) == -1)
		return (1);

	if (get_envcolor(env, &data, &gc) == -1)
		return (1);

	fileArgs = check_args(&flags, argv + 1, argc - 1, &gc);
	if (!fileArgs)
		return (-1);

	while (fileArgs) {
		dir_t	d = getfiles_at((char *)fileArgs->data, &gc);

		if (IS_NULL_DIR(d))
			return (1);

		for (int i = 0; d._files[i] != 0; i++)
			ft_fprintf(1, "%s %d %s %s %d %s %s%s%s\n", d._files[i]->_permissions, d._files[i]->_linksCount, d._files[i]->_owner, d._files[i]->_group, d._files[i]->_size, d._files[i]->_dateTime, data._colors[d._files[i]->_fileT], d._files[i]->_name, "\e[0m");
		fileArgs = fileArgs->next;
	}

	clean_garbage(&gc);
	return (0);
}
