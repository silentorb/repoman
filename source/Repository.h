#pragma once

#include <string>
#include "Tree.h"
#include <memory>
#include <functional>

struct git_repository;

namespace repoman {

  class User;

  class Object;

  class Commit;

  class Tag;

  using Status_Delegate = std::function<void(const std::string &path, unsigned int flags)>;

  class Repository {
      git_repository *id = nullptr;
      std::string path;
      std::unique_ptr<Tree> current_tree;

      Object get_commit_object(const std::string &pattern);
      Object get_last_commit_object();

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
      void tag_last_commit(const std::string &label);
      void checkout(const Tag &tag);
      Tag get_tag(const std::string tag_name);
  };
}