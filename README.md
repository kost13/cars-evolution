# Cars Evolution
![GitHub Actions](https://github.com/kost13/cars-evolution/workflows/cars-evolution-ci/badge.svg)

## Dependencies
- CMake >= 3.10
- Qt >= 5.9.5 + QtQuick and QtQuick Controls
- Boost >= 1.65
- Windows: MSVC 14 (Microsoft Visual Studio Express for Dekstop 2015)

For development
- clang-format
- clang-tidy

### Linux dependencies installation
run *install.sh* script

### Repository setup
```
git clone git@github.com:kost13/cars_evolution.git
```
or
```
git clone https://github.com/kost13/cars_evolution
```

```
cd cars_evolution
git submodule update --init
```

### Building
a) run *build.sh* from *scripts* <br/>
b) run the following commands

```
mkdir build && cd build
cmake ../ -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=<QT_PATH>
cmake --build . --target all
```

## Code managements
Source code is formated using clang-format. \
Static code analysis is done using clang-tidy:
*cppcoreguidelines, boost, google, misc, modernize, performance, readability*. \
Description: https://clang.llvm.org/extra/clang-tidy/#using-clang-tidy

## Code style
Consistent with https://google.github.io/styleguide/cppguide.html
exceptions:
- methods and functions start with lowercase
- files names - upper camel case
