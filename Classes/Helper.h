#ifndef __HELPER_H__
#define __HELPER_H__

#include <array>
#include <string>
#include <vector>
#include <limits>

std::array<std::string, 2> Splitext(const std::string &filename);

std::vector<std::string> Split(const std::string &str, const char *sep, size_t maxsplit = std::numeric_limits<size_t>::max());

#endif