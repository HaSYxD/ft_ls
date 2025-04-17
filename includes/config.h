/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasyxd <aliaudet@student.42lehavre.fr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 13:06:15 by hasyxd            #+#    #+#             */
/*   Updated: 2025/04/17 18:10:16 by hasyxd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_H
# define CONFIG_H

# include <unistd.h>
# include <stdint.h>
# include <dirent.h>
# include <sys/stat.h>
# include <time.h>
# include <pwd.h>
# include <grp.h>
# include <limits.h>

# include <libft.h>

// Flag defines
# define	FLAG_COUNT	5
# define	SHORT_FLAGS	"lRart"
# define	LONG_FLAGS	(char*[]){"long", "recursive", "all", "reverse", "time"}

// File defines
# define	MODES	(char*[]){"---", "--x", "-w-", "-wx", \
				"r--", "r-x", "rw-", "rwx"}

typedef int8_t	flagid_t;

enum	flagid_e {
	LONG = 0,
	RECURSIVE,
	ALL,
	REVERSE,
	TIME
};

typedef struct file_s {
	ino_t			_UUID;
	char			_permissions[11];
	uint32_t		_linksCount;
	char *			_owner;
	char *			_group;
	size_t			_size;
	char *			_dateTime;
	time_t			_timestamp;
	char *			_name;
	struct file_s **	_next;
}	file_t;

t_list *	check_args(bool (*flags)[FLAG_COUNT], const char **args, const size_t count, t_garb *gc);

void	test_dir(t_list *fileArgs);
file_t **	getfiles_at(const char *path, t_garb *gc);

#endif
