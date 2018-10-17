#include <iostream>
#include <vector>

using namespace std;

struct Value {
    int x;
    Value(int x_): x(x_) { 
        cout << "Value CONstructor" << endl;
    }
    ~Value() {
        cout << "Value DEStructor" << endl;
    }
};

struct Cofactor {
    int x;
    Cofactor(int x_): x(x_) { 
        cout << "Cofactor CONstructor" << endl;
    }
    ~Cofactor() {
        cout << "Cofactor DEStructor" << endl;
    }
};

template <typename REF, typename VALUE>
struct Aggregator {
private:
    Aggregator() { }

public:
    static Aggregator& getInstance() {
        static Aggregator instance;
        return instance;
    }

    Aggregator& operator*(const REF& r) {
        pointers.push_back(&r);
        return *this;
    }

    Aggregator& operator*(const VALUE& v) {
        values.push_back(v);
        return *this;
    }

    Aggregator(const Aggregator&) = delete;
    Aggregator& operator=(const Aggregator&) = delete;

    void reset() {
        pointers.clear();
        values.clear();
    }

    std::vector<const REF*> pointers;
    std::vector<VALUE> values;
};

Aggregator<Cofactor, Value>& operator*(long int a, const Cofactor& r) {
    return Aggregator<Cofactor, Value>::getInstance() * r;
}

Aggregator<Cofactor, Value>& operator*(long int a, const Value& v) {
    return Aggregator<Cofactor, Value>::getInstance() * v;
}

Aggregator<Cofactor, Value>& operator*(const Cofactor& r, long int a) {
    return Aggregator<Cofactor, Value>::getInstance() * r;
}

Aggregator<Cofactor, Value>& operator*(const Cofactor& r1, const Cofactor& r2) {
    return Aggregator<Cofactor, Value>::getInstance() * r1 * r2;
}

Aggregator<Cofactor, Value>& operator*(const Cofactor& r, const Value& v) {
    return Aggregator<Cofactor, Value>::getInstance() * r * v;
}

Aggregator<Cofactor, Value>& operator*(const Value& v, long int a) {
    return Aggregator<Cofactor, Value>::getInstance() * v;
}

Aggregator<Cofactor, Value>& operator*(const Value& v, const Cofactor& r) {
    return Aggregator<Cofactor, Value>::getInstance() * v * r;
}

Aggregator<Cofactor, Value>& operator*(const Value& v1, const Value& v2) {
    return Aggregator<Cofactor, Value>::getInstance() * v1 * v2;
}


Value lift(int x) {
    cout << "LIFT " << x << endl;
    return Value(x - 100);
}

#define VALUE_OR(opt, fallback)          \
    [&](const auto& optional) {          \
        if (optional.has_value())        \
            return optional.value();     \
        return static_cast<T>(fallback); \
    }(opt)

int main(int argc, char** argv) 
{
    // auto a = Value(3).x * Value(42).x

    // Cofactor a1(45);
    // Cofactor a2(3);
    // Cofactor a3(21);

    // // Aggregator<Cofactor, Value>& a = a1 * a3;
        
    // cout << "CAPACITY " << Aggregator<Cofactor, Value>::getInstance().values.capacity() << endl;

    // auto& a = lift(-3) * a1 * lift(1234) * lift(254) * a1 * a2 * a3 * lift(4);

    // cout << "POINTERS" << endl;
    // for (auto i: a.pointers)
    //   std::cout << i->x << ' ';
    // cout << endl;
    // cout << "VALUES" << endl;
    // for (auto i: a.values)
    //   std::cout << i.x << ' ';
    // cout << endl;

    // cout << "CAPACITY " << Aggregator<Cofactor, Value>::getInstance().values.capacity() << endl;

    // cout << "VALUES SINGLETON" << endl;
    // for (auto i: Aggregator<Cofactor, Value>::getInstance().values)
    //   std::cout << i.x << ' ';
    // cout << endl;

    // a.reset();
    // cout << "VALUES SINGLETON AFTER RESET" << endl;
    // for (auto i: Aggregator<Cofactor, Value>::getInstance().values)
    //   std::cout << i.x << ' ';
    // cout << endl;

    // auto& b = lift(23) * a3 * lift(-23) * lift(7) * a1 * a2 * a3 * lift(210);

    // cout << "POINTERS" << endl;
    // for (auto i: b.pointers)
    //   std::cout << i->x << ' ';
    // cout << endl;
    // cout << "VALUES" << endl;
    // for (auto i: b.values)
    //   std::cout << i.x << ' ';
    // cout << endl;

    // cout << "CAPACITY" << Aggregator<Cofactor, Value>::getInstance().values.capacity() << endl;

    return 0;
}