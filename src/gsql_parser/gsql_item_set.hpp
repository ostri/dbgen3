#ifndef GSQL_ITEM_SET_H
#define GSQL_ITEM_SET_H

#include <map>

#include "gsql_item.hpp"

namespace dbgen3
{
  class gsql_item_set : public std::map<str_t, gsql_item>
  {
  public:
    gsql_item_set()                     = default;
    ~gsql_item_set()                    = default;
    gsql_item_set(const gsql_item_set&) = default;
    gsql_item_set(gsql_item_set&&)      = default;
    gsql_item_set& operator=(const gsql_item_set&) = default;
    gsql_item_set& operator=(gsql_item_set&&) = default;

  private:
  };
}; // namespace dbgen3
#endif
