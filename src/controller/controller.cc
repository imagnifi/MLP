#include "controller.h"

#include <memory>

namespace s21 {

Controller::Controller(int hidden_layers_count, NetworkType network_type) {
  if (network_type == NetworkType::GRAPH) {
    network_ = std::make_unique<GraphNetwork>(hidden_layers_count);
  } else if (network_type == NetworkType::MATRIX) {
    network_ = std::make_unique<MatrixNetwork>(hidden_layers_count);
  } else
    throw std::invalid_argument("incorrect network type");
}

auto Controller::UpdateNetwork(int hidden_layers_count,
                               NetworkType network_type) -> void {
  if (network_type == NetworkType::GRAPH) {
    network_ = std::make_unique<GraphNetwork>(hidden_layers_count);
  } else if (network_type == NetworkType::MATRIX) {
    network_ = std::make_unique<MatrixNetwork>(hidden_layers_count);
  }
}

}  // namespace s21
