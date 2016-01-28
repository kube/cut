
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

# define CUT_CHECK_MARK "\u2714"
# define CUT_CROSS_MARK "\u2718"

# define __CUT_COL_WIDTH    42
# define __CUT_INDENT_SIZE  1


/*
**  CUT REPORTER INTERFACE
*/

# define __CUT_PRINT_SUITE(_suite)                                          \
{                                                                           \
    printf("%sSuite%s %s\n",                                                \
        COLOR_SLATE, COLOR_NORMAL, _suite->title);                          \
                                                                            \
    __CUT_PRINT_CHILD_NODES(_suite);                                        \
}                                                                           \


# define __CUT_PRINT_DESCRIBE(_describe)                                    \
{                                                                           \
        _describe->status == CUT_SUCCESS ?                                  \
            printf(" %s%s", COLOR_GREEN, CUT_CHECK_MARK) :                  \
            printf(" %s%s", COLOR_RED, CUT_CROSS_MARK);                     \
                                                                            \
    printf("%-*s%s\n",                                                      \
        (__CUT_INDENT_SIZE * _describe->depth), "",                         \
        _describe->title);                                                  \
                                                                            \
    __CUT_PRINT_CHILD_NODES(_describe);                                     \
}                                                                           \


# define __CUT_PRINT_IT(_it)                                                \
{                                                                           \
    if (_it->status != CUT_SUCCESS)                                         \
    {                                                                       \
        printf(" %*s",                                                      \
            (__CUT_INDENT_SIZE * _it->depth), "");                          \
                                                                            \
        _it->status == CUT_SUCCESS ?                                        \
            printf("%s%s", COLOR_GREEN, CUT_CHECK_MARK) :                   \
            printf("%s%s", COLOR_RED, CUT_CROSS_MARK);                      \
                                                                            \
        printf("%s It%s %-*.*s%s%s%s\n",                                    \
            COLOR_SLATE, COLOR_NORMAL,                                      \
            __CUT_COL_WIDTH, __CUT_COL_WIDTH, _it->title,                   \
            COLOR_SLATE,                                                    \
            (strlen(_it->title) > __CUT_COL_WIDTH ? "..." : "   "),         \
            COLOR_NORMAL);                                                  \
                                                                            \
        __CUT_PRINT_CHILD_NODES(_it);                                       \
    }                                                                       \
}                                                                           \


# define __CUT_PRINT_ASSERTION(_assertion)                                  \
{                                                                           \
    char* color;                                                            \
                                                                            \
    switch (_assertion->status)                                             \
    {                                                                       \
        case CUT_SIGSEGV:                                                   \
            color = COLOR_YELLOW;                                           \
            break;                                                          \
                                                                            \
        default:                                                            \
            color = COLOR_RED;                                              \
            break;                                                          \
    }                                                                       \
                                                                            \
    printf("  %-*s%s%s%s",                                                  \
        (__CUT_INDENT_SIZE * _assertion->depth), "",                        \
        color, _assertion->title, COLOR_NORMAL);                            \
}                                                                           \

#endif
