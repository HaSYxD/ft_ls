/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasyxd <aliaudet@student.42lehavre.fr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:30:00 by hasyxd            #+#    #+#             */
/*   Updated: 2025/04/16 17:04:15 by hasyxd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <config.h>

void	test_dir(t_list *fileArgs)
{
	while (fileArgs) {
		DIR *	dir = opendir((char *)fileArgs->data);

		struct dirent *	dirDT = readdir(dir);

		ft_fprintf(1, "%s:\n", (char *)fileArgs->data);
		while (dirDT) {
			ft_fprintf(1, "%s ", dirDT->d_name);
			dirDT = readdir(dir);
		}
		ft_fprintf(1, "\n\n");
		closedir(dir);
		fileArgs = fileArgs->next;
	}
}
