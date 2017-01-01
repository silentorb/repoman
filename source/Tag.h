#pragma once

struct git_tag;
struct git_oid;

namespace repoman {

  class Repository;

  class Tag {
      git_tag *id = nullptr;

  public:
      Tag(git_tag *id) : id(id) {}

      Tag(Tag &&other) {
        this->id = other.id;
        other.id = nullptr;
      }

      Tag(Repository &repo, const git_oid *id);
      ~Tag();

      git_tag *get_id() const {
        return id;
      }
  };
}