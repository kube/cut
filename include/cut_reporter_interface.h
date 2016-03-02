
      /*#######.
     ########",#:
   #########',##".
  ##'##'## .##',##.
   ## ## ## # ##",#.
    ## ## ## ## ##'
     ## ## ## :##
      ## ## ##*/

#ifndef CUT_REPORTER_INTERFACE_H
#define CUT_REPORTER_INTERFACE_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>


/*
**  CUT STRUCTURES AND TYPES
*/

typedef enum
{
    CUT_RUNNING,
    CUT_SUCCESS,
    CUT_FAIL,
    CUT_SIGNAL
}   __cut_status;

typedef enum
{
    CUT_SUITE,
    CUT_DESCRIBE,
    CUT_IT,
    CUT_ASSERTION
}   __cut_node_type;

typedef struct                  __s_cut_node
{
    __cut_node_type             type;
    char*                       title;
    __cut_status                status;
    unsigned int				depth;
    struct __s_cut_node*        first_child;
    struct __s_cut_node*        last_child;
    struct __s_cut_node*        parent_node;
    struct __s_cut_node*        next_sibling;
}                               __cut_node;

struct
{
    __cut_node*                 current_node;
}                               __cut_state;


/*
**  CUT REPORTER PRIVATE
*/

# define CUT__FOR_EACH_CHILD_NODE(_node, _fn)                               \
{                                                                           \
    if (_node->first_child)                                                 \
    {                                                                       \
        __cut_node*    current_child;                                       \
                                                                            \
        current_child = _node->first_child;                                 \
        while(current_child)                                                \
        {                                                                   \
            _fn(current_child);                                             \
            current_child = current_child->next_sibling;                    \
        }                                                                   \
    }                                                                       \
}                                                                           \

#endif
