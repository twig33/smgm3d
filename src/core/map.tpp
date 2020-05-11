#include "map.hpp"
#include "output.hpp"
#include <stdexcept>

template<typename T>
unsigned int IntMap<T>::Insert(T value){
  unsigned int id = 0;
  if (vec.empty()){
    vec.push_back(std::make_pair(id, value));
    return id;
  }
  for (std::size_t index = 0; index < vec.size()-1; ++index){
    if (vec[index+1].first != vec[index].first + 1){
      id = vec[index].first+1;
      vec.insert(vec.begin()+index+1, std::make_pair(id, value));
      return id;
    }
  }
  id = vec.back().first+1;
  vec.push_back(std::make_pair(id, value));
  return id;
}

template<typename T>
std::size_t IntMap<T>::IndexByKey(unsigned int key) const{
  int l = 0, r = vec.size()-1, m;
  while (r >= l){
    m = (l+r)/2;
    if (vec[m].first == key){
      return m;
    }
    else if (vec[m].first > key){
      r = m-1;
    }
    else {
      l = m+1;
    }
  }
  return -1;
}

template<typename T>
size_t IntMap<T>::size() const {
  return vec.size();
}

template<typename T>
T& IntMap<T>::operator[](size_t n){
  return vec[n].second;
}

template<typename T>
void IntMap<T>::erase(unsigned int index){
  if (index >= vec.size()){
    Output::stream << "IntMap::erase invalid index\n";
    return;
  }
  vec.erase(vec.begin() + index);
}

template<typename T>
T& IntMap<T>::GetByKey(unsigned int key){
  unsigned int index = IndexByKey(key);
  if (index == -1){
    throw std::logic_error("IntMap::Get object with given key doesn't exist");
  }
  return vec[index].second;
}

template<typename T>
void IntMap<T>::EraseByKey(unsigned int key){
  unsigned int index = IndexByKey(key);
  if (index == -1){
    Output::stream << "IntMap::EraseByKey object already removed or wasnt ever inserted\n";
    return;
  }
  vec.erase(vec.begin() + index);
}
