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
      std::unique_ptr<Tree> current_tree;

  public:
      Repository(const std::string &path);
      ~Repository();

      git_repository *get_id() const {
        return id;
      }

      void initialize();
      void clone(const std::string &url);
      void add_all();
      void commit(const std::string &message);

  };
}