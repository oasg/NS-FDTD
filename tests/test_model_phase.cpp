#include <gtest/gtest.h>
#include "model/model_builder.hpp"
// Demonstrate some basic assertions.
TEST(ModelBuilder, jsonPhase) {
  std::string path = "../tests/phase.json";
  ModelBuilder builder(path);
  bool result = builder.buildModel();
  ASSERT_TRUE(result);
  
}