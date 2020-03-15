#define BOOST_TEST_MODULE sample_test test
#include <boost/test/unit_test.hpp>
#include <vector>
#include "core/World.h"

BOOST_AUTO_TEST_CASE(sample_test) {
  std::vector<int> v = {1, 2, 3, 4, 5};

  BOOST_TEST(v.size() == size_t(5));
  BOOST_TEST(!v.empty());

  physics::World w;
  BOOST_TEST(w.vecNorm() == 5.0f);
}
