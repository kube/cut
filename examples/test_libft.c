
      /*#######.
     ########",#:
   #########',##".
  ##'##'## .##',##.
   ## ## ## # ##",#.
    ## ## ## ## ##'
     ## ## ## :##
      ## ## ##*/

/*
**  Cut Testing for 42 libft
**  Rework of OpenTester tests by Nax
**  https://github.com/OpenFT/OpenTester
*/

///CUTRUN:LINK_LIBRARY      ft

#include "cut_framework.h"
#include "libft.h"

SUITE(libft, {

    DESCRIBE("ft_memset", {
        char    b1[100];
        char    b2[100];

        IT("returns dst",
            ASSERT(b1 == ft_memset(b1, 99, 0)))

        IT("fills a buffer of length 100 with 42", {
            memset(b2, 42, 100);
            ft_memset(b1, 42, 100);
            ASSERT(memcmp(b1, b2, 100) == 0)
        })

        IT("doesn't write to buffer when length is 0", {
            b1[0] = 1;
            ft_memset(b1, 0, 0);
            ASSERT(b1[0] == 1)
        })
    })

    DESCRIBE("ft_bzero", {
        char    b1[42];
        char    b2[42];

        IT("zeroes a buffer of length 10", {
            bzero(b1, 10);
            ft_bzero(b2, 10);
            ASSERT(memcmp(b1, b2, 10) == 0)
        })

        IT("zeroes a buffer of length 42", {
            bzero(b1, 42);
            ft_bzero(b2, 42);
            ASSERT(memcmp(b1, b2, 42) == 0)
        })

        IT("doesn't write to buffer when length is 0", {
            b1[0] = 1;
            ft_bzero(b1, 0);
            ASSERT(b1[0] == 1)
        })
    })

    DESCRIBE("ft_memcpy", {
        char    b1[10];
        char    b2[10] = INIT_LIST(7, 42, 68, -44, 0, 6, 8, 9, 0, 77);

        ft_memcpy(b1, b2, 10);

        IT("copies a buffer of length 10", {
            ASSERT(memcmp(b1, b2, 10) == 0)
            ASSERT(b1 == ft_memcpy(b1, b2, 0))
        })
    })

    DESCRIBE("ft_memccpy", {
        char    buf1[] = "Ceci est un test.";
        char    buf2[200];
        void    *p1;
        void    *p2;

        p1 = memccpy(buf2, buf1, 'i', 10);
        p2 = ft_memccpy(buf2, buf1, 'i', 10);

        IT("copies a buffer of size 10 with 'i' as a delimiter",
            ASSERT(p1 == p2))

        IT("returns NULL on not found",
            ASSERT(ft_memccpy(buf2, buf1, 'k', 5) == NULL))

        IT("behaves correctly with a length of 0",
            ASSERT(ft_memccpy(buf2, buf1, 0, 0) == memccpy(buf2, buf1, 0, 0)))

        IT("behaves correctly when seeking for a NUL byte",
            ASSERT(ft_memccpy(buf2, buf1, 0, sizeof(buf1)) == memccpy(buf2, buf1, 0, sizeof(buf1))))

        IT("behaves correctly on match",
            ASSERT(ft_memccpy(buf2, buf1, 'C', 10) == (buf2 + 1)))
    })

    DESCRIBE("ft_memmove", {
        char    test[] = "abcdef";

        IT("copies forward and backward", {
            ft_memmove(test + 1, test, 5);
            ft_memmove(test + 1, test, 0);
            ASSERT(strcmp(test, "aabcde") == 0)
        })
    })

    DESCRIBE("ft_memchr", {
        char    test[] = "abcdef";

        IT("matches the first char in a string",
            ASSERT(ft_memchr(test, 'a', 5) == test))

        IT("matches a char in a string",
            ASSERT(ft_memchr(test, 'c', 5) == test + 2))

        IT("behaves correctly when seeking for a NUL byte",
            ASSERT(ft_memchr(test, '\0', 7) == memchr(test, '\0', 7)))

        IT("returns NULL on not found",
            ASSERT(ft_memchr(test, 'z', 6) == NULL))
    })

    DESCRIBE("ft_memcmp", {
        char*   t = "ab\0ab";

        IT("returns zero when compared against itself",
            ASSERT(ft_memcmp(t, t, 5) == 0))

        IT("returns non-zero when bytes are different",
            ASSERT(ft_memcmp(t, "abcab", 5) != 0))

        IT("returns a negative integer when s2 > s1",
            ASSERT(ft_memcmp("aaa", "aab", 4) < 0))

        IT("returns zero when length is 0",
            ASSERT(ft_memcmp("aww", "bpp", 0) == 0))
    })

    DESCRIBE("ft_strlen", {

        IT("returns zero when empty string",
            ASSERT(ft_strlen("") == 0))

        IT("returns 4 on a string of length 4",
            ASSERT(ft_strlen("chat") == 4))

        IT("returns 5 on a string of length 5",
            ASSERT(ft_strlen("abcde") == 5))
    })

    DESCRIBE("ft_strdup", {
        char    *c = "AbC";

        IT("returns a string with the same content",
            ASSERT(strcmp(ft_strdup("aaaaa"), "aaaaa") == 0))

        IT("returns an empty string when called with empty string",
            ASSERT(strcmp(ft_strdup(""), "") == 0))

        IT("returns a string with a different address",
            ASSERT(c != ft_strdup(c)))

        IT("returns a NUL terminated string",
            ASSERT(ft_strdup("abc")[4] == 0))
    })

    DESCRIBE("ft_strcpy", {
        char    buf[6];

        IT("copies a string of length 5", {
            ft_strcpy(buf, "abcde");
            ASSERT(strcmp(buf, "abcde") == 0)
        })
    })

    DESCRIBE("ft_strncpy", {
        char    b[6];

        IT("copies a string of length 3 in a buffer of capacity 6", {
            ft_strncpy(b, "abc", 6);
            ASSERT(memcmp(b, "abc\0\0", 6) == 0)
        })

        IT("truncates and copies a string of length 9 in a buffer of capacity 6", {
            ft_strncpy(b, "abcdefghi", 6);
            ASSERT(memcmp(b, "abcdefghi", 6) == 0)
        })
    })

    DESCRIBE("ft_strcat", {
        char    buf[9];

        IT("concats multiple strings, including empty string", {
            bzero(buf, 9);
            ft_strcat(buf, "Bon");
            ft_strcat(buf, "jour.");
            ft_strcat(buf, "");
            ASSERT(strcmp(buf, "Bonjour.") == 0)
        })
    })

    DESCRIBE("ft_strncat", {
        char    buf[20];

        strcpy(buf, "To be ");
        ft_strncat(buf, "or not to be", 6);

        IT("concats and truncate multiple strings",
            ASSERT(strcmp(buf, "To be or not") == 0))

        IT("doesn't write to string when length is zero", {
            ft_strncat(buf, "efefef", 0);
            ASSERT(strcmp(buf, "To be or not") == 0)
        })

        IT("returns dst",
            ASSERT(buf == ft_strncat(buf, "", 0)))
    })

    DESCRIBE("ft_strlcat", {
        char    buf[10];

        bzero(buf, 10);
        strcpy(buf, "abc");
        ft_strlcat(buf, "abcdefghijklmnop", 10);

        IT("concats and truncate a string",
            ASSERT(strcmp(buf, "abcabcdef") == 0))

        IT("returns length of the full string",
            ASSERT(ft_strlcat(buf, "abcd", 2) == 6))

        IT("returns length of the full string", {
            bzero(buf, 10);
            ASSERT(ft_strlcat(buf, "abc", 10) == 3)
            ASSERT(ft_strlcat(buf, "def", 10) == 6)
        })

        IT("returns length of the full string", {
            bzero(buf, 10);
            memset(buf, 'a', 10);
            ASSERT(ft_strlcat(buf, "ccc", 10) == 13)
        })
    })

    DESCRIBE("ft_strchr", {
        char    buf[] = "Je suis un poisson.";

        IT("matches on 'p'",
            ASSERT(strchr(buf, 'p') == ft_strchr(buf, 'p')))

        IT("matches on NUL byte",
            ASSERT(strchr(buf, 0) == ft_strchr(buf, 0)))

        IT("matches on first char",
            ASSERT(ft_strchr(buf, 'J') == buf))

        IT("returns NULL on not found",
            ASSERT(ft_strchr(buf, 'z') == 0))
    })

    DESCRIBE("ft_strrchr", {
        char    buf[] = "abcdedcba";

        IT("matches on 'a'",
            ASSERT(strrchr(buf, 'a') == ft_strrchr(buf, 'a')))

        IT("matches on NUL byte",
            ASSERT(strrchr(buf, 0) == ft_strrchr(buf, 0)))

        IT("returns NULL on not found",
            ASSERT(ft_strrchr(buf, 'z') == 0))

        buf[5] = 0;

        IT("matches on first char",
            ASSERT(ft_strrchr(buf, 'a') == buf))
    })

    DESCRIBE("ft_strstr", {
        char    buf[] = "Ceci n'est pas une pipe.";

        IT("matches a substring",
            ASSERT(strstr(buf, "une") == ft_strstr(buf, "une")))

        IT("matches empty string",
            ASSERT(strstr(buf, "") == ft_strstr(buf, "")))

        IT("returns NULL on not found", {
            ASSERT(strstr(buf, "aaaaa") == 0)
            ASSERT(ft_strstr(buf, "BWAAAAAAAAAAAAAAAAAAA") == 0)
        })
    })

    DESCRIBE("ft_strnstr", {
        char    buf[10];

        bzero(buf, 10);
        strcpy(buf, "un deux 9");

        IT("matches a substring in the first 10 chars",
            ASSERT(strnstr(buf, "deux", 10) == ft_strnstr(buf, "deux", 10)))

        IT("matches a substring in the first 3 chars",
            ASSERT(strnstr(buf, "9", 3) == ft_strnstr(buf, "9", 3)))

        IT("matches the empty string",
            ASSERT(ft_strnstr(buf, "", 6) == buf))

        IT("doesn't search past the first 5 chars",
            ASSERT(ft_strnstr(buf, "deux", 5) == strnstr(buf, "deux", 5)))

        IT("matches the last char",
            ASSERT(ft_strnstr(buf, "9", 10) == strnstr(buf, "9", 10)))

        buf[9] = '6';

        IT("returns NULL on not found",
            ASSERT(strnstr(buf, "cinq", 10) == ft_strnstr(buf, "cinq", 10)))

        IT("matches in the first 10 chars",
            ASSERT(strnstr(buf, "deux", 10) == ft_strnstr(buf, "deux", 10)))

        IT("doesn't match past the end",
            ASSERT(strnstr(buf, "9682", 10) == ft_strnstr(buf, "9682", 10)))

        IT("matches in the first 10 chars",
            ASSERT(strnstr(buf, "6", 10) == ft_strnstr(buf, "6", 10)))

        IT("matches nothing on the empty string", {
            buf[1] = 0;
            ASSERT(strnstr(buf, "deux", 10) == ft_strnstr(buf, "deux", 10))
        })
    })

    DESCRIBE("ft_strcmp", {

        IT("returns zero on equal strings",
            ASSERT(ft_strcmp("abc", "abc") == 0))

        IT("returns a negative integer when s1 < s2",
            ASSERT(ft_strcmp("abc", "abd") < 0))

        IT("returns a nonzero integer on different strings",
            ASSERT(ft_strcmp("a", "abcde") != 0))
    })

    DESCRIBE("ft_strncmp", {

        IT("returns zero when the first 3 char are identical",
            ASSERT(ft_strncmp("abc", "abcde", 3) == 0))

        IT("returns zero on identical strings",
            ASSERT(ft_strncmp("abc", "abc\0defg", 100) == 0))

        IT("returns nonzero on different strings",
            ASSERT(ft_strncmp("ab\0cde", "abcc\0e", 20) != 0))
    })

    DESCRIBE("ft_atoi", {
        IT("returns 0 on non numerical string",
            ASSERT(ft_atoi(" -sfecf") == 0))

        IT("handles +",
            ASSERT(ft_atoi("+2798") == 2798))

        IT("handles whitespaces",
            ASSERT(ft_atoi("  \t\n  \r\r\v\f-899") == -899))

        IT("handles trailing zeroes",
            ASSERT(ft_atoi("+0089") == 89))

        IT("handles minimum integer",
            ASSERT(ft_atoi("-2147483648") == -2147483648))

        IT("stops on first non numerical char",
            ASSERT(ft_atoi("a56") == 0))

        IT("stops on whitespace",
            ASSERT(ft_atoi("    555 5555555555555555") == 555))
    })

    DESCRIBE("ft_isalpha", {

        IT("returns true on 'a'",
            ASSERT(ft_isalpha('a')))

        IT("returns false on non ASCII",
            ASSERT(!ft_isalpha('a' + 0x100)))

        IT("returns false on '2'",
            ASSERT(!ft_isalpha('2')))

        IT("returns true on 'Z'",
            ASSERT(ft_isalpha('Z')))

        IT("returns true on 't'",
            ASSERT(ft_isalpha('t')))
    })

    DESCRIBE("ft_isdigit", {

        IT("returns false on NUL",
            ASSERT(!ft_isdigit(0)))

        IT("returns true on '8'",
            ASSERT(ft_isdigit('8')))

        IT("returns false on non-ASCII",
            ASSERT(!ft_isdigit('8' + 0x100)))
    })

    DESCRIBE("ft_isalnum", {

        IT("returns false on space",
            ASSERT(!ft_isalnum(' ')))

        IT("returns true on '6'",
            ASSERT(ft_isalnum('6')))

        IT("returns true on 'Z'",
            ASSERT(ft_isalnum('Z')))

        IT("returns false on non-ASCII",
            ASSERT(!ft_isalnum('5' + 0x100)))
    })

    DESCRIBE("ft_isascii", {

        IT("returns false on non-ASCII",
            ASSERT(!ft_isascii(200)))

        IT("returns true on NUL",
            ASSERT(ft_isascii(0)))

        IT("returns true on DEL",
            ASSERT(ft_isascii(127)))
    })

    DESCRIBE("ft_isprint", {

        IT("returns false on NUL",
            ASSERT(!ft_isprint(0)))

        IT("returns false on DEL",
            ASSERT(!ft_isprint(127)))

        IT("returns true on space",
            ASSERT(ft_isprint(' ')))

        IT("returns true on 'a'",
            ASSERT(ft_isprint('a')))

        IT("returns true on '~'",
            ASSERT(ft_isprint('~')))
    })

    DESCRIBE("ft_toupper", {

        IT("maps 'a' to 'A'",
            ASSERT(ft_toupper('a') == 'A'))

        IT("maps 12345 to 12345",
            ASSERT(ft_toupper(12345) == 12345))

        IT("maps space to space",
            ASSERT(ft_toupper(' ') == ' '))

        IT("maps 'Z' to 'Z'",
            ASSERT(ft_toupper('Z') == 'Z'))

        IT("maps 'z' to 'Z'",
            ASSERT(ft_toupper('z') == 'Z'))
    })

    DESCRIBE("ft_tolower", {

        IT("maps 'T' to 't'",
            ASSERT(ft_tolower('T') == 't'))

        IT("maps 99999 to 99999",
            ASSERT(ft_tolower(99999) == 99999))

        IT("maps 'e' to 'e'",
            ASSERT(ft_tolower('e') == 'e'))

        IT("maps 'Z' to 'z'",
            ASSERT(ft_tolower('Z') == 'z'))
    })

    DESCRIBE("ft_memalloc", {
        void    *mem1;
        void    *mem2;

        mem1 = malloc(100);
        bzero(mem1, 100);
        mem2 = ft_memalloc(100);

        IT("allocates a 100 byte blob filled with zeroes",
            ASSERT(memcmp(mem1, mem2, 100) == 0))

        IT("returns NULL on size 0",
            ASSERT(ft_memalloc(0) == 0))

        IT("returns NULL on allocation error",
            ASSERT(ft_memalloc((size_t)-1) == 0))
    })

    DESCRIBE("ft_memdel", {
        void    *mem;

        mem = malloc(2000);
        ft_memdel(&mem);

        IT("sets the freed pointer to NULL",
            ASSERT(mem == 0))
    })

    DESCRIBE("ft_strnew", {
        char    *str;
        char    *t;

        IT("creates an empty string",
            ASSERT(ft_strnew(0) != NULL))

        IT("creates a 101 bytes blob filled with zeroes", {
            str = ft_strnew(100);
            t = malloc(101);
            bzero(t, 101);

            ASSERT(memcmp(str, t, 101) == 0)
        })

        IT("returns NULL on error",
            ASSERT(ft_strnew((size_t)-10) == NULL))
    })

    DESCRIBE("ft_strdel", {
        char    *str;

        IT("sets the freed pointer to NULL", {
            str = malloc(123);
            ft_strdel(&str);

            ASSERT(str == 0)
        })
    })

    DESCRIBE("ft_strclr", {
        char    str[] = "tttttt";

        IT("clears a string of length 7", {
            ft_strclr(str);
            ASSERT(memcmp(str, "\0\0\0\0\0\0", 7) == 0)
        })
    })

    // DESCRIBE("ft_striter", {
    //     char    str[] = "aBcDeF";

    //     ft_striter(str, it_test);
    //     ASSERT(strcmp(str, "bCdEfG"), "should iterate a function") == 0)
    // })
})
