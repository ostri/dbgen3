#ifndef MULTI_LINE_HPP
#define MULTI_LINE_HPP
#include "common.hpp"

namespace dbgen3
{
  class multi_line
  {
  public:
    multi_line()  = default;
    ~multi_line() = default;
    multi_line(const str_vec& o)
    : lines_(o){};
    multi_line(const std::string& o);
    multi_line(multi_line&&) = default;
    multi_line&    operator=(const multi_line&) = default;
    multi_line&    operator=(multi_line&&) = default;
    // const multi_line    operator=(const std::string& o)
    // {
    //   multi_line ml(o);
    //   return ml;
    // }

    static str_vec trim_left(const str_vec& o);
    std::string    dump(const std::string o, int offs) const;

  private:
    str_vec lines_;
  };
} // namespace dbgen3
#endif // MULTI_LINE_HPP
