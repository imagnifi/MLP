#ifndef CPP7_MLP_SRC_MODEL_NETWORK_MATRIX_NETWORK_H_
#define CPP7_MLP_SRC_MODEL_NETWORK_MATRIX_NETWORK_H_
#include <fstream>

#include "../../matrix/s21_matrix_oop.h"
#include "../basic_network.h"
namespace s21 {

class MatrixNetwork : public BasicNetwork {
 public:
  explicit MatrixNetwork(int hidden_layers) {
    InitRandomWeights_(hidden_layers);
  }
  ~MatrixNetwork() override = default;
  void InitNetwork(int hidden_layers) override;
  inline NetworkType GetType() override { return NetworkType::MATRIX; }
  [[nodiscard]] size_t GetSize() const override{ return neurons_.size(); }
  void ExportWeights(const std::string &filename) override;
  void ImportWeights(const std::string &filename) override;
  void SetInput(const std::vector<double> &sample) override;

 protected:
  void ForwardPropagation_() override;
  void BackPropagation_(int expected) override;
  void CalcDeltas_(int layer) override;
  void LastDeltas_(int expected) override;
  [[nodiscard]] std::vector<double> LastLayerValues_() const override;

 private:
  std::vector<S21Matrix> neurons_;
  std::vector<S21Matrix> weights_;
  std::vector<LayersInfo> size_layers_;
  std::vector<double> standard_output_values_;
  std::vector<double> result_errors_;
  std::vector<std::pair<std::pair<int, double>, std::pair<char, char>>>
      result_classes_input_data_;

  void InitRandomWeights_(int hidden_layers);
  void SetLayers_(std::vector<LayersInfo> &size_layers);
  static void InitWeightMatrix_(S21Matrix &matrix, std::uniform_int_distribution<int> &distr, std::mt19937 &mt);
  bool CheckLayersSizes_(const std::vector<size_t> &sizes);
};

}  // namespace s21

#endif  // CPP7_MLP_SRC_MODEL_NETWORK_MATRIX_NETWORK_H_
