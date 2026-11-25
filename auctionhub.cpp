#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

struct Bid {
    string bidderId;
    double amount;
    long long timestamp;
    int auctionId;
};

void printBids(const vector<Bid>& v, size_t limit = 10) {
    for (size_t i = 0; i < min(v.size(), limit); ++i)
        cout << "[" << v[i].bidderId << "] $" << v[i].amount
             << " @ " << v[i].timestamp << "\n";
    if (v.size() > limit) cout << "... (" << v.size() << " total)\n";
}

bool loadBids(const string& path, vector<Bid>& out) {
    ifstream f(path);
    if (!f) return false;
    Bid b;
    while (f >> b.bidderId >> b.amount >> b.timestamp >> b.auctionId)
        out.push_back(b);
    return true;
}

bool isLess(const Bid& a, const Bid& b) {
    if (a.amount != b.amount) return a.amount < b.amount;
    return a.timestamp < b.timestamp;
}

void insertionSort(vector<Bid>& v) {
    for (size_t i = 1; i < v.size(); i++) {
        Bid key = v[i];
        int j = i - 1;
        while (j >= 0 && isLess(key, v[j])) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = key;
    }
}

void selectionSort(vector<Bid>& v) {
    for (size_t i = 0; i < v.size() - 1; i++) {
        size_t maxIdx = i;
        for (size_t j = i + 1; j < v.size(); j++) {
            if (v[j].amount > v[maxIdx].amount) {
                maxIdx = j;
            }
        }
        if (maxIdx != i) {
            swap(v[i], v[maxIdx]);
        }
    }
    cout << "--- Top 3 Bids ---\n";
    printBids(v, 3);
}

void interchangeSort(vector<Bid>& v) {
    long long swapCount = 0;
    for (size_t i = 0; i < v.size() - 1; i++) {
        for (size_t j = i + 1; j < v.size(); j++) {
            if (isLess(v[j], v[i])) {
                swap(v[i], v[j]);
                swapCount++;
            }
        }
    }
    cout << "Interchange Sort total swaps: " << swapCount << "\n";
}

bool bubbleSortEarlyStop(vector<Bid>& v) {
    bool firstPassNoSwap = true;
    for (size_t i = 0; i < v.size() - 1; i++) {
        bool swapped = false;
        for (size_t j = 0; j < v.size() - 1 - i; j++) {
            if (isLess(v[j + 1], v[j])) {
                swap(v[j], v[j + 1]);
                swapped = true;
            }
        }
        if (i == 0 && swapped) {
            firstPassNoSwap = false;
        }
        if (!swapped) break;
    }
    
    if (firstPassNoSwap) cout << "Stream was: STABLE\n";
    else cout << "Stream was: UNSTABLE\n";
    
    return firstPassNoSwap;
}

void quickSort(vector<Bid>& v, int lo, int hi) {
    if (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (isLess(v[mid], v[lo])) swap(v[lo], v[mid]);
        if (isLess(v[hi], v[lo])) swap(v[lo], v[hi]);
        if (isLess(v[hi], v[mid])) swap(v[mid], v[hi]);
        
        swap(v[mid], v[hi]);
        Bid pivot = v[hi];
        
        int i = lo - 1;
        for (int j = lo; j < hi; j++) {
            if (isLess(v[j], pivot) || (v[j].amount == pivot.amount && v[j].timestamp == pivot.timestamp)) {
                i++;
                swap(v[i], v[j]);
            }
        }
        swap(v[i + 1], v[hi]);
        int pi = i + 1;
        
        quickSort(v, lo, pi - 1);
        quickSort(v, pi + 1, hi);
    }
}

void quickSortWrapper(vector<Bid>& v) {
    if (v.empty()) return;
    auto start = high_resolution_clock::now();
    quickSort(v, 0, v.size() - 1);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Quick Sort took " << duration.count() << " ms.\n";
}

int main(int argc, char* argv[]) {
    vector<Bid> testBids = {
        {"alice", 100.0, 1000, 1},
        {"bob", 102.5, 1005, 1},
        {"carol", 101.0, 1003, 1},
        {"dave", 103.0, 1010, 1}
    };

    cout << "\n=== TASK A: Insertion Sort ===\n";
    vector<Bid> listA = testBids;
    insertionSort(listA);
    printBids(listA);

    cout << "\n=== TASK B: Selection Sort ===\n";
    vector<Bid> listB = testBids;
    selectionSort(listB);

    cout << "\n=== TASK C: Interchange Sort ===\n";
    vector<Bid> listC = testBids;
    interchangeSort(listC);

    cout << "\n=== TASK D: Bubble Sort (Early Stop) ===\n";
    vector<Bid> listD1 = testBids;
    cout << "Testing Unsorted Array: ";
    bubbleSortEarlyStop(listD1);
    
    vector<Bid> listD2 = listA;
    cout << "Testing Sorted Array: ";
    bubbleSortEarlyStop(listD2);

    cout << "\n=== TASK E: Quick Sort (Large Data) ===\n";
    vector<Bid> largeBids;
    mt19937 gen(42);
    uniform_real_distribution<> dis(10.0, 1000.0);
    for (int i = 0; i < 100000; i++) {
        largeBids.push_back({"user" + to_string(i), dis(gen), i * 10LL, 1});
    }
    quickSortWrapper(largeBids);

    return 0;
}