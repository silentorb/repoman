#pragma once

struct git_object;

#include <string>
#include "Repository.h"

namespace repoman {

  class Object {
      git_object *id = nullptr;

  public:
      Object() {}

      Object(Object &&other) {
        this->id = other.id;
        other.id = nullptr;
      }

      Object(git_object *id);
      ~Object();

      git_object *get_id() {
        return id;
      }
  };
}