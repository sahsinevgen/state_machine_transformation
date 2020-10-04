#ifndef My_bitset
#define My_bitset

class my_bitset {

public:
    int n;
    int ar_sz;
    unsigned int* ar;
    const int sz = 32;

    my_bitset() {
        n = -1;
        ar = nullptr;
    }

    my_bitset(int n_) {
        n = n_;
        ar_sz = (n / sz) + bool(n % sz);
        ar = new unsigned int[ar_sz];
        for (int i = 0; i < ar_sz; i++) {
            ar[i] = 0;
        }
    }

    my_bitset(const my_bitset &a) {
        n = a.n;
        ar_sz = a.ar_sz;
        ar = new unsigned int[ar_sz];
        for (int i = 0; i < ar_sz; i++) {
            ar[i] = a.ar[i];
        }
    }

    ~my_bitset() {
        delete [] ar;
    }

    my_bitset& operator= (const my_bitset& a) {
        n = a.n;
        ar_sz = a.ar_sz;
        ar = new unsigned int[ar_sz];
        for (int i = 0; i < ar_sz; i++) {
            ar[i] = a.ar[i];
        }
    }

    bool operator<(const my_bitset &a) const {
        for (int i = 0; i < ar_sz; i++) {
            if (ar[i] != a.ar[i]) {
                return ar[i] < a.ar[i];
            }
        }
        return false;
    }

    bool operator==(const my_bitset &a) const {
        for (int i = 0; i < ar_sz; i++) {
            if (ar[i] != a.ar[i]) {
                return false;
            }
        }
        return true;
    }

    my_bitset& operator|=(const my_bitset &a) {
        for (int i = 0; i < ar_sz; i++) {
            ar[i] |= a.ar[i];
        }
        return (*this);
    }

    my_bitset operator|(const my_bitset &a) const {
        my_bitset ans(*this);
        ans |= a;
        return ans;
    }

    void set(int i, int x) {
        int i1, i2;
        parse(i, i1, i2);
        if (x == 1) {
            ar[i1] |= (1 << i2);
        } else {
            ar[i1] |= (1 << i2);
            ar[i1] ^= (1 << i2);
        }
    }

    bool get(int i) const {
        int i1, i2;
        parse(i, i1, i2);
        return (ar[i1] & (1 << i2));
    }

    /*void print() const{
        for (int i = 0; i < n; i++) {
            std::cout << get(i);
        }
        std::cout << std::endl;

        for (int i = 0; i < ar_sz; i++) {
            std::cout << ar[i] << " ";
        }
        std::cout << std::endl;
    }*/

    int next_true(int i) const {
        int i1, i2;
        parse(i, i1, i2);
        i2++;
        while (i2 != sz) {
            if (get((i1 * sz) + i2))
                return (i1 * sz) + i2;
            i2++;
        }
        i1++;
        while (i1 < ar_sz && ar[i1] == 0) {
            i1++;
        }
        if (i1 == ar_sz) {
            return -1;
        }
        i2 = 0;
        while (1) {
            if (get((i1 * sz) + i2))
                return (i1 * sz) + i2;
            i2++;
        }
    }

    void parse(int i, int &i1, int &i2) const {
        i1 = i / sz;
        i2 = i % sz;
    }
};

#endif //My_bitset