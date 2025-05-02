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

static void	display_simple(dir_t dir, bool (*flags)[FLAG_COUNT], env_t *env)
{
	// Get the number of count
	size_t	file_count = 0;
	while (dir._files[file_count])
		file_count++;

	// Print to the correct format if there is more than one directory to print (usr param or Recursive flag)
	if (env->_multi_entry_format)
		ft_fprintf(1, "%s:\n", dir._name);

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
		ft_fprintf(1, "%s%s%s  ", env->_colors[dir._files[i]->_fileT], dir._files[i]->_name, "\e[0m");
		i += inc;
	}

	// Print to the correct format if there is more than one directory to print (usr param or Recursive flag)
	if (env->_multi_entry_format)
		ft_fprintf(1, "\n");
	ft_fprintf(1, "\n");
}

static void	display_long(dir_t dirs, bool (*flags)[FLAG_COUNT], env_t *env)
{
	(void)env;
	(void)dirs;
	(void)flags;
}

void	display(dir_t dir, bool (*flags)[FLAG_COUNT], env_t *env)
{
	if ((*flags)[LONG] == true)
		display_long(dir, flags, env);
	else
		display_simple(dir, flags, env);
}
