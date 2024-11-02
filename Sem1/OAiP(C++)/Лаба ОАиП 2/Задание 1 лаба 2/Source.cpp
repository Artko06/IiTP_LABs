#include <iostream>
using namespace std;
int main() {
    long long x, y;
    cout << "Print x: ";
    cin >> x;
    cout << "Ptint y: ";
    cin >> y;
    if (x == 0 && y != 0) {
        if (x % y == 0) {
            cout << "YES";
        }
        else if (x % y != 0) {
            cout << "NO";
        }
    }
    if (x != 0 && y == 0) {
        if (y % x == 0) {
            cout << "YES";
        }
        else if (y % x != 0) {
            cout << "NO";
        }
    }
    if (x != 0 && y != 0) {
        if (x % y == 0 || y % x == 0) {
            cout << "YES";
        }
        else if (x % y != 0 || y % x != 0) {
            cout << "NO";

        }
    }
    if (x == 0 && y == 0) {
            cout << "ERROR";
    }
return 0;
}