#include <iostream>
#include <string>
#include <vector>

using namespace std;
int main() {
    cout << "Hello, World!" << endl;
    vector<int> numbers = {1, 2, 3, 4, 5};
    for (const auto& num : numbers) {
        cout << num << " ";
    }
    return 0;
}