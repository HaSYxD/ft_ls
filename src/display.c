/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasyxd <aliaudet@student.42lehavre.fr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:40:00 by hasyxd            #+#    #+#             */
/*   Updated: 2025/04/28 19:09:05 by hasyxd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <config.h>

static void	display_simple(t_list *dirs, const size_t dcount, bool (*flags)[FLAG_COUNT], env_t *env)
{
	t_list *	start = dirs;

	while (dirs) {

		dir_t *	dir = (dir_t *)dirs->data;
		
		if ((*flags)[TIME] == true)
			sort_files(dir->_files, true);

		// Get the number of count
		size_t	file_count = 0;
		while (dir->_files[file_count])
			file_count++;

		// Print to the correct format if there is more than one directory to print (usr param or Recursive flag)
		if (dcount > 1)
			ft_fprintf(1, "%s:\n", dir->_name);

		// Init the file loop's iterator according to the flags
		int	i = 0;
		int	inc = 1;
		if ((*flags)[REVERSE]) {
			inc = -1;
			i = file_count - 1;
		}

		while (dir->_files[i]) {
			// Loop's end condition for reverse flag
			if (i < 0)
				break ;

			if (!(*flags)[ALL] && dir->_files[i]->_name[0] == '.') {
				i += inc;
				continue ;
			}
			ft_fprintf(1, "%s%s%s  ", env->_colors[dir->_files[i]->_fileT], dir->_files[i]->_name, "\e[0m");
			i += inc;
		}
		dirs = dirs->next;

		// Print to the correct format if there is more than one directory to print (usr param or Recursive flag)
		if (dcount > 1 && dirs)
			ft_fprintf(1, "\n");
		ft_fprintf(1, "\n");
	}
	dirs = start;
}

static void	display_long(t_list *dirs, const size_t dcount, bool (*flags)[FLAG_COUNT], env_t *env)
{
	(void)dcount;
	(void)env;
	(void)dirs;
	(void)flags;
}

void	display(t_list *dirs, const size_t dcount, bool (*flags)[FLAG_COUNT], env_t *env)
{
	if ((*flags)[LONG] == true)
		display_long(dirs, dcount, flags, env);
	else
		display_simple(dirs, dcount, flags, env);
}
