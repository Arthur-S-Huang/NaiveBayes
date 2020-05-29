// Copyright (c) 2020 [Your Name]. All rights reserved.

#ifndef BAYES_MODEL_H_
#define BAYES_MODEL_H_

#include <cstdlib>
#include <vector>

#include "image.h"

namespace bayes {

constexpr size_t kNumClasses = 10;
// Shaded or not shaded.
constexpr size_t kNumShades = 2;

class Model {
  
 private:
  vector<vector<int>> index_vector;
  vector<int> class_labels;
  vector<int> train_labels;
  vector<int> test_labels;
  
 public:
  // our 4D array that represents the model
  double probs[kNumClasses][kImageSize][kImageSize][kNumShades];
  vector<Image> images;
  // laplace smoothing value
  const double kLaplace = 0.2;
  // constructor for Model class
  Model();
  // Given two label vectors, generate a vector of indices(line number) in which
  // each class(number) is found in the train labels.
  void GenerateClassIndices(const vector<int>& train_labels_correct,
                            const vector<int>& test_labels_correct);
  // Fill the 4D array that represents the model.
  void GenerateModel();
  // Set up the model by filling it with 0's.
  void InitializeModel();
  // Find the number of times a certain class appears in the label file.
  int FindNumberOfOccurrences(int toFind);
  // Attempt to classify an image given its pixels.
  int ClassifyImage(string image);
  // Calculate model accuracy using the number of images classified correctly.
  double CalculateAccuracy(const vector<int>& classified);
};

}  // namespace bayes

#endif  // BAYES_MODEL_H_
