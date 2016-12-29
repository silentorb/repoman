#pragma once

struct git_index;

namespace repoman {

  class Repository;

  class Index {
      git_index *id = nullptr;
      Repository &repo;

  public:
      Index(Repository &repo);
      virtual ~Index();

      git_index *get_id() const {
        return id;
      }

      Repository &get_repo() const {
        return repo;
      }
  };
}