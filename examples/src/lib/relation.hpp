#ifndef RELATION_HPP
#define RELATION_HPP

#include <string>
#include <vector>
#include <functional>
#include "dispatcher.hpp"
#include "csvreader.hpp"

class IRelation {
  protected:
    string name;
    string path;
    char delimiter;
    bool static_table;

  public:
    IRelation(string _name, string _path, char _delimiter, bool _static_table) 
        : name(_name), path(_path), delimiter(_delimiter), static_table(_static_table) { }

    virtual ~IRelation() { }

    string get_name() { return name; }
    
    bool is_static() { return static_table; }
    
    virtual size_t size() = 0;
    
    virtual void load() = 0;
    
    virtual void clear() = 0;

    virtual Dispatcher* create_dispatcher(dbtoaster::data_t& data) = 0;
};

template <typename T>
class Relation : public IRelation {
  protected:
    std::vector<T> tuples;

  public:
    Relation(string name, string path, char delimiter, bool static_table)
        : IRelation(name, path, delimiter, static_table) { }

    size_t size() { return tuples.size(); }

    void load() { readFromFile(tuples, path, delimiter); }

    void clear() { tuples.clear(); }
};

template <typename T>
class EventDispatchableRelation : public Relation<T> {
  protected:
    typedef std::function<void(T&)> Func;
    typedef std::function<Func(dbtoaster::data_t&)> DataFunc;
    
    DataFunc data_func;
    Func func;

  public: 
    EventDispatchableRelation(string name, string path, char delimiter, bool static_table, DataFunc f)
        : Relation<T>(name, path, delimiter, static_table), data_func(f), func(nullptr) { }

    Dispatcher* create_dispatcher(dbtoaster::data_t& data) {
        func = data_func(data);
        return new EventDispatcher<T>(this->tuples, func);
    }
};

#ifdef BATCH_SIZE

template <typename T>
class BatchDispatchableRelation : public Relation<T> {
  protected:
    typedef typename std::vector<T>::iterator Iterator;
    typedef std::function<void(const Iterator&, const Iterator&)> Func;
    typedef std::function<Func(dbtoaster::data_t&)> DataFunc;

    DataFunc data_func;
    Func func;

  public: 
    BatchDispatchableRelation(string name, string path, char delimiter, bool static_table, DataFunc f)
        : Relation<T>(name, path, delimiter, static_table), data_func(f), func(nullptr) { }

    Dispatcher* create_dispatcher(dbtoaster::data_t& data) {
        func = data_func(data);
        return new BatchDispatcher<T, BATCH_SIZE>(this->tuples, func);
    }
};

#endif

#endif /* RELATION_HPP */