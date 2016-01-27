
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
# include <sys/mman.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <string.h>
# include <fcntl.h>

# define COLOR_NORMAL   "\x1B[0m"
# define COLOR_RED      "\x1B[31m"
# define COLOR_GREEN    "\x1B[32m"
# define COLOR_YELLOW   "\x1B[33m"
# define COLOR_BLUE     "\x1B[34m"
# define COLOR_MAGENTA  "\x1B[35m"
# define COLOR_CYAN     "\x1B[36m"
# define COLOR_WHITE    "\x1B[37m"
# define COLOR_SLATE    "\x1B[30;01m"


typedef enum
{
    IT_SUCCESS,
    IT_FAIL,
    IT_SIGSEGV
}   __cut_it_status;



typedef struct                  __s_cut_assertion
{
    char*                       title;
    struct __s_cut_assertion*   next;
}                               __cut_assertion;

typedef struct                  __s_cut_it
{
    char*                       title;
    __cut_it_status             status;
    __cut_assertion*            assertions;
    struct __s_cut_it*          next;
}                               __cut_it;

struct
{
    __cut_it*                   it;
    int                         fd;
    void*                       memory_page;
}                               __cut_state;



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
    _test_suite_ ## _title ();                                              \
}                                                                           \


# define DESCRIBE(_title, _block)                                           \
{                                                                           \
    printf("\n%sDescribe%s %s\n",                                           \
        COLOR_SLATE, COLOR_NORMAL, _title);                                 \
    _block                                                                  \
}                                                                           \


# define IT(_title, _block)                                                 \
{                                                                           \
    pid_t       child_process;                                              \
    int         is_child_process;                                           \
    int         child_status;                                               \
    int         pipe_fd[2];                                                 \
    FILE*       pipe_file;                                                  \
                                                                            \
    __cut_state.it = __CUT_NEW(it);                                         \
    __cut_state.it->title = _title;                                         \
                                                                            \
    /* Create a pipe before fork to communicate with child */               \
    pipe(pipe_fd);                                                          \
                                                                            \
    /* Run block in separate process to catch signals */                    \
    child_process = fork();                                                 \
    is_child_process = !child_process;                                      \
                                                                            \
    /* Child Process */                                                     \
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
    /* Parent Process */                                                    \
    else                                                                    \
    {                                                                       \
        char*       pipe_buffer;                                            \
        size_t      pipe_buffer_size;                                       \
        int         number_of_errors;                                       \
                                                                            \
        /* Close pipe write-side */                                         \
        close(pipe_fd[1]);                                                  \
                                                                            \
        pipe_buffer_size = 2048;                                            \
        pipe_buffer = malloc(pipe_buffer_size);                             \
        pipe_file = fdopen(pipe_fd[0], "r");                                \
                                                                            \
        number_of_errors = 0;                                               \
                                                                            \
        while (getline(&pipe_buffer, &pipe_buffer_size, pipe_file) != -1)   \
        {                                                                   \
            printf("%s\n", pipe_buffer);                                    \
            number_of_errors++;                                             \
            __cut_state.it->status = IT_FAIL;                               \
        }                                                                   \
                                                                            \
        waitpid(child_process, &child_status, 0);                           \
                                                                            \
        /* Child was killed by signal */                                    \
        if (WIFSIGNALED(child_status))                                      \
        {                                                                   \
            switch (WTERMSIG(child_status))                                 \
            {                                                               \
                case SIGSEGV:                                               \
                    __cut_state.it->status = IT_SIGSEGV;                    \
                    break;                                                  \
            }                                                               \
        }                                                                   \
                                                                            \
        fclose(pipe_file);                                                  \
    }                                                                       \
    __CUT_PRINT_IT(__cut_state.it)                                          \
}                                                                           \


# define EXPECT_SIGNAL(_signal, _block)                                     \
{                                                                           \
    pid_t   child_process;                                                  \
    int     is_child_process;                                               \
    int     child_status;                                                   \
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
    fprintf(pipe_file, "%s\n", _message);                                   \
}                                                                           \
    

# define ASSERT(_assertion)                                                 \
{                                                                           \
    if (!(_assertion))                                                      \
    {                                                                       \
        ASSERTION_FAIL("Expected "#_assertion)                              \
    }                                                                       \
}                                                                           \


# define TEST_INIT()                                                        \
{                                                                           \
}                                                                           \


# define TEST_END()                                                         \
{                                                                           \
    printf("%sSuccess!%s\n\n", COLOR_GREEN, COLOR_NORMAL);                  \
}                                                                           \



/*
**  CUT PRIVATE MACROS
*/

# define __CUT_ALLOC(_size)                                                 \
    (memset(malloc(_size), 0, _size))                                       \


# define __CUT_NEW(_type)                                                   \
    (__CUT_ALLOC(sizeof(__cut_ ## _type)))                                  \


# define __CUT_STR_DUP(_str)                                                \
    (strcpy(__CUT_ALLOC(strlen(_str)), _str))                               \


# define __CUT_APPEND_ASSERTION(_assertion_text)                            \
{                                                                           \
    __cut_assertion*    assertion;                                          \
    __cut_assertion*    last_assertion;                                     \
                                                                            \
    assertion = __CUT_NEW(assertion);                                       \
    assertion->title = _assertion_text;                                     \
                                                                            \
    if (!__cut_state.it->assertions)                                        \
    {                                                                       \
        __cut_state.it->assertions = assertion;                             \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        last_assertion = __cut_state.it->assertions;                        \
        while (last_assertion->next)                                        \
            last_assertion = last_assertion->next;                          \
        last_assertion->next = assertion;                                   \
        last_assertion->next = NULL;                                        \
    }                                                                       \
}                                                                           \


# define __CUT_PRINT_ASSERTION(_assertion)                                  \
    printf("    %sExpected %s%s\n",                                         \
        COLOR_RED, _assertion->title, COLOR_NORMAL);                        \


# define __CUT_PRINT_IT_RESULT(result, color)                               \
    printf(" %s[%s]%s\n", COLOR_ ## color, result, COLOR_NORMAL);           \


# define __CUT_PRINT_IT(it)                                                 \
{                                                                           \
    printf("  %sIt%s %s ",                                                  \
        COLOR_SLATE, COLOR_NORMAL, it->title);                              \
                                                                            \
    switch(it->status)                                                      \
    {                                                                       \
        case IT_SUCCESS:                                                    \
            __CUT_PRINT_IT_RESULT("OK", GREEN)                              \
            break;                                                          \
                                                                            \
        case IT_FAIL:                                                       \
            __CUT_PRINT_IT_RESULT("FAIL", RED)                              \
            break;                                                          \
                                                                            \
        case IT_SIGSEGV:                                                    \
            __CUT_PRINT_IT_RESULT("SIGSEGV", YELLOW)                        \
            break;                                                          \
    }                                                                       \
                                                                            \
    /* Print assertions */                                                  \
    if (it->assertions)                                                     \
    {                                                                       \
        __cut_assertion*    current;                                        \
        current = it->assertions;                                           \
        while(current)                                                      \
        {                                                                   \
            __CUT_PRINT_ASSERTION(current)                                  \
            current = current->next;                                        \
        }                                                                   \
    }                                                                       \
}                                                                           \


#endif
