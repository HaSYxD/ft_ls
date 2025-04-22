/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasyxd <aliaudet@student.42lehavre.fr      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:30:00 by hasyxd            #+#    #+#             */
/*   Updated: 2025/04/22 16:07:48 by hasyxd           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <config.h>

static char *	add_pathsuffix(const char *path, const char *suffix, t_garb *gc)
{
	char *	new = NULL;
	int	i = ft_strlen(path);

	if (path[i] != '/') {
		new = ft_strjoin(path, "/", gc);
		new = ft_strjoin(new, suffix, gc);
	}
	else
		new = ft_strjoin(path, suffix, gc);
	return (new);
}

static char *	get_fileusrname(const uid_t uid, t_garb *gc)
{
	struct passwd *	pass = getpwuid(uid);

	return (ft_strdup(pass->pw_name, gc));
}

static char *	get_filegrpname(const gid_t gid, t_garb *gc)
{
	struct group *	grp = getgrgid(gid);

	return (ft_strdup(grp->gr_name, gc));
}

static char	get_filetype(mode_t mode)
{
	switch (mode & S_IFMT) {
		case S_IFSOCK:
			return ('s');
		case S_IFLNK:
			return ('l');
		case S_IFREG:
			return ('-');
		case S_IFBLK:
			return ('b');
		case S_IFDIR:
			return ('d');
		case S_IFCHR:
			return ('c');
		case S_IFIFO:
			return ('p');
		default:
			return ('?');
	}
}

static void	decode_filemode(mode_t mode, char (*buff)[11])
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
	(*buff)[0] = get_filetype(mode);
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
	time_t		currSmallest = LONG_MAX;
	uint32_t	currSmallestIdx = UINT_MAX;

	for (int j = -1; files[j + 1]; j++) {
		for (int i = j + 1; files[i]; i++) {
			if (files[i]->_timestamp < currSmallest) {
				currSmallest = files[i]->_timestamp;
				currSmallestIdx = i;
			}
		}

		file_t *	tmp = files[j + 1];

		files[j + 1] = files[currSmallestIdx];
		files[currSmallestIdx] = tmp;
		currSmallest = LONG_MAX;
		currSmallestIdx = UINT_MAX;
	}
	return (files);
}

static file_t **	sortname_files(file_t **files)
{
	unsigned char	currSmallest = 255;
	uint32_t	currSmallestIdx = UINT_MAX;

	for (int j = -1; files[j + 1]; j++) {
		for (int i = j + 1; files[i]; i++) {
			if (ft_tolower(files[i]->_name[0]) < currSmallest) {
				currSmallest = files[i]->_name[0];
				currSmallestIdx = i;
			}
		}

		file_t *	tmp = files[j + 1];

		files[j + 1] = files[currSmallestIdx];
		files[currSmallestIdx] = tmp;
		currSmallest = 255;
		currSmallestIdx = UINT_MAX;
	}
	return (files);
}

static file_t **	sort_files(file_t **files, const bool time)
{
	if (time)
		return (sorttime_files(files));
	else
		return (sortname_files(files));
}

dir_t	getfiles_at(const char *path, t_garb *gc)
{
	int	fCount = get_filecount(path);
	if (fCount == -1)
		return (NULL_DIR);

	// Open the directory to get the file datas
	DIR *		dir = opendir(path);
	if (!dir)
		return (ft_fprintf(2, "Error: \"%s\" no such file or directory\n", path), NULL_DIR);

	// Allocate the file tree
	file_t **	files = allocate(sizeof(file_t *) * (fCount + 1), gc);
	for (int i = 0; i < fCount; i++)
		files[i] = allocate(sizeof(file_t), gc);
	files[fCount] = NULL;

	// Get the files informations and store it in the file tree
	struct dirent *	dirDT = readdir(dir);
	for (int i = 0; i < fCount; i++) {
		struct stat	buff;
		char *		fullPath = add_pathsuffix(path, dirDT->d_name, gc);

		if (stat(fullPath, &buff) == -1)
			return (ft_fprintf(2, "Error: \"%s\" could not get file infos\n", path), NULL_DIR);
		
		files[i]->_UUID = buff.st_ino;
		files[i]->_linksCount = buff.st_nlink;
		files[i]->_size = buff.st_size;
		files[i]->_dateTime = ft_substr(ctime(&buff.st_mtime), 4, 12, gc);
		files[i]->_name = ft_strdup(dirDT->d_name, gc);
		files[i]->_next = NULL;
		files[i]->_owner = get_fileusrname(buff.st_uid, gc);
		files[i]->_group = get_filegrpname(buff.st_gid, gc);
		files[i]->_timestamp = buff.st_mtime;
		decode_filemode(buff.st_mode, &files[i]->_permissions);
		dirDT = readdir(dir);
	}

	// Sort the files (By default in alphabetical order)
	files = sort_files(files, false);

	closedir(dir);
	return (dir_t){ft_strdup(path, gc), files};
}

