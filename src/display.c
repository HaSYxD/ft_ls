/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasyxd <aliaudet@student.42lehavre.fr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:40:00 by hasyxd            #+#    #+#             */
/*   Updated: 2025/04/28 15:04:52 by hasyxd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <config.h>

static void	display_simple(dir_t *dirs, bool (*flags)[FLAG_COUNT], env_t *env)
{
	// Get the number of directory
	size_t	dir_count = 0;
	while (!IS_NULL_DIR(dirs[dir_count]))
			dir_count++;

	// Init the directory loop's iterator according to the flags
	int	j = dir_count - 1;
	int	dirinc = -1;
	if ((*flags)[REVERSE]) {
		dirinc = 1;
		j = 0;
	}
	

	while (!IS_NULL_DIR(dirs[j])) {
		// Loop's end condition for reverse flag
		if (j < 0)
			break ;

		// Get the number of count
		size_t	file_count = 0;
		while (dirs[j]._files[file_count])
			file_count++;

		// Print to the correct format if there is more than one directory to print (usr param or Recursive flag)
		if (dir_count > 1)
			ft_fprintf(1, "%s:\n", dirs[j]._name);

		// Init the file loop's iterator according to the flags
		int	i = 0;
		int	inc = 1;
		if ((*flags)[REVERSE]) {
			inc = -1;
			i = file_count - 1;
		}

		while (dirs[j]._files[i]) {
			// Loop's end condition for reverse flag
			if (i < 0)
				break ;

			if (!(*flags)[ALL] && dirs[j]._files[i]->_name[0] == '.') {
				i += inc;
				continue ;
			}
			ft_fprintf(1, "%s%s%s  ", env->_colors[dirs[j]._files[i]->_fileT], dirs[j]._files[i]->_name, "\e[0m");
			i += inc;
		}
		// Print to the correct format if there is more than one directory to print (usr param or Recursive flag)
		if (dir_count > 1 && ((dirinc == -1 && j != 0) || (dirinc == 1 && !IS_NULL_DIR(dirs[j + 1]))))
			ft_fprintf(1, "\n");
		ft_fprintf(1, "\n");
		j += dirinc;
	}
}

static void	display_long(dir_t *dirs, bool (*flags)[FLAG_COUNT], env_t *env)
{
	(void)env;
	(void)dirs;
	(void)flags;
}

void	display(dir_t *dirs, bool (*flags)[FLAG_COUNT], env_t *env)
{
	if ((*flags)[TIME] == true)
		sort_files(dirs[0]._files, true);

	if ((*flags)[LONG] == true)
		display_long(dirs, flags, env);
	else
		display_simple(dirs, flags, env);
}
