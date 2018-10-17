#include "types.hpp"
#include "serialization.hpp"

using namespace dbtoaster;

struct RingAvg{
    int count;
    DOUBLE_TYPE sum;

    static RingAvg zero;

    RingAvg(int c, DOUBLE_TYPE s): count(c), sum(s){
    }

    RingAvg(): count(0), sum(0.0){}

    RingAvg& operator += (const RingAvg &r){
        this->count += r.count;
        this->sum += r.sum;
        return *this;
    }

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) const {
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, count);
        ar << ELEM_SEPARATOR;
        DBT_SERIALIZATION_NVP(ar, sum);
    }    
};

RingAvg RingAvg::zero = RingAvg(0, 0.0);

RingAvg operator * (const RingAvg &l, const RingAvg &r){
    return RingAvg(l.count * r.count, l.sum * r.count + r.sum * l.count);
}

RingAvg operator * (int alpha, const RingAvg &r){
    return RingAvg(alpha * r.count, alpha * r.sum);
}

RingAvg Ulift(DOUBLE_TYPE a){
    return RingAvg(1, a);
}

