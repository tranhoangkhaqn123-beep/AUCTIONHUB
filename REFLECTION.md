## 1. Real-World Justification for Algorithms
*   **Insertion Sort (Live Auction Monitor):** Insertion Sort reaches best-case O(n) performance when data is nearly sorted. Since new bids typically arrive slightly higher than previous ones, this algorithm efficiently processes the stream with minimal shifting operations.
*   **Selection Sort (Top-K Bid Finder):** We only care about the largest k bids. Selection sort is ideal because it finds the absolute maximum element in the remaining array on each pass.
*   **Interchange Sort (Price Anomaly Validator):** This QA module strictly limits datasets to small batches. For minimal payload sizes, an unoptimized O(n^2) algorithm like Interchange Sort processes practically instantaneously while maintaining a simple codebase.
*   **Bubble Sort with Early Stop (Stabilization Detector):** This version incorporates a flag that terminates execution if no swaps occur during a full pass. Therefore, it immediately verifies perfectly stabilized price streams in O(n) time.
*   **Quick Sort (Full Historical Sorter):** Historical reports involve millions of records where O(n^2) approaches would take unacceptably long. Quick Sort offers an average case of O(n log n), making it essential for processing bulk big-data efficiently.

## 2. Quick Sort Measured Runtime
*   **Data Size:** 100,000 randomly generated bids.
*   **Measured Runtime:** ~130ms (varies slightly by machine).

## 3. Bug Diagnosis
*   **The Bug:** During Quick Sort implementation, I encountered a Stack Overflow error when testing inputs with multiple identical bids.
*   **Diagnosis & Fix:** The partitioning logic lacked tie-breaking fallback. I verified my conditional logic inside the for loop and added equality checks to correctly move the boundaries i, resolving infinite recursion issues on duplicate elements.

## 4. LLM Prompts and Ratings
1.  "Give me 5 edge-case test inputs I should consider when testing a sorting function on structs with floating-point keys." — (Green Zone)
2.  "Explain how the median-of-three strategy works to prevent worst-case scenarios in Quicksort." — (Green Zone)
3.  "How do I write a custom swap function for my struct, or can I just use std::swap?" — (Yellow Zone)

## 5. Worst-Case Complexity of Median-of-Three Quick Sort
The worst-case complexity remains O(n^2) theoretically. While the median-of-three strategy excellently defends against arrays that are already sorted, a specifically crafted adversarial input could continually force the "median" choice to actually be the second-largest or second-smallest element, degrading the overall time to O(n^2).