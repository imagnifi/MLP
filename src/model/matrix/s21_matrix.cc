#include <cmath>
#include <iostream>

#include "s21_matrix_oop.h"

void S21Matrix::AllocateMem(int rows, int columns) {
  matrix_ = new double*[rows];
  for (int i = 0; i < rows; i++) {
    matrix_[i] = new double[columns]{0};
  }
}

S21Matrix::S21Matrix() {
  rows_ = 0;
  columns_ = 0;
  matrix_ = nullptr;
}
S21Matrix::S21Matrix(int rows, int columns) {
  if (rows <= 0 || columns <= 0) {
    throw std::invalid_argument("Too small size");
  } else {
    rows_ = rows;
    columns_ = columns;
    this->AllocateMem(rows, columns);
  }
}

// конструктор копирования
S21Matrix::S21Matrix(const S21Matrix& other) {
  if (this != &other) {
    rows_ = other.rows_;
    columns_ = other.columns_;
    this->AllocateMem(other.rows_, other.columns_);
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < columns_; j++) {
        matrix_[i][j] = other.matrix_[i][j];
      }
    }
  }
}

S21Matrix::S21Matrix(S21Matrix&& other) noexcept {
  if (this != &other) {
    rows_ = other.rows_;
    columns_ = other.columns_;
    matrix_ = other.matrix_;
    other.matrix_ = nullptr;
    other.rows_ = 0;
    other.columns_ = 0;
  }
}

S21Matrix::~S21Matrix() {
  if (matrix_ != nullptr) {
    for (int i = 0; i < rows_; i++) {
      delete matrix_[i];
    }
    delete matrix_;
  }
  matrix_ = nullptr;
  rows_ = 0;
  columns_ = 0;
}

S21Matrix& S21Matrix::operator=(const S21Matrix& other) {
  if (this != &other) {
    this->~S21Matrix();
    rows_ = other.rows_;
    columns_ = other.columns_;
    AllocateMem(rows_, columns_);
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < columns_; j++) {
        matrix_[i][j] = other.matrix_[i][j];
      }
    }
  }
  return *this;
}

double& S21Matrix::operator()(int i, int j) {
  if (i >= rows_ || j >= columns_) {
    throw std::out_of_range("(): index is out of range");
  } else {
    return matrix_[i][j];
  }
}

int S21Matrix::GetRow() { return rows_; }

int S21Matrix::GetColumn() { return columns_; }

[[maybe_unused]] void S21Matrix::SetRow(int value) {
  if (value > 0) {
    this->~S21Matrix();
    rows_ = value;
    this->AllocateMem(rows_, columns_);
  } else {
    throw std::invalid_argument("Row value less or equal to zero");
  }
}

[[maybe_unused]] void S21Matrix::SetColumn(int value) {
  if (value > 0) {
    this->~S21Matrix();
    columns_ = value;
    this->AllocateMem(rows_, columns_);
  } else {
    throw std::invalid_argument("Column value less or equal to zero");
  }
}

S21Matrix S21Matrix::operator+(const S21Matrix& other) {
  S21Matrix res = *this;
  res.SumMatrix(other);
  return res;
}

S21Matrix S21Matrix::operator-(const S21Matrix& other) {
  S21Matrix res = *this;
  res.SubMatrix(other);
  return res;
}

S21Matrix& S21Matrix::operator-=(const S21Matrix& other) {
  this->SubMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator+=(const S21Matrix& other) {
  this->SumMatrix(other);
  return *this;
}

S21Matrix S21Matrix::operator*(const S21Matrix& other) {
  S21Matrix res = *this;
  res.MulMatrix(other);
  return res;
}

S21Matrix S21Matrix::operator*(double number) {
  S21Matrix res = *this;
  res.MulNumber(number);
  return res;
}

S21Matrix& S21Matrix::operator*=(const S21Matrix& other) {
  this->MulMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator*=(double number) {
  this->MulNumber(number);
  return *this;
}

bool S21Matrix::operator==(const S21Matrix& other) {
  bool res = this->EqMatrix(other);
  return res;
}

void S21Matrix::GetMinor(const S21Matrix src, int row, int column) {
  if (rows_ + 1 == src.rows_ && columns_ + 1 == src.columns_) {
    for (int i = 0, flag_i = 0; i < src.rows_; i++) {
      if (i == row) {
        flag_i = 1;
      } else {
        for (int j = 0, flag_d = 0; j < src.columns_; j++) {
          if (j == column) {
            flag_d = 1;
          } else {
            matrix_[i - flag_i][j - flag_d] = src.matrix_[i][j];
          }
        }
      }
    }
  } else {
    throw std::invalid_argument("Incorrect size of matrix");
  }
}

double S21Matrix::Determinant() {
  double D = 0;
  if (columns_ == rows_) {
    if (rows_ == 1) return matrix_[0][0];
    S21Matrix temp(rows_ - 1, columns_ - 1);
    for (int i = 0; i < rows_; i++) {
      temp.GetMinor(*this, 0, i);
      int one = 1;
      if (i % 2 == 1) one = -1;
      D += matrix_[0][i] * temp.Determinant() * one;
    }
  } else {
    throw std::invalid_argument("matrix is not a sqare");
  }

  return D;
}

S21Matrix S21Matrix::Transpose() {
  S21Matrix res(columns_, rows_);
  for (int i = 0; i < columns_; i++) {
    for (int j = 0; j < rows_; j++) {
      res.matrix_[i][j] = matrix_[j][i];
    }
  }
  return res;
}

S21Matrix S21Matrix::CalcComplements() {
  S21Matrix res(rows_, columns_);
  if (res.rows_ == res.columns_) {
    S21Matrix calc(rows_ - 1, columns_ - 1);
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < columns_; j++) {
        calc.GetMinor(*this, i, j);
        res.matrix_[i][j] = calc.Determinant();
        if ((i + j) % 2 == 1) res.matrix_[i][j] *= -1;
      }
    }
  } else {
    throw std::invalid_argument("matrix is not a sqare");
  }
  return res;
}

