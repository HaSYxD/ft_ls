/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasyxd <aliaudet@student.42lehavre.fr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:30:00 by hasyxd            #+#    #+#             */
/*   Updated: 2025/05/02 16:22:56 by hasyxd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <config.h>

static char *	add_pathsuffix(const char *path, const char *suffix, arena_t *a)
{
	char *	new = NULL;
	int	i = ft_strlen(path);

	if (path[i] != '/') {
		new = ft_strjoin(path, "/", (alloc_ctx_t){a, ARENA});
		new = ft_strjoin(new, suffix, (alloc_ctx_t){a, ARENA});
	}
	else
		new = ft_strjoin(path, suffix, (alloc_ctx_t){a, ARENA});
	return (new);
}

static char *	get_fileusrname(const uid_t uid, arena_t *a)
{
	struct passwd *	pass = getpwuid(uid);

	return (ft_strdup(pass->pw_name, (alloc_ctx_t){a, ARENA}));
}

static char *	get_filegrpname(const gid_t gid, arena_t *a)
{
	struct group *	grp = getgrgid(gid);

	return (ft_strdup(grp->gr_name, (alloc_ctx_t){a, ARENA}));
}

static char	get_filetype(mode_t mode)
{
	switch (mode & S_IFMT) {
		case S_IFSOCK:
			return (FT_SOCK);
		case S_IFLNK:
			return (FT_LINK);
		case S_IFREG:
			return (FT_FILE);
		case S_IFBLK:
			return (FT_BLK);
		case S_IFDIR:
			return (FT_DIR);
		case S_IFCHR:
			return (FT_CHR);
		case S_IFIFO:
			return (FT_FIFO);
		default:
			return (FT_NONE);
	}
}

static char *	get_linkTarget(char *fullPath, arena_t *file_arena)
{
	char	buff[1024] = {0};
	int	len = 0;

	len = readlink(fullPath, buff, 1024);
	if(len == -1)
		return (NULL);
	buff[len] = 0;
	return (ft_strdup(buff, (alloc_ctx_t){file_arena, ARENA}));
}

static filet_t	decode_filemode(mode_t mode, char (*buff)[11])
{
	uint8_t	ownerMode = (mode >> 0) & 0x07;
	uint8_t	groupMode = (mode >> 3) & 0x07;
	uint8_t	otherMode = (mode >> 6) & 0x07;

	int	j = 7;
	for (int i = 0; i < 3; i++)
		(*buff)[j++] = MODES[ownerMode][i];
	j = 4;
	for (int i = 0; i < 3; i++)
		(*buff)[j++] = MODES[groupMode][i];
	j = 1;
	for (int i = 0; i < 3; i++)
		(*buff)[j++] = MODES[otherMode][i];
	(*buff)[10] = '\0';

	filet_t	FT = get_filetype(mode);
	if (FT == FT_FILE && ((*buff)[9] == 'x' || (*buff)[6] == 'x' || (*buff)[3] == 'x'))
		FT = FT_EX;
	(*buff)[0] = FILET_CHAR[FT];
	return (FT);
}

static size_t	get_filelen(file_t **files)
{
	size_t	i = 0;

	while (files[i])
		i++;
	return (i);
}

static int	get_filecount(const char *path)
{
	DIR *		dir = opendir(path);

	if (!dir)
		return (ft_fprintf(2, "Error: \"%s\" no such file or directory\n", path), -1);

	struct dirent *	dirDT = readdir(dir);
	size_t		fCount = 0;
	
	while (dirDT) {
		fCount++;
		dirDT = readdir(dir);
	}
	closedir(dir);
	return (fCount);
}

static file_t **	sorttime_files(file_t **files)
{
	time_t		currSmallest = LONG_MIN;
	uint32_t	currSmallestIdx = 0;

	for (int j = -1; files[j + 1]; j++) {
		for (int i = j + 1; files[i]; i++) {
			if (files[i]->_timestamp > currSmallest) {
				currSmallest = files[i]->_timestamp;
				currSmallestIdx = i;
			}
		}
		file_t *	tmp = files[j + 1];

		files[j + 1] = files[currSmallestIdx];
		files[currSmallestIdx] = tmp;
		currSmallest = LONG_MIN;
		currSmallestIdx = 0;
	}
	return (files);
}

static char	get_sortchar(const char *str, int offset)
{
	if (str[0] == '.')
		offset++;
	return (ft_tolower(str[offset]));
}

static file_t **	sortname_files(file_t **files, const size_t start, const size_t len, const int offset)
{
	unsigned char	currSmallest = 255;
	uint32_t	currSmallestIdx = UINT_MAX;

	for (size_t j = start; j < (start + len); j++) {
		for (size_t i = j; i < (start + len); i++) {
			char	sortchar = get_sortchar(files[i]->_name, offset);
			if (sortchar < currSmallest) {
				currSmallest = sortchar;
				currSmallestIdx = i;
			}
		}

		file_t *	tmp = files[j];

		files[j] = files[currSmallestIdx];
		files[currSmallestIdx] = tmp;
		currSmallest = 255;
		currSmallestIdx = UINT_MAX;
	}

	unsigned char	last = 255;
	size_t		identicales = 0;

	for (size_t i = start; i < (start + len); i++) {
		unsigned char	curr = get_sortchar(files[i]->_name, offset);

		if (curr == last)
			identicales++;
		else if (i > start + 1 && curr != last && identicales > 0) {
			files = sortname_files(files, i - identicales - 1, identicales + 1, offset + 1);
			identicales = 0;
		}
		if (i == (start + len - 2) && identicales > 0)
			files = sortname_files(files, i - identicales - 1, identicales + 1, offset + 1);
		last = curr;
	}
	return (files);
}

file_t **		sort_files(file_t **files, const bool time)
{
	if (time)
		return (sorttime_files(files));
	else
		return (sortname_files(files, 0, get_filelen(files), 0));
}

t_list *	getfiles_at(const char *path, bool (*flags)[FLAG_COUNT], env_t *env, t_list **fileArgs, t_garb *gc)
{
	int	fCount = get_filecount(path);
	if (fCount == -1)
		return (NULL);

	// Open the directory to get the file datas
	DIR *		dir = opendir(path);
	if (!dir)
		return (ft_fprintf(2, "Error: \"%s\" no such file or directory\n", path), NULL);

	t_list *	recurs_dirs = NULL;
	arena_t *	file_arena = arena_init(ARENA_HUGE);

	// Allocate the file tree
	file_t **	files = arena_allocate(sizeof(file_t *) * (fCount + 1), file_arena);
	if (!files) {
		arena_destroy(file_arena);
		closedir(dir);
		return (ft_fprintf(2, "Error while allocating files: %s\n", arena_geterrlog(g_arena_err)), NULL);
	}
	for (int i = 0; i < fCount; i++) {
		files[i] = arena_allocate(sizeof(file_t), file_arena);
		if (!files[i]) {
			arena_destroy(file_arena);
			closedir(dir);
			return (ft_fprintf(2, "Error while allocating file: %s\n", arena_geterrlog(g_arena_err)), NULL);
		}
	}
	files[fCount] = NULL;

	// Get the files informations and store it in the file tree
	struct dirent *	dirDT = readdir(dir);
	size_t		ttlblks = 0;
	for (int i = 0; i < fCount; i++) {
		struct stat	buff;
		char *		fullPath = add_pathsuffix(path, dirDT->d_name, file_arena);
		if (!fullPath) {
			arena_destroy(file_arena);
			closedir(dir);
			return (ft_fprintf(2, "Error while processing fullpath: %s\n", arena_geterrlog(g_arena_err)), NULL);
		}

		if (lstat(fullPath, &buff) == -1) {
			arena_destroy(file_arena);
			closedir(dir);
			return (ft_fprintf(2, "Error: \"%s\" could not get file infos\n(errno):%s\n", fullPath, strerror(errno)), NULL);
		}
		
		// recover all the informations of the file
		files[i]->_UUID = buff.st_ino;
		files[i]->_linksCount = buff.st_nlink;
		files[i]->_size = buff.st_size;
		files[i]->_dateTime = ft_substr(ctime(&buff.st_mtime), 4, 12, (alloc_ctx_t){file_arena, ARENA});
		files[i]->_name = ft_strdup(dirDT->d_name, (alloc_ctx_t){file_arena, ARENA});
		files[i]->_owner = get_fileusrname(buff.st_uid, file_arena);
		files[i]->_group = get_filegrpname(buff.st_gid, file_arena);
		files[i]->_timestamp = buff.st_mtime;
		files[i]->_fileT = decode_filemode(buff.st_mode, &files[i]->_permissions);
		files[i]->_linkTarget = get_linkTarget(fullPath, file_arena);
	
		// increment the total size if necessary
		if (files[i]->_name[0] != '.' || (files[i]->_name[0] == '.' && (*flags)[ALL]))
			ttlblks += buff.st_blocks / 2;
		
		// recover nested directory in case of a recursive call
		if ((*flags)[RECURSIVE] && files[i]->_fileT == FT_DIR) {
			if (!(*flags)[ALL] && files[i]->_name[0] == '.') {
				dirDT = readdir(dir);
				continue ;
			}
			if (ft_strncmp(files[i]->_name, ".", 2) == 0 || ft_strnstr(files[i]->_name, "..", ft_strlen(files[i]->_name)) != NULL) {
				dirDT = readdir(dir);
				continue ;
			}
			ft_lstadd_back(fileArgs, ft_lstnew((alloc_ctx_t){gc, GARBAGE_COLLECTOR}, (void *)ft_strdup(fullPath, (alloc_ctx_t){gc, GARBAGE_COLLECTOR})));
		}

		// read the file stream
		dirDT = readdir(dir);
	}

	// Sort the files (By default in alphabetical order)
	files = sort_files(files, (*flags)[TIME]);
	display((dir_t){ft_strdup(path, (alloc_ctx_t){file_arena, ARENA}), files, ttlblks}, flags, env);

	arena_destroy(file_arena);
	closedir(dir);
	return (recurs_dirs);
}

