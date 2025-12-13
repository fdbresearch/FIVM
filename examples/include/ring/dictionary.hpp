#ifndef DBTOASTER_DICTIONARY_HPP
#define DBTOASTER_DICTIONARY_HPP

#include <vector>

struct Tuple {
    union {
        uint64_t key;
        uint32_t slots[2]; /* composite key can consist of two 4B keys */
    };
    double value;
};
static_assert(sizeof(Tuple) == 16, "size of Tuple not 16 bytes");

struct Dictionary {
    std::vector<Tuple> tuples;

    Dictionary() { }

    explicit Dictionary(size_t n) {
        tuples.reserve(n);
    }

    explicit Dictionary(Tuple t) : tuples { t } { }

    inline bool isEmpty() const { return tuples.empty(); }

    Dictionary& operator+=(const Dictionary& other) {
        for (size_t i = 0; i < other.tuples.size(); i++) {
            bool found = false;
            for (size_t j = 0; !found && j < tuples.size(); j++) {
                if (tuples[j].key == other.tuples[i].key) {
                    tuples[j].value += other.tuples[i].value;
                    found = true;
                }
            }
            if (!found) {
                tuples.push_back(other.tuples[i]);
            }
        }
        return *this;
    }

    Dictionary operator*(const Dictionary& other) const {
        if (other.isEmpty()) return Dictionary();
        return multiply(other);
    }

    Dictionary multiply(const Dictionary& other) const {
        Dictionary r(tuples.size() * other.tuples.size());
        for (size_t i = 0; i < tuples.size(); i++) {
            for (size_t j = 0; j < other.tuples.size(); j++) {
                Tuple t = {
                    .slots = { tuples[i].slots[0], other.tuples[j].slots[0] },
                    .value = tuples[i].value * other.tuples[j].value
                };
                r.tuples.push_back(t);
            }
        }
        return r;
    }

    Dictionary operator*(double alpha) const {
        if (alpha == 0.0) return Dictionary();
        return multiply(alpha);
    }

    Dictionary multiply(double alpha) const {
        Dictionary r(tuples.size());
        for (size_t i = 0; i < tuples.size(); i++) {
            Tuple t = {
                .key = tuples[i].key,
                .value = tuples[i].value * alpha
            };
            r.tuples.push_back(t);
        }
        return r;
    }

    template <typename Archive>
    void serialize(Archive & ar, const unsigned int version) const {
        ar << "{ ";
        for (auto &t : tuples) {
            ar << t.key << " -> " << t.value << ", ";
        }
        ar << " }";
    }

};

#endif /* DBTOASTER_DICTIONARY_HPP */
