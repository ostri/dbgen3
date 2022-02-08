/* auto generated 'Tue Feb  8 01:16:13 2022' - do not alter */
#include <array>
#include <span>
#include <string>
#include <string_view>

#include "dbgen3_templ.hpp"
/*
you can insert arbitrary c++ code here 
#include <string>
#include <array>
*/
namespace UT
{
  using uint    = std::size_t;
  using str_t   = std::string;
  using cstr_t  = std::string_view;
  using bstr_t  = std::span<uint8_t>;
  using cbstr_t = std::span<const uint8_t>;
  /*....Q_1...........................................................................................*/
  namespace Q_1
  {
    /*....Q-1::par....................................................................................*/
    template <std::size_t N>
    class qp_id : public db::buffer_root<N>
    {
    public: /*...public methods...*/
      constexpr qp_id() : db::buffer_root<N>()
      {
        db::buffer_root<N>::attr_vec().emplace_back(&par_1_);
      }
      /// @brief trivial getters
      //@{
      int16_t                  par_1() const {return par_1(0);}
      //@}
      /// @brief getters
      //@{
      int16_t                  par_1(uint ndx) const {return par_1_.value(ndx);}
      //@}
      /// @brief trivial setters
      //@{
      void set_par_1(int16_t v) {set_par_1(v, 0);}
      //@}
      /// @brief setters
      //@{
      void set_par_1(int16_t v, uint ndx) {par_1_.set_value(v, ndx);}
      //@}
    private:/*...private methods & attributes...*/
      constexpr static const uint par_1_dec = 0; // number of decimal places
      db::atomic <int16_t, SQL_SMALLINT, par_1_dec, N> par_1_{}; 
      constexpr static const uint max_attr=1;
      std::array<db::attr_root_root<N>*, max_attr> attr_
      {
        &par_1_      
      };
    }; // class qp_id
    /*....Q-1::res....................................................................................*/
    template <std::size_t N>
    class qr_id : public db::buffer_root<N>
    {
    public: /*...public methods...*/
      constexpr qr_id() : db::buffer_root<N>()
      {
        db::buffer_root<N>::attr_vec().emplace_back(&c1_smallint_             );
        db::buffer_root<N>::attr_vec().emplace_back(&c2_int_                  );
        db::buffer_root<N>::attr_vec().emplace_back(&c3_bigint_               );
        db::buffer_root<N>::attr_vec().emplace_back(&c4_numeric_              );
        db::buffer_root<N>::attr_vec().emplace_back(&c5_decimal_              );
        db::buffer_root<N>::attr_vec().emplace_back(&c6_dec_                  );
        db::buffer_root<N>::attr_vec().emplace_back(&c7_char_                 );
        db::buffer_root<N>::attr_vec().emplace_back(&c8_varchar_              );
        db::buffer_root<N>::attr_vec().emplace_back(&c9_date_                 );
        db::buffer_root<N>::attr_vec().emplace_back(&c10_time_                );
        db::buffer_root<N>::attr_vec().emplace_back(&c11_timestamp_           );
        db::buffer_root<N>::attr_vec().emplace_back(&c14_char_for_bit_data_   );
        db::buffer_root<N>::attr_vec().emplace_back(&c15_double_              );
        db::buffer_root<N>::attr_vec().emplace_back(&c16_float_               );
        db::buffer_root<N>::attr_vec().emplace_back(&c17_varchar_for_bit_data_);
      }
      /// @brief trivial getters
      //@{
      int16_t                  c1_smallint             () const {return c1_smallint(0);}
      int32_t                  c2_int                  () const {return c2_int(0);}
      int64_t                  c3_bigint               () const {return c3_bigint(0);}
      cstr_t                   c4_numeric              () const {return c4_numeric(0);}
      cstr_t                   c5_decimal              () const {return c5_decimal(0);}
      cstr_t                   c6_dec                  () const {return c6_dec(0);}
      cstr_t                   c7_char                 () const {return c7_char(0);}
      cstr_t                   c8_varchar              () const {return c8_varchar(0);}
      SQL_DATE_STRUCT          c9_date                 () const {return c9_date(0);}
      SQL_TIME_STRUCT          c10_time                () const {return c10_time(0);}
      SQL_TIMESTAMP_STRUCT     c11_timestamp           () const {return c11_timestamp(0);}
      std::span<const uint8_t> c14_char_for_bit_data   () const {return c14_char_for_bit_data(0);}
      double                   c15_double              () const {return c15_double(0);}
      double                   c16_float               () const {return c16_float(0);}
      std::span<const uint8_t> c17_varchar_for_bit_data() const {return c17_varchar_for_bit_data(0);}
      //@}
      /// @brief getters
      //@{
      int16_t                  c1_smallint             (uint ndx) const {return c1_smallint_.value(ndx);}
      int32_t                  c2_int                  (uint ndx) const {return c2_int_.value(ndx);}
      int64_t                  c3_bigint               (uint ndx) const {return c3_bigint_.value(ndx);}
      cstr_t                   c4_numeric              (uint ndx) const {return c4_numeric_.value(ndx);}
      cstr_t                   c5_decimal              (uint ndx) const {return c5_decimal_.value(ndx);}
      cstr_t                   c6_dec                  (uint ndx) const {return c6_dec_.value(ndx);}
      cstr_t                   c7_char                 (uint ndx) const {return c7_char_.value(ndx);}
      cstr_t                   c8_varchar              (uint ndx) const {return c8_varchar_.value(ndx);}
      SQL_DATE_STRUCT          c9_date                 (uint ndx) const {return c9_date_.value(ndx);}
      SQL_TIME_STRUCT          c10_time                (uint ndx) const {return c10_time_.value(ndx);}
      SQL_TIMESTAMP_STRUCT     c11_timestamp           (uint ndx) const {return c11_timestamp_.value(ndx);}
      std::span<const uint8_t> c14_char_for_bit_data   (uint ndx) const {return c14_char_for_bit_data_.value(ndx);}
      double                   c15_double              (uint ndx) const {return c15_double_.value(ndx);}
      double                   c16_float               (uint ndx) const {return c16_float_.value(ndx);}
      std::span<const uint8_t> c17_varchar_for_bit_data(uint ndx) const {return c17_varchar_for_bit_data_.value(ndx);}
      //@}
      /// @brief trivial setters
      //@{
      void set_c1_smallint             (int16_t v) {set_c1_smallint(v, 0);}
      void set_c2_int                  (int32_t v) {set_c2_int(v, 0);}
      void set_c3_bigint               (int64_t v) {set_c3_bigint(v, 0);}
      void set_c4_numeric              (cstr_t  v) {set_c4_numeric(v, 0);}
      void set_c5_decimal              (cstr_t  v) {set_c5_decimal(v, 0);}
      void set_c6_dec                  (cstr_t  v) {set_c6_dec(v, 0);}
      void set_c7_char                 (cstr_t  v) {set_c7_char(v, 0);}
      void set_c8_varchar              (cstr_t  v) {set_c8_varchar(v, 0);}
      void set_c9_date                 (SQL_DATE_STRUCT v) {set_c9_date(v, 0);}
      void set_c10_time                (SQL_TIME_STRUCT v) {set_c10_time(v, 0);}
      void set_c11_timestamp           (SQL_TIMESTAMP_STRUCT v) {set_c11_timestamp(v, 0);}
      void set_c14_char_for_bit_data   (std::span<uint8_t> v) {set_c14_char_for_bit_data(v, 0);}
      void set_c15_double              (double  v) {set_c15_double(v, 0);}
      void set_c16_float               (double  v) {set_c16_float(v, 0);}
      void set_c17_varchar_for_bit_data(std::span<uint8_t> v) {set_c17_varchar_for_bit_data(v, 0);}
      //@}
      /// @brief setters
      //@{
      void set_c1_smallint             (int16_t v, uint ndx) {c1_smallint_.set_value(v, ndx);}
      void set_c2_int                  (int32_t v, uint ndx) {c2_int_.set_value(v, ndx);}
      void set_c3_bigint               (int64_t v, uint ndx) {c3_bigint_.set_value(v, ndx);}
      void set_c4_numeric              (cstr_t  v, uint ndx) {c4_numeric_.set_value(v, ndx);}
      void set_c5_decimal              (cstr_t  v, uint ndx) {c5_decimal_.set_value(v, ndx);}
      void set_c6_dec                  (cstr_t  v, uint ndx) {c6_dec_.set_value(v, ndx);}
      void set_c7_char                 (cstr_t  v, uint ndx) {c7_char_.set_value(v, ndx);}
      void set_c8_varchar              (cstr_t  v, uint ndx) {c8_varchar_.set_value(v, ndx);}
      void set_c9_date                 (SQL_DATE_STRUCT v, uint ndx) {c9_date_.set_value(v, ndx);}
      void set_c10_time                (SQL_TIME_STRUCT v, uint ndx) {c10_time_.set_value(v, ndx);}
      void set_c11_timestamp           (SQL_TIMESTAMP_STRUCT v, uint ndx) {c11_timestamp_.set_value(v, ndx);}
      void set_c14_char_for_bit_data   (std::span<uint8_t> v, uint ndx) {c14_char_for_bit_data_.set_value(v, ndx);}
      void set_c15_double              (double  v, uint ndx) {c15_double_.set_value(v, ndx);}
      void set_c16_float               (double  v, uint ndx) {c16_float_.set_value(v, ndx);}
      void set_c17_varchar_for_bit_data(std::span<uint8_t> v, uint ndx) {c17_varchar_for_bit_data_.set_value(v, ndx);}
      //@}
    private:/*...private methods & attributes...*/
      constexpr static const uint c1_smallint_dec = 0; // number of decimal places
      constexpr static const uint c2_int_dec = 0; // number of decimal places
      constexpr static const uint c3_bigint_dec = 0; // number of decimal places
      constexpr static const uint c4_numeric_len               = 31; // max (b) string length
      constexpr static const uint c5_decimal_len               = 31; // max (b) string length
      constexpr static const uint c6_dec_len                   = 31; // max (b) string length
      constexpr static const uint c7_char_len                  = 10; // max (b) string length
      constexpr static const uint c8_varchar_len               = 10; // max (b) string length
      constexpr static const uint c9_date_dec = 0; // number of decimal places
      constexpr static const uint c10_time_dec = 0; // number of decimal places
      constexpr static const uint c11_timestamp_dec = 6; // number of decimal places
      constexpr static const uint c14_char_for_bit_data_len    = 15; // max (b) string length
      constexpr static const uint c15_double_dec = 0; // number of decimal places
      constexpr static const uint c16_float_dec = 0; // number of decimal places
      constexpr static const uint c17_varchar_for_bit_data_len = 15; // max (b) string length
      db::atomic <int16_t, SQL_SMALLINT, c1_smallint_dec, N> c1_smallint_{}; 
      db::atomic <int32_t, SQL_INTEGER, c2_int_dec, N> c2_int_{}; 
      db::atomic <int64_t, SQL_BIGINT, c3_bigint_dec, N> c3_bigint_{}; 
      db::string <std::array<char, c4_numeric_len>, SQL_DECIMAL, N, char, 5> c4_numeric_{}; 
      db::string <std::array<char, c5_decimal_len>, SQL_DECIMAL, N, char, 5> c5_decimal_{}; 
      db::string <std::array<char, c6_dec_len>, SQL_DECIMAL, N, char, 5> c6_dec_{}; 
      db::string <std::array<char, c7_char_len>, SQL_CHAR, N, char, 0> c7_char_{}; 
      db::string <std::array<char, c8_varchar_len>, SQL_VARCHAR, N, char, 0> c8_varchar_{}; 
      db::atomic <SQL_DATE_STRUCT, SQL_TYPE_DATE, c9_date_dec, N> c9_date_{}; 
      db::atomic <SQL_TIME_STRUCT, SQL_TYPE_TIME, c10_time_dec, N> c10_time_{}; 
      db::atomic <SQL_TIMESTAMP_STRUCT, SQL_TYPE_TIMESTAMP, c11_timestamp_dec, N> c11_timestamp_{}; 
      db::bstring<std::array<uint8_t, c14_char_for_bit_data_len>, SQL_BINARY, N, uint8_t, 0> c14_char_for_bit_data_{}; 
      db::atomic <double, SQL_DOUBLE, c15_double_dec, N> c15_double_{}; 
      db::atomic <double, SQL_DOUBLE, c16_float_dec, N> c16_float_{}; 
      db::bstring<std::array<uint8_t, c17_varchar_for_bit_data_len>, SQL_VARBINARY, N, uint8_t, 0> c17_varchar_for_bit_data_{}; 
      constexpr static const uint max_attr=15;
      std::array<db::attr_root_root<N>*, max_attr> attr_
      {
        &c1_smallint_,
        &c2_int_,
        &c3_bigint_,
        &c4_numeric_,
        &c5_decimal_,
        &c6_dec_,
        &c7_char_,
        &c8_varchar_,
        &c9_date_,
        &c10_time_,
        &c11_timestamp_,
        &c14_char_for_bit_data_,
        &c15_double_,
        &c16_float_,
        &c17_varchar_for_bit_data_      
      };
    }; // class qr_id
    class utl: public db::statement
    {
    public:
      explicit utl(db::connection* c): db::statement(c,sql) {}
      auto exec() { return exec_direct(sql, false); }
    private:
      constexpr static const auto* sql =
      R"k0a1f2k3a4(
        select *
        from tbl
        where 
          c1_smallint=?
      )k0a1f2k3a4";
    };
  }; //namespace Q_1
  /*....del_tbl_rec...................................................................................*/
  namespace del_tbl_rec
  {
    class utl: public db::statement
    {
    public:
      explicit utl(db::connection* c): db::statement(c,sql) {}
      auto exec() { return exec_direct(sql, false); }
    private:
      constexpr static const auto* sql =
      R"k0a1f2k3a4(
        delete from tbl;
      )k0a1f2k3a4";
    };
  }; //namespace del_tbl_rec
  /*....insert........................................................................................*/
  namespace insert
  {
    class utl: public db::statement
    {
    public:
      explicit utl(db::connection* c): db::statement(c,sql) {}
      auto exec() { return exec_direct(sql, false); }
    private:
      constexpr static const auto* sql =
      R"k0a1f2k3a4(
        insert into tbl 
        (
            C1_smallint, 
            C2_int, 
            C3_bigint
        ) 
        values 
        (
            ?,
            ?,
            ?
        )
      )k0a1f2k3a4";
    };
  }; //namespace insert
  /*!!!! There is no valid SQL statement in query 'q_2'. No code is generated. !!!!!!!!!!!!!!!!!!!!!!!*/
  /*!!!! There is no valid SQL statement in query 'q_3'. No code is generated. !!!!!!!!!!!!!!!!!!!!!!!*/
}; //namespace UT
