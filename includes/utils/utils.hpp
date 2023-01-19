#ifndef UTILS_HPP
#define UTILS_HPP

#include <map>
#include <string>
#include <vector>

/* Macros of default paths */
#define D_CONFIG_PATH "../webserv/"
#define D_CONFIG_FILE "../webserv/default"
#define D_ROOT_PATH   "../webserv/website/"

/* Macro of default methods */
#define D_METHODS                                                              \
 { "GET", "POST", "DELETE" }

/* Macro for body size */
#define MAX_BODY_SIZE 2147483648
#define BODY_SIZE     1048576 // 1MB

#endif /* UTILS_HPP */
