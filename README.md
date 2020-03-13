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
cmake <PATH_TO_SOURCE_CODE> <FLAGS>
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
