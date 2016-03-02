
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

# ifndef CUT_REPORTER_HEADER
# define CUT_REPORTER_HEADER "reporters/cut_default_reporter.h"
# endif

# include CUT_REPORTER_HEADER


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
    CUT__APPEND_NEW_NODE(SUITE)                                             \
    CUT__CURRENT_NODE->title = #_title;                                     \
    CUT__CURRENT_NODE->status = CUT_SUCCESS;                                \
                                                                            \
    CUT__ON_SUITE_START(CUT__CURRENT_NODE)                                  \
                                                                            \
    _test_suite_ ## _title ();                                              \
                                                                            \
    CUT__ON_SUITE_END(CUT__CURRENT_NODE)                                    \
    CUT__FINISH_CURRENT_NODE()                                              \
}                                                                           \


# define DESCRIBE(_title, _block)                                           \
{                                                                           \
    CUT__APPEND_NEW_NODE(DESCRIBE)                                          \
    CUT__CURRENT_NODE->title = CUT__STRDUP(_title);                         \
    CUT__CURRENT_NODE->status = CUT_SUCCESS;                                \
                                                                            \
    CUT__ON_DESCRIBE_START(CUT__CURRENT_NODE)                               \
                                                                            \
    _block                                                                  \
                                                                            \
    CUT__ON_DESCRIBE_END(CUT__CURRENT_NODE)                                 \
    CUT__FINISH_CURRENT_NODE()                                              \
}                                                                           \


# define IT(_title, _block)                                                 \
{                                                                           \
    pid_t       child_process;                                              \
    int         is_child_process;                                           \
    int         child_status;                                               \
    int         pipe_fd[2];                                                 \
    FILE*       pipe_file;                                                  \
                                                                            \
    CUT__APPEND_NEW_NODE(IT)                                                \
    CUT__CURRENT_NODE->title = _title;                                      \
    CUT__CURRENT_NODE->status = CUT_SUCCESS;                                \
                                                                            \
    CUT__ON_IT_START(CUT__CURRENT_NODE)                                     \
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
                                                                            \
        /* Close pipe write-side */                                         \
        close(pipe_fd[1]);                                                  \
                                                                            \
        pipe_buffer_size = 2048;                                            \
        pipe_buffer = malloc(pipe_buffer_size);                             \
        pipe_file = fdopen(pipe_fd[0], "r");                                \
                                                                            \
        /* Get failed assertions from child through pipe */                 \
        while (getline(&pipe_buffer, &pipe_buffer_size, pipe_file) != -1)   \
        {                                                                   \
            CUT__APPEND_ASSERTION(pipe_buffer, CUT_FAIL)                    \
        }                                                                   \
                                                                            \
        waitpid(child_process, &child_status, 0);                           \
                                                                            \
        /* Child was killed by signal */                                    \
        if (WIFSIGNALED(child_status))                                      \
        {                                                                   \
            int signal = WTERMSIG(child_status);                            \
            char* message = NULL;                                           \
                                                                            \
            asprintf(&message, "Unexpected %s\n", strsignal(signal));       \
            CUT__APPEND_ASSERTION(message, CUT_SIGNAL);                     \
        }                                                                   \
                                                                            \
        fclose(pipe_file);                                                  \
    }                                                                       \
    CUT__ON_IT_END(CUT__CURRENT_NODE)                                       \
    CUT__FINISH_CURRENT_NODE()                                              \
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

# define CUT__CURRENT_NODE                                                  \
    (__cut_state.current_node)                                              \

# define CUT__ALLOC(_size)                                                  \
    (memset(malloc(_size), 0, _size))                                       \

# define CUT__FREE(_pointer))                                               \
    (free(_pointer))                                                        \

# define CUT__NEW(_type)                                                    \
    (CUT__ALLOC(sizeof(__cut_ ## _type)))                                   \

# define CUT__STRDUP(_str)                                                  \
    (strcpy(CUT__ALLOC(strlen(_str)), _str))                                \


// Append a new child node to current_node
// And set it as the new current_node
# define CUT__APPEND_NEW_NODE(_type)                                        \
{                                                                           \
    __cut_node*    new_node;                                                \
                                                                            \
    new_node = CUT__NEW(node);                                              \
    new_node->type = CUT_ ## _type;                                         \
                                                                            \
    /* No current_node, set new_node as root */                             \
    if (!CUT__CURRENT_NODE)                                                 \
    {                                                                       \
        CUT__CURRENT_NODE = new_node;                                       \
        new_node->depth = 0;                                                \
    }                                                                       \
    /* current_node has no child, set new_node as first child */            \
    else if (!CUT__CURRENT_NODE->first_child)                               \
    {                                                                       \
        CUT__CURRENT_NODE->first_child = new_node;                          \
        new_node->parent_node = CUT__CURRENT_NODE;                          \
        CUT__CURRENT_NODE = new_node;                                       \
        new_node->parent_node->last_child = new_node;                       \
        new_node->depth = new_node->parent_node->depth + 1;                 \
    }                                                                       \
    /* current_node has child, set new_node as last child */                \
    else                                                                    \
    {                                                                       \
        CUT__CURRENT_NODE->last_child->next_sibling = new_node;             \
        new_node->parent_node = CUT__CURRENT_NODE;                          \
        CUT__CURRENT_NODE = new_node;                                       \
        new_node->parent_node->last_child = new_node;                       \
        new_node->depth = new_node->parent_node->depth + 1;                 \
    }                                                                       \
}                                                                           \


// Set current_node back to its parent
# define CUT__FINISH_CURRENT_NODE()                                         \
{                                                                           \
    CUT__CURRENT_NODE = CUT__CURRENT_NODE->parent_node;                     \
}                                                                           \


# define CUT__SET_PARENTS_FAIL(_node)                                       \
{                                                                           \
    __cut_node* node;                                                       \
                                                                            \
    node = _node;                                                           \
    while (node)                                                            \
    {                                                                       \
        node->status = CUT_FAIL;                                            \
        node = node->parent_node;                                           \
    }                                                                       \
}                                                                           \


# define CUT__APPEND_ASSERTION(_title, _status)                             \
{                                                                           \
    CUT__APPEND_NEW_NODE(ASSERTION)                                         \
    CUT__SET_PARENTS_FAIL(CUT__CURRENT_NODE)                                \
                                                                            \
    CUT__CURRENT_NODE->title = CUT__STRDUP(_title);                         \
    CUT__CURRENT_NODE->status = _status;                                    \
                                                                            \
    CUT__ON_ASSERTION(CUT__CURRENT_NODE)                                    \
    CUT__FINISH_CURRENT_NODE()                                              \
}                                                                           \

#endif
