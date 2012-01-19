#pragma once

#include "utilities.hpp"

/** A custom exception that is thrown when a local ASSERT fails */
class LocalAssert: public std::exception
{
public:
    enum Status{ Error, NoError };

    /** Creates the exception with a default message */
    LocalAssert( std::string info = "Location unknown", std::string message = "Local Assertion Failed" );

    /** Destructs the exception */
    ~LocalAssert() throw();

    /** Returns the message describing the cause of the exception */
    virtual const char* what() const throw();

    /** Returns the message describing the cause of the exception */
    std::string message();

    /** Returns the location where the exception occurred */
    std::string where();

    /** Returns a backtrace of execution before the exception occurred */
    std::string trace();

    /** Returns a string describing the error in detail */
    std::string details();

    /** Fetches and resets the current status of LocalAssert */
    static LocalAssert::Status fetchStatus();

    /** Resets the state of the status flag */
    static void resetStatus();

private:
    /** Describes the message set during throw */
    std::string _message;
    std::string _info;
    std::string _trace;

    static LocalAssert::Status status;
};



/** Improves the assert() idiom by allowing it to be caught in a try block */
#define ASSERT( expr )                                                \
    if( ( expr ) == false )                                           \
        throw LocalAssert( std::string("in function ") + PRINT_FUNC + \
                           "  of file " + __FILE__ +                  \
                           " at line " + num2str(__LINE__) );

/** Allows an assertion with a failure message */
#define ASSERT_MSG( expr, msg )                                       \
    if( ( expr ) == false )                                           \
        throw LocalAssert( std::string("in function ") + PRINT_FUNC + \
                           "  of file " + __FILE__ +                  \
                           " at line " + num2str(__LINE__),           \
                           std::string( msg ) );

/** Sets a local variable to a value contingent on a logical expression */
#define ASSERT_SET( var, val, expr, msg ) \
    { ASSERT_MSG( expr, msg ); \
      var = val; } \

/** Breaks execution at this point */
#define ABORT \
    throw LocalAssert( std::string(" in function ") + PRINT_FUNC + \
                       "  of file " + __FILE__ +                   \
                       " at line " + num2str(__LINE__),            \
                      "Aborted" );

/** Breaks execution at this point */
#define ABORT_MSG( msg ) \
    throw LocalAssert( std::string(" in function ") + PRINT_FUNC + \
                       "  of file " + __FILE__ +                   \
                       " at line " + num2str(__LINE__),            \
                       std::string( msg ) );

/** Catches a Local Assert and prints details to stdout         */
#define CATCH_LASS                                                            \
    catch( LocalAssert& err )                                                 \
    {                                                                         \
        cout << "Warning:  Local Assert failed:" << endl;                     \
        cout << "    " << err.what() << endl;                                 \
        cout << "    " << err.where() << endl;                                \
    }                                                                         \
    catch( ... )                                                              \
    {                                                                         \
        cout << "Warning:  Unknown Exeption occured: Re-trhowing..." << endl; \
        throw;                                                                \
    }
