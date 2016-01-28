
      /*#######.
     ########",#:
   #########',##".
  ##'##'## .##',##.
   ## ## ## # ##",#.
    ## ## ## ## ##'
     ## ## ## :##
      ## ## ##*/

#ifndef CUT_FRAMEWORK_H
#define CUT_FRAMEWORK_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "cut_reporter_interface.h"
# include "cut_default_reporter.h"


/*
**  CUT PUBLIC MACROS
*/

# define INIT_LIST(...)                                                     \
    { __VA_ARGS__ }                                                         \


# ifdef CUT_SINGLE_FILE

# define SUITE(_title, _block)                                              \
    void _test_suite_ ## _title (void)                                      \
    _block                                                                  \
                                                                            \
    int main(void)                                                          \
    {                                                                       \
        TEST_SUITE(_title)                                                  \
        return 0;                                                           \
    }                                                                       \

# else

# define SUITE(_title, _block)                                              \
    void _test_suite_ ## _title (void)                                      \
    _block                                                                  \

# endif


# define TEST_SUITE(_title)                                                 \
{                                                                           \
    void _test_suite_ ## _title (void);                                     \
                                                                            \
    __CUT_APPEND_NEW_NODE(SUITE)                                            \
    __cut_state.current_node->title = #_title;                              \
    __cut_state.current_node->status = CUT_SUCCESS;                         \
                                                                            \
    _test_suite_ ## _title ();                                              \
                                                                            \
    __cut_print_node(__cut_state.current_node);                             \
    __CUT_FINISH_CURRENT_NODE()                                             \
}                                                                           \


# define DESCRIBE(_title, _block)                                           \
{                                                                           \
    __CUT_APPEND_NEW_NODE(DESCRIBE)                                         \
    __cut_state.current_node->title = __CUT_STRDUP(_title);                 \
    __cut_state.current_node->status = CUT_SUCCESS;                         \
                                                                            \
    _block                                                                  \
                                                                            \
    __CUT_FINISH_CURRENT_NODE()                                             \
}                                                                           \


# define IT(_title, _block)                                                 \
{                                                                           \
    pid_t       child_process;                                              \
    int         is_child_process;                                           \
    int         child_status;                                               \
    int         pipe_fd[2];                                                 \
    FILE*       pipe_file;                                                  \
                                                                            \
    __CUT_APPEND_NEW_NODE(IT)                                               \
    __cut_state.current_node->title = _title;                               \
    __cut_state.current_node->status = CUT_SUCCESS;                         \
                                                                            \
    /* Create a pipe before fork to communicate with child */               \
    pipe(pipe_fd);                                                          \
                                                                            \
    /* Run block in separate process to catch signals */                    \
    child_process = fork();                                                 \
    is_child_process = !child_process;                                      \
                                                                            \
    if (is_child_process)                                                   \
    {                                                                       \
        /* Close pipe read-side */                                          \
        close(pipe_fd[0]);                                                  \
                                                                            \
        pipe_file = fdopen(pipe_fd[1], "w");                                \
                                                                            \
        _block                                                              \
                                                                            \
        fclose(pipe_file);                                                  \
        exit(0);                                                            \
    }                                                                       \
    else                                                                    \
    /* Parent Process */                                                    \
    {                                                                       \
        char*       pipe_buffer;                                            \
        size_t      pipe_buffer_size;                                       \
        int         nb_errors;                                              \
                                                                            \
        /* Close pipe write-side */                                         \
        close(pipe_fd[1]);                                                  \
                                                                            \
        pipe_buffer_size = 2048;                                            \
        pipe_buffer = malloc(pipe_buffer_size);                             \
        pipe_file = fdopen(pipe_fd[0], "r");                                \
                                                                            \
        nb_errors = 0;                                                      \
                                                                            \
        while (getline(&pipe_buffer, &pipe_buffer_size, pipe_file) != -1)   \
        {                                                                   \
            __CUT_APPEND_ASSERTION(pipe_buffer, CUT_FAIL)                   \
            nb_errors++;                                                    \
        }                                                                   \
        __cut_state.current_node->status = nb_errors > 0 ?                  \
            CUT_FAIL : CUT_SUCCESS;                                         \
                                                                            \
        waitpid(child_process, &child_status, 0);                           \
                                                                            \
        /* Child was killed by signal */                                    \
        if (WIFSIGNALED(child_status))                                      \
        {                                                                   \
            switch (WTERMSIG(child_status))                                 \
            {                                                               \
                case SIGSEGV:                                               \
                    __cut_state.current_node->status = CUT_SIGSEGV;         \
                    break;                                                  \
            }                                                               \
        }                                                                   \
                                                                            \
        fclose(pipe_file);                                                  \
    }                                                                       \
    __CUT_FINISH_CURRENT_NODE()                                             \
}                                                                           \


