#ifndef KEYMANAGER_H
#define KEYMANAGER_H

#include <map>
#include <utility>
#include <exception>
#include <string>
#include <type_traits>

namespace sse
{
template<typename K, typename V>
class KeyManager
{
public:
    KeyManager() {}

    std::map<K, V> map;

    void add(const K& key, V value)
    {
        map.insert(std::make_pair(key, value));
    }

    void remove(const K& key)
    {
        auto result = map.find(key);
        if (result != map.end()) {
            map.erase(key);
        }
    }

    V get(const K& key)
    {
        auto result = map.find(key);
        if (result == map.end()) {
            // If the key is a std::string or char*, include it in the error message.
            std::string error_msg("Bad key");
            if (std::is_same<K, std::string>::value || std::is_same<K, char*>::value) {
                error_msg += (": " + key);
            }
            throw std::runtime_error(error_msg);
        }
        return result->second;
    }

    int get_count() const
    {
        return map.size();
    }
};
}

#endif // KEYMANAGER_H
