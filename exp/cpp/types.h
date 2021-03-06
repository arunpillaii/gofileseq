#ifndef _GOFILESEQ_CPP_TYPES_H_
#define _GOFILESEQ_CPP_TYPES_H_

#include <ostream>

namespace fileseq {

enum PadStyle {
    PadStyleHash1 = 0,
    PadStyleHash4 = 1,
    PadStyleDefault = PadStyleHash4
};

typedef std::vector<FileSequence> FileSequences;

/*!
Status is used to communicate whether various calls
have succeeded or failed. It can be treated like a
bool to test the success, and converted to a
std::string to receive the error message.
*/
class Status {

public:
    Status() {}

    //! If Status evaluates to false, it indicates an error state
    operator bool() const { return m_error.empty(); }

    //! Retrieve the string error message
    operator std::string() const { return m_error; }

    Status(const Status& rhs) : m_error(rhs.m_error) {}

    Status& operator=(const Status& rhs) {
        m_error = rhs.m_error;
        return *this;
    }

    void clearError() { m_error.clear(); }
    void setError(const std::string &err) { m_error = err; }
    void setError(const char* err) { m_error = err; }

    friend std::ostream& operator<< (std::ostream& stream, const Status& stat) {
        stream << (std::string)stat;
        return stream;
    }

private:
    std::string m_error;

};


} // fileseq

#endif // _GOFILESEQ_CPP_TYPES_H_

