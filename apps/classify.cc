// Copyright (c) 2020 [Your Name]. All rights reserved.

#include <bayes/classifier.h>
#include <bayes/image.h>
#include <bayes/model.h>
#include <gflags/gflags.h>

#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::ios;
using std::stoi;



int main(int argc, char** argv) {
  string training_images_path;
  string training_labels_path;
  string test_images_path;
  string test_labels_path;
  
  if (argc > 1)
  {
    training_images_path = argv[1];
    training_labels_path = argv [2];
    test_images_path = argv[3];
    test_labels_path = argv[4];
  }

  int classified;
  string line;
  string image_string;
  Image image;
  bayes::Model model;
  vector<int> train_labels_correct;
  vector<int> test_labels_correct;
  vector<int> classified_results;
  
  // loading test labels and train labels
  ifstream train_labels(training_labels_path);
  if (train_labels.fail()) {
    cout << "Failed to load training labels, exiting!" << endl;
    exit(1);
  }
  while (!train_labels.eof()) {
    getline(train_labels, line);
    // converting string to integer
    train_labels_correct.push_back(stoi(line));
  }

  ifstream test_labels(test_labels_path);
  if (test_labels.fail()) {
    cout << "Failed to load test labels, exiting!" << endl;
    exit(1);
  }
  while (!test_labels.eof()) {
    getline(test_labels, line);
    test_labels_correct.push_back(stoi(line));
  }
  
  ifstream training_images(training_images_path);
  if (training_images.fail()) {
    cout << "Failed to load training images, exiting!" << endl;
    exit(1);
  }
  
  // counting the number of lines in training images
  int count = 0;
  while (getline(training_images, line)) {
    count++;
  }
  // resetting the reader
  training_images.clear();
  training_images.seekg(0, ios::beg);
  // for every 28 lines, initialize an image
  for (int i = 1; i <= count; i++) {
    getline(training_images, line);
    image_string += line;
    if (i % kImageSize == 0) {
      image.SetString(image_string);
      image_string = "";
      model.images.push_back(image);
    }
  }
  count = 0;
  // setting up the model
  model.GenerateClassIndices(train_labels_correct, test_labels_correct);
  model.InitializeModel();
  model.GenerateModel();
  
  ifstream test_images(test_images_path);
  if (test_images.fail()) {
    cout << "Failed to load test images, exiting!" << endl;
    exit(1);
  }
  // count the number of lines in test images
  while (getline(test_images, line)) {
    count++;
  }
  test_images.clear();
  test_images.seekg(0, ios::beg);
  
  for (int i = 1; i <= count; i++) {
    getline(test_images, line);
    image_string += line;
    if (i % kImageSize == 0) {
      image.SetString(image_string);
      image_string = "";
      // for every 28 lines, initialize and image and calssify it using
      // the model that was generated.
      classified = model.ClassifyImage(image.getString());
      classified_results.push_back(classified);
      cout << classified << endl;
    }
  }
  cout << "ACCURACY: " << model.CalculateAccuracy(classified_results) << endl;
  return EXIT_SUCCESS;
}