# define EXPECT_SIGNAL(_signal, _block)                                     \
{                                                                           \
    pid_t   child_process;                                                  \
    int     is_child_process;                                               \
    int     child_status;                                                   \
                                                                            \
    /* Flush to prevent duplicate previous assertions */                    \
    fflush(pipe_file);                                                      \
                                                                            \
    /* Run in separate process to catch signals */                          \
    child_process = fork();                                                 \
    is_child_process = !child_process;                                      \
                                                                            \
    if (is_child_process)                                                   \
    {                                                                       \
        _block                                                              \
        exit(0);                                                            \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        waitpid(child_process, &child_status, 0);                           \
                                                                            \
        if (WIFSIGNALED(child_status)                                       \
            && WTERMSIG(child_status) == _signal)                           \
        {                                                                   \
            exit(0);                                                        \
        }                                                                   \
        else                                                                \
        {                                                                   \
            ASSERTION_FAIL("Expected signal "#_signal" in block")           \
            exit(1);                                                        \
        }                                                                   \
    }                                                                       \
}                                                                           \


# define ASSERTION_FAIL(_message)                                           \
{                                                                           \
    fprintf(pipe_file, "Line %d: %s\n", __LINE__, _message);                \
}                                                                           \


# define ASSERT(_assertion)                                                 \
{                                                                           \
    if (!(_assertion))                                                      \
    {                                                                       \
        ASSERTION_FAIL("Expected "#_assertion)                              \
    }                                                                       \
}                                                                           \


/*
**  CUT PRIVATE MACROS
*/

# define __CUT_ALLOC(_size)                                                 \
    (memset(malloc(_size), 0, _size))                                       \


# define __CUT_FREE(_pointer))                                              \
    (free(_pointer))                                                        \


# define __CUT_NEW(_type)                                                   \
    (__CUT_ALLOC(sizeof(__cut_ ## _type)))                                  \


# define __CUT_STRDUP(_str)                                                 \
    (strcpy(__CUT_ALLOC(strlen(_str)), _str))                               \


// Append a new child node to current_node
// And set it as the new current_node
# define __CUT_APPEND_NEW_NODE(_type)                                       \
{                                                                           \
    __cut_node*    new_node;                                                \
                                                                            \
    new_node = __CUT_NEW(node);                                             \
    new_node->type = CUT_ ## _type;                                         \
                                                                            \
    /* No current_node, set new_node as root */                             \
    if (!__cut_state.current_node)                                          \
    {                                                                       \
        __cut_state.current_node = new_node;                                \
        new_node->depth = 0;                                                \
    }                                                                       \
    /* current_node has no child, set new_node as first child */            \
    else if (!__cut_state.current_node->first_child)                        \
    {                                                                       \
        __cut_state.current_node->first_child = new_node;                   \
        new_node->parent_node = __cut_state.current_node;                   \
        __cut_state.current_node = new_node;                                \
        new_node->parent_node->last_child = new_node;                       \
        new_node->depth = new_node->parent_node->depth + 1;                 \
    }                                                                       \
    /* current_node has child, set new_node as last child */                \
    else                                                                    \
    {                                                                       \
        __cut_state.current_node->last_child->next_sibling = new_node;      \
        new_node->parent_node = __cut_state.current_node;                   \
        __cut_state.current_node = new_node;                                \
        new_node->parent_node->last_child = new_node;                       \
        new_node->depth = new_node->parent_node->depth + 1;                 \
    }                                                                       \
}                                                                           \


// Set current_node back to its parent
# define __CUT_FINISH_CURRENT_NODE()                                        \
{                                                                           \
    __cut_state.current_node = __cut_state.current_node->parent_node;       \
}                                                                           \


# define __CUT_APPEND_ASSERTION(_title, _status)                            \
{                                                                           \
    __CUT_APPEND_NEW_NODE(ASSERTION)                                        \
    __cut_state.current_node->title = __CUT_STRDUP(_title);                 \
    __cut_state.current_node->status = _status;                             \
    __CUT_FINISH_CURRENT_NODE()                                             \
}                                                                           \


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
