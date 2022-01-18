#ifndef MULTI_LINE_HPP
#define MULTI_LINE_HPP
#include "common.hpp"

namespace dbgen3
{
  class multi_line
  {
  public:
    multi_line()                  = default;
    ~multi_line()                 = default;
    multi_line(const multi_line&) = default;
    multi_line(const str_vec& o)
    : lines_(o)
    { 
    }
    multi_line(cstr_t a_str)
    : lines_(str_to_vec(a_str))
    { }
    multi_line(const std::string& a_str)
    : lines_(str_to_vec(a_str))
    {
    }
    // multi_line(multi_line&&) = default;
    multi_line& operator=(const multi_line&) = default;
    //  multi_line&    operator=(multi_line&&) = default;
    multi_line& operator=(const std::string& o)
    {
      lines_ = str_to_vec(o);
      return *this;
    }

    std::string    dump() const;
    std::string    dump(int offs) const;
    std::string    dump(cstr_t o, int offs) const;
    static str_vec str_to_vec(cstr_t a_str);
    static str_vec trim_left(const str_vec& o);
  private:
    /*.......................................................*/
    str_vec lines_; //!< text split into lines and timmed left
  };
} // namespace dbgen3
#endif // MULTI_LINE_HPP
