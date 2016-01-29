
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

# define __CUT_COL_WIDTH    58
# define __CUT_INDENT_SIZE  2


/*
**  CUT REPORTER INTERFACE
*/

# define __CUT_ON_SUITE_START(_suite)                                       \
{                                                                           \
    __CUT_PRINT_SUITE(_suite)                                               \
}                                                                           \

# define __CUT_ON_SUITE_END(_suite)                                         \
{                                                                           \
    if (_suite->status == CUT_SUCCESS)                                      \
    {                                                                       \
        printf("\n%sSuite %s%s%s passed successfully.%s\n",                 \
            COLOR_GREEN, COLOR_NORMAL, _suite->title,                       \
            COLOR_GREEN, COLOR_NORMAL);                                     \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        printf("\n%sSuite %s%s%s failed.%s\n",                              \
            COLOR_RED, COLOR_NORMAL, _suite->title,                         \
            COLOR_RED, COLOR_NORMAL);                                       \
    }                                                                       \
}                                                                           \

# define __CUT_ON_DESCRIBE_START(_describe)                                 \
{                                                                           \
    __CUT_PRINT_DESCRIBE(_describe)                                         \
}                                                                           \

# define __CUT_ON_DESCRIBE_END(_describe)                                   \
{                                                                           \
}                                                                           \

# define __CUT_ON_IT_START(_it)                                             \
{                                                                           \
}                                                                           \

# define __CUT_ON_IT_END(_it)                                               \
{                                                                           \
    __CUT_PRINT_IT(_it)                                                     \
    __CUT_FOR_EACH_CHILD_NODE(_it, __cut_print_node)                        \
}                                                                           \

# define __CUT_ON_ASSERTION(_assertion)                                     \
{                                                                           \
}                                                                           \


/*
**  CUT REPORTER PRIVATE
*/

# define __CUT_PRINT_SUITE(_suite)                                          \
{                                                                           \
    printf("%sSuite%s %s\n",                                                \
        COLOR_SLATE, COLOR_NORMAL, _suite->title);                          \
}                                                                           \


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
}                                                                           \


# define __CUT_PRINT_IT(_it)                                                \
{                                                                           \
    printf("%*s",                                                           \
        (__CUT_INDENT_SIZE * _it->depth), "");                              \
                                                                            \
    _it->status == CUT_SUCCESS ?                                            \
        printf("%s%s", COLOR_GREEN, CUT_CHECK_MARK) :                       \
        printf("%s%s", COLOR_RED, CUT_CROSS_MARK);                          \
                                                                            \
    printf("%s It%s %-*.*s%s%s%s\n",                                        \
        COLOR_SLATE, COLOR_NORMAL,                                          \
        __CUT_COL_WIDTH, __CUT_COL_WIDTH, _it->title,                       \
        COLOR_SLATE,                                                        \
        (strlen(_it->title) > __CUT_COL_WIDTH ? "..." : "   "),             \
        COLOR_NORMAL);                                                      \
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
    printf("%-*s%s%s%s",                                                    \
        (__CUT_INDENT_SIZE * _assertion->depth), "",                        \
        color, _assertion->title, COLOR_NORMAL);                            \
}                                                                           \


/*
**  CUT REPORTER PRIVATE FUNCTIONS
**  Only print_node is defined as a function to permit recursion
*/

void __cut_print_node(__cut_node* _node)
{
    switch (_node->type)
    {
        case CUT_SUITE:
            __CUT_PRINT_SUITE(_node)
            break;

        case CUT_DESCRIBE:
            __CUT_PRINT_DESCRIBE(_node)
            break;

        case CUT_IT:
            __CUT_PRINT_IT(_node)
            break;

        case CUT_ASSERTION:
            __CUT_PRINT_ASSERTION(_node)
            break;

        default:
            break;
    }
}

#endif
