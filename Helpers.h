//
// Created by William Liu on 2019-07-21.
//

#pragma once

#include <string>
#include <curl/curl.h>

namespace helpers {
inline void trim(std::string &str) {
  size_t first = str.find_first_not_of(' ');
  if (std::string::npos == first) return;
  size_t last = str.find_last_not_of(' ');
  str = str.substr(first, last - first + 1);
}
inline std::string urlDecode(std::string &encoded) {
  CURL * curl = curl_easy_init();
  int length;
  char *start =
      curl_easy_unescape(curl, encoded.c_str(), encoded.length(), &length);
  std::string retVal(start, start + length);
  curl_free(start);
  curl_easy_cleanup(curl);
  return retVal;
}
}
