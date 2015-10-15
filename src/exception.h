#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <string>
#include <sstream>
#include <exception>
#include <iostream>

class Exception : public std::exception
{
    std::stringstream    ss;
    bool                 is_printed;

protected:
    Exception(const char* exception_class) : is_printed(false)
    {
        if (exception_class) { ss << exception_class << " "; }
    }

public:
    Exception(const Exception& other) : ss(other.ss.str()), is_printed(other.is_printed)
    {
        // Print during copy when the message is already constructed
        if (!is_printed)
        {
            std::cerr << ss.str() << std::endl;
            is_printed = true;
        }
    }

    template <class T>
    Exception& operator<<(const T& item)    { ss << item; return *this; }

    virtual const char* what() const throw() override
    {
        return ss.str().c_str();
    }

    static const char* strip_file(const char* file) { return file; }
    static const char* strip_func(const char* func) { return func; }
};

#define DEFINE_EXCEPTION(_name, _base)                                   \
    class _name : public _base                                           \
    {                                                                    \
    protected:                                                           \
        _name(const char* exception_class) : _base(exception_class) {}   \
                                                                         \
    public:                                                              \
        _name() : _base(#_name) {};                                      \
    };


DEFINE_EXCEPTION(IOException,                   Exception);
DEFINE_EXCEPTION(FileNotFoundException,         IOException);

DEFINE_EXCEPTION(AssertException,               Exception);
DEFINE_EXCEPTION(IllegalArgumentException,      Exception);
DEFINE_EXCEPTION(IllegalStateException,         Exception);
DEFINE_EXCEPTION(NullPointerException,          Exception);
DEFINE_EXCEPTION(IndexOutOfBoundsException,     Exception);

// Throw and include some diagnostics (stack trace tbd)
#define THROW(_ex)                                             \
    throw _ex << Exception::strip_file(__FILE__) << ':'        \
              << __LINE__ << ' '                               \
              << Exception::strip_func(__FUNCTION__) << "() "

#define check_assert(_expr)    if (!(_expr)) { THROW (AssertException())           << #_expr; }
#define check_argument(_expr)  if (!(_expr)) { THROW (IllegalArgumentException())  << #_expr; }
#define check_bounds(_expr)    if (!(_expr)) { THROW (IndexOutOfBoundsException()) << #_expr; }
#define check_state(_expr)     if (!(_expr)) { THROW (IllegalStateException())     << #_expr; }
#define check_not_null(_expr)  if (!(_expr)) { THROW (NullPointerException())      << #_expr; }

#define check_state_msg(_expr, _msg)     if (!(_expr)) { THROW (IllegalStateException())     << #_expr << ' ' << _msg; }

#define check_stream_open(_strm, _fname) if (!(_strm).is_open()) { THROW (FileNotFoundException()) << _fname; }

#endif