#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

ByteStream::ByteStream(const size_t capacity):capa(capacity),readed(0),written(0),endof(false), str(""){  }

size_t ByteStream::write(const string &data) {
    size_t enable=min(data.length(),capa-(written- readed));
    str=str+data.substr(0,enable);
    written+=enable;
    return enable;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    return str.substr(0,len);
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) { 
    readed+=len;
    str=str.substr(len);
 }

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string temp=peek_output(len);
    pop_output(len);
    return temp;
}

void ByteStream::end_input() {endof=true;}

bool ByteStream::input_ended() const { return endof; }

size_t ByteStream::buffer_size() const { return written-readed; }

bool ByteStream::buffer_empty() const { return written==readed; }

bool ByteStream::eof() const { return written==readed&&endof; }

size_t ByteStream::bytes_written() const { return written; }

size_t ByteStream::bytes_read() const { return readed; }

size_t ByteStream::remaining_capacity() const { return capa-(written-readed); }
