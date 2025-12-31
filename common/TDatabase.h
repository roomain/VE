#pragma once
/***********************************************
* @headerfile TDatabase.h
* @date  / 12 / 2025
* @author Roomain
************************************************/
#include <vector>
#include <unordered_map>
#include "TObjectsComs.h"

template<typename ColumData, typename Data>
struct TColumn
{
    ColumData column;           /*!< data for entire column*/
    std::vector<Data> data;     /*!< column data*/
};

/*@brief represents a database*/
template<typename Key, typename ColumData, typename Data>
class TDatabase : private std::unordered_map<Key, TColumn<ColumData, Data>>
{
public:
    using Super = std::unordered_map<Key, TColumn<ColumData, Data>>;
    using iterator = Super::iterator;
    using const_iterator = Super::const_iterator;

    TSignal<Key> onAdd;      /*!< signal indicate add object in list of key*/  
    TSignal<Key> onRemove;   /*!< signal indicate remove object in list of key*/
    
    // using base class functions
    using Super::begin;
    using Super::end;
    using Super::cbegin;
    using Super::cend;
    using Super::contains;
    using Super::at;


    NOT_COPIABLE(TDatabase)
    TDatabase() = default;
    ~TDatabase() = default;

    void updateColumnData(const Key& a_key, const ColumData& a_colData)
    {
        Super::at(a_key).column = a_colData;
    }

    void updateColumnData(const Key& a_key, ColumData&& a_colData)
    {
        Super::at(a_key).column = a_colData;
    }

    [[nodiscard]] ColumData columnData(const Key& a_key)const
    {
        return Super::at(a_key).column;
    }

    /*@brief Apply function for all object data in list for key*/
    template<typename Function>
    void for_each(const Key& a_key, const Function a_fun)
    {
        for(auto& dataObj : Super::at(a_key).data)
            a_fun(dataObj);
    }

    /*@brief Apply function for all object data in list for key*/
    template<typename KeyFunction>
    void for_all(const KeyFunction a_KeyFun)
    {
        for (auto& [key, value] : *this)
            a_KeyFun(key, value.data);
    }

    /*@brief sort list for key*/
    template<typename SortFunction>
    void sort(const Key& a_key, SortFunction a_sort)
    {
        std::ranges::sort(Super::at(a_key).data, a_sort);
    }
    
    /*@brief add object data in list for key*/
    void add(const Key& a_key,const Data& a_data)
    {
        (*this)[a_key].data.emplace_back(a_data);
        onAdd(a_key);
    }
    
    /*@brief add object data in list for key*/
    //void add(const Key& a_key, Data&& a_data)
    //{
    //    (*this)[a_key].data.emplace_back(a_data);
    //    onAdd(a_key);
    //}

    /*@brief remove data in list for key*/
    //void remove(const Key& a_key,const Data& a_data)
    //{
    //    if(const auto iter = std::ranges::find(Super::at(a_key).data, a_data); iter != Super::at(a_key).data.end())
    //    {
    //        Super::at(a_key).data.erase(iter);
    //        onRemove(a_key);
    //    }
    //}

    void remove(const Key& a_key, const uint32_t a_index)
    {
        Super::at(a_key).data.erase(Super::at(a_key).data.begin() + a_index);
        onRemove(a_key);
    }

    /*@brief remove all data of key*/
    void remove(const Key& a_key)
    {
        Super::at(a_key).data.clear();
        onRemove(a_key);
    }
};