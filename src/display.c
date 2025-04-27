/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasyxd <aliaudet@student.42lehavre.fr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:40:00 by hasyxd            #+#    #+#             */
/*   Updated: 2025/04/27 05:02:33 by hasyxd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <config.h>

static void	display_simple(dir_t *dirs, bool (*flags)[FLAG_COUNT], env_t *env)
{
	if ((*flags)[REVERSE]) {
		int i = 0;

		while (dirs[0]._files[i])
			i++;
		while (--i >= 0) {
			if (!(*flags)[ALL] && dirs[0]._files[i]->_name[0] == '.')
				continue ;
			ft_fprintf(1, "%s%s%s  ", env->_colors[dirs[0]._files[i]->_fileT], dirs[0]._files[i]->_name, "\e[0m");
		}
		ft_fprintf(1, "\n");
		return ;
	}
	for (int i = 0; dirs[0]._files[i]; i++) {
		if (!(*flags)[ALL] && dirs[0]._files[i]->_name[0] == '.')
			continue ;
		ft_fprintf(1, "%s%s%s  ", env->_colors[dirs[0]._files[i]->_fileT], dirs[0]._files[i]->_name, "\e[0m");
	}
	ft_fprintf(1, "\n");
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
