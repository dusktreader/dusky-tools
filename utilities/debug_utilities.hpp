#pragma once

// The DB_REP Macros are very convenient for debugging.
// Each macro prints a message when the program reaches it in execution

#define SUPRESS_DB_REPS
#ifndef SUPRESS_DB_REPS

//#warning "DB_REPS Not Supressed"

/** Prints the file and line */
#define DB_REP { std::cout << __FILE__ << ":" << __LINE__ << ": reporting" << std::endl; }

/** Prints the file name, line and a message
  * @param  msg - The message to print
  */
#define DB_REP_MSG( msg ) { std::cout << __FILE__ ":" << __LINE__ << ": message: " << (msg) << std::endl; }

/** Prints the file, line, variable name, and variable value
  * @param  var - The variable to report
  */
#define DB_REP_VAR( var ) { std::ostringstream op; \
                            op << __FILE__ ":" << __LINE__ << ": "; \
                            printVar( var, #var, op ); \
                            std::cout << op.str() << std::flush; }

/** Prints the file, line, variable name, and variable value, and a message
  * @param  var - The variable to report
  */
#define DB_REP_MSGVAR( msg, var ) { std::ostringstream op; \
                                    op << __FILE__ ":" << __LINE__ << ": "; \
                                    op << (msg) << ": "; \
                                    printVar( var, #var, op ); \
                                    std::cout << op.str() << std::flush; }

/** Prints the file name, line and a message if a condition is true
  * @param  cnd - The condition to test
  * @param  msg - The message to print
  */
#define DB_REP_CND( cnd, msg ) if(cnd) { std::cout << __FILE__ ":" << __LINE__ << " : messsage:" << (msg) << std::endl; }

/** Prints the file, line, list name, and list values
  * @param  list - The list to print
  * @param  l    - The length of the list
  */
#define DB_REP_LIST( list, l ) { std::ostringstream op; \
                                 op << __FILE__ ":" << __LINE__ << " : " << std::flush; \
                                 printList( list, l, #list, op ); \
                                 std::cout << op.str() << std::flush; }

/** Prints the file, line, and thread# */
#define DB_REP_OMP _Pragma( "omp critical" ){ \
                   std::ostringstream op; \
                   op << __FILE__ ":" << __LINE__ << ": "; \
                   op << "thread #" << omp_get_thread_num() << " reporting"; \
                   std::cout << op.str() << std::endl; }

/** Prints the file, line, thread#, and a message
  * @param  msg - The message to print
  */
#define DB_REP_OMP_MSG( msg ) _Pragma( "omp critical" ){ \
                              std::ostringstream op; \
                              op << __FILE__ ":" << __LINE__ << ":"; \
                              op << "thread #" << omp_get_thread_num() << ": message: " << msg; \
                              std::cout << op.str() << std::endl; }

/** Prints the file, line, thread#, variable name, and variable value
  * @param  var - The variable to report
  */
#define DB_REP_OMP_VAR( var ) _Pragma( "omp critical" ){ \
                              std::ostringstream op; \
                              op << __FILE__ ":" << __LINE__ << ": "; \
                              op << "thread #" << omp_get_thread_num() << ": "; \
                              printVar( var, #var, op ); \
                              std::cout << op.str() << std::flush; }

/** Prints the file, line, thread#, variable name, and variable value
  * @param  var - The variable to report
  */
#define DB_REP_OMP_LIST( list, l ) _Pragma( "omp critical" ){ \
                                   std::ostringstream op; \
                                   op << __FILE__ ":" << __LINE__ << ": " << std::flush; \
                                   op << "thread #" << omp_get_thread_num() << ": " << std::flush; \
                                   printList( list, l, #list, op ); \
                                   std::cout << op.str() << std::flush; }

#else
//#warning "DB_REPS Supressed"
#define DB_REP {}
#define DB_REP_MSG( msg ) {}
#define DB_REP_VAR( var ) {}
#define DB_REP_MSGVAR( msg, var ) {}
#define DB_REP_CND( cnd, msg ) {}
#define DB_REP_LIST( list, l ) {}
#define DB_REP_OMP {}
#define DB_REP_OMP_MSG( msg ) {}
#define DB_REP_OMP_VAR( var ) {}
#define DB_REP_OMP_LIST( list, l ) {}
#endif
