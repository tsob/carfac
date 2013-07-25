// Copyright 2013 The CARFAC Authors. All Rights Reserved.
// Author: Ron Weiss <ronw@google.com>
//
// This file is part of an implementation of Lyon's cochlear model:
// "Cascade of Asymmetric Resonators with Fast-Acting Compression"
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Shared test utilities.

#ifndef CARFAC_TEST_UTIL_H
#define CARFAC_TEST_UTIL_H

#include <fstream>
#include <string>

#include <Eigen/Core>

#include "gtest/gtest.h"

#include "common.h"

// Location of the text files produced by 'CARFAC_GenerateTestData.m' for
// comparing the ouput of the Matlab implementation with the C++ one.
static const char* kTestDataDir = "../test_data/";

// Reads a size rows by columns Eigen matrix from a text file written
// using the Matlab dlmwrite function.
ArrayXX LoadMatrix(const std::string& filename, int rows, int columns) {
  std::string fullfile = kTestDataDir + filename;
  std::ifstream file(fullfile.c_str());
  ArrayXX output(rows, columns);
  assert(file.is_open());
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      file >> output(i, j);
    }
  }
  file.close();
  return output;
}

void WriteMatrix(const std::string& filename, const ArrayXX& matrix) {
  std::string fullfile = kTestDataDir + filename;
  std::ofstream ofile(fullfile.c_str());
  const int kPrecision = 9;
  ofile.precision(kPrecision);
  if (ofile.is_open()) {
    Eigen::IOFormat ioformat(kPrecision, Eigen::DontAlignCols);
    ofile << matrix.format(ioformat) << std::endl;
  }
  ofile.close();
}

void AssertArrayNear(const ArrayXX& expected, const ArrayXX& actual,
                     double precision) {
  ArrayXX abs_difference = (expected - actual).cwiseAbs();
  ASSERT_TRUE(abs_difference.maxCoeff() <= precision)
      << "expected differs from actual by more than " << precision
      << "\n  max(abs(expected - actual)) = " << abs_difference.maxCoeff()
      << "\n  max(abs(expected)) = " << expected.cwiseAbs().maxCoeff()
      << "\n  max(abs(actual)) = " << actual.cwiseAbs().maxCoeff();
}

#endif  // CARFAC_TEST_UTIL_H
