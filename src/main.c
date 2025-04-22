/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasyxd <aliaudet@student.42lehavre.fr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:11:55 by hasyxd            #+#    #+#             */
/*   Updated: 2025/04/22 17:00:49 by hasyxd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <config.h>

uint32_t	g_term_width = 0;

int	get_termw(const char **env)
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
	g_term_width = tgetnum("co");
	return (0);
}

int	main(const int argc, const char **argv, const char **env)
{
	bool		flags[FLAG_COUNT] = {false};
	t_garb		gc = init_gc();
	t_list *	fileArgs = NULL;

	if (get_termw(env) == -1)
		return (1);

	fileArgs = check_args(&flags, argv + 1, argc - 1, &gc);
	if (!fileArgs)
		return (-1);

	while (fileArgs) {
		dir_t	d = getfiles_at((char *)fileArgs->data, &gc);

		if (IS_NULL_DIR(d))
			return (1);

		for (int i = 0; d._files[i] != 0; i++)
			ft_fprintf(1, "%s %d %s %s %d %s %s\n", d._files[i]->_permissions, d._files[i]->_linksCount, d._files[i]->_owner, d._files[i]->_group, d._files[i]->_size, d._files[i]->_dateTime, d._files[i]->_name);
		fileArgs = fileArgs->next;
	}

	clean_garbage(&gc);
	return (0);
}
