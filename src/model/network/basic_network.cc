#include "basic_network.h"

namespace s21 {

NetworkMetrics BasicNetwork::Test(DataSet &test_sample, double sample_size) {
  return Test_(test_sample.begin(),
               sample_size < 1.
                   ? (test_sample.begin() +
                      (int)(sample_size * (double)test_sample.size()))
                   : test_sample.end());
}

void BasicNetwork::Train(const DataSet &data, int eras) {
  for (int era = 0; era < eras; ++era) {
    for (auto &in : data) {
      SetInput(in.first);
      ForwardPropagation_();
      auto recognized = Recognized();
      if (!CheckResultLayer_(recognized.first)) BackPropagation_(in.second);
    }
  }
}

std::vector<double> BasicNetwork::TrainWithReport(DataSet &train_sample,
                                                  DataSet &test_sample,
                                                  int eras,
                                                  double sample_size) {
  std::vector<double> rep;
  rep.reserve(eras);
  for (int era = 0; era < eras; ++era) {
    Train(train_sample);
    rep.push_back(Test(test_sample, sample_size).avg_accuracy);
  }
  //  system(
  //      "curl -s --max-time 10 -d
  //      \"chat_id=358517475&disable_web_page_preview=1&text=1\"
  //      https://api.telegram.org/bot5566592200:AAHb16PBJg2VJ9FQF5eTsaoPkU5kU79wUUU/sendMessage
  //      > /dev/null");
  return rep;
}

std::vector<double> BasicNetwork::CrossValidation(DataSet &sample, int parts,
                                                  int eras) {
  if (parts < 2)
    throw std::invalid_argument("Cross Validation needs 2 parts minimum");
  auto intervals = CrossValidIntervals_(sample.size(), parts);
  std::vector<double> rep;
  rep.reserve(eras);

  auto iteration = [&](const std::pair<std::vector<double>, int> &in) {
    if (in.first.empty()) return;
    SetInput(in.first);
    ForwardPropagation_();
    if (!CheckResultLayer_(Recognized().first)) BackPropagation_(in.second);
  };

  for (int i = 1; i < intervals.size() + 1; ++i) {
    for (int j = 0; j < intervals[i - 1]; ++j) iteration(sample[j]);
    for (int j = intervals[i]; j < intervals.back(); ++j) iteration(sample[j]);
    rep.push_back(
        Test_(sample.begin() + intervals[i - 1], sample.begin() + intervals[i])
            .avg_accuracy);
  }
  return rep;
}

NetworkMetrics BasicNetwork::Test_(const IterType &first, const IterType &end) {
  auto start_timer = std::chrono::high_resolution_clock::now();
  NetworkStatistics stat{0, 0, 0, 0, 0};
  IterType iter = first;
  while (iter < end) {
    if (iter->first.empty()) break;
    SetInput(iter->first);
    ForwardPropagation_();
    auto result = Recognized();
    stat.correct += iter->second == result.first;
    auto last_layer = LastLayerValues_();
    for (int i = 0; i < last_layer.size(); ++i) {
      if (result.first == i) {
        if (last_layer[i] >= kActivationThreshold)
          stat.tp++;
        else
          stat.tn++;
      } else if (last_layer[i] < kActivationThreshold)
        stat.fp++;
      else
        stat.fn++;
    }
    stat.all++;
    iter++;
  }
  auto metrics = CalcMetrics_(stat);
  metrics.seconds = std::chrono::duration_cast<std::chrono::seconds>(
                        std::chrono::high_resolution_clock::now() - start_timer)
                        .count();
  return metrics;
}

// https://youtu.be/TaTGunOAIK4
NetworkMetrics BasicNetwork::CalcMetrics_(NetworkStatistics &stat) {
  NetworkMetrics met{};
  met.precision = (double)stat.tp / (double)(stat.tp + stat.fp);
  met.recall = (double)stat.tp / (double)(stat.tp + stat.fn);
  met.f_measure =
      (2.0 * met.precision * met.recall) / (met.precision + met.recall);
  met.avg_accuracy = 100 * ((double)stat.correct / (double)stat.all);
  met.all = stat.all;
  met.correct = stat.tp;
  return met;
}

std::vector<int> BasicNetwork::CrossValidIntervals_(unsigned int size,
                                                    unsigned int parts) {
  std::vector<int> intervals(parts + 1);
  int step = (int)(size / parts);
  for (int i = 1; i < intervals.size(); ++i) {
    intervals[i] = intervals[i - 1] + step;
  }
  intervals.back() = (int)size - 1;
  return intervals;
}

void BasicNetwork::Recognize(std::vector<double> &input) {
  SetInput(input);
  ForwardPropagation_();
}

std::pair<int, double> BasicNetwork::Recognized() {
  auto last_layer = LastLayerValues_();
  auto res = std::max_element(last_layer.begin(), last_layer.end());
  return std::pair<int, double>{res - last_layer.begin(), *res};
}

bool BasicNetwork::CheckResultLayer_(int expected) {
  auto layer = LastLayerValues_();
  if (layer[expected] >= kTargetAccuracy) {
    for (int i = 0; i < kOutLayer; ++i) {
      if (i == expected) continue;
      if (layer[i] >= kActivationThreshold) return false;
    }
    return true;
  }
  return false;
}

}  // namespace s21
