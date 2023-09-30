//
// Created by Vanita Jacki on 12/3/22.
//

#include "graph_network.h"
#include <fstream>

namespace s21 {

void GraphNetwork::InitNetwork(int hidden_layers) {
  InitRandomWeights_(hidden_layers);
}

void GraphNetwork::InitRandomWeights_(int hidden_layers) {
  if (hidden_layers < 2 or hidden_layers > 5) throw std::invalid_argument("Hidden layers could be from 2 to 5");
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> distr(-1000000000, 1000000000);
  auto weights_generator = [&mt, &distr](int count) {
    std::vector<double> weights;
    weights.reserve(count);
    for (int i = 0; i < count; ++i) {
      weights.push_back((double) distr(mt) / (double) distr.max());
    }
    return weights;
  };
  layers_.clear();
  layers_.resize(hidden_layers + 2);

  for (int i = 0; i < kInputLayer; ++i)
    layers_[0].emplace_back(std::move(weights_generator(kHiddenLayer)));
//    layers_[0][i].setWeights(std::move(weights_generator(kHiddenLayer)));
  for (int i = 1; i < hidden_layers; ++i)
    for (int j = 0; j < kHiddenLayer; ++j)
      layers_[i].emplace_back(std::move(weights_generator(kHiddenLayer)));
  for (int j = 0; j < kHiddenLayer; ++j)
    layers_[hidden_layers].emplace_back(weights_generator(kOutLayer));
//      layers_[i][j].setWeights(std::move(weights_generator(kHiddenLayer)));
  for (int j = 0; j < kOutLayer; ++j)
    layers_[hidden_layers + 1].emplace_back();
//    layers_[hidden_layers + 1][j].setWeights(std::move(weights_generator(kOutLayer)));
//    layers_[0][i]
}

void GraphNetwork::ExportWeights(const std::string &filename) {
  std::ofstream file(filename, std::ios_base::out);
  if (file.is_open()) {
    for (int i = 0; i < layers_.size() - 1; ++i)
      for (auto &j : layers_[i])
        for (int k = 0; k < j.size(); ++k)
          file << j[k] << std::endl;
    file.close();
  }
}

void GraphNetwork::ImportWeights(const std::string &filename) {
  std::ifstream file(filename);
  if (file.is_open()) {
    for (auto &i : layers_)
      for (auto &j : i)
        for (int k = 0; k < j.size(); ++k) file >> j[k];
    file.close();
  }
}

void GraphNetwork::SetInput(const std::vector<double> &sample) {
  auto w = sample.begin();
  for (auto &i : layers_[0]) {
    i.setValue(*w);
    ++w;
  }
}

void GraphNetwork::ForwardPropagation_() {
  for (int i = 0; i < layers_.size() - 1; ++i)
    for (int j = 0; j < layers_[i + 1].size(); ++j) {
      double tmp = 0.;
      for (int k = 0; k < layers_[i].size(); ++k) {
        tmp += layers_[i][k][j] * layers_[i][k].getValue();
      }
      layers_[i + 1][j].setValue(SigmoidActivation_(tmp));
    }
}

void GraphNetwork::BackPropagation_(int expected) {
  deltas_.clear();
  for (int i = (int) layers_.size() - 2; i; --i) {
    if (i == layers_.size() - 2) {
      LastDeltas_(expected);
    } else {
      CalcDeltas_(i + 1);
    }
    for (int j = 0; j < layers_[i].size(); ++j)
      for (int k = 0; k < layers_[i + 1].size(); ++k) {
        layers_[i][j][k] +=
            deltas_[k] * layers_[i][j].getValue() * kLearningRate;
      }
  }
}

void GraphNetwork::CalcDeltas_(int layer) {
  std::vector<double> new_deltas;
  new_deltas.reserve(layers_[layer].size());
  for (int i = 0; i < layers_[layer].size(); ++i) {
    double err{0};
    for (int j = 0; j < layers_[layer + 1].size(); ++j)
      err += layers_[layer][i][j] * deltas_[j];
    new_deltas.push_back(err *
        SigmoidDerivative_(layers_[layer][i].getValue()));
  }
  deltas_ = std::move(new_deltas);
}

void GraphNetwork::LastDeltas_(int expected) {
  std::vector<double> expected_res;
  deltas_.clear();
  expected_res.resize(kOutLayer, 0);
  expected_res[expected] = 1.0;
  int layer = (int) layers_.size() - 1;
  for (int i = 0; i < layers_[layer].size(); ++i) {
    double err = expected_res[i] - layers_[layer][i].getValue();
    deltas_.push_back(err * SigmoidDerivative_(layers_[layer][i].getValue()));
  }
//  expected_res[expected - 1] = 0.0;
}

std::vector<double> GraphNetwork::LastLayerValues_() const {
  std::vector<double> vec;
  vec.reserve(kOutLayer);
  for (int i = 0; i < kOutLayer; ++i) vec.push_back(layers_.back()[i].getValue());
  return vec;
}

} // s21