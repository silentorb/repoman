#include <git2/object.h>
#include <git2.h>
#include "Object.h"

namespace repoman {

  Object::~Object() {
    if (id)
      git_object_free(id);
  }

  Object::Object(git_object *id) :
    id(id) {}
}