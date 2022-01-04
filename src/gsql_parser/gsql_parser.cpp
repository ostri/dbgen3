#include "gsql_parser.hpp"
#include "xerces_strings.hpp"

namespace dbgen3
{
  namespace x = xercesc;
  gsql_parser::gsql_parser()
  : valid_(false)
  {
    try
    {
      valid_ = true;
    }
    catch (const x::XMLException& e)
    {
      auto msg = toNative(e.getMessage());
      err << "Error during initialization! :\n" << msg << "\n";
    }
  };
}; // namespace dbgen3
