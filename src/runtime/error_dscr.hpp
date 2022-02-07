/*!
 * \brief It holds error description data
 *
 * \ingroup runtime
 *
 * Class error_dscr holds information about one error
 * description (there can be several)
 */
#ifndef ERROR_DSCR_HPP
#define ERROR_DSCR_HPP
#include <string>
#include <string_view>
namespace db
{
  using cstr_t = std::string_view;
  using str_t  = std::string;
  /**
   * @brief vector of erro descriptions
   *
   */
  class error_dscr
  {
  public:
    /// constructor
    error_dscr()=default;
    error_dscr(int code, cstr_t state, cstr_t msg);
    /// copy constructor
    error_dscr(const error_dscr& o) = default;
    error_dscr(error_dscr&& o)      = default;
    error_dscr& operator=(const error_dscr& o) = default;
    error_dscr& operator=(error_dscr&& o) = default;
    /// destructor
    virtual ~error_dscr() = default;
    /// getters
    int                get_code() const { return code_; }
    const std::string& get_state() const { return state_; }
    const std::string& get_msg() const { return msg_; }
    str_t              dump(cstr_t a_msg) const;
  private:
    int   code_  = 0;  //!< sql code
    str_t state_ = {}; //!< sql state
    str_t msg_   = {}; //!< error description
  };
} // namespace db
#endif // ERROR_DSCR_HPP
