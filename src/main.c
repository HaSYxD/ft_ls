/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasyxd <aliaudet@student.42lehavre.fr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:11:55 by hasyxd            #+#    #+#             */
/*   Updated: 2025/04/17 19:13:19 by hasyxd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <config.h>

int	main(const int argc, const char **argv)
{
	bool		flags[FLAG_COUNT] = {false};
	t_garb		gc = init_gc();
	t_list *	fileArgs = NULL;

	fileArgs = check_args(&flags, argv + 1, argc - 1, &gc);
	if (!fileArgs)
		return (-1);
	
	while (fileArgs) {
		file_t **	files = getfiles_at((char *)fileArgs->data, &gc);

		if (!files)
			return (1);

		for (int i = 0; files[i] != 0; i++)
			ft_fprintf(1, "%s %d %s %s %d %s %s\n", files[i]->_permissions, files[i]->_linksCount, files[i]->_owner, files[i]->_group, files[i]->_size, files[i]->_dateTime, files[i]->_name);
		fileArgs = fileArgs->next;
	}

	clean_garbage(&gc);
	return (0);
}
