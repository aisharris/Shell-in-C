#ifndef HEADERS_H_
#define HEADERS_H_

#define _GNU_SOURCE

//Bold high intensity text
#define BLACK "\e[1;90m"
#define RED "\e[1;91m"
#define GREEN "\e[1;92m"
#define YELLOW "\e[1;93m"
#define BLUE "\e[1;94m"
#define MAGENTA "\e[1;95m"
#define CYAN "\e[1;96m"
#define WHITE "\e[1;97m"

//Reset
#define reset "\e[0m"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <stdarg.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <time.h>
#include <math.h>
#include <ftw.h>
#include <fcntl.h>
#include <ctype.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>
#include <netdb.h>
#include <termios.h>

#include "evaluate.h"
#include "parser.h"
#include "prompt.h"
#include "tokenize.h"
#include "bg_process_comp.h"
#include "pastevents.h"
#include "seek.h"
#include "iwoman.h"
#include "neonate.h"
#include "activities.h"

#endif