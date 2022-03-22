#include <iostream>

#include "e1.hpp"
int main()
{
  db::connection c("test");         // establish connection to the rdbms
  {                                 // insert record
    e1::q_1::utl q(&c);             // create query structure
    auto*        par = q.par_buf(); // par = buffer for query parameter values
    par->set_par_1(3);              // set value of the parameter to 3
    auto rc = q.exec();             // execute sql query (insert)
    if (rc != SQL_SUCCESS) std::cerr << db::error(q.handle(), SQL_HANDLE_STMT).dump() << std::endl;
    c.commit();
  }
  {                                   // select records
    e1::q_2::utl<> q(&c);             // create query stucture
    auto*          res = q.res_buf(); // obtain result buffer structure
    auto           rc  = q.exec();    // execute query (select)
    if (rc != SQL_SUCCESS) std::cerr << db::error(q.handle(), SQL_HANDLE_STMT).dump() << std::endl;

    while (q.fetch() == SQL_SUCCESS) { std::cerr << res->dump("-->") << std::endl; }
    c.commit();
  }
  return 0;
}