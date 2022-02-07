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
    explicit multi_line(str_vec o);
    explicit multi_line(cstr_t a_str);
    explicit multi_line(const std::string& a_str);
    multi_line(multi_line&&) = default;
    multi_line& operator=(const multi_line&) = default;
    multi_line& operator=(multi_line&&) = default;
    multi_line& operator                =(const std::string& o);
    explicit    operator std::string() const;
    // operator const std::string () const {return static_cast<const std::string>(*this);}
    std::string    dump() const;
    std::string    dump(uint offs) const;
    std::string    dump(cstr_t o, uint offs) const;
    static str_vec str_to_vec(cstr_t a_str);
    static str_vec trim_left(const str_vec& o);
    str_vec&       lines() { return this->lines_; }
    const str_vec& lines() const { return this->lines_; }
  private:
    /*.......................................................*/
    str_vec lines_; //!< text split into lines and timmed left
  };
} // namespace dbgen3
#endif // MULTI_LINE_HPP
