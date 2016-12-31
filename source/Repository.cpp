#include <git2.h>
#include "Repository.h"
#include "Git_Manager.h"
#include "utility.h"
#include "User.h"
#include "Signature.h"
#include "Index.h"
#include "Commit.h"
#include "Object.h"
#include "Tag.h"

using namespace std;

namespace repoman {

  typedef struct { /* … */ } match_data;

  int matcher(const char *path, const char *spec, void *payload) {
    match_data *d = (match_data *) payload;
    /*
     * return 0 to add/remove this path,
     * a positive number to skip this path,
     * or a negative number to abort the operation.
     */
    return 0;
  }

  Repository::Repository(const std::string &path) :
    path(path) {
    Git_Manager::ensure_initialized();
  }

  Repository::~Repository() {
    git_repository_free(id);
  }

  void Repository::initialize() {
    check_error(git_repository_init(&id, path.c_str(), false), "initializing repo");
  }

  void Repository::clone(const std::string &url) {
    check_error(git_clone(&id, url.c_str(), path.c_str(), nullptr), "clone repo");
  }

  void Repository::open() {
    check_error(git_repository_open(&id, path.c_str()), "open repo");
  }

  void Repository::add_all() {
//    git_index *index = nullptr;

//    check_error(git_index_add_bypath(index, "README.md"), "add all files to index");

    auto index = Index(*this);
    const char *paths[] = {"."};
    git_strarray git_paths = {const_cast<char **>(paths), 1};

    match_data data;
    check_error(git_index_add_all(index.get_id(), &git_paths, GIT_INDEX_ADD_DEFAULT, matcher, &data),
                "add all files to index");

    check_error(git_index_update_all(index.get_id(), &git_paths, matcher, &data), "add all files to index");
    check_error(git_index_write(index.get_id()), "save index");
    current_tree = unique_ptr<Tree>(new Tree(index));
  }

  Object Repository::get_commit_object(const std::string &pattern) {
    git_object *object_id = NULL;
    auto result = git_revparse_single(&object_id, id, pattern.c_str());
    Object object(object_id);
    if (result == GIT_ENOTFOUND) {
      return Object();
    }
    else if (result != 0) {
      throw std::runtime_error("Git Error: retrieving commit.");
    }

    return object;
  }

  Object Repository::get_last_commit_object() {
    return get_commit_object("HEAD^{commit}");
  }

  void Repository::commit(const std::string &message) {
    auto signature = Signature(*this);

    auto last_commit_object = get_last_commit_object();
    Commit last_commit(*this, last_commit_object);

    const git_commit *parents = last_commit_object.get_id()
                                ? last_commit.get_id()
                                : nullptr;

    git_oid new_commit_id = {0};
    check_error(git_commit_create(
      &new_commit_id,
      id,
      "HEAD",                      /* name of ref to update */
      signature.get_id(),                          /* author */
      signature.get_id(),                          /* committer */
      "UTF-8",                     /* message encoding */
      message.c_str(),  /* message */
      current_tree->get_id(),                        /* root tree */
      last_commit.get_id() ? 1 : 0,                           /* parent count */
      &parents), "create commit");                    /* parents */
  }

  int status_callback(const char *path, unsigned int flags, void *payload) {
    const Status_Delegate &delegate = *static_cast<Status_Delegate *>(payload);
    delegate(path, flags);
    return 0;
  }

  void Repository::enumerate_status(const Status_Delegate &delegate) {
    check_error(git_status_foreach(id, status_callback, const_cast<Status_Delegate *>(&delegate)),
                "enumerate repo status");
  }

  void Repository::tag_last_commit(const std::string &label) {
    auto last_commit = get_last_commit_object();
    git_oid object_id = {0};
    Signature signature(*this);

    check_error(git_tag_create(
      &object_id,               /* new object id */
      id,               /* repo */
      label.c_str(),           /* name */
      last_commit.get_id(),             /* target */
      signature.get_id(),             /* name/email/timestamp */
      "Released 10/5/11", /* message */
      false             /* force? */
    ), "create tag");
  }

  struct tag_search_data {
      string tag_name;
      git_oid *target;
  };

  int find_tag_callback(const char *name, git_oid *target, void *payload) {
    tag_search_data *data = (tag_search_data *) payload;
    if (name == data->tag_name) {
      data->target = target;
      return 1;
    }
    return 0;
  }

  Tag Repository::find_tag(const std::string tag_name) {
    tag_search_data data;
    data.tag_name = tag_name;
    check_error(git_tag_foreach(id, find_tag_callback, &data), "find a tag");
    return Tag(*this, data.target);
  }

  void Repository::checkout(Tag &tag) {

  }
}