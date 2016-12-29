#pragma once

namespace repoman {

  class Git_Manager {

  public:
      Git_Manager();
      virtual ~Git_Manager();
      static void ensure_initialized();
  };
}