# La micro-benchmarking toolbox
a simple time profiler toolbox aiming at being easy to use.

usage example:

```cpp

LAMBT_TIC("Foo")
Foo();
LAMBT_TOC("Foo")

LAMBT_TICTOC(Bar())

```

Once the program exit it prints:

```terminal

----------------------------------
Foo:
  last:     Xus
  average:  Xus
  variance: Xus
  calls:    1
----------------------------------
Bar():
  last:     Xus
  average:  Xus
  variance: Xus
  calls:    1

```

where `X` is a duration.
