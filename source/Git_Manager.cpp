#include "Git_Manager.h"
#include <memory>
#include <git2.h>

using namespace std;

namespace repoman {

  static unique_ptr<Git_Manager> instance;

  void Git_Manager::ensure_initialized() {
    if (instance.get())
      return;

    instance = unique_ptr<Git_Manager>(new Git_Manager());
  }

  Git_Manager::Git_Manager() {
    git_libgit2_init();
  }

  Git_Manager::~Git_Manager() {
    git_libgit2_shutdown();
  }
}