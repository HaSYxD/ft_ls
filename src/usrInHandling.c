/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usrInHandling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasyxd <aliaudet@student.42lehavre.fr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 13:10:19 by hasyxd            #+#    #+#             */
/*   Updated: 2025/04/28 18:44:15 by hasyxd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <config.h>

static int	handle_shortFlag(bool (*flags)[FLAG_COUNT], const char *str)
{
	flagid_t	lastFlag = -1;

	for (int i = 0; str[i]; i++) {
		lastFlag = -1;
		for (int j = 0; j < FLAG_COUNT; j++) {
			if (str[i] == SHORT_FLAGS[j]) {
				lastFlag = j;
				(*flags)[j] = true;
			}
		}
		if (lastFlag == -1)
			return (ft_fprintf(2, "Error: \"%s\"(%d) is not a known flag\n", str, i), -1);
	}
	return (lastFlag);
}

static int	handle_longFlag(const char *str)
{
	for (int i = 0; i < FLAG_COUNT; i++) {
		if (ft_strncmp(str, LONG_FLAGS[i], ft_strlen(LONG_FLAGS[i]) + 1) == 0)
			return (i);
	}
	ft_fprintf(2, "Error: \"%s\" is not a known flag\n", str);
	return (-1);
}

static t_list *	handle_filesArgs(t_list *fileArg, const char *str, arena_t *a)
{
	DIR *	dir = opendir(str);

	if (!dir) {
		ft_fprintf(2, "Error: \"%s\" no such file or directory\n", str);
		return (NULL);
	}

	ft_lstadd_back(&fileArg, ft_lstnew(a, (void *)str));
	closedir(dir);
	return (fileArg);
}

t_list *	check_args(bool (*flags)[FLAG_COUNT], const char **args, const size_t count, arena_t *a)
{
	flagid_t	flagID = -2;
	t_list *	fileArg = NULL;

	for (size_t i = 0; i < count; i++) {

		size_t	dashCount = ft_strinstcount(args[i], '-');

		if (args[i][0] != '-')
			fileArg = handle_filesArgs(fileArg, args[i], a);
		
		if (args[i][0] == '-' && dashCount == 1)
			flagID = handle_shortFlag(flags, args[i] + 1);
		else if (args[i][0] == '-' && dashCount == 2)
			flagID = handle_longFlag(args[i] + 2);
		else if (dashCount > 2)
			return (ft_fprintf(2, "Error: \"%s\" is an invalid argument (too many dashes)\n", args[i]), NULL);

		if (flagID == -1)
			return (NULL);
		(*flags)[flagID] = true;
	}
	if (!fileArg)
		fileArg = ft_lstnew(a, ".");
	return (fileArg);
}
