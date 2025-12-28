# 🚀 Dynamic Traffic Navigation System

**A high-performance route optimization engine built in C++ using Dijkstra's Algorithm and a real-time dynamic graph update mechanism.**

---

## 📌 Project Overview
In urban environments, static shortest-path calculations are insufficient due to unpredictable traffic congestion and road accidents. This project implements a **weighted, dynamic graph-based navigation system** that recalibrates the optimal route in real-time based on varying traffic intensities and road blockages.



## 🔑 Key Features
* **Dynamic Pathfinding:** Real-time route recalculation using a **Min-Priority Queue** optimized Dijkstra’s Algorithm.
* **Traffic Intensity Modeling:** Roads feature weight multipliers for **Clear (1.0x), Moderate (1.8x), and Heavy (3.5x)** traffic.
* **Interactive TUI:** A responsive Terminal User Interface with color-coded live monitoring and an ASCII spatial map.
* **Platform Optimized:** Specifically engineered for Windows-native execution using `windows.h`.

---

## 🛠 Technical Architecture

### 1. Data Structures
* **Adjacency List:** Memory-efficient representation of the city road network.
* **Priority Queue (Min-Heap):** Used to achieve $O(E \log V)$ time complexity.
* **Custom Edge Struct:** Stores base distance and traffic status to calculate "Effective Weight" on-the-fly.

### 2. The Weight Function
The "cost" of a road is calculated dynamically:
$$Cost = Distance \times TrafficMultiplier$$



---

## 📊 Complexity Analysis

| Factor | Complexity |
| :--- | :--- |
| **Time Complexity** | $O((V + E) \log V)$ |
| **Space Complexity** | $O(V + E)$ |
| **Search Strategy** | Greedy / Informed search via Priority Queue |

---

## 🚀 Getting Started

### Prerequisites
* A C++ compiler (MinGW/GCC recommended)
* Windows OS (for `windows.h` support)

### Installation & Execution
1.  **Compile the source:**
    ```bash
    g++ traffic.cpp -o traffic
    ```
2.  **Run the system:**
    ```bash
    ./traffic
    ```

---

## 🖥 User Interface Preview
The system features a live spatial map and color-coded status updates:
* **Green:** Optimal/Clear Path
* **Yellow/Red:** Congested Routes
* **Cyan:** Current Active Navigation
