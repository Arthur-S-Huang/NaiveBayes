// Copyright (c) 2020 [Your Name]. All rights reserved.

#ifndef BAYES_IMAGE_H_
#define BAYES_IMAGE_H_

#include <cstdlib>
#include <istream>
#include <ostream>
#include <string>


using std::istream;
using std::ostream;
using std::vector;
using std::string;


constexpr size_t kImageSize = 28;

class Image {
 private:
  string formatted_string;
  
 public:
  // Constructor
  Image();
  // Given a string representation of an image, format it into just 1's and 0's
  void SetString(const string& img);
  // Return the formatted string.
  string getString();
  // Overloaded to print an image
  friend ostream &operator<<(ostream &output, const Image &I) {
    output << I.formatted_string;
    return output;
  }
};

#endif  // BAYES_IMAGE_H_
