#pragma once

#include "Repository.h"

namespace repoman {

  class User;
  class Repository;

  class Signature {
      git_signature *id = nullptr;

  public:
      Signature(const Repository &repo);
      virtual ~Signature();

      git_signature *get_id() const {
        return id;
      }
  };
}