#ifndef __FILEUTILS__
#define __FILEUTILS__

#include <stdio.h>

FILE* open_file_if_valid(const char *filename, char **error_message );
char* readLongString(FILE* fp);

#endif