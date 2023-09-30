#ifndef MLP_SRC_MODEL_NETWORK_NEURON_H_
#define MLP_SRC_MODEL_NETWORK_NEURON_H_
#include <cstdio>
#include <vector>

namespace s21 {

class Neuron {
 public:
  /*-------------------------------constructors destructors-------------------*/
  Neuron() = default;
  explicit Neuron(int weights, double val = 0.);
  explicit Neuron(std::vector<double> &&weights) : weights_(weights) {};
  Neuron(const Neuron &other);
  Neuron(Neuron &&other) noexcept;

  ~Neuron() = default;

  /*----------------------------------------getters----------------------------*/
  auto getValue() const ->  double ;
  auto getWeights() -> std::vector<double> &;
  inline auto getWeight(std::size_t n) -> double { return weights_[n]; };
  auto getSumWeights() -> double;
  inline auto getDelta() const -> double { return delta_; }

  /*----------------------------------------setters----------------------------*/
  inline auto setValue(double value) -> void { self_value_ = value; }
  inline auto setDelta(double value) -> void { delta_ = value; }
  auto setWeights(std::vector<double> &&) -> void;
  inline auto setWeight(std::size_t n, double value) { weights_[n] = value; }

  /*----------------------------------------operators--------------------------*/
  auto operator[](std::size_t) -> double &;
  auto operator=(const Neuron &other) -> Neuron &;
  auto operator=(Neuron &&other) noexcept -> Neuron &;
  auto operator+=(double n) -> Neuron &;

  /*----------------------------------------other methods----------------------*/
  auto ClearWeights() -> void;
  auto AddOneWeight(double) -> void;
  inline auto IsActivate() const -> bool { return self_value_ >= 0.5; }
  inline auto size() -> size_t { return weights_.size(); }

 private:
  std::vector<double> weights_;
  double self_value_{0.0};
  double delta_{0.0};
};  //  class Neuron

}  //  namespace s21

#endif  // MLP_SRC_MODEL_NETWORK_NEURON_H_
