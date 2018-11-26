# include <ctype.h>
# include <dirent.h>
# include <pwd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

int isNumeric(char *s);

int isNumeric(char *s) {
		if (s == NULL || *s == '\0' || isspace(*s)) return 0;
		char * p;
		strtod (s, &p);
		return *p == '\0';
}

int forEachProcessDir(void (*callback)(FILE *fd, char *pid)) {
				DIR *dir;
				struct dirent *ent;
				FILE *fd;
				char filename[32];

				chdir("/proc");
				if ((dir = opendir (".")) == NULL) {
								  perror ("Could not open directory");
									return EXIT_FAILURE;
				}

				while ((ent = readdir(dir)) != NULL) {
								if(!isNumeric(ent->d_name)) continue;
								char *pid = ent->d_name;
								strcpy(filename, pid);
								strcat(filename, "/status");
								if ((fd = fopen(filename, "r")) != NULL) {
												callback(fd, pid);
												fclose(fd);
								}
				}
				closedir (dir);
				
				return EXIT_SUCCESS;
}
