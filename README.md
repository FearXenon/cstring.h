# cstring.h

`cstring.h` is a lightweight, C-style string library built around a flexible `string_t` struct. It provides heap-safe, expressive, and convenient string manipulation features in plain C — offering a modern approach to working with strings without relying on external dependencies.

## Features

- Automatic memory management with `str_auto`
- Empty string support with `str_null`
- Safe heap-aware strings (`STR_HEAP`, `STR_OWNER` flags)
- String building, formatting, and tokenization
- Full set of string transformations and utilities
- In-place and out-of-place functions
- Change string encoding (WIP)

## 🧰 Usage

Include the header and source in your project:

```c
#include "cstring.h"
```

### Initialization

```c
// From stack-allocated char*
string_t s = str("hello");

// From heap-allocated char*
string_t s2 = strh(pointer_to_heap_char);
```

### Concatenation and Prepending

```c
string_t hello = str("Hello")););
string_t name = str_cat(hello, ", %s!", "World");
str_println(name);
```

### Trimming

```c
string_t trimmed = str_trim(str("  padded  "));
str_println(trimmed);
```

### Explode and Implode

```c
string_t text = str("one,two,three")););
string_t *parts;
size_t len;

str_explode(text, str(","), &parts, &len);
string_t joined = str_implode(str("-"), parts, len);
str_println(joined);
```

---

## 📦 API Overview

| Function                  | Description                              |
|--------------------------|------------------------------------------|
| `str()`    | Init from stack string                   |
| `strh()` or `str_acquire()`          | Init from heap-allocated string          |
| `str_transfer()`         | Transfer ownership                       |
| `str_borrow()` | Create a string with reference to another strings char*
| `str_clone()`            | Deep copy of string                      |
| `str_cat()`              | Concatenate with format support          |
| `str_prepend()`          | Prepend with format support              |
| `str_tolower()`          | Convert to lowercase                     |
| `str_toupper()`          | Convert to uppercase                     |
| `str_trim()`             | Trim whitespace                          |
| `str_substr()`           | Slice substring                          |
| `str_explode()`          | Split by delimiter                       |
| `str_implode()`          | Join array of strings                    |
| `str_replace()`          | Replace substring                        |
| `str_rev()`              | Reverse string                           |
| `str_equals()`           | Compare strings                          |
| `str_print()`            | Print to stdout                          |
| `str_free()`             | Free resources (if owned)                |

Each function has a corresponding `strr_` version which operates in-place.

---

## 🔧 Memory Ownership Flags

Strings are tracked using flags:

- `STR_HEAP`: String is heap-allocated.
- `STR_OWNER`: This `string_t` instance owns the buffer and is responsible for freeing it.

Use `str_heapify()` when a string must be safely modified.

---

## 🧪 Tests & Example

To run tests or examples:

```bash
make # build the default test binary
make run # runs the default test binary
```

---

## 📜 License

MIT License. See [LICENSE](./LICENSE) for details.

---

## 🤝 Contributing

Contributions welcome! Please open issues or PRs for:

- Bug fixes
- Performance optimizations
- Feature additions

---
