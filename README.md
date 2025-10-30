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
2. You can insert any CSV data with similar columns into 'data' folder or press 1 in menu. 

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
1. Load dataset → enter data/nyc_taxi.csv
2. Build index → choose Grid or KD-Tree
3. Search → input bounding box (minLat maxLat minLon maxLon)
4. (Optional) Compare performance


## Contributors to this project
- Xinyu Shen
- Wanchang Xiong
- Dongyuan Li
