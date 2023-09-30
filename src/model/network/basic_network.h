#ifndef CPP7_MLP_SRC_MODEL_NETWORK_BASIC_NETWORK_H_
#define CPP7_MLP_SRC_MODEL_NETWORK_BASIC_NETWORK_H_

#include <algorithm>
#include <chrono>
#include <cmath>
#include <memory>
#include <random>
#include <stdexcept>
#include <vector>

#include "../../s21_types.h"

namespace s21 {

class BasicNetwork {
 public:
  //  using IterType = std::__wrap_iter<std::vector
  //   <std::pair<std::vector<double>, int>,
  //   std::allocator<std::pair<std::vector<double>, int>>>::pointer>;
  static constexpr double kLearningRate = 0.1;
  static constexpr double kActivationThreshold = 0.5;
  static constexpr double kTargetAccuracy = 0.8;

  BasicNetwork() = default;
  //   ~BasicNetwork() = default;
  virtual ~BasicNetwork() = default;

  virtual void InitNetwork(int hidden_layers) = 0;
  virtual NetworkType GetType() { return NetworkType::BASIC; };
  [[nodiscard]] virtual size_t GetSize() const = 0;

  virtual void ExportWeights(const std::string &filename) = 0;
  virtual void ImportWeights(const std::string &filename) = 0;

  virtual void SetInput(const std::vector<double> &sample) = 0;

  /// Train without report
  void Train(const DataSet &data, int eras = 1);
  std::vector<double> TrainWithReport(DataSet &train_sample,
                                      DataSet &test_sample, int eras,
                                      double sample_size);
  std::vector<double> CrossValidation(DataSet &sample, int parts, int eras);

  NetworkMetrics Test(DataSet &test_sample, double sample_size);

  void Recognize(std::vector<double> &input);
  std::pair<int, double> Recognized();

 protected:
  ///  @brief Signal pass forward
  virtual void ForwardPropagation_() = 0;
  /** Signal return with weight correction
   * @param expected number of letter [0-25] that should be equals 1, others 0
   */
  virtual void BackPropagation_(int expected) = 0;
  /**
   * @brief Calculating deltas of weights for every layer except last
   * @param deltas vector with old deltas, it updates with new
   * @param layer number of layer
   */
  virtual void CalcDeltas_(int layer) = 0;
  /**
   * @brief Calculating deltas of weights for only last layer
   * @param deltas vector with old deltas, it updates with new
   * @param expected number of letter [0-25] that should be equals 1, others 0
   */
  virtual void LastDeltas_(int expected) = 0;

  //  virtual NetworkStatistics Test_(IterType first, IterType end) = 0;
  /**
   *
   * @param first
   * @param end iterator after last tests case
   */
  NetworkMetrics Test_(const IterType &first, const IterType &end);

  static inline double SigmoidActivation_(double value) {
    return 1.0 / (1.0 + exp(-value));
  }
  static inline double SigmoidDerivative_(double num) {
    return num * (1. - num);
  }

  /**
   * @return Values of last layer, needs to testing a network
   */
  [[nodiscard]] virtual std::vector<double> LastLayerValues_() const = 0;

  /**
   * @brief Vector with deltas, it must be rewritten every back propagation
   * cycle. Declared here to avoid memory reallocating. Used in BackPropagation,
   * CalcDeltas, LastDeltas
   */
  std::vector<double> deltas_{};

 private:
  typedef struct {
    unsigned int all, correct, tp, fp, tn, fn;
  } NetworkStatistics;

  static NetworkMetrics CalcMetrics_(NetworkStatistics &stat);
  static std::vector<int> CrossValidIntervals_(unsigned size, unsigned parts);
  bool CheckResultLayer_(int expected);
};
}  // namespace s21

#endif  // CPP7_MLP_SRC_MODEL_NETWORK_BASIC_NETWORK_H_
