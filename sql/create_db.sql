connect reset;

drop db test;

create db test pagesize 32 K;

connect to test;

CREATE USER TEMPORARY TABLESPACE "UTMP4K" 
         PAGESIZE 32 K MANAGED BY AUTOMATIC STORAGE
         USING STOGROUP "IBMSTOGROUP"
         EXTENTSIZE 4
         PREFETCHSIZE AUTOMATIC
         BUFFERPOOL "IBMDEFAULTBP"
         OVERHEAD INHERIT
         TRANSFERRATE INHERIT
         FILE SYSTEM CACHING
         DROPPED TABLE RECOVERY OFF;

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
    c13_blob blob(20),
    c14_clob clob(20),
    c15_char_for_bit_data char (15) for bit data,
    c16_double double,
    c17_float  float,
    c18_varchar_for_bit_data varchar(15) for bit data
);
connect reset;
