#!/bin/bash
# this file should be called from build directory
lcov --zerocounters --directory .
$1
lcov --directory . --capture --output-file full_cars_evolution_coverage.info
lcov --remove full_cars_evolution_coverage.info '*/extern/*' '/usr/include/*' '*tests*' '*boost*' '*.h' -o cars_evolution_coverage.info
genhtml --output-directory coverage --demangle-cpp --num-spaces 2 --sort --title "Cars Evolution Test Coverage" --function-coverage --legend cars_evolution_coverage.info

