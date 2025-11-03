# NYC Taxi Analyzer (Course Project)
This project compares two spatial indexing methods—**Grid Index** and **KD-Tree**—for speeding up geographic range queries on the **2015 NYC Yellow Taxi** dataset. The program loads a large CSV of trips, builds an index, and lets you run bounding-box searches (minLat, maxLat, minLon, maxLon). It also reports **build time**, **query latency**, and optionally memory usage so you can evaluate trade-offs between the two approaches.

## Installation
**Prerequisites**
- C++17 compiler (clang or g++)
- CMake 3.16+
- Git (optional, for cloning)
- (Optional) CLion IDE

## Data
1. By default, we have 2015 NYC Yellow Taxi CSV trip records from kaggle.

## Usage
### Run the program
```bash
./build/nyc_taxi_analyzer    # if built with CMake
# or
./nyc_taxi_analyzer          # if built with g++
```
When prompted for the dataset path, enter:
data/nyc_taxi.csv

### Typical Workflow
1. Load dataset in the back.
2. Build index for Grid and KD-Tree
3. Search → input bounding box (minLat maxLat minLon maxLon) and print out if user enters choice 1.
4. Compare performance of Grid and KD-Tree if user enters choice 2.
5. Exit the program if user enters choice 3.


## Contributors to this project
- Xinyu Shen
- Wanchang Xiong
- Dongyuan Li
