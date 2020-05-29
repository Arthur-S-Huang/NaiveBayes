// Copyright 2020 [Your Name]. All rights reserved.

#include <bayes/model.h>
#include <nlohmann/json.hpp>
#include <iostream>

using std::cout;
using std::endl;
using nlohmann::json;


namespace bayes {

Model::Model() = default;

void Model::GenerateClassIndices(const vector<int>& train_labels_correct,
                                 const vector<int>& test_labels_correct) {
  vector<vector<int>> index_in_order(kNumClasses);
  train_labels = train_labels_correct;
  test_labels = test_labels_correct;
  for (int label_index = 0; label_index < train_labels.size(); label_index++) {
    for (int index = 0; index < kNumClasses; index++) {
      if (train_labels[label_index] == index) {
        index_in_order[index].push_back(label_index);
      }
    }
  }
  index_vector = index_in_order;
}

void Model::GenerateModel() {
  vector<int> numbers;
  string current_image;
  double current_count;
  int label_count;
  // a vector containing the classes 0 - 9
  for (int number = 0; number < kNumClasses; number++) {
    numbers.push_back(number);
  }
  class_labels = numbers;
  // filling up the model with how many times the
  // pixel is found in that position
  for (int label = 0; label < kNumClasses; label++) {
    for (int j = 0; j < index_vector[label].size(); j++) {
      current_image = images[index_vector[label][j]].getString();
      for (int row = 0; row < kImageSize; row++) {
        for (int col = 0; col < kImageSize; col++) {
          if (row > 0) {
            // if pixel is not white
            if (current_image.at(col + row * kImageSize) == '1') {
              probs[label][row][col][1] += 1;
            } else {
              probs[label][row][col][0] += 1;
            }
          } else {
            if (current_image.at(col) == '1') {
              probs[label][row][col][1] += 1;
            } else {
              probs[label][row][col][0] += 1;
            }
          }
        }
      }
    }
  }
  // once all the members of the model are filled, we
  // compute the probabilities of each
  for (int label  = 0; label  < kNumClasses; label++) {
    label_count = FindNumberOfOccurrences(label);
    for (int p = 0; p < kImageSize; p++) {
      for (int t = 0; t < kImageSize; t++) {
        for (int value = 0; value < kNumShades; value++) {
          current_count = probs[label][p][t][value];
          // applying Bayes theorem
          probs[label][p][t][value] =
              (kLaplace + current_count) / (2 * kLaplace + label_count);
        }
      }
    }
  }
  //json model = probs;
}

void Model::InitializeModel() {
  for (int label  = 0; label < kNumClasses; label++) {
    for (int x = 0; x < kImageSize; x++) {
      for (int y = 0; y < kImageSize; y++) {
        for (int value = 0; value < kNumShades; value++) {
          probs[label][x][y][value] = 0;
        }
      }
    }
  }
}

int Model::FindNumberOfOccurrences(int to_find) {
  int count = 0;
  for (int i = 0; i < train_labels.size(); i++) {
    if (train_labels[i] == to_find) {
      count++;
    }
  }
  return count;
}

int Model::ClassifyImage(string image) { 
  vector<double> posterior_values;
  double pixels_value;
  double highest_posterior_value;
  int answer;
  
  // Traversing through all the pixels in an image
  // finding the posterior values using our model that was built
  for (int label = 0; label < class_labels.size(); label++) {
    // the probability of the current class occurring
    pixels_value = log(FindNumberOfOccurrences(label) / kNumClasses);
    for (int x = 0; x < kImageSize; x++) {
      for (int y = 0; y < kImageSize; y++) {
        if (x > 0) {
          if (image.at(y + x * kImageSize) == '1') {
            // using log to avoid arithmetic underflow
            pixels_value += log(probs[label][x][y][1]);
          } else {
            pixels_value += log(probs[label][x][y][0]);
          }
        } else {
          if (image.at(y) == '1') {
            pixels_value += log(probs[label][x][y][1]);
          } else {
            pixels_value += log(probs[label][x][y][0]);
          }
        }
      }
    }
    posterior_values.push_back(pixels_value);
  }
  // finding the highest posterior value to calssify image
  highest_posterior_value = posterior_values[0];
  for (int index = 0; index < posterior_values.size(); index++) {
    if (posterior_values[index] > highest_posterior_value) {
      highest_posterior_value = posterior_values[index];
      answer = index;
    }
  }
  return answer;
}

double Model::CalculateAccuracy(const vector<int>& classified) { 
  int number_of_incorrect = 0;
  for (int i = 0; i < classified.size(); i++) {
    if (classified[i] != test_labels[i]) {
      number_of_incorrect++;
    }
  }
  return (double)(test_labels.size() - number_of_incorrect) / 
                                       (test_labels.size());
}

}  // namespace bayes