void S21Matrix::SubMatrix(const S21Matrix& other) {
  if (matrix_ == nullptr || other.matrix_ == nullptr)
    throw std::invalid_argument("Matrix is not exist");
  if (rows_ == other.rows_ && columns_ == other.columns_) {
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < columns_; j++) {
        matrix_[i][j] -= other.matrix_[i][j];
      }
    }
  } else {
    throw std::invalid_argument("different sizes");
  }
}

void S21Matrix::SumMatrix(const S21Matrix& other) {
  if (matrix_ == nullptr || other.matrix_ == nullptr)
    throw std::invalid_argument("matrix is not exist");
  if (rows_ == other.rows_ && columns_ == other.columns_) {
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < columns_; j++) {
        matrix_[i][j] += other.matrix_[i][j];
      }
    }
  } else {
    throw std::invalid_argument("different sizes");
  }
}

bool S21Matrix::EqMatrix(const S21Matrix& other) {
  bool res = true;
  if (rows_ == other.rows_ && columns_ == other.columns_) {
    for (int i = 0; i < rows_ && res; i++) {
      for (int j = 0; j < columns_ && res; j++) {
        if (fabs(matrix_[i][j] - other.matrix_[i][j]) > 0.000001) res = false;
      }
    }
  } else {
    res = false;
  }
  return res;
}

void S21Matrix::MulNumber(double number) {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < columns_; j++) {
      matrix_[i][j] = matrix_[i][j] * number;
    }
  }
}

void S21Matrix::MulMatrix(const S21Matrix& other) {
  if (columns_ != other.rows_) {
    throw std::invalid_argument("Rows from 1st M != cols from 2nd M");
  }
  S21Matrix A(rows_, other.columns_);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < other.columns_; j++) {
      for (int k = 0; k < columns_; k++) {
        A.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
      }
    }
  }
  this->~S21Matrix();
  matrix_ = new double*[A.rows_];
  for (int i = 0; i < A.rows_; i++) {
    matrix_[i] = new double[columns_];
  }
  *this = A;
}

[[maybe_unused]] S21Matrix S21Matrix::inverse_matrix() {
  S21Matrix res(rows_, columns_);
  if (rows_ == columns_ && this->IsNormalMatrix()) {
    double D = this->Determinant();
    if (D != 0) {
      if (rows_ == 1) {
        res[0][0] = 1.0 / D;
      } else {
        res = this->CalcComplements();
        res = res.Transpose();
        res *= (1 / D);
      }
    } else {
      throw std::invalid_argument("matrix Determinant is equal to 0");
    }
  } else {
    throw std::invalid_argument("matrix is not a sqare");
  }
  return res;
}

void S21Matrix::show() {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < columns_; j++) {
      std::cout << this->matrix_[i][j] << " ";
    }
    std::cout << "\n";
  }
  std::cout << "rows " << rows_ << " col " << columns_ << "\n";
}

bool S21Matrix::IsNormalMatrix() {
  bool res = false;
  if (rows_ > 0 && columns_ > 0 && this->matrix_ != nullptr) res = true;
  return res;
}

[[maybe_unused]] bool S21Matrix::IsNull() {
  bool res = false;
  void* ptr = static_cast<void*>(this->matrix_);
  if (ptr == nullptr) {
    res = true;
  }
  return res;
}

double* S21Matrix::operator[](int i) const {
  double* out;
  if (i >= rows_) {
    out = nullptr;
    throw std::out_of_range("[]: index is out of range");
  }
  out = matrix_[i];
  return out;
}

void S21Matrix::clear() {
  if (matrix_ != nullptr) {
    for (int i = 0; i < rows_; i++) {
      delete matrix_[i];
    }
    delete matrix_;
  }
  matrix_ = nullptr;
  rows_ = 0;
  columns_ = 0;
}
