// Copyright 2020 [Your Name]. All rights reserved.

#include <bayes/image.h>
using std::string;
using std::ifstream;
using std::istringstream;
using std::ios;
using std::stoi;

Image::Image() = default;

void Image::SetString(const string& img) {
  formatted_string = "";
  for (int index = 0; index < img.length(); index++) {
    if (img.at(index) != ' ') {
      formatted_string += "1";
    } else {
      formatted_string += "0";
    }
  }
}

string Image::getString() { 
  return formatted_string;
}
 


