#pragma once

struct git_commit;

#include <string>
#include "Repository.h"

namespace repoman {

  class Commit {
      git_commit *id = nullptr;

  public:
      Commit(Repository &repo, Object &source);

      ~Commit();

      git_commit *get_id() const {
        return id;
      }

  };
}