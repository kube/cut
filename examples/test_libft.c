
      /*#######.
     ########",#:
   #########',##".
  ##'##'## .##',##.
   ## ## ## # ##",#.
    ## ## ## ## ##'
     ## ## ## :##
      ## ## ##*/

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
            ASSERT(b1[0] != 1)
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
})
