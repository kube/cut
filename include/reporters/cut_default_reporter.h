
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

# define __COLOR_NORMAL     "\x1B[0m"
# define __COLOR_RED        "\x1B[31m"
# define __COLOR_GREEN      "\x1B[32m"
# define __COLOR_YELLOW     "\x1B[33m"
# define __COLOR_BLUE       "\x1B[34m"
# define __COLOR_MAGENTA    "\x1B[35m"
# define __COLOR_CYAN       "\x1B[36m"
# define __COLOR_WHITE      "\x1B[37m"
# define __COLOR_SLATE      "\x1B[30;01m"

# define __CHECK_MARK       "\u2714"
# define __CROSS_MARK       "\u2718"

# define __COL_WIDTH        58
# define __INDENT_SIZE      2


/*
**  CUT REPORTER INTERFACE
*/

# define CUT__ON_SUITE_START(_suite)                                        \
{                                                                           \
    __PRINT_SUITE(_suite)                                                   \
}                                                                           \

# define CUT__ON_SUITE_END(_suite)                                          \
{                                                                           \
    if (_suite->status == CUT_SUCCESS)                                      \
    {                                                                       \
        printf("\n%sSuite %s%s%s passed successfully.%s\n",                 \
            __COLOR_GREEN, __COLOR_NORMAL, _suite->title,                   \
            __COLOR_GREEN, __COLOR_NORMAL);                                 \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        printf("\n%sSuite %s%s%s failed.%s\n",                              \
            __COLOR_RED, __COLOR_NORMAL, _suite->title,                     \
            __COLOR_RED, __COLOR_NORMAL);                                   \
    }                                                                       \
}                                                                           \

# define CUT__ON_DESCRIBE_START(_describe)                                  \
{                                                                           \
    __PRINT_DESCRIBE(_describe)                                             \
}                                                                           \

# define CUT__ON_DESCRIBE_END(_describe)                                    \
{                                                                           \
}                                                                           \

# define CUT__ON_IT_START(_it)                                              \
{                                                                           \
}                                                                           \

# define CUT__ON_IT_END(_it)                                                \
{                                                                           \
    __PRINT_IT(_it)                                                         \
    CUT__FOR_EACH_CHILD_NODE(_it, __cut_print_node)                         \
}                                                                           \

# define CUT__ON_ASSERTION(_assertion)                                      \
{                                                                           \
}                                                                           \


/*
**  CUT REPORTER PRIVATE
*/

# define __PRINT_SUITE(_suite)                                              \
{                                                                           \
    printf("%sSuite%s %s\n",                                                \
        __COLOR_SLATE, __COLOR_NORMAL, _suite->title);                      \
}                                                                           \


# define __PRINT_DESCRIBE(_describe)                                        \
{                                                                           \
    /* Empty line if not first Describe */                                  \
    if (_describe != _describe->parent_node->first_child)                   \
    {                                                                       \
        printf("\n");                                                       \
    }                                                                       \
                                                                            \
    printf("%-*s%sDescribe%s %s\n",                                         \
        (__INDENT_SIZE * _describe->depth), "",                             \
        __COLOR_SLATE, __COLOR_NORMAL, _describe->title);                   \
}                                                                           \


# define __PRINT_IT(_it)                                                    \
{                                                                           \
    printf("%*s",                                                           \
        (__INDENT_SIZE * _it->depth), "");                                  \
                                                                            \
    _it->status == CUT_SUCCESS ?                                            \
        printf("%s%s", __COLOR_GREEN, __CHECK_MARK) :                       \
        printf("%s%s", __COLOR_RED, __CROSS_MARK);                          \
                                                                            \
    printf("%s It%s %-*.*s%s%s%s\n",                                        \
        __COLOR_SLATE, __COLOR_NORMAL,                                      \
        __COL_WIDTH, __COL_WIDTH, _it->title,                               \
        __COLOR_SLATE,                                                      \
        (strlen(_it->title) > __COL_WIDTH ? "..." : "   "),                 \
        __COLOR_NORMAL);                                                    \
}                                                                           \


# define __PRINT_ASSERTION(_assertion)                                      \
{                                                                           \
    char* color;                                                            \
                                                                            \
    switch (_assertion->status)                                             \
    {                                                                       \
        case CUT_SIGNAL:                                                    \
            color = __COLOR_YELLOW;                                         \
            break;                                                          \
                                                                            \
        default:                                                            \
            color = __COLOR_RED;                                            \
            break;                                                          \
    }                                                                       \
                                                                            \
    printf("%-*s%s%s%s",                                                    \
        (__INDENT_SIZE * _assertion->depth), "",                            \
        color, _assertion->title, __COLOR_NORMAL);                          \
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
            __PRINT_SUITE(_node)
            break;

        case CUT_DESCRIBE:
            __PRINT_DESCRIBE(_node)
            break;

        case CUT_IT:
            __PRINT_IT(_node)
            break;

        case CUT_ASSERTION:
            __PRINT_ASSERTION(_node)
            break;

        default:
            break;
    }
}

#endif
