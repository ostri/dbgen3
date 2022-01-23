#ifndef DBGEN3_TEMPL_HPP
#define DBGEN3_TEMPL_HPP

#include <array>
#include <string>
#include <string_view>
#include <cstring>
#include <span>
#include <iostream>

namespace db
{
  using cstr_t = std::string_view;
  using str_t  = std::string;

  using a5 = std::array<char, 5>;

  template <typename T, std::size_t arr_size>
  class atomic
  {
  public:
    using t_vec = std::array<T, arr_size>;
    T           value() const;
    T           value(std::size_t ndx) const;
    void        set_value(const T& data);
    void        set_value(const T& data, std::size_t ndx);
    std::size_t size();
  private:
    t_vec data_;
  public:
  };
  /**
   * @brief block of memory with at most <n> bytes in lenght
   *
   * @tparam T
   * @tparam arr_size
   */
  template <std::size_t blk_size, std::size_t arr_size, typename EL>
  class mem_blk
  {
  public:
    using T     = std::array<EL, blk_size>;
    using T_ref = std::span<const EL>;
    using t_vec = std::array<T, arr_size>;
    T_ref value() const { return value(0); }
    T_ref value(std::size_t ndx) const { return T_ref(this->data_.at(ndx).data(), el_max_size()); }

    void set_value(const T_ref data) { set_value(data, 0); }
    void set_value(const T_ref data, std::size_t ndx)
    {
      auto len = std::min(data.size(), el_max_size());
      std::copy_n(data.begin(), len, data_.at(ndx).begin());
    }

    std::size_t size() const { return data_.size(); }
    std::size_t el_max_size() const { return data_[0].size(); }
  private:
    t_vec data_;
  public:
  };

  /**
   * @brief
   *
   * @tparam item_size
   * @tparam arr_size
   * @tparam T         type of character TODO works only for char
   */
  template <std::size_t item_size, std::size_t arr_size, typename T = char>
  class string
  {
  public:
    using t_vec = std::array<std::array<T, item_size + 1>, arr_size>;
    cstr_t      value() const;
    cstr_t      value(std::size_t ndx) const;
    void        set_value(cstr_t data);
    void        set_value(cstr_t data, std::size_t ndx);
    std::size_t size();
  private:
    t_vec data_;
  public:
  };

  template <std::size_t item_size, std::size_t arr_size, typename T>
  inline cstr_t string<item_size, arr_size, T>::value() const
  {
    return value(0);
  }

  template <std::size_t item_size, std::size_t arr_size, typename T>
  inline cstr_t string<item_size, arr_size, T>::value(std::size_t ndx) const
  {
    auto ref = this->data_.at(ndx).data();
    return cstr_t(ref, std::strlen(ref));
  }

  template <std::size_t item_size, std::size_t arr_size, typename T>
  inline void string<item_size, arr_size, T>::set_value(cstr_t data)
  {
    set_value(data, 0);
  }

  template <std::size_t item_size, std::size_t arr_size, typename T>
  inline void string<item_size, arr_size, T>::set_value(cstr_t data, std::size_t ndx)
  {
    const std::size_t max_len = data_.at(0).size() - 1; // trailing \0
    auto              src     = data.data();
    auto              dst     = data_.at(ndx).data();
    if (src != dst)
    {
      // too long inputs are trimmed. This is on purpose.
      std::strncpy(dst, src, max_len);
      dst[max_len] = 0;
    }
  }

  template <std::size_t item_size, std::size_t arr_size, typename T>
  inline std::size_t string<item_size, arr_size, T>::size()
  {
    return data_.size();
  }

  template <typename T, std::size_t arr_size>
  inline T atomic<T, arr_size>::value() const
  {
    return data_[0];
  }

  template <typename T, std::size_t arr_size>
  inline T atomic<T, arr_size>::value(std::size_t ndx) const
  {
    return this->data_.at(ndx);
  }

  template <typename T, std::size_t arr_size>
  inline void atomic<T, arr_size>::set_value(const T& data)
  {
    set_value(data, 0);
  }

  template <typename T, std::size_t arr_size>
  inline void atomic<T, arr_size>::set_value(const T& data, std::size_t ndx)
  {
    this->data_.at(ndx) = data;
  }

  template <typename T, std::size_t arr_size>
  inline std::size_t atomic<T, arr_size>::size()
  {
    return data_.size();
  }
};     // namespace db
#endif // DBGEN3_TEMPL_HPP
