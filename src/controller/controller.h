#ifndef MLP_SRC_CONTROLLER_CONTROLLER_H_
#define MLP_SRC_CONTROLLER_CONTROLLER_H_

#include <memory>

#include "../model/network/graph/graph_network.h"
#include "../model/network/matrix/matrix_network.h"
#include "../model/reader/reader.h"
#include "../s21_types.h"

namespace s21 {

class Controller {
 public:
  explicit Controller(int hidden_layers_count = 2,
                      NetworkType network_type = NetworkType::GRAPH);
  ~Controller() = default;

  [[nodiscard]] const DataSet &TrainSample() const { return train_sample_; }
  [[nodiscard]] const DataSet &TestSample() const { return test_sample_; }

  void UpdateNetwork(int hidden_layers_count, NetworkType network_type);
  inline NetworkType GetType() { return network_->GetType(); }
  inline size_t GetSize() { return network_->GetSize(); }

  void ExportWeights(const std::string &filename) {
    network_->ExportWeights(filename);
  }
  void ImportWeights(const std::string &filename) {
    network_->ImportWeights(filename);
  }

  inline size_t ReadTrainDataset(const std::string &filename) {
    return (train_sample_ = Reader::ReadFull(filename)).size();
  }
  inline size_t ReadTestDataset(const std::string &filename) {
    return (test_sample_ = Reader::ReadFull(filename)).size();
  }

  inline void Train(int eras) { network_->Train(train_sample_, eras); }
  inline std::vector<double> TrainWithReport(int eras, double sample_size) {
    return network_->TrainWithReport(train_sample_, test_sample_, eras,
                                     sample_size);
  }
  inline std::vector<double> CrossValidation(int parts, int eras) {
    return network_->CrossValidation(train_sample_, parts, eras);
  }

  inline NetworkMetrics Test(double sample_size) {
    return network_->Test(test_sample_, sample_size);
  }
  inline std::pair<int, double> RecognizeLetter(std::vector<double> &input) {
    network_->Recognize(input);
    return network_->Recognized();
  }
  auto IsNull() -> bool {
    return network_ == nullptr or network_->GetType() == NetworkType::BASIC;
  }
  auto Recognize(std::vector<double> input) -> void {
    network_->Recognize(input);
  }
  auto Recognized() -> std::pair<int, double> { return network_->Recognized(); }

 private:
  std::unique_ptr<s21::BasicNetwork> network_;
  DataSet train_sample_;
  DataSet test_sample_;
};

}  // namespace s21

#endif  // MLP_SRC_CONTROLLER_CONTROLLER_H_
