#ifndef SUBSTRING_H___
#define SUBSTRING_H___

#include <string>
#include <iostream>

/**
 * An immutable wrapper class for substrings of std::string to prevent copying when
 * passing substrings to auxiliary functions/classes.
 *
 * Actually I ended up not using it...
 */
class substring {
private:
    const std::string& _string;
    const std::size_t _start, _end;
public:
    using iterator = std::string::const_iterator;
    using reverse_iterator = std::string::const_reverse_iterator;
    
    explicit substring(const std::string& str, std::size_t pos, std::size_t count = std::string::npos);
    explicit substring(const std::string& str);
    
    std::size_t start_pos() const;
    std::size_t end_pos() const;
    const std::string& string() const;
    std::string make() const;

    const char& at(std::size_t i) const;
    const char& operator[](std::size_t i) const;
    const char& front() const;
    const char& back() const;

    bool empty() const;
    std::size_t size() const;
    std::size_t length() const;

    iterator begin() const;
    iterator end() const;
    reverse_iterator rbegin() const;
    reverse_iterator rend() const;

    substring substr(std::size_t pos, std::size_t count = std::string::npos) const;

    friend std::ostream& operator<<(std::ostream& out, const substring& substr);
};

#endif // SUBSTRING_H___
