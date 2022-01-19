#include "../includes/concordance.hpp"

// ALREADY DONE: READ FROM A TEXT FILE LINE BY LINE
void concordance::readText(std::istream& in, const std::string& ignore) {
  std::string line;

  for (size_t i = 1; getline(in, line, '\n'); i++) {
    std::transform(line.begin(), line.end(), line.begin(), ::tolower);

    typedef std::list<std::string> container;
    container words;  // split into container, dump ignore chars
    split<container>(line, words, ignore);

    typename container::const_iterator cit;
    for (cit = words.cbegin(); cit != words.cend(); ++cit) {
      addWord(*cit, i);
    }
  }
}

void concordance::addWord(const std::string& word, size_t line) {
  if (this->word_map_.find(word) == std::end(this->word_map_)) {
    word_map_.insert(std::pair<std::string, size_t>(word, line));
  }
}

size_t concordance::totalWords(size_t size) const {
  /*
   * Return the total number of words with a given length
   */
  return std::accumulate(
      std::begin(this->word_map_), std::end(this->word_map_), 0,
      [&size](const size_t& lhs, const std::pair<std::string, size_t> rhs) {
        size_t amount = rhs.first.length() == size ? 1 : 0;
        return lhs + amount;
      });
}

// TO DO: return the total number of words of all lengths
size_t concordance::totalWords() const {
  std::vector<size_t> _range =
      std::vector<size_t>(maxwordlen_ - minwordlen_ + 1);
  std::iota(_range.begin(), _range.end(), minwordlen_);

  return std::accumulate(
      std::begin(_range), std::end(_range), 0,
      [this](const size_t& lhs, size_t& i) { return lhs + totalWords(i++); });
}

void concordance::wordAtLength(std::vector<std::string>& v,
                               size_t length) const {
  for (auto it = word_map_.begin(); it != word_map_.end();
       it = word_map_.upper_bound(it->first)) {
    if (it->first.length() == length) {
      v.push_back(it->first);
    }
  }
  // sanitizeVector(v);
}

void concordance::findValsOfKey(std::vector<size_t>& v,
                                const std::string& key) const {
  auto result = word_map_.equal_range(key);
  for (auto iterator = result.first; iterator != result.second; ++iterator) {
    v.push_back(
        iterator->second);  // add all values to the vector unconditionally
  }
  // sanitizeVector(v);
}

template <typename T>
void concordance::print(const std::vector<T>& v) const {
  for (auto& a : v) {  // loop through the vector
    if (&a == &v.back()) {
      std::cout << " " << a << std::endl;
      break;
    }  // if we are in the last element of the vector, we need to add a new line
       // and immediately break out of the loop
    std::cout << " " << a;  // else just print the way we did before
  }
}

void concordance::print(std::ostream& out) const {
  std::vector<size_t> v;
  std::vector<std::string> wal;
  std::map<size_t, size_t> masterMap;
  for (size_t i = minwordlen_; i <= maxwordlen_; ++i) {
    wordAtLength(wal, i);
    if (wal.size() > 0) {
      std::cout << "+--------------------+" << std::endl;
      std::cout << "|  " << i << " Letter Words…  |" << std::endl;
      std::cout << "+--------------------+" << std::endl;
      masterMap[i] = wal.size();
    }
    for (std::string& s : wal) {
      findValsOfKey(v, s);
      out << s << ":";  // finally print our key
      print(v);         // this functionality is given to an overloaded print
      v.clear();        // clear the vector for the next word
    }
    std::cout << "..." << wal.size() << " words of length: " << i << std::endl;
    wal.clear();
  }
  std::cout << "+--------------------+" << std::endl;
  std::cout << "| Word Length Distro |" << std::endl;
  std::cout << "+--------------------+" << std::endl << std::endl;
  for (auto wordLength : masterMap) {
    std::cout << "Length: " << wordLength.first << "\t..." << wordLength.second
              << " words" << std::endl;
  }
  masterMap.clear();
}
