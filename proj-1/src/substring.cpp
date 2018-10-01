#include "substring.h"

substring::substring(const std::string& str, std::size_t pos, std::size_t count):
    _string(str), _start(pos),
    _end(count == std::string::npos ? str.size() : (pos + count)) {}

substring::substring(const std::string& str):
    _string(str), _start(0), _end(str.size()) {}

std::size_t substring::start_pos() const {
    return _start;
}

std::size_t substring::end_pos() const {
    return _end;
}

const std::string& substring::string() const {
    return _string;
}

std::string substring::make() const {
    return _string.substr(_start, _end - _start);
}

const char& substring::at(std::size_t i) const {
    return _string.at(i + _start);
}

const char& substring::operator[](std::size_t i) const {
    return _string[i + _start];
}

const char& substring::front() const {
    return _string[_start];
}

const char& substring::back() const {
    return _string[_end - 1];
}

bool substring::empty() const {
    return _start == _end;
}

std::size_t substring::size() const {
    return _end - _start;
}

std::size_t substring::length() const {
    return _end - _start;
}

substring::iterator substring::begin() const {
    return _string.cbegin() + _start;
}

substring::iterator substring::end() const {
    return _string.cbegin() + _end;
}

substring::reverse_iterator substring::rbegin() const {
    return _string.rbegin() + (_string.size() - _end);
}

substring::reverse_iterator substring::rend() const {
    return _string.rbegin() + (_string.size() - _start);
}

substring substring::substr(std::size_t pos, std::size_t count) const {
    if (count == std::string::npos) {
        count = _end - (_start + pos);
    }
    return substring(_string, _start + pos, count);
}

std::ostream& operator<<(std::ostream& out, const substring& substr) {
    return out << substr.make();
}
