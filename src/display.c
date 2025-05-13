/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasyxd <aliaudet@student.42lehavre.fr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:40:00 by hasyxd            #+#    #+#             */
/*   Updated: 2025/05/02 16:37:49 by hasyxd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <config.h>

static void	display_simple(file_t *file, env_t *env)
{
	ft_fprintf(1, "%s%s%s  ", env->_colors[file->_fileT], file->_name, "\e[0m");
}

static void	display_long(file_t *file, env_t *env)
{
	ft_fprintf(1, "%s %d %s %s %d %s %s%s%s",file->_permissions, file->_linksCount, file->_owner, file->_group, file->_size, file->_dateTime, env->_colors[file->_fileT], file->_name, "\e[0m");
	if (file->_fileT == FT_LINK)
		ft_fprintf(1, " -> %s", file->_linkTarget);
	ft_fprintf(1, "\n");
}

void	display(dir_t dir, bool (*flags)[FLAG_COUNT], env_t *env)
{
	// Get the number of count
	size_t	file_count = 0;
	while (dir._files[file_count])
		file_count++;

	// Print to the correct format if there is more than one directory to print (usr param or Recursive flag)
	if (env->_multi_entry_format)
		ft_fprintf(1, "%s:\n", dir._name);

	if ((*flags)[LONG])
		ft_fprintf(1, "total %d\n", dir._ttlblks);

	// Init the file loop's iterator according to the flags
	int	i = 0;
	int	inc = 1;
	if ((*flags)[REVERSE]) {
		inc = -1;
		i = file_count - 1;
	}

	while (dir._files[i]) {
		// Loop's end condition for reverse flag
		if (i < 0)
			break ;

		if (!(*flags)[ALL] && dir._files[i]->_name[0] == '.') {
			i += inc;
			continue ;
		}
		if ((*flags)[LONG])
			display_long(dir._files[i], env);
		else
			display_simple(dir._files[i], env);
		i += inc;
	}

	// Print to the correct format if there is more than one directory to print (usr param or Recursive flag)
	if (env->_multi_entry_format)
		ft_fprintf(1, "\n");
	ft_fprintf(1, "\n");
}
