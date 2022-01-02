#include "xerces_strings.hpp"
/*!
 * \file
 * \brief implementation of XercesSting utilities
 *
 * Utilities are ued for transformation between xerces unicode and
 * ascii strings. The catch lies in fact that transcode (i.e.
 * xerces-c conversion utility) allocates space on heap, which should
 * be later released in order to avoid memory leaks.
 *
 */
// #include <string>
// #include <xercesc/util/XMLString.hpp>
namespace dbgen3
{
  class XMLChReleaser // NOLINT
  {
  public:
    explicit XMLChReleaser(XMLCh* a_ptr)
    : ptr_(a_ptr)
    {
    }
    ~XMLChReleaser() { xercesc::XMLString::release(&this->ptr_); }
    auto g_ptr() const { return ptr_; }

  private:
    XMLCh* ptr_;
  };

  class CharReleaser // NOLINT
  {
  public:
    explicit CharReleaser(char* a_ptr)
    : ptr_(a_ptr)
    {
    }
    ~CharReleaser() { xercesc::XMLString::release(&this->ptr_); }
    auto g_ptr() const { return ptr_; }

  private:
    char* ptr_; // NOLINT
  };
  /// Converts from a narrow-character string to a wide-character string.
  XercesString fromNative(const char* str)
  {
    XMLChReleaser r(xercesc::XMLString::transcode(str));
    return {r.g_ptr()};
  }

  /// Converts from a narrow-character string to a wide-charactr string.
  XercesString fromNative(const std::string& str)
  {
    return fromNative(str.c_str());
  }

  /// Converts from a wide-character string to a narrow-character string.
  std::string toNative(const XMLCh* str)
  {
    CharReleaser r(xercesc::XMLString::transcode(str));
    return {r.g_ptr()};
  }

  /// Converts from a wide-character string to a narrow-character string.
  std::string toNative(const XercesString& str)
  {
    return toNative(str.c_str());
  }
  /// Converts from a Xerces String to a std::wstring
  std::wstring xercesToWstring(const XercesString& str)
  {
    //  return std::wstring(str.begin(), str.end());
    return {str.begin(), str.end()};
  }

  /// Converts from a std::wstring to a XercesString
  XercesString wstringToXerces(const std::wstring& str)
  {
    //  return XercesString(str.begin(), str.end());
    return {str.begin(), str.end()};
  }
} // namespace dbgen3
