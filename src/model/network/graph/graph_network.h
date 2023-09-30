#ifndef CPP7_MLP_SRC_MODEL_NETWORK_GRAPH_NETWORK_H_
#define CPP7_MLP_SRC_MODEL_NETWORK_GRAPH_NETWORK_H_

#include "../basic_network.h"
#include "neuron.h"

namespace s21 {

class GraphNetwork : public BasicNetwork {
 public:
  explicit GraphNetwork(int hidden_layers) { InitRandomWeights_(hidden_layers); }
  ~GraphNetwork() override = default;
  [[nodiscard]] size_t GetSize() const override { return layers_.size(); };
  void InitNetwork(int hidden_layers) override;
  inline NetworkType GetType() override { return NetworkType::GRAPH; }

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
  using layer_t = std::vector<Neuron>;
  void InitRandomWeights_(int count);

  std::vector<layer_t> layers_;
};

} // s21

#endif //CPP7_MLP_SRC_MODEL_NETWORK_GRAPH_NETWORK_H_
