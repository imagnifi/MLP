#include "matrix_network.h"

#include <utility>

namespace s21 {

void MatrixNetwork::InitNetwork(int hidden_layers) {
  InitRandomWeights_(hidden_layers);
}
void MatrixNetwork::ExportWeights(const std::string &filename) {
  std::fstream file;
  file.open(filename, std::fstream::out);
  file << "Network weights" << std::endl;
  for (auto &size_layer : size_layers_) {
    file << size_layer << ' ';
  }
  file << '\n';
  for (auto &weight : weights_) {
    for (int i = 0; i < weight.GetRow(); i++) {
      for (int j = 0; j < weight.GetColumn(); j++) {
        file << weight[i][j] << std::endl;
      }
    }
  }
  file.close();
}
void MatrixNetwork::ImportWeights(const std::string &filename) {
  setlocale(LC_NUMERIC, "C");
  std::fstream file;
  file.open(filename, std::fstream::in);

  std::string checkFile;
  std::getline(file, checkFile, '\n');

  std::string num;
  char c = 0;
  std::vector<size_t> lay_sizes;
  while (c != '\n' && !file.eof()) {
    c = 0;
    while (c != ' ' && c != '\n' && !file.eof()) {
      file.get(c);
      if (c != ' ' && c != '\n' && !file.eof()) {
        num += c;
      }
    }
    if (c != '\n') lay_sizes.push_back(std::stol(num));
    num = "";
  }
  if (CheckLayersSizes_(lay_sizes)) {
    for (auto &weight : weights_) {
      for (int i = 0; i < weight.GetRow(); i++) {
        for (int j = 0; j < weight.GetColumn(); j++) {
          std::getline(file, num, '\n');
          double number = std::stod(num);
          weight[i][j] = number;
        }
      }
    }
  }
  file.close();
}
void MatrixNetwork::SetInput(const std::vector<double> &sample) {
  for (int i = 0; i < sample.size(); i++) {
    neurons_[0][i][0] = sample[i];
  }
}

void MatrixNetwork::ForwardPropagation_() {
  for (size_t i = 0; i < neurons_.size() - 1; i++) {
    neurons_[i + 1] = weights_[i] * neurons_[i];
    for (int j = 0; j < neurons_[i + 1].GetRow(); j++) {
      neurons_[i + 1](j, 0) =
          SigmoidActivation_(neurons_[i + 1](j, 0));
    }
  }
}
void MatrixNetwork::BackPropagation_(int expected) {
  deltas_.clear();
  for (int i = (int) weights_.size() - 1; i >= 0; --i) {
    if (i == static_cast<int>(size_layers_.size()) - 2) {
      LastDeltas_(expected);
    } else {
      CalcDeltas_(i + 1);
    }
    for (int k = 0; k < weights_[i].GetColumn(); ++k) {
      for (int j = 0; j < weights_[i].GetRow(); ++j) {
        weights_[i][j][k] += kLearningRate * deltas_[j] * neurons_[i][k][0];
      }
    }
  }
}

void MatrixNetwork::CalcDeltas_(int layer) {
  std::vector<double> new_grads;
  for (int i = 0; i < weights_[layer].GetColumn(); i++) {
    double res = 0.0;
    for (int j = 0; j < weights_[layer].GetRow(); j++) {
      res += deltas_[j] * weights_[layer][j][i];
    }
    new_grads.push_back(res * SigmoidDerivative_(neurons_[layer][i][0]));
  }
  deltas_ = new_grads;
}

void MatrixNetwork::LastDeltas_(int expected) {
  std::vector<double> expect_values(26, 0);
  expect_values[expected] = 1;
  deltas_.clear();
  for (int i = 0; i < neurons_.back().GetRow(); i++) {
    double error = expect_values[i] - neurons_.back()[i][0];
    deltas_.push_back(error * SigmoidDerivative_(neurons_.back()[i][0]));
  }
}

std::vector<double> MatrixNetwork::LastLayerValues_() const {
  std::vector<double> res(size_layers_.back());
  for (int i = 0; i < (int) res.size(); i++) {
    res[i] = neurons_.back()[i][0];
  }
  return res;
}

void MatrixNetwork::InitRandomWeights_(int hidden_layers) {
  if (hidden_layers < 2 or hidden_layers > 5)
    throw std::invalid_argument("Hidden layers could be from 2 to 5");
  std::vector<LayersInfo> vec({kInputLayer});
  for (int i = 0; i < hidden_layers; i++) {
    vec.push_back(kHiddenLayer);
  }
  vec.push_back(kOutLayer);
  SetLayers_(vec);
}

void MatrixNetwork::SetLayers_(std::vector<LayersInfo> &size_layers) {
  size_layers_ = size_layers;
  for (auto &neuron : neurons_) {
    neuron.clear();
  }
  for (auto &weight : weights_) {
    weight.clear();
  }
  weights_.clear();
  neurons_.clear();

  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> distr(-1000000000, 1000000000);

  for (size_t i = 0; i < size_layers_.size(); ++i) {
    neurons_.emplace_back(size_layers_[i], 1);
    if (i != size_layers_.size() - 1) {
      weights_.emplace_back(size_layers_[i + 1], size_layers_[i]);
      InitWeightMatrix_(weights_.back(), distr, mt);
    }
  }
}

void MatrixNetwork::InitWeightMatrix_(S21Matrix &matrix,
                                      std::uniform_int_distribution<int> &distr,
                                      std::mt19937 &mt) {
  for (int i = 0; i < matrix.GetRow(); i++) {
    for (int j = 0; j < matrix.GetColumn(); j++) {
      matrix(i, j) = static_cast<double>(distr(mt)) / static_cast<double>(distr.max());
    }
  }
}

bool MatrixNetwork::CheckLayersSizes_(const std::vector<size_t> &sizes) {
  bool res = true;
  if (sizes.size() != size_layers_.size()) {
    res = false;
  } else {
    for (size_t i = 0; i < sizes.size(); i++) {
      if (size_layers_[i] != sizes[i]) res = false;
    }
  }
  return res;
}

}  // namespace s21
