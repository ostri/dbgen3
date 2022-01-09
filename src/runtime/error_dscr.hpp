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
namespace db
{
  /**
   * @brief vector of erro descriptions
   * 
   */
  class error_dscr
  {
  public:
    ///constructor
    error_dscr(int code, const std::string& state, const std::string& msg);
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
    std::string        dump(const std::string& a_msg) const;
  private:
    int         code_ = 0; //!< sql code
    std::string state_;    //!< sql state
    std::string msg_;      //!< error description
  };
};     // namespace dbgen3
#endif // ERROR_DSCR_HPP
