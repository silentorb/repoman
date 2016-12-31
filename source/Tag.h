#pragma once

#include <git2/types.h>

namespace repoman {

  class Repository;

  class Tag {
      git_tag *id = nullptr;

  public:
      Tag(Tag &&other) {
        this->id = other.id;
        other.id = nullptr;
      }

      Tag(Repository & repo,const git_oid *id);
      ~Tag();
  };
}