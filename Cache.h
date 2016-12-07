#include <list>
#include <unordered_map>

template < class Key, class Value, class Func >
class Cache
{
public:
    Cache( size_t max_size, Func f )
        : m_max_size( max_size )
        , f( f )
    {
    }

    ~Cache( )
    {
    }

    const Value& operator[]( const Key& key )
    {
        const auto iterator = m_values.find( key );
        if ( iterator == m_values.cend( ) )
        {
            // Check before inserting new elements that we didn't reach
            // cache limit of elements. If we reach the limit remove
            // the element less accessed
            if ( m_keys.size( ) == m_max_size )
            {
                m_values.erase( m_values.find( m_keys.front( ) ) );
                m_keys.pop_front( );
            }

            m_keys.push_back( key );
            const auto value = f( key );
            const auto key_iterator = std::prev( m_keys.cend( ) );
            const auto insert_iterator = m_values.insert( {key, {value, key_iterator}} ).first;

            return insert_iterator->second.first;
        }
        else
        {
            // Whenever we access an element it must be moved to the end of the list
            // of keys to be removed
            const auto key_iterator = iterator->second.second;
            m_keys.splice( key_iterator, m_keys, std::next( key_iterator ), m_keys.cend( ) );
            return iterator->second.first;
        }
    }

private:
    using ConstKeyIterator = typename std::list< Key >::const_iterator;

    std::unordered_map< Key, std::pair< Value, ConstKeyIterator > > m_values;
    std::list< Key > m_keys;
    size_t m_max_size;
    Func f;
};
