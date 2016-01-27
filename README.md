
#![Cut](./images/cut.png)
##C Unit Testing Framework


Cut is a simple C Unit Testing Framework composed by a bunch of macros in a single header file. Its syntax is inpired by Node testing framework Mocha.


```c
SUITE(hitchhiker, {
    DESCRIBE("Deep Thought", {
        IT("returns meaning of life",
            ASSERT(deep_thought() == 42))
    })
})

```

###Suite, Describe, It and Assert
The


###Signals
Cut will catch signals thrown during execution of each *it* block. If an unexpected signal appears, the block status will be passed to the signal name.

```
Describe Marvin
    It segfaults when asked to be happy [SIGSEGV]
```

But sometimes, you will expect this behaviour. In this case, simply use an `EXPECT_SIGNAL` wrapper :

```c
IT("segfaults when asked to be happy", {
    EXPECT_SIGNAL(SIGSEGV, {
        marvin_be_happy()
    })
})
```

Your test will then pass succesfully, **ONLY** if block emitted the expected signal, or will fail otherwise:
```
Describe Marvin
    It segfaults when asked to be happy [OK]
```

##CutRun
**CutRun** is a simple tool that will permit you to run your tests easily without having to create a Makefile or call the compiler from the command-line. It will handle everything for you.

![CutRun](./images/cutrun.png)

###Directives

All compilation properties are passed to CutRun through a simple set of directives, directly in the test source file:

```c
///CUTRUN:LINK_LIBRARY      ft
///CUTRUN:LIBRARY_FOLDER    ../
///CUTRUN:INCLUDE_FOLDER    ../include

SUITE(hello {
	DESCRIBE("world", {
		...
	})
})
```

You can also pass these directives directly to `cutrun` executable:

```sh
cutrun tests/hashmap.c -I../include -L../ -lft
```

###Installation

```
git clone git@github.com:kube/cut
cd cut
make install
```
