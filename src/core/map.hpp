#ifndef MAP_HPP_
#define MAP_HPP_

#include <vector>

//TODO use a list instead of a vector
template<typename T>
class IntMap {
public:
  T& GetByKey(unsigned int key);
  unsigned int Insert(T value);
  std::size_t size() const;
  T& operator[](std::size_t n);
private:
  std::size_t IndexByKey(unsigned int key) const;
  std::vector<std::pair<unsigned int, T>> vec;
};

/* Template class implementation */
#include "map.tpp"

#endif
