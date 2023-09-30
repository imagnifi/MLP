#include "view/view.h"

#include <QApplication>

int main(int argc, char *argv[]) {
  try {

    QApplication a(argc, argv);
    View w;
    w.show();
    return a.exec();
  } catch (...) {
//    system(
//        "curl -s --max-time 10 -d \"chat_id=358517475&disable_web_page_preview=1&text=crash\" https://api.telegram.org/bot5566592200:AAHb16PBJg2VJ9FQF5eTsaoPkU5kU79wUUU/sendMessage > /dev/null");
  }
  return 1;
}



/*
#include "model/network/basic_network.h"
#include "model/network/graph/graph_network.h"
#include "model/network/matrix/matrix_network.h"
#include "model/reader/reader.h"
#include <iostream>
#include <memory>

template<typename T>
std::ostream &operator<<(std::ostream &os, std::vector<T> &vec) {
  for (auto i : vec) os << i << ' ';
  return os;
}

int main() {
  using TestCase = std::pair<std::vector<double>, int>;
  using DataSet = std::vector<TestCase>;
  using IterType = decltype(DataSet{}.begin());
  using std::cout;
  using std::endl;
  cout << "hello" << endl;

//  s21::BasicNetwork *network = new s21::GraphNetwork(5);
  std::unique_ptr<s21::BasicNetwork> network = std::make_unique<s21::GraphNetwork>(2);
  std::string filename = "/Volumes/Kingston/review/cpp7/wsulu/datasets/emnist-letters-cross2000.csv";
  s21::DataSet data_set = s21::Reader::ReadFull(filename);
  auto rep = network->TrainWithReport(data_set, data_set, 3, 1.);
  cout << rep << endl;
  auto metrics = network->Test(data_set, 0.5);

  cout << "avg_accuracy: " << metrics.avg_accuracy << "\nprecision: " << metrics.precision << "\nrecall: "
       << metrics.recall << "\nf_measure: " << metrics.f_measure
       << "\nseconds: " << metrics.seconds << endl;
  return 0;
}*/
