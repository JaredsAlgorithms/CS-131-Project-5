#pragma once

#include <algorithm> /* std::transform */
#include <iostream>
#include <list>
#include <map>    /* std::multimap<T> and std::map<T> (not used) */
#include <string> /* not needed for Mac OSX */
#include <vector>
#include <numeric>

class StringLenCmp {
   public:
    StringLenCmp() = default;

    // sort by length first, and then alphabetically
    bool operator()(const std::string& a, const std::string& b) const {
        return (a.size() < b.size()) || (a.size() == b.size() && a < b);
    }

   private:
    // no data
};

class concordance {
    typedef std::multimap<std::string, size_t, StringLenCmp> wordDictType;

   public:
    concordance(size_t minwordlen = 1, size_t maxwordlen = 50)
        : minwordlen_(minwordlen), maxwordlen_(maxwordlen), words_at_length_() {
        for (size_t i = 0; i < MAXWORDLEN; ++i) {
            words_at_length_.push_back(0);
        }
    }

    void addWord(const std::string&, size_t);  // TO DO
    size_t totalWords() const;                 // TO DO
    size_t totalWords(size_t length) const;    // TO DO
    void wordAtLength(std::vector<std::string>& v, size_t length) const;
    void print(std::ostream&) const;  // TO DO
    template <typename T>
    void print(const std::vector<T>& v) const;

    void readText(std::istream&, const std::string&);  // ALREADY DONE
    void findValsOfKey(std::vector<size_t>& v, const std::string& key) const;

   private:
    static const size_t MAXWORDLEN = 40;

    wordDictType word_map_;  // multimap with key=word and value=page number
    size_t minwordlen_, maxwordlen_;
    std::vector<size_t> words_at_length_;  // number of words of each length
};

// ALREADY DONE: BREAK A LINE INTO A LIST OF WORDS
// Courtesy of Martin Broadhurst --
// http://www.martinbroadhurst.com/how-to-split-a-string-in-c.html
template <class Container>
void split(const std::string& str, Container& cont,
           const std::string& delims = " ") {
    std::size_t curr, prev = 0;
    curr = str.find_first_of(delims);

    while (curr != std::string::npos) {  // largest possible unsigned number
        cont.push_back(str.substr(prev, curr - prev));
        prev = curr + 1;
        curr = str.find_first_of(delims, prev);
    }

    cont.push_back(str.substr(prev, curr - prev));
}
