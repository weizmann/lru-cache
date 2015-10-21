#ifndef _LRU_CACHE_USING_STD_H
#define _LRU_CACHE_USING_STD_H

#include <cassert>
#include <list>

using namespace::std;

template <
    typename K,
    typename V,
    template <typename ...> class MAP
> class lru_cache_using_std
{
public:
    typedef K key_type;
    typedef V value_type;

    typedef list<key_type> key_tracker_type;
    typedef MAP<
        key_type,
        pair<value_type, typename key_tracker_type::iterator>
        > key_to_value_type;

    lru_cache_using_std(value_type (*f) (const key_type&), size_t c)
    : _fn(f)
    , _capacity(c)
    {
        assert(_capacity > 0)
    }

    value_type operator() (const key_type& k) 
    {
        const typename key_to_value_type::iterator it = _key_to_value.find(k);
        if (it == _key_to_value.end())
        {
            const value_type v = _fn(k);
            insert(k, v);
            return v;
        }
        else 
        {
            _key_tracker.splice(_key_tracker.end(), _key_tracker, (*it).second.second);    
            return (*it).second.second;
        }
    }

private:
    void insert(const key_type& k, const value_type& v)
    {
       assert(_key_to_value.find(k) == _key_to_value.end()); 

       if (_key_to_value.size() == _capacity)
       {
           evict();
       }

       typename key_tracker_type::iterator it = _key_tracker.insert(_key_tracker.end(), k);
       _key_to_value.insert(make_pair(k, make_pair(v, it)));
    }

    void evict()
    {
        assert(!_key_to_value.empty()); 

        const typename key_to_value::iterator it = _key_to_value.find(_key_tracker.front());
        assert(it != _key_to_value.end());

        _key_tracker.erase(it);
        _key_tracker.pop_front();
    }

private:
    value_type (*_fn) (const key_type&);
    const size_t _capacity;
    key_tracker_type _key_tracker;
    key_to_value_type _key_to_value;


};

#endif
