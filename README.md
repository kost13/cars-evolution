# Ewolucja pojazdów

## Dependencies
- CMake >= 3.10
- Qt  5.9.5 + QtQuick and QtQuick Controls
- Boost 1.65
- Windows: MSVC 14 (Microsoft Visual Studio Express for Dekstop 2015)

For development
- clang-format
- clang-tidy

### Linux dependencies installation
For Ubuntu 18.04 LTS
```
sudo apt-get install <PACKAGE>
```
packages
```
build-essential
qt5-default
qtdeclarative5-dev
qml-module-qtquick-controls
libboost-all-dev
cmake
```

## Uruchomienie
### pobranie plików
```
git clone git@github.com:kost13/cars_evolution.git
```
lub 
```
git clone https://github.com/kost13/cars_evolution
```

```
cd cars_evolution
git submodule update --init
```

### budowanie 
```
mkdir build && cd build
cmake <FLAGI> ..
```

flagi:
- BUILD_TESTS
- BUILD_GUI
np.
```
cmake -DBUILD_GUI=OFF ..
```

budowanie linux
```
cmake --build . --target all
```
budowanie windows
```
cmake --build . --target BUILD_ALL #windows
```
instalacja
```
cmake --build . --target install
```

## Zarzadzanie kodem
Kod źródłowy jest formatowany za pomocą clang-format. \
Jakość kodu jest sprawdzana za pomocą clang-tidy:
*cppcoreguidelines, boost, google, misc, modernize, performance, readability*. \
Opis: https://clang.llvm.org/extra/clang-tidy/#using-clang-tidy

