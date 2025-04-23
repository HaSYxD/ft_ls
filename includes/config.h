/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasyxd <aliaudet@student.42lehavre.fr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 13:06:15 by hasyxd            #+#    #+#             */
/*   Updated: 2025/04/23 15:51:29 by hasyxd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_H
# define CONFIG_H

# include <unistd.h>
# include <stdint.h>
# include <dirent.h>
# include <sys/stat.h>
# include <term.h>
# include <time.h>
# include <pwd.h>
# include <grp.h>
# include <limits.h>

# include <libft.h>

extern uint32_t	term_width;

// Flag defines
# define FLAG_COUNT	5
# define SHORT_FLAGS	"lRart"
# define LONG_FLAGS	(char*[]){"long", "recursive", "all", "reverse", "time"}

// File defines
# define MODES	(char*[]){"---", "--x", "-w-", "-wx", \
				"r--", "r-x", "rw-", "rwx"}

# define LS_COLORSCOUNT	7
# define LS_COLORSENT	(char*[]){"di=", "fi=", "ln=", "pi=", "so=", "bd=", "cd="}

# define FILET_CHAR	(char[]){'d', '-', 'l', 'p', 's', 'b', 'c', '?'}

typedef int8_t	flagid_t;
enum	flagid_e {
	LONG = 0,
	RECURSIVE,
	ALL,
	REVERSE,
	TIME
};

typedef int8_t	filet_t;
enum	filet_e {
	FT_DIR = 0,
	FT_FILE,
	FT_LINK,
	FT_FIFO,
	FT_SOCK,
	FT_BLK,
	FT_CHR,
	FT_NONE
};

typedef struct file_s {
	filet_t			_fileT;
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

typedef struct dir_s {
	char *		_name;
	file_t **	_files;
}	dir_t;

typedef struct env_s {
	uint32_t	_term_width;
	char *		_colors[LS_COLORSCOUNT];
}	env_t;

# define NULL_DIR	(dir_t){NULL, NULL}
# define IS_NULL_DIR(d)	(d._name == NULL && d._files == NULL)

t_list *	check_args(bool (*flags)[FLAG_COUNT], const char **args, const size_t count, t_garb *gc);
dir_t		getfiles_at(const char *path, t_garb *gc);
file_t **	sort_files(file_t **files, const bool time);
void		display(dir_t *dirs, bool (*flags)[FLAG_COUNT]);

#endif
