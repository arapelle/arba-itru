# `arba-itru`

A C++ library providing intrusive types and helper functions using them.

# Install #
## Requirements ##

Binaries:

- A C++20 compiler (ex: g++-14)
- CMake 3.26 or later

Testing Libraries (optional):

- [Google Test](https://github.com/google/googletest) 1.14 or later (optional)

## Clone

```
git clone https://github.com/arapelle/arba-itru
```

## Use with `conan`

Create the conan package.
```
conan create . --build=missing -c
```
Add a requirement in your conanfile project file.
```python
    def requirements(self):
        self.requires("arba-itru/0.5.0")
```

## Quick Install ##
There is a cmake script at the root of the project which builds the library in *Release* mode and install it (default options are used).
```
cd /path/to/arba-itru
cmake -P cmake/scripts/quick_install.cmake
```
Use the following to quickly install a different mode.
```
cmake -P cmake/scripts/quick_install.cmake -- TESTS BUILD Debug DIR /tmp/local
```

## Uninstall ##
There is a uninstall cmake script created during installation. You can use it to uninstall properly this library.
```
cd /path/to/installed-arba-itru/
cmake -P uninstall.cmake
```

# How to use
## Example - Create intrusive shared and weak pointers
```c++
#include <arba/itru/version.hpp>
#include <arba/itru/intrusive_ref_counter.hpp>
#include <arba/itru/intrusive_weak_ptr.hpp>
#include <iostream>

class intrusive_data : public itru::intrusive_ref_counters<>
{
public:
    explicit intrusive_data(const std::string& str) : data(str) {}
    std::string data;
};

int main()
{
    std::cout << std::format("arba-itru {}", itru::version) << std::endl;

    itru::intrusive_shared_ptr data_isptr = itru::make_intrusive_shared_ptr<intrusive_data>("hello there");
    itru::intrusive_shared_ptr data_isptr_2 = data_isptr;
    itru::intrusive_weak_ptr<intrusive_data> data_iwptr(data_isptr);
    itru::intrusive_weak_ptr data_iwptr_2 = data_iwptr;
    std::cout << data_isptr->use_count() << std::endl;
    std::cout << data_isptr->latent_count() << std::endl;
    std::cout << data_iwptr.use_count() << std::endl;
    std::cout << std::boolalpha << data_iwptr.expired() << std::endl;
    itru::intrusive_shared_ptr data_isptr_3 = data_iwptr_2.lock();
    data_isptr.release();
    data_isptr_2.release();
    std::cout << data_iwptr.use_count() << std::endl;
    std::cout << std::boolalpha << data_iwptr.expired() << std::endl;
    data_isptr_3.release();
    std::cout << data_iwptr.use_count() << std::endl;
    std::cout << std::boolalpha << data_iwptr.expired() << std::endl;

    return EXIT_SUCCESS;
}

```

## Example - Using *arba-itru* in a CMake project
See *basic_cmake_project* in example, and more specifically the *CMakeLists.txt* to see how to use *arba-itru* in your CMake projects.

# License

[MIT License](./LICENSE.md) © arba-itru
