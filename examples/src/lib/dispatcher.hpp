#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include <vector>
#include <functional>
#include <memory>
#include <iterator>
#include <initializer_list>

class Dispatcher {
  public:
    virtual ~Dispatcher() { }
    virtual bool has_next() const = 0;
    virtual void next() = 0;
};

template <typename T>
class EventDispatcher : public Dispatcher {
  private:
    typedef typename std::vector<T>::iterator Iterator;
    typedef std::function<void(T&)> Func;

    Iterator it;
    const Iterator end;
    const Func& func;

  public:
    EventDispatcher(std::vector<T>& v, const Func& f) : it(v.begin()), end(v.end()), func(f) { }

    EventDispatcher(std::vector<T>& v, Func&& f) = delete;

    inline bool has_next() const { return it != end; }

    inline void next() { func(*it++); }
};

template <typename T, size_t BATCH_SZ>
class BatchDispatcher : public Dispatcher {
  private:
    typedef typename std::vector<T>::iterator Iterator;
    typedef std::function<void(const Iterator&, const Iterator&)> Func;

    Iterator it;
    const Iterator end;
    const Func& func;

  public:
    BatchDispatcher(std::vector<T>& v, const Func& f) : it(v.begin()), end(v.end()), func(f) { }

    BatchDispatcher(std::vector<T>& v, Func&& f) = delete;

    inline bool has_next() const { return it != end; }

    inline void next() {
        size_t dist = std::distance(it, end);
        size_t batchSz = (BATCH_SZ < dist ? BATCH_SZ : dist);
        func(it, it + batchSz); 
        it += batchSz;
    }
};

class Multiplexer : public Dispatcher {
  private:
    std::vector<std::unique_ptr<Dispatcher>> v;
    size_t idx, active;
 
  public:
    Multiplexer(): idx(0), active(0) { }

    Multiplexer(std::initializer_list<Dispatcher*> dispatchers) : idx(0), active(0) {
        for (auto d : dispatchers) {
            add_dispatcher(d);
        }
    }

    inline bool has_next() const { return active > 0; }

    inline void next() {
        v[idx]->next();

        if (v[idx]->has_next()) {
            idx++;
        }
        else {
            active--;
            for (size_t i = idx; i < active; i++) {
                std::swap(v[i], v[i + 1]);
            }
        }

        if (idx >= active) { idx = 0; }
    }

    void add_dispatcher(Dispatcher* d) {
        if (d->has_next()) {
            v.push_back(std::unique_ptr<Dispatcher>(d));
            active++;
        }
    }

    void clear() {
        v.clear();
        idx = 0;
        active = 0;
    }
};

#endif /* DISPATCHER_HPP */