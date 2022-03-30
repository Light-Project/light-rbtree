# Light-rbtree user manual 

Light-rbtree is a red black tree library implemented in C language. Although it has a relatively small amount of code, its performance is very powerful. It supports single command insertion, deletion and search. The best thing is that it supports C language for style traversal iterators and supports medium order traversal and subsequent traversal respectively.

## Compilation Instructions

We don't need any external libraries, so just make it directly in the project root directory.

```shell
cd light-rbtree
make
```

### Run some simple tests

After you finish compiling above, you can run some simple tests.

```shell
./examples/simple
./examples/benchmark
```

Here is the output of benchmark:

```shell
Generate 1000000 Node:
  real time: 0.420000
  user time: 0.410000
  kern time: 0.000000
  rb deepth: 24
Middle Iteration:
  real time: 0.080000
  user time: 0.080000
  kern time: 0.000000
Postorder Iteration:
  real time: 0.080000
  user time: 0.070000
  kern time: 0.000000
Deletion All Node...
Done.
```

## Based on Development 

Light-rbtree library requires only two files to complete the migration.

```
src/rbtree.c
src/rbtree.h
```

### Principle introduction

The light rbtree library itself does not perform operations such as comparison, but uses a callback function to let users compare and return a result (greater than zero, less than zero and equal to zero). Therefore, in theory, we can insert infinite data into the red black tree. This design concept is applied to finding nodes (passing in a private data) and finding parent nodes during insertion (comparing two red black tree nodes).

## License

This is an open source red black tree library, which uses the GPLV2 protocol

## Discussion Group

The Tencent QQ Group number is: [763756024](https://jq.qq.com/?_wv=1027&k=UhogIfXA)

