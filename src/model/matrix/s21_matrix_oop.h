#include <exception>
#include <iostream>
#include <string>

class S21Matrix {
 private:
  int rows_ = 0;
  int columns_ = 0;
  double **matrix_ = nullptr;

 public:
  S21Matrix();
  S21Matrix(int rows, int columns);
  S21Matrix(const S21Matrix &other);
  S21Matrix(S21Matrix &&other) noexcept;

  ~S21Matrix();

  int GetRow();
  int GetColumn();

  [[maybe_unused]] void SetRow([[maybe_unused]] int value);
  [[maybe_unused]] void SetColumn(int value);

  double *operator[](int i) const;
  double &operator()(int i, int j);
  [[maybe_unused]] bool IsNull();

  S21Matrix operator+(const S21Matrix &other);
  S21Matrix operator-(const S21Matrix &other);
  S21Matrix operator*(const S21Matrix &other);
  S21Matrix operator*(double number);
  double Determinant();
  S21Matrix Transpose();
  S21Matrix CalcComplements();
  void SubMatrix(const S21Matrix &other);
  void SumMatrix(const S21Matrix &other);
  void MulNumber(double num);
  void MulMatrix(const S21Matrix &other);
  [[maybe_unused]] S21Matrix inverse_matrix();

  S21Matrix &operator=(const S21Matrix &other);

  S21Matrix &operator+=(const S21Matrix &other);
  S21Matrix &operator-=(const S21Matrix &other);
  S21Matrix &operator*=(const S21Matrix &other);
  S21Matrix &operator*=(double number);

  bool operator==(const S21Matrix &other);
  bool EqMatrix(const S21Matrix &other);

  void GetMinor(S21Matrix src, int rows, int column);
  void show();
  void clear();

 private:
  void AllocateMem(int rows, int columns);
  bool IsNormalMatrix();
};
