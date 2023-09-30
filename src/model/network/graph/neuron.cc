#include "neuron.h"

#include <random>

namespace s21 {

/*-------------------------------constructors destructors-------------------*/
Neuron::Neuron(int weights, double val) : self_value_(val) {
  std::random_device rd;
  std::mt19937 mt(rd());
  //  std::mt19937 mt(444);
  weights_.reserve(weights);
  for (int i = 0; i < weights; ++i)
    weights_.push_back((double) ((int) mt() % 100000) / 100000);
  //    weights_.push_back(fmod((double) mt() / (double) RAND_MAX, 1.));
}

Neuron::Neuron(const Neuron &other) {
  if (this != &other) {
    self_value_ = other.self_value_;
    weights_ = other.weights_;
  }
}

Neuron::Neuron(Neuron &&other) noexcept {
  if (this != &other) {
    self_value_ = other.self_value_;
    weights_ = std::move(other.weights_);
    other.self_value_ = 0.0;
    other.weights_ = {};
  }
}

/*----------------------------------------getters----------------------------*/
auto Neuron::getValue() const ->  double  { return self_value_; }

auto Neuron::getWeights() -> std::vector<double> & { return weights_; }

auto Neuron::getSumWeights() -> double {
  double res = 0.0;
  for (auto elem : weights_) {
    res += elem;
  }
  return res;
}

/*----------------------------------------setters----------------------------*/

auto Neuron::setWeights(std::vector<double> &&wei) -> void {
  ClearWeights();
  weights_ = wei;
}

/*----------------------------------------operators--------------------------*/
auto Neuron::operator[](std::size_t index) -> double & {
  return weights_.at(index);
}

auto Neuron::operator=(const Neuron &other) -> Neuron & {
  if (this != &other) {
    weights_ = other.weights_;
    self_value_ = other.self_value_;
  }
  return *this;
}

auto Neuron::operator=(Neuron &&other) noexcept -> Neuron & {
  if (this != &other) {
    weights_ = std::move(other.weights_);
    self_value_ = other.self_value_;
  }
  return *this;
}

auto Neuron::operator+=(double n) -> Neuron & {
  self_value_ += n;
  return *this;
}
/*----------------------------------------other methods----------------------*/
auto Neuron::ClearWeights() -> void { if (!weights_.empty()) weights_.clear(); }

auto Neuron::AddOneWeight(double value) -> void { weights_.push_back(value); }

}  // namespace s21
