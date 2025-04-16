/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usrInHandling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasyxd <aliaudet@student.42lehavre.fr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 13:10:19 by hasyxd            #+#    #+#             */
/*   Updated: 2025/04/16 14:18:34 by hasyxd           ###   ########.fr       */
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

int	check_args(bool (*flags)[FLAG_COUNT], const char **args, const size_t count)
{
	flagid_t	flagID;

	for (size_t i = 0; i < count; i++) {
		if (args[i][0] != '-')
			return (ft_fprintf(2, "Error: \"%s\" is an invalid argument (not a flag)\n", args[i]), -1);

		size_t	dashCount = ft_strinstcount(args[i], '-');

		if (dashCount == 1)
			flagID = handle_shortFlag(flags, args[i] + 1);
		else if (dashCount == 2)
			flagID = handle_longFlag(args[i] + 2);
		else if (dashCount > 2)
			return (ft_fprintf(2, "Error: \"%s\" is an invalid argument (too many dashes)\n", args[i]), -1);

		if (flagID == -1)
			return (-1);
		(*flags)[flagID] = true;
	}
	return (0);
}
	
