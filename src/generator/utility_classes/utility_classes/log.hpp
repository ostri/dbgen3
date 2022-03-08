#ifndef LOG_HPP
#define LOG_HPP
namespace dbgen3
{
  class log
  {
  };
} // namespace dbgen3
//using log = dbgen3::log;
#define info LOG(INFO)    // NOLINT
#define warn LOG(WARNING) // NOLINT
#define err LOG(ERROR)    // NOLINT
#define fatal LOG(FATAL)  // NOLINT

#endif // LOG_HPP
