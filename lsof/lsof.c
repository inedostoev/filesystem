#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int is_num (const char *s) {
    if (s == NULL || *s == '\0' || isspace(*s))
      return 0;
    char * p;
    strtol(s, &p, 10);
    return *p == '\0';
}

int main(int argc, char const *argv[]) {
	
	struct dirent *cur_proc_dir;
	DIR *proc_dir;

	if ((proc_dir = opendir("/proc")) == NULL) {
 		printf("Can't open proc\n");
 		return 1;
	}

	char curproc_name[PATH_MAX];
	char curfd_dirname[PATH_MAX];
	char curfd_name[PATH_MAX];
	char curfd_realname[PATH_MAX];

	while ((cur_proc_dir = readdir(proc_dir)) != NULL) {
 		struct stat curproc_stat;
		if (!is_num(cur_proc_dir -> d_name)) {
			continue;
		}
 		snprintf(curproc_name, PATH_MAX - 1, "/proc/%s", cur_proc_dir -> d_name);
 		if (stat(curproc_name, &curproc_stat) == -1) {
			printf("Unable to stat file: %s error: %s\n", curproc_name, strerror(errno));
  			continue;
		}
		if ((curproc_stat.st_mode & S_IFMT) != S_IFDIR)
  			continue;

		struct dirent *curfd_dirent;
		DIR *curfd_dir;

		snprintf(curfd_dirname, PATH_MAX - 1, "%s/fd", curproc_name);
		if ((curfd_dir = opendir(curfd_dirname)) == NULL) {
	 		printf("Can't open fd %s\n", curfd_dirname);
	 		continue;
		}

		while ((curfd_dirent = readdir(curfd_dir)) != NULL) {
			if (!is_num(curfd_dirent -> d_name)) {
				continue;
			}
			snprintf(curfd_name, PATH_MAX - 1, "%s/%s", curfd_dirname, curfd_dirent -> d_name);

			int len = 0;
			if ((len = readlink(curfd_name, curfd_realname, PATH_MAX - 1)) == -1) {
				printf("Failed to read link for %s, error: %s\n", curfd_name, strerror(errno));
				continue;
			}
			curfd_realname[len] = '\0';
			printf("%s\t%s\n", cur_proc_dir -> d_name, curfd_realname);
		}

		closedir(curfd_dir);
	}

	closedir(proc_dir);
	return 0;
}
