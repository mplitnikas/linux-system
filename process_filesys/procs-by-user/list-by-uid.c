# include <ctype.h>
# include <dirent.h>
# include <pwd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

uid_t userIdFromName(const char *name);
int isNumeric(char *s);
int findPidsByUid(int uid);
void printIfMatchesUid(FILE *fh, char *uid, char *pid);

uid_t userIdFromName(const char *name)
{
				struct passwd *pwd;
				uid_t u;
				char *endptr;
				if(name == NULL || *name == '\0') return -1;

				u = strtol(name, &endptr, 10);
				if(*endptr == '\0') return u;

				pwd = getpwnam(name);
				if(pwd == NULL) return -1;

				return pwd->pw_uid;
}

int isNumeric(char *s) {
		if (s == NULL || *s == '\0' || isspace(*s)) return 0;
		char * p;
		strtod (s, &p);
		return *p == '\0';
}

int findPidsByUid(int uid) {
				DIR *dir;
				struct dirent *ent;
				FILE *fd;
				char filename[32];
				char s_uid[16];
				sprintf(s_uid, "%d", uid);

				chdir("/proc");
				if ((dir = opendir (".")) != NULL) {
								  while ((ent = readdir(dir)) != NULL) {
													if(!isNumeric(ent->d_name)) continue;
													char *pid = ent->d_name;
													strcpy(filename, pid);
													strcat(filename, "/status");
													if ((fd = fopen(filename, "r")) != NULL) {
																	printIfMatchesUid(fd, s_uid, pid);
																	fclose(fd);
													}
									}
									closedir (dir);
				} else {
								  /* could not open directory */
								  perror ("");
									return EXIT_FAILURE;
				}
				return 0;
}

void printIfMatchesUid(FILE *fh, char *uid, char *pid) {
				char line[255];
				char *delim = "\t";
				char *nameKey = "Name:\t";
				char *uidKey = "Uid:\t";
				char foundName[32];
				char foundUid[32];
				while (fgets(line, 255, fh) != NULL) {
								if (strncmp(line, nameKey, strlen(nameKey)) == 0) {
								// i.e. if line starts with "Name:\t"
												strcpy(foundName, strtok(line+strlen(nameKey), delim));
								} else if (strncmp(line, uidKey, strlen(uidKey)) == 0) {
												strcpy(foundUid, strtok(line+strlen(uidKey), delim));
								}
				}
				if (strcmp(foundUid, uid) == 0) {
								printf("%s: %s", pid, foundName);
				}
}

void main(int argc, char * argv[]) {
				if(argc != 2) {
								printf("please supply one arg: a user to search by\n");
								exit(EXIT_FAILURE);
				}
				char *name = argv[1];
				int uid = userIdFromName(name);
				printf("user %s has the following PIDs open:\n", name);
				findPidsByUid(uid);
}
