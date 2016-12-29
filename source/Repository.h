#pragma once

#include <string>

struct git_repository;

namespace repoman {

  class Repository {
      git_repository *repo = nullptr;
      std::string path;
      std::string url;

  public:
      Repository(const std::string &path, const std::string &url = "");
  };
}