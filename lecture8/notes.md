# Lecture 8: Oct 24 2019

## Stream iterators

[`ostream.cpp`](ostream.cpp)
```c++
```

- Iterators are elements of _sequences_, not containers.
- Iterators can be made from streams as well.
- `ostream_iterator` can write to an `ostream` with the same interface as a container.
  - `ostream_iterator` cannot be read from. What I said earlier about all iterators having `*` and `->` was a simplication. There are _input_ and _output_ iterators, and `ostream_iterator`s are output iterators only.

[`istream.cpp`](istream.cpp)
```c++
```

- Each time an `istream_iterator` is incremented, it will read from its stream.
- When it is dereferenced, it will return the value last read.
- The default-constructed `istream_iterator` represents the end-of-stream iterator.

## Associative containers

- Containers that store a collection of key-value pairs (or something similar), which are not ordered by the user.

### First: `pair`

[`.cpp`](.cpp)
```c++
```

- Combines two objects of any type.
- In `<utility>`.
- Use `make_pair` to avoid typing out the types.
- Access the elements using `.first` and `.second` (note these are member _objects_, not member _functions_).
- Generalized by `tuple`.
- One possible implementation for an associative container is just a sequence container of `pair`s.

### `map`

[`map1.cpp`](map1.cpp)
```c++
```

- Typically implemented using a balanced BST like a red-black tree.
- `[]` returns a reference to the value corresponding to a key, if the key exists in the map. If the key doesn't exist in the map, it will be created.
- Other familiar member functions are also available:
  - `at` accesses a key's value. If the key doesn't exist, an exception is thrown.
  - `empty`, `size`, `clear`, `erase`.
  - `insert` and `emplace` are just like the functions on sequential containers, except they take a `pair` (no iterator required, since we don't need to manage the order ourselves).
- Iterating through the container will always go in sorted order.

[`map2.cpp`](map2.cpp)
```c++
```

- If your key is a custom class, you'll need to specify an ordering to use.
  - By default `map` uses `<`.
  - You can customize this by passing in a comparison _function object_ (more on this later) as a template argument.
    - The standard library has a few built in: `less<Key>`, which uses `operator<` for your class, is the default.
    - Others include `greater<Key>`, `less_equal<Key>`, and `greater_equal<Key>`.
- `map` doesn't use `operator==` to check for equality; we _only_ require an ordering.
  - If `!(a < b) && !(b < a)` then `map` considers `a` and `b` equivalent.

### `unordered_map`

[`unordered_map.cpp`](unordered_map.cpp)
```c++
```

- `map` uses a BST, with logarithmic complexity for its operations.
- Hash tables can do better, and that's what `unordered_map` is.
  - Average constant time complexity of its operations.
- Otherwise, `unordered_map` has basically the same interface as `map`.
- What should we do for custom key types?

### `hash`

- Other languages have hash codes built in to the language.
  - This requires a common base class.
  - But not everything in C++ is an object!
  - Also a virtual call may be required if we use a hash code method. This forced overhead is really not in the spirit of C++.
- This also uses a comparison object (so we'll talk about it in a later lecture).
- `unordered_map` also needs `operator==`, to deal with hash collisions.

### `set` and `unordered_set`

- These containers are basically the `map` and `unordered_map` containers, but without a value attached to the keys.

### `multi` variants

- There are also the `multimap`, `unordered_multimap`, `multiset`, and `unordered_multiset` classes for associative containers that can have multiple copies of a key.
- These containers can return a sequence of results using a start and end iterator.

## Templates

[`.cpp`](.cpp)
```c++
```

- Templates are used for _parametric polymorphism_, for writing code that does not depend on the type of a value.


[`.cpp`](.cpp)
```c++
```

- The things we've talked about in the past 2 weeks are _not_ object-oriented.
  - If it were, there would be overhead in the form of virtual function calls when trying to perform operations abstractly.
