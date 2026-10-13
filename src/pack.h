#ifndef PACK_H
#define PACK_H

namespace pack_detail{
// PackIndexToType

    // Primary template - none of the specializations match
    template <int QueryIndex, typename... Args> 
    struct _packIndexToType {
        // Pack is empty - if size is negative returns an error
        static_assert(sizeof...(Args) > 0, "Index is out of range");
    };

    // Base case template - index is 0 theres only one type
    template <typename T, typename... Args>
    struct _packIndexToType<0, T, Args...> {
        using type = T;
    };

    // Recursion case - decrement the index and remove type
    template <int QueryIndex, typename T, typename... Args>
    struct _packIndexToType<QueryIndex, T, Args...> {
        static_assert(QueryIndex > 0, "Index is out of range");  // Index cant be negative
        using type = typename _packIndexToType<QueryIndex -1, Args...>::type;
    };

    template <int QueryIndex, typename... Args>
    using packIndexToType = typename _packIndexToType<QueryIndex, Args...>::type;


    // PackTypeToIndex

    // Primary template - none of the specializations match
    template <int Accumulated, typename QueryType, typename... Args>
    // List is empty - the type was not ffound
    constexpr int _packTypeToIndex = -1;

    // Current type in the list matches
    template <int Accumulated, typename QueryType, typename... Args>
    constexpr int _packTypeToIndex<Accumulated, QueryType, QueryType, Args...> = Accumulated;

    // Current type in list doesnt match, increment accumulated
    template <int Accumulated, typename QueryType, typename T, typename... Args>
    constexpr int _packTypeToIndex<Accumulated, QueryType, T, Args...> = _packTypeToIndex<Accumulated + 1, QueryType, Args...>;

    template <typename QueryType, typename... Args>
    constexpr int packTypeToIndex = _packTypeToIndex<0, QueryType, Args...>;
}

template <typename... Args>
struct Pack {
    constexpr int packSize = sizeof...(Args);

    template <int QueryIndex>
    using type = pack_detail::packIndexToType<QueryIndex, Args...>;

    template <typename QueryType>
    static constexpr int index = pack_detail::packTypeToIndex<QueryType, Args...>;

};

#endif  // PACK_H
