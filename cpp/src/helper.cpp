#include "helper.h"
#include "Video.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

std::string trim(std::string toTrim) {
  size_t trimPos = toTrim.find_first_not_of(" \t");
  toTrim.erase(0, trimPos);
  trimPos = toTrim.find_last_not_of(" \t");
  if (std::string::npos != trimPos) {
    toTrim.erase(trimPos + 1);
  }
  return toTrim;
}

std::vector<std::string> splitlines(std::string output) {
  std::vector<std::string> commandOutput;
  std::stringstream ss(output);
  std::string line;
  while (std::getline(ss, line, '\n')) {
    commandOutput.emplace_back(line);
  }
  return commandOutput;
}

//takes in a string, and outputs that string in all caps as a right value (not editing the original string)
std::string stringToUpper(const std::string input)
{
  std::string output(input);
  std::transform(output.begin(), output.end(), output.begin(), [](char c)
                 { return static_cast<char>(std::toupper(c)); });
  return output;
}
