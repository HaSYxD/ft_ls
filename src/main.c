/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasyxd <aliaudet@student.42lehavre.fr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:11:55 by hasyxd            #+#    #+#             */
/*   Updated: 2025/05/02 16:36:58 by hasyxd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <config.h>

static int	get_envcolor(const char **env, env_t *data, arena_t *a)
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
				data->_colors[i] = ft_strjoin("\e[", w + ft_strlenc(w, '=') + 1, (alloc_ctx_t){a, ARENA});
				data->_colors[i] = ft_strjoin(data->_colors[i], "m", (alloc_ctx_t){a, ARENA});
			}
		}
	}

	// Set missing color values to the default color
	for (int i = 0; i < LS_COLORSCOUNT; i++)
		if (!data->_colors[i])
			data->_colors[i] = ft_strdup("\e[0m", (alloc_ctx_t){a, ARENA});

	return (0);
}

int	main(const int argc, const char **argv, const char **env)
{
	arena_t *	env_arena = arena_init(ARENA_SMALL);
	t_garb		gc = (t_garb){NULL, 0};
	env_t		data = {0, {NULL}, false};

	if (get_envcolor(env, &data, env_arena) == -1)
		return (1);

	bool		flags[FLAG_COUNT] = {false};
	t_list *	fileArgs = check_args(&flags, argv + 1, argc - 1, &gc);
	if (!fileArgs)
		return (-1);

	if (ft_lstsize(fileArgs) > 1 || flags[RECURSIVE])
		data._multi_entry_format = true;

	while (fileArgs) {
		getfiles_at((char *)fileArgs->data, &flags, &data, &fileArgs, &gc);

		t_list *	next = fileArgs->next;

		deallocate(fileArgs, &gc);
		fileArgs = next;
	}
	arena_destroy(env_arena);
	clean_garbage(&gc);
	return (0);
}
