/*!
 * \file
 * \brief declaration of XerceString utilities
 *
 * The utilities offer conversion between xerces wide strings
 * and applicaiton narrow (native) 8 bit strings.
 */
#ifndef XERCES_STRINGS_HPP
#define XERCES_STRINGS_HPP

#include <string>
#include <xercesc/util/XMLString.hpp>
namespace dbgen3
{
  using XercesString = std::basic_string<XMLCh>;
  /// Converts from a narrow-character string to a wide-character string.
  XercesString fromNative(const char* str);
  /// Converts from a narrow-character string to a wide-charactr string.
  XercesString fromNative(const std::string& str);
  /// Converts from a wide-character string to a narrow-character string.
  std::string toNative(const XMLCh* str);
  /// Converts from a wide-character string to a narrow-character string.
  std::string toNative(const XercesString& str);
  /// Converts from a Xerces String to a std::wstring
  std::wstring xercesToWstring(const XercesString& str);
  /// Converts from a std::wstring to a XercesString
  XercesString wstringToXerces(const std::wstring& str);
}; // namespace dbgen3
#endif
