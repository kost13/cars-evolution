# Ewolucja pojazdów

## Dependencies
- CMake 
- Qt 
- Boost 

## Uruchomienie
### pobranie plików
```
git clone git@github.com:kost13/cars_evolution.git
cd cars_evolution
git submodule update --init
```

### budowanie 
```
mkdir build && cd build
cmake <FLAGI> <SCIEZKA_DO_KODU> 
```

flagi:
- NO_TESTS - nie zbuduje testów
- NO_QT - nie zbuduje GUI

budowanie linux
```
cmake build --target all
```
budowanie windows
```
cmake build --target BUILD_ALL #windows
```
instalacja
```
cmake build --install
```

## Zarzadzanie kodem
Kod źródłowy jest formatowany za pomocą clang-format. \
Jakość kodu jest sprawdzana za pomocą clang-tidy:
*cppcoreguidelines, boost, google, misc, modernize, performance, readability*. \
Opis: https://clang.llvm.org/extra/clang-tidy/#using-clang-tidy

