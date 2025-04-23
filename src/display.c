/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasyxd <aliaudet@student.42lehavre.fr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 11:40:00 by hasyxd            #+#    #+#             */
/*   Updated: 2025/04/22 20:04:33 by hasyxd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <config.h>

static void	display_simple(dir_t *dirs, bool (*flags)[FLAG_COUNT])
{
	(void)dirs;
	(void)flags;
}

static void	display_long(dir_t *dirs, bool (*flags)[FLAG_COUNT])
{
	(void)dirs;
	(void)flags;
}

void	display(dir_t *dirs, bool (*flags)[FLAG_COUNT])
{
	if ((*flags)[TIME] == true)
		sort_files(dirs[0]._files, true);

	if ((*flags)[LONG] == true)
		display_long(dirs, flags);
	else
		display_simple(dirs, flags);
}
