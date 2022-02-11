#ifndef DOM_ERROR_HANDLER_HPP
#define DOM_ERROR_HANDLER_HPP

#include <iostream>
#include <xercesc/dom/DOMError.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>
#include <xercesc/dom/DOMLocator.hpp>

#include "exceptions.hpp"
#include "fmt/core.h"
#include "program_status.hpp"
#include "program_status.hpp"
#include "program_status.hpp"
#include "xerces_strings.hpp"

namespace dbgen3
{
  namespace x = xercesc;
  class dom_error_handler : public x::DOMErrorHandler
  {
  public:
    dom_error_handler() = default;
    bool  failed() const;
    str_t msg() const;
    bool  handleError(const x::DOMError& e) override;
    str_t uri() const;
    str_t e_msg() const;
    uint line() const;
    uint col() const;
    str_t e_type() const;
  private:
    bool  failed_{};
    str_t msg_{};
    str_t uri_{};
    str_t e_msg_{};
    uint  line_{};
    uint  col_{};
    str_t e_type_{};
  };
};     // namespace dbgen3
#endif // DOM_ERROR_HANDLER_HPP
