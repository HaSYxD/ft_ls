/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasyxd <aliaudet@student.42lehavre.fr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 11:11:55 by hasyxd            #+#    #+#             */
/*   Updated: 2025/04/17 13:16:33 by hasyxd           ###   ########.fr       */
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
	
	
	test_dir(fileArgs);	// TO REMOVE

	clean_garbage(&gc);
	return (0);
}
