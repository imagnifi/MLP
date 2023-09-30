#ifndef MLP_SRC_MODEL_READER_READER_H_
#define MLP_SRC_MODEL_READER_READER_H_

#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "../../s21_types.h"

namespace s21 {

class Reader {
 public:
  Reader() = default;
  inline ~Reader() {
    if (file_.is_open()) file_.close();
  }
  auto Open(const std::string &filename) -> void;
  inline auto Close(const std::string &filename) -> void {
    if (file_.is_open()) file_.close();
  };
  inline auto is_open() const -> bool { return file_.is_open(); };
  static auto ReadStr(std::fstream &file)
      -> std::pair<std::vector<double>, int>;
  auto ReadFull() -> std::vector<std::pair<std::vector<double>, int>>;
  auto static ReadFull(const std::string &filename)
      -> std::vector<std::pair<std::vector<double>, int>>;

 private:
  std::fstream file_;
};
}  // namespace s21
#endif  // MLP_SRC_MODEL_READER_READER_H_
