#include <gtest/gtest.h>

#include <iostream>
#include <utility>

#include "../model/network/basic_network.h"
#include "../model/network/graph/graph_network.h"
#include "../model/network/matrix/matrix_network.h"
#include "../model/reader/reader.h"

auto kBigSample = s21::Reader::ReadFull("tests/emnist-2000.csv");
auto kSmallSample = s21::Reader::ReadFull("tests/emnist-400.csv");

TEST(matrix, test_init) {
  s21::MatrixNetwork network(2);
  EXPECT_ANY_THROW(network.InitNetwork(1));
  EXPECT_ANY_THROW(network.InitNetwork(6));
  for (int i = 2; i <= 5; ++i) {
    network.InitNetwork(i);
    EXPECT_EQ(network.GetSize(), i + 2);
  }
}

TEST(matrix, import_export) {
  s21::MatrixNetwork network1(2);
  s21::MatrixNetwork network2(2);
  std::string filename = "matrix_import_export_test.weights";
  EXPECT_NO_THROW(network1.ExportWeights(filename));
  EXPECT_NO_THROW(network2.ImportWeights(filename));
  filename = "rm " + filename;
  system(filename.c_str());
}

TEST(matrix, train_test) {
  s21::MatrixNetwork network(2);
  std::vector<double> rep;
  EXPECT_NO_THROW(
      rep = network.TrainWithReport(kSmallSample, kBigSample, 1, 0.1));
  EXPECT_EQ(rep.size(), 1);
  s21::NetworkMetrics metr1 = network.Test(kBigSample, 1);
}

TEST(matrix, cross_validation) {
  s21::MatrixNetwork network(2);
  std::vector<double> rep;
  EXPECT_NO_THROW(rep = network.CrossValidation(kSmallSample, 4, 2));
  EXPECT_FALSE(rep.empty());
}

TEST(graph, test_init) {
  s21::GraphNetwork network(2);
  EXPECT_ANY_THROW(network.InitNetwork(1));
  EXPECT_ANY_THROW(network.InitNetwork(6));
  for (int i = 2; i <= 5; ++i) {
    network.InitNetwork(i);
    EXPECT_EQ(network.GetSize(), i + 2);
  }
}

TEST(graph, import_export) {
  s21::GraphNetwork network1(2);
  s21::GraphNetwork network2(2);
  std::string filename = "matrix_import_export_test.weights";
  EXPECT_NO_THROW(network1.ExportWeights(filename));
  EXPECT_NO_THROW(network2.ImportWeights(filename));
  filename = "rm " + filename;
  system(filename.c_str());
}

TEST(graph, train_test) {
  s21::GraphNetwork network(2);
  std::vector<double> rep;
  EXPECT_NO_THROW(
      rep = network.TrainWithReport(kSmallSample, kBigSample, 1, 0.1));
  EXPECT_EQ(rep.size(), 1);
  s21::NetworkMetrics metr1 = network.Test(kBigSample, 1);
}

TEST(graph, cross_validation) {
  s21::GraphNetwork network(2);
  std::vector<double> rep;
  EXPECT_NO_THROW(rep = network.CrossValidation(kSmallSample, 4, 2));
  EXPECT_FALSE(rep.empty());
}

int main() {
  system("pwd");
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}
