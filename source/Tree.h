#pragma once

struct git_tree;

namespace repoman {


  class Index;

  class Tree {
      git_tree *id = nullptr;

  public:
      Tree(Index &index);
      virtual ~Tree();

      git_tree *get_id() const {
        return id;
      }
  };
}