
#include <chrono>

#include "gen_cpp.hpp"
namespace dbgen3
{
  str_t gen_cpp::gen_buf(const gsql_q& q, const BUF_TYPE& a_type, uint offs)
  {
    str_t r;
    auto  bd = q.buf_dscr((a_type));
    if (! bd.should_skip())
    {
      auto c_name = snake_case(bd.id());
      r += line(73, offs);
      r += out::sl(fmt::format("class {}", c_name), offs);
      r += out::sl("{", offs);
      r += out::sl(fmt::format("}}; // class {}", c_name), offs);
    }
    return r;
  }
  str_t gen_cpp::gen_utl(const gsql_q& q, uint offs)
  {
    str_t r;
    auto  sql = snake_case(q.id());
    r += out::sl(r, offs);
    return r;
  }
  str_t gen_cpp::gen_file(uint offs)
  {
    str_t       r;
    auto        now      = std::chrono::system_clock::now();
    std::time_t time     = std::chrono::system_clock::to_time_t(now);
    std::string time_str = std::ctime(&time);

    r += out::sl(fmt::format("/* auto generated '{}' - do not alter */",
                             time_str.substr(0, time_str.size() - 1)),
                 offs);
    auto ns = snake_case(set().id());
    r += out::sl(fmt::format("namespace {}", ns), offs);
    r += out::sl("{", offs);
    for (auto q : set().q_dic())
    { //
      auto ns = snake_case(q.second.id());
      r += line(75, offs + 2, '.');
      r += out::sl(fmt::format("namespace {}", ns), offs + 2);
      r += out::sl("{", offs + 2);
      r += gen_buf(q.second, BUF_TYPE::par, offs + 4);
      r += gen_buf(q.second, BUF_TYPE::res, offs + 4);
      r += gen_utl(q.second, offs + 4);
      r += out::sl(fmt::format("}}; //namespace {}", ns), offs + 2);
    }
    r += out::sl(fmt::format("}}; //namespace {}", ns), offs);

    return r;
  }

}; // namespace dbgen3