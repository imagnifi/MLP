#ifndef CPP7_MLP_SRC_S21_TYPES_H_
#define CPP7_MLP_SRC_S21_TYPES_H_
#include <vector>
namespace s21 {
typedef struct {
//  double avg_recognize;
  unsigned int all, correct;
  long long seconds;

  double avg_accuracy;
  double precision;
  double recall;
  double f_measure;
} NetworkMetrics;

enum class NetworkType {
  BASIC, GRAPH, MATRIX
};

using TestCase = std::pair<std::vector<double>, int>;
using DataSet = std::vector<TestCase>;
enum LayersInfo { kInputLayer = 784, kHiddenLayer = 100, kOutLayer = 26 };
using IterType = decltype(DataSet{}.begin());
} // s21

#endif //CPP7_MLP_SRC_S21_TYPES_H_
