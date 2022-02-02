drop table tbl;
create table tbl
(
    c1_smallint  smallint,
    c2_int  int,
    c3_bigint  bigint,
    c4_numeric  numeric(31,5),
    c5_decimal  decimal(31,5),
    c6_dec  dec(31,5),
    c7_char  char(10),
    c8_varchar  varchar(10),
    c9_date  DATE,
    c10_time TIME,
    c11_timestamp timestamp,
--    c12 timestamp with time zone--,
--    c13_blob blob(2000),
--    c14_clob clob(2000),
    c14_char_for_bit_data char (15) for bit data,
    c15_double double,
    c16_float  float,
    c17_varchar_for_bit_data varchar(15) for bit data

);
