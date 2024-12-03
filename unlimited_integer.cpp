#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class unlimited_integer {
private:
    string s;
    vector<long long> lli1;
    vector<long long> lli2;

    static const long long BASE = 1000000000000000000LL; // Base for chunking (18 digits)

public:
    unlimited_integer() : s("") {}

    unlimited_integer(string s) : s(s) {}

    string leadingzero(string s) {
        int i = 0;
        while (i < s.size() && s[i] == '0') {
            i++;
        }
        return i == s.size() ? "0" : s.substr(i);
    }

    string fixsize(long long x) {
        string s = to_string(x);
        while (s.size() < 18) {
            s = "0" + s;
        }
        return s;
    }

    void chunking(const unlimited_integer& num1, const unlimited_integer& num2) {
        lli1.clear();
        lli2.clear();

        int sz1 = num1.s.size();
        int sz2 = num2.s.size();

        for (int i = sz1 - 18; i >= -17; i -= 18) {
            lli1.push_back(stoll(num1.s.substr(max(0, i), min(18, sz1 - max(0, i)))));
        }
        for (int j = sz2 - 18; j >= -17; j -= 18) {
            lli2.push_back(stoll(num2.s.substr(max(0, j), min(18, sz2 - max(0, j)))));
        }

        reverse(lli1.begin(), lli1.end());
        reverse(lli2.begin(), lli2.end());
    }

    string addOp(unlimited_integer num1, unlimited_integer num2) {
        chunking(num1, num2);

        vector<long long> result;
        int carry = 0;

        int l1 = lli1.size() - 1, l2 = lli2.size() - 1;
        while (l1 >= 0 || l2 >= 0 || carry > 0) {
            long long sum = (l1 >= 0 ? lli1[l1--] : 0) +
                            (l2 >= 0 ? lli2[l2--] : 0) +
                            carry;

            carry = sum / BASE;
            result.push_back(sum % BASE);
        }

        reverse(result.begin(), result.end());
        string output;
        for (size_t i = 0; i < result.size(); ++i) {
            output += (i == 0 ? to_string(result[i]) : fixsize(result[i]));
        }
        return leadingzero(output);
    }

    string subOp(unlimited_integer num1, unlimited_integer num2) {
        chunking(num1, num2);

        // Determine if the result is negative
        bool isNegative = false;
        if (num1.s.size() < num2.s.size() || (num1.s.size() == num2.s.size() && num1.s < num2.s)) {
            swap(lli1, lli2);
            isNegative = true;
        }

        vector<long long> result;
        int borrow = 0;

        for (int i = lli1.size() - 1, j = lli2.size() - 1; i >= 0 || j >= 0; i--, j--) {
            long long val1 = (i >= 0 ? lli1[i] : 0);
            long long val2 = (j >= 0 ? lli2[j] : 0) + borrow;

            if (val1 < val2) {
                val1 += BASE;
                borrow = 1;
            } else {
                borrow = 0;
            }

            result.push_back(val1 - val2);
        }

        reverse(result.begin(), result.end());
        string output;
        for (size_t i = 0; i < result.size(); ++i) {
            output += (i == 0 ? to_string(result[i]) : fixsize(result[i]));
        }
        output = leadingzero(output);

        return isNegative ? "-" + output : output;
    }

    unlimited_integer& operator=(string s1) {
        this->s = leadingzero(s1);
        return *this;
    }

    unlimited_integer operator+(unlimited_integer num) {
        unlimited_integer res;
        res.s = addOp(*this, num);
        return res;
    }

    unlimited_integer operator-(unlimited_integer num) {
        unlimited_integer res;
        res.s = subOp(*this, num);
        return res;
    }

    friend ostream& operator<<(ostream& os, const unlimited_integer& unInt);
    friend istream& operator>>(istream& is, unlimited_integer& unInt);
};

ostream& operator<<(ostream& os, const unlimited_integer& unInt) {
    os << unInt.s;
    return os;
}

istream& operator>>(istream& is, unlimited_integer& unInt) {
    is >> unInt.s;
    unInt.s = unInt.leadingzero(unInt.s);
    return is;
}

int main() {
    unlimited_integer num1, num2;
    cout << "Enter two numbers: " << endl;
    cin >> num1 >> num2;

    unlimited_integer ans;
    ans = num1 - num2;
    cout << "The difference is: " << ans << endl;

    ans = num1 + num2;
    cout << "The sum is: " << ans << endl;

    return 0;
}
