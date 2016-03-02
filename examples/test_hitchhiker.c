
      /*#######.
     ########",#:
   #########',##".
  ##'##'## .##',##.
   ## ## ## # ##",#.
    ## ## ## ## ##'
     ## ## ## :##
      ## ## ##*/

#include "cut_framework.h"

static unsigned int     deep_thought()
{
    return 42;
}

SUITE(hello, {

    DESCRIBE("deep_thought", {

        IT("returns meaning of life",
            ASSERT(deep_thought() == 42))
    })
})
