#include "dom_error_handler.hpp"
#include "exceptions.hpp"
#include "program_status.hpp"
namespace dbgen3
{
  bool  dom_error_handler::failed() const { return failed_; }
  str_t dom_error_handler::msg() const { return msg_; }
  bool  dom_error_handler::handleError(const x::DOMError& e)
  {
    bool x_warn(e.getSeverity() == x::DOMError::DOM_SEVERITY_WARNING);

    if (! x_warn) failed_ = true;

    x::DOMLocator* loc(e.getLocation());
    uri_    = toNative(loc->getURI());
    e_msg_  = toNative(e.getMessage());
    line_   = loc->getLineNumber();
    col_    = loc->getColumnNumber();
    e_type_ = x_warn ? "warning" : "error";
    //throw dbgen3_exc(P_STS::inv_grammar_syntax, "kr neki");
    return true;
  }
  str_t dom_error_handler::uri() const { return this->uri_; }
  str_t dom_error_handler::e_msg() const { return this->e_msg_; }
  uint  dom_error_handler::line() const { return this->line_; }
  uint  dom_error_handler::col() const { return this->col_; }
  str_t dom_error_handler::e_type() const { return this->e_type_; }
}; // namespace dbgen3