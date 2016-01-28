
      /*#######.
     ########",#:
   #########',##".
  ##'##'## .##',##.
   ## ## ## # ##",#.
    ## ## ## ## ##'
     ## ## ## :##
      ## ## ##*/

#ifndef CUT_DEFAULT_REPORTER_H
#define CUT_DEFAULT_REPORTER_H

# include <stdio.h>
# include "cut_reporter_interface.h"

/*
**  CUT REPORTER CONSTANTS
*/

# define COLOR_NORMAL   "\x1B[0m"
# define COLOR_RED      "\x1B[31m"
# define COLOR_GREEN    "\x1B[32m"
# define COLOR_YELLOW   "\x1B[33m"
# define COLOR_BLUE     "\x1B[34m"
# define COLOR_MAGENTA  "\x1B[35m"
# define COLOR_CYAN     "\x1B[36m"
# define COLOR_WHITE    "\x1B[37m"
# define COLOR_SLATE    "\x1B[30;01m"

# define __CUT_COL_WIDTH    58
# define __CUT_INDENT_SIZE  2

/*
**  CUT REPORTER PRIVATE MACROS
*/

# define __CUT_PRINT_IT_RESULT(_result, _color)                             \
    printf(" %s[%s]%s\n", COLOR_ ## _color, _result, COLOR_NORMAL);         \


/*
**  CUT REPORTER INTERFACE
*/

# undef __CUT_PRINT_SUITE
# define __CUT_PRINT_SUITE(_suite)                                          \
{                                                                           \
    printf("%sSuite%s %s\n",                                                \
        COLOR_SLATE, COLOR_NORMAL, _suite->title);                          \
                                                                            \
    __CUT_PRINT_CHILD_NODES(_suite);                                        \
}                                                                           \


# undef __CUT_PRINT_DESCRIBE
# define __CUT_PRINT_DESCRIBE(_describe)                                    \
{                                                                           \
    /* Empty line if not first Describe */                                  \
    if (_describe != _describe->parent_node->first_child)                   \
    {                                                                       \
        printf("\n");                                                       \
    }                                                                       \
                                                                            \
    printf("%-*s%sDescribe%s %s\n",                                         \
        (__CUT_INDENT_SIZE * _describe->depth), "",                         \
        COLOR_SLATE, COLOR_NORMAL, _describe->title);                       \
                                                                            \
    __CUT_PRINT_CHILD_NODES(_describe);                                     \
}                                                                           \


# undef __CUT_PRINT_IT
# define __CUT_PRINT_IT(_it)                                                \
{                                                                           \
    printf("%-*s%sIt%s %-*.*s%s%s%s ",                                      \
        (__CUT_INDENT_SIZE * _it->depth), "",                               \
        COLOR_SLATE, COLOR_NORMAL,                                          \
        __CUT_COL_WIDTH, __CUT_COL_WIDTH, _it->title,                       \
        COLOR_SLATE,                                                        \
        (strlen(_it->title) > __CUT_COL_WIDTH ? "..." : "   "),             \
        COLOR_NORMAL);                                                      \
                                                                            \
    switch(_it->status)                                                     \
    {                                                                       \
        case CUT_SUCCESS:                                                   \
            __CUT_PRINT_IT_RESULT("OK", GREEN)                              \
            break;                                                          \
                                                                            \
        case CUT_FAIL:                                                      \
            __CUT_PRINT_IT_RESULT("FAIL", RED)                              \
            break;                                                          \
                                                                            \
        case CUT_SIGSEGV:                                                   \
            __CUT_PRINT_IT_RESULT("SIGSEGV", YELLOW)                        \
            break;                                                          \
    }                                                                       \
                                                                            \
    __CUT_PRINT_CHILD_NODES(_it);                                           \
}                                                                           \


# undef __CUT_PRINT_ASSERTION
# define __CUT_PRINT_ASSERTION(_assertion)                                  \
    printf("%-*s%s%s%s",                                                    \
        (__CUT_INDENT_SIZE * _assertion->depth), "",                        \
        COLOR_RED, _assertion->title, COLOR_NORMAL);                        \

#endif
