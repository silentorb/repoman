#pragma once

#include <string>
#include "Tree.h"
#include <memory>

struct git_repository;

namespace repoman {

  class User;

  class Repository {
      git_repository *id = nullptr;
      std::string path;
      std::string url;
      std::unique_ptr<Tree> current_tree;

  public:
      Repository(const std::string &path, const std::string &url = "");
      ~Repository();
      void add_all();
      void commit(const std::string &message);

      git_repository *get_id() const {
        return id;
      }
  };
}