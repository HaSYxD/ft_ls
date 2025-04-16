/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasyxd <aliaudet@student.42lehavre.fr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:11:55 by hasyxd            #+#    #+#             */
/*   Updated: 2025/04/16 13:59:46 by hasyxd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <config.h>

int	main(const int argc, const char **argv)
{
	bool		flags[FLAG_COUNT] = {false};

	check_args(&flags, argv + 1, argc - 1);
	for (int i = 0; i < FLAG_COUNT; i++) {
		if (flags[i])
			ft_fprintf(1, "%s: is present\n", LONG_FLAGS[i]);
	}
	return (0);
}
