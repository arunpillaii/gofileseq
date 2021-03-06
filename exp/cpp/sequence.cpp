#include "sequence.h"
#include "private/fileseq_p.h"

#include <iostream>

namespace fileseq {

FileSequence::FileSequence(const std::string &frange, Status* ok)
    : m_valid(false)
    , m_id(0)
    , m_fsetId(0) {

    init(frange, PadStyleDefault, ok);
}

FileSequence::FileSequence(const std::string &frange, PadStyle padStyle, Status* ok)
    : m_valid(false)
    , m_id(0)
    , m_fsetId(0) {

    init(frange, padStyle, ok);
}

void FileSequence::init(const std::string &frange, PadStyle padStyle, Status* ok) {
    internal::FileSequence_New_Pad_return fs = internal::FileSequence_New_Pad(
                const_cast<char*>(frange.c_str()), int(padStyle));

    if (fs.r1 != NULL) {
        std::string err = "Failed to create sequence(" + frange + "): " + std::string(fs.r1);
        if (ok != NULL) {
            ok->setError(err);
        } else {
            internal::printErrorIgnored(err);
        }
        free(fs.r1);
        return;
    }

    if (ok != NULL) {
        ok->clearError();
    }

    m_id = fs.r0;
    m_valid = true;
}

FileSequence::FileSequence(uint64_t id)
    : m_valid(false)
    , m_id(id)
    , m_fsetId(0)
{
//    std::cout << "FileSequence from id " << id << std::endl;
    m_valid = (id > 0);
}

FileSequence::~FileSequence() {
    if (m_valid) {
//        std::cout << "FileSequence destroy " << m_id << std::endl;
        m_valid = false;
        internal::FileSequence_Decref(m_id);
        m_id = 0;
        m_fsetId = 0;
    }
}

FileSequence::FileSequence(const FileSequence& rhs)
    : m_valid(rhs.m_valid)
    , m_id(rhs.m_id)
    , m_fsetId(0)
{
    if (m_id != 0) {
//        std::cout << "FileSequence copy " << rhs.m_id << std::endl;
        internal::FileSequence_Incref(m_id);
        m_valid = true;
    }
}

FileSequence& FileSequence::operator=(const FileSequence& rhs) {
    // Decref the previous value
//    std::cout << "FileSequence assign " << rhs.m_id << std::endl;

    uint64_t oldId = m_id;

    // Take on the reference from the other
    m_id = rhs.m_id;
    m_fsetId = 0;

    if (m_id != 0) {
        m_valid = true;
        internal::FileSequence_Incref(m_id);
    } else {
        m_valid = false;
    }

    if (oldId != 0) {
        internal::FileSequence_Decref(oldId);
//        std::cout << "   decref old " << oldId << std::endl;
    }

    return *this;
}

size_t FileSequence::length() const {
    return internal::FileSequence_Len(m_id);
}

std::string FileSequence::string() const {
    internal::StringProxy str = internal::FileSequence_String(m_id);
    return str;
}

std::string FileSequence::format(const std::string& fmt, Status* ok) const {
    internal::FileSequence_Format_return ret;
    ret = internal::FileSequence_Format(m_id, const_cast<char*>(fmt.c_str()));
    std::string str;

    // Handle value
    if (ret.r0) {
        str.assign(ret.r0);
        free(ret.r0);
    }

    // Handle err
    if (ret.r1) {
        if (ok != NULL) {
            ok->setError(ret.r1);
        } else {
            internal::printErrorIgnored(ret.r1);
        }
        free(ret.r1);
        return str;
    }

    if (ok != NULL) {
        ok->clearError();
    }
    return str;
}

std::string FileSequence::dirname() const {
    internal::StringProxy str = internal::FileSequence_Dirname(m_id);
    return str;
}

void FileSequence::setDirname(const std::string& dirname) const {
    internal::FileSequence_SetDirname(m_id, const_cast<char*>(dirname.c_str()));
}

std::string FileSequence::basename() const {
    internal::StringProxy str = internal::FileSequence_Basename(m_id);
    return str;
}

void FileSequence::setBasename(const std::string& basename) const {
    internal::FileSequence_SetBasename(m_id, const_cast<char*>(basename.c_str()));
}

std::string FileSequence::ext() const {
    internal::StringProxy str = internal::FileSequence_Ext(m_id);
    return str;
}

void FileSequence::setExt(const std::string& ext) const {
    internal::FileSequence_SetExt(m_id, const_cast<char*>(ext.c_str()));
}

std::string FileSequence::padding() const {
    internal::StringProxy str = internal::FileSequence_Padding(m_id);
    return str;
}

void FileSequence::setPadding(const std::string &padChars) const {
    internal::FileSequence_SetPadding(m_id, const_cast<char*>(padChars.c_str()));
}

PadStyle FileSequence::paddingStyle() const {
    return (PadStyle)internal::FileSequence_PaddingStyle(m_id);
}

void FileSequence::setPaddingStyle(PadStyle style) const {
    internal::FileSequence_SetPaddingStyle(m_id, int(style));
}

int FileSequence::start() const {
    return internal::FileSequence_Start(m_id);
}

int FileSequence::end() const {
    return internal::FileSequence_End(m_id);
}

int FileSequence::zfill() const {
    return internal::FileSequence_ZFill(m_id);
}

std::string FileSequence::frameRange(bool padded) const {
    internal::StringProxy str;
    if (padded) {
        str = internal::FileSequence_FrameRangePadded(m_id);
    } else {
        str = internal::FileSequence_FrameRange(m_id);
    }
    return str;
}

void FileSequence::setFrameRange(const std::string &frange, Status* ok) {
    internal::StringProxy err = internal::FileSequence_SetFrameRange(
                m_id, const_cast<char*>(frange.c_str()));

    // Clear the cached FrameSet id, since it has now changed.
    // Next access to the FrameSet will update with a new id.
    m_fsetId = 0;

    if (err) {
        if (ok) {
            ok->setError(err);
        } else {
            internal::printErrorIgnored(err);
        }
        return;
    }

    if (ok) {
        ok->clearError();
    }
}

std::string FileSequence::invertedFrameRange(bool padded) const {
    internal::StringProxy str;
    if (padded) {
        str = internal::FileSequence_InvertedFrameRangePadded(m_id);
    } else {
        str = internal::FileSequence_InvertedFrameRange(m_id);
    }
    return str;
}

std::string FileSequence::index(int idx) const {
    internal::StringProxy ret = internal::FileSequence_Index(m_id, idx);
    return ret;
}

std::string FileSequence::frame(int frame) const {
    internal::StringProxy ret = internal::FileSequence_Frame_Int(m_id, frame);
    return ret;
}

std::string FileSequence::frame(const std::string &fillPattern) const {
    internal::StringProxy ret = internal::FileSequence_Frame_Fill(
                m_id, const_cast<char*>(fillPattern.c_str()));
    return ret;
}

FrameSet FileSequence::frameSet() {
    if (m_fsetId == 0) {
        m_fsetId = internal::FileSequence_FrameSet(m_id);
    }
    return FrameSet(m_fsetId);
}

void FileSequence::setFrameSet(FrameSet &frameSet) {
    bool ok = internal::FileSequence_SetFrameSet(m_id, frameSet.m_id);
    if (ok) {
        m_fsetId = frameSet.m_id;
    }
}

} // fileseq
