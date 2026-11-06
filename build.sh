mkdir -p dist
g++ -std=c++17 -Wall -Wextra -pedantic -pthread -fopenmp -c src/main.cpp -o dist/main.o