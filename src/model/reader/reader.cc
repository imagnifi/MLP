#include "reader.h"
namespace s21 {
auto Reader::Open(const std::string &filename) -> void {
  if (file_.is_open()) file_.close();
  file_ = std::fstream(filename);
  if (!file_.is_open())
    throw std::invalid_argument("file_ \"" + filename + "\" don't exist");
}

auto Reader::ReadStr(std::fstream &file)
    -> std::pair<std::vector<double>, int> {
  if (!file.is_open()) throw std::runtime_error("file isn't open");
  std::pair<std::vector<double>, int> res{{}, 0};
  res.first.reserve(784);
  std::string str;
  char c = ' ';
  double input;
  while (c != ',' and !file.eof()) {
    file.get(c);
    str += c;
  }
  if (!file.eof()) res.second = std::stoi(str) - 1;
  file.get(c);
  while (c != '\n' and !file.eof()) {
    str.clear();
    while (c != ',' and c != '\n' and !file.eof()) {
      str += c;
      file.get(c);
    }
    res.first.push_back(stod(str) / 255.0);
    if (c == '\n' or file.eof()) break;
    file.get(c);
  }
  return res;
}

auto Reader::ReadFull() -> std::vector<std::pair<std::vector<double>, int>> {
  if (!file_.is_open()) throw std::runtime_error("file_ isn't open");
  std::vector<std::pair<std::vector<double>, int>> res;
  while (!file_.eof()) res.push_back(std::move(ReadStr(file_)));
  if (res.back().first.empty()) res.pop_back();
  file_.close();
  return res;
}

auto Reader::ReadFull(const std::string &filename)
    -> std::vector<std::pair<std::vector<double>, int>> {
  std::fstream fp(filename);
  if (!fp.is_open())
    throw std::invalid_argument("file_ \"" + filename + "\" don't exist");

  std::vector<std::pair<std::vector<double>, int>> res;
  while (!fp.eof()) res.push_back(std::move(ReadStr(fp)));
  if (res.back().first.empty()) res.pop_back();
  fp.close();
  return res;
}

}  // namespace s21
