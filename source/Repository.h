#pragma once

#include <string>
#include "Tree.h"
#include <memory>
#include <functional>

struct git_repository;

namespace repoman {

  class User;

  using Status_Delegate = std::function<void(const std::string &path, unsigned int flags)>;

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
      void open();
      void add_all();
      void commit(const std::string &message);
      void enumerate_status(const Status_Delegate &delegate);
  };
}