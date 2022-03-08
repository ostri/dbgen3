#ifndef GSQL_QBUF_DSCR_HPP
#define GSQL_QBUF_DSCR_HPP
#include "utility_classes/common.hpp"
#include "db_integration/fld_dscr.hpp"
#include "magic_enum.hpp"
#include "runtime/odbc_db.hpp"
#include "utility_classes/enums.hpp"
#include "utility_classes/string_format.hpp"
#include <algorithm>

namespace dbgen3
{
  using int_vec = std::vector<int>;
  class gsql_qbuf_dscr
  {
  public:
    gsql_qbuf_dscr()  = default;
    ~gsql_qbuf_dscr() = default;
    explicit gsql_qbuf_dscr(const db::BUF_TYPE& a_type);
    gsql_qbuf_dscr(const db::BUF_TYPE& a_type,
                   cstr_t              an_id,
                   bool                should_skip
                   );
    gsql_qbuf_dscr(const db::BUF_TYPE& a_type, bool should_skip);
    gsql_qbuf_dscr(const gsql_qbuf_dscr&) = default;
    gsql_qbuf_dscr(gsql_qbuf_dscr&&)      = default;
    gsql_qbuf_dscr& operator=(const gsql_qbuf_dscr& o) = default;
    gsql_qbuf_dscr& operator=(gsql_qbuf_dscr&& o) = default;
    db::BUF_TYPE    type() const;
    std::string     id() const;
    std::string     class_name() const;
    bool            should_skip() const;
    const str_vec&  names() const { return names_; }
    cstr_t          names(std::size_t ndx) const;
    const fld_vec&  flds() const;
    bool            must_generate() const;
    bool            is_required() const;
    uint            max_name_len() const;
    uint            max_ctype_len() const;

    std::string dump() const;
    std::string dump(uint offs) const;
    std::string dump(const std::string& a_msg, uint offs) const;

    void           set_type(const db::BUF_TYPE& type);
    void           set_id(cstr_t id);
    void           set_skip(bool skip);
    void           set_names(cstr_t names);
    void           set_flds(const fld_vec& flds);
  private:
    static uint calc_max_name_length(const fld_vec& vec);
    static uint calc_max_ctype_length(const fld_vec& vec);
    /*........................................................................................*/
    db::BUF_TYPE type_ = db::BUF_TYPE::res; //!< type fo the buffer definition
    std::string  id_{};                     //!< buffer unique name
    bool         skip_{};          //!< should we skip this buffer from the generation phase
    fld_vec      flds_{};          //!< set of field descriptions
    str_vec      names_{};         //!< alternative names to parameter/result columns
    uint         max_name_len_{};  //!< maximum filed name length
    uint         max_ctype_len_{}; //!< maximum filed name length
  };

  inline gsql_qbuf_dscr::gsql_qbuf_dscr(const db::BUF_TYPE& a_type,
                                        cstr_t              an_id,
                                        bool                should_skip
                                        )
  : type_(a_type)
  , id_(an_id)
  , skip_(should_skip)
  {
  }

  inline db::BUF_TYPE gsql_qbuf_dscr::type() const { return type_; }

  inline gsql_qbuf_dscr::gsql_qbuf_dscr(const db::BUF_TYPE& a_type)
  : type_(a_type)
  { }

  inline std::string gsql_qbuf_dscr::id() const { return this->id_; }

  inline bool gsql_qbuf_dscr::should_skip() const { return this->skip_; }

  inline const fld_vec& gsql_qbuf_dscr::flds() const { return this->flds_; }

  inline void gsql_qbuf_dscr::set_type(const db::BUF_TYPE& type) { this->type_ = type; }

  inline void gsql_qbuf_dscr::set_id(cstr_t id) { this->id_ = id; }

  inline void gsql_qbuf_dscr::set_skip(bool skip) { this->skip_ = skip; }

  inline void gsql_qbuf_dscr::set_flds(const fld_vec& flds)
  {
    max_name_len_  = calc_max_name_length(flds);
    max_ctype_len_ = calc_max_ctype_length(flds);
    flds_          = flds;
  }
} // namespace dbgen3

#endif // GSQL_QBUF_DSCR_HPP
