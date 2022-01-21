#ifndef FLD_DSCR_HPP
#define FLD_DSCR_HPP
#include <cstdint>
#include <vector>

#include "enums.hpp"
#include "string_format.hpp"

namespace dbgen3
{
  class fld_dscr;
  using fld_vec = std::vector<fld_dscr>;

  class fld_dscr
  {
  public:
    fld_dscr(int              pos,     //!< position within the list
             const str_t&     name,    //!< name of the result or parameter column
             const ODBC_TYPE& type,    //!< query column type
             uint32_t         width,   //!< colmn width
             int16_t          dec,     //!< column decimaks
             const DBN&       nullable //!< is nullable
             )
    : pos_(pos)
    , name_(name)
    , type_(type)
    , width_(width)
    , dec_(dec)
    , nullable_(nullable)
    { }
    // @name getters
    //@{
    int              pos() const { return this->pos_; }
    str_t            name() const { return this->name_; }
    const ODBC_TYPE& type() const { return this->type_; }
    uint32_t         width() const { return this->width_; }
    int16_t          dec() const { return this->dec_; }
    int16_t          nullable() const;
    //@}
    /// @name getters
    //@{
    void set_pos(int pos) { this->pos_ = pos; }
    void set_name(const str_t& name) { this->name_ = name; }
    void set_type(const ODBC_TYPE& type) { this->type_ = type; }
    void set_width(const uint32_t& width) { this->width_ = width; }
    void set_dec(const int16_t& dec) { this->dec_ = dec; }
    void set_nullable(const DBN&& nullable) { this->nullable_ = nullable; }
    //@}

    str_t dump() const { return dump("", 0); }
    str_t dump(cstr_t a_msg) const { return dump(a_msg, 0); }
    str_t dump(uint offs) const { return dump("", offs); }
    str_t dump(cstr_t a_msg, uint offs) const
    {
      str_t r;
      r = out::sl(
        fmt::format(
          "{}[{:3}]:{{name: {:25} type: {:5} t.name: {:20} width: {:5} dec: {:2} null: {:12} }}",
          a_msg,
          pos_,
          name_ ,
          ME::enum_integer<ODBC_TYPE>(type_),
          ME::enum_name<ODBC_TYPE>(type_),
          width_,
          dec_,
          ME::enum_name<DBN>(nullable_)
          ),
        offs);
      return r;
    }
  protected:
  private:
    int       pos_;      //!< positiion within the parameter/result list (starting from 1)
    str_t     name_;     //!< name of the column or parameter
    ODBC_TYPE type_;     //!< query column type
    uint32_t  width_;    //!< colmn width
    int16_t   dec_;      //!< column decimaks
    DBN       nullable_; //!< is nullable
  public:
  };

  inline int16_t          fld_dscr::nullable() const { return ME::enum_integer<DBN>(this->nullable_); }
}; // namespace dbgen3

#endif // FLD_DSCR_HPP
