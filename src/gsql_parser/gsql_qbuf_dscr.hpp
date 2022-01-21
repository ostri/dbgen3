#ifndef GSQL_QBUF_DSCR_HPP
#define GSQL_QBUF_DSCR_HPP
#include "common.hpp"
#include "string_format.hpp"
#include "enums.hpp"
#include "fld_dscr.hpp"

namespace dbgen3
{
  class gsql_qbuf_dscr
  {
  public:
    gsql_qbuf_dscr();
    gsql_qbuf_dscr(const BUF_TYPE& a_type);
    gsql_qbuf_dscr(const BUF_TYPE& a_type, cstr_t an_id, bool should_skip);
    gsql_qbuf_dscr(const gsql_qbuf_dscr&) = default;
    gsql_qbuf_dscr(gsql_qbuf_dscr&&)      = default;
    gsql_qbuf_dscr operator               =(const gsql_qbuf_dscr& o);
    BUF_TYPE       type() const;
    std::string    id() const;
    bool           should_skip() const;
    const fld_vec& flds() const;

    std::string dump() const;
    std::string dump(uint offs) const;
    std::string dump(const std::string& a_msg, uint offs) const;

    void set_type(const BUF_TYPE& type);
    void set_id(const std::string& id);
    void set_skip(bool skip);
    void set_flds(const fld_vec& flds);
  private:
    BUF_TYPE    type_ = BUF_TYPE::unk; //!< type fo the buffer definition
    std::string id_;                   //!< buffer unique name
    bool        skip_;                 //!< should we skip this buffer from the generation phase
    fld_vec     flds_;                 //!< set of field descriptions
  };

  inline gsql_qbuf_dscr::gsql_qbuf_dscr()
  : type_(BUF_TYPE::unk){};

  inline gsql_qbuf_dscr::gsql_qbuf_dscr(const BUF_TYPE& a_type, cstr_t an_id, bool should_skip)
  : type_(a_type)
  , id_(an_id)
  , skip_(should_skip)
  { }

  inline gsql_qbuf_dscr gsql_qbuf_dscr::operator=(const gsql_qbuf_dscr& o)
  {
    if (this != &o)
    {
      type_ = o.type_;
      id_   = o.id_;
      skip_ = o.skip_;
    }
    return *this;
  }

  inline BUF_TYPE gsql_qbuf_dscr::type() const { return type_; }

  inline gsql_qbuf_dscr::gsql_qbuf_dscr(const BUF_TYPE& a_type)
  : type_(a_type)
  { }

  inline std::string gsql_qbuf_dscr::id() const { return this->id_; }

  inline bool gsql_qbuf_dscr::should_skip() const { return this->skip_; }

  inline const fld_vec& gsql_qbuf_dscr::flds() const { return this->flds_; }

  inline void gsql_qbuf_dscr::set_type(const BUF_TYPE& type) { this->type_ = type; }

  inline void gsql_qbuf_dscr::set_id(const std::string& id) { this->id_ = id; }

  inline void gsql_qbuf_dscr::set_skip(bool skip) { this->skip_ = skip; }

  inline void gsql_qbuf_dscr::set_flds(const fld_vec& flds) { this->flds_ = flds; }
} // namespace dbgen3

#endif // GSQL_QBUF_DSCR_HPP
