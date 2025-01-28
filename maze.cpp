#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>
#include "Grid.h" // Assuming Grid.h is provided and properly configured

using namespace std;

// Recursive helper function to find the path through the maze
bool find_maze_path(Grid& grid, int row, int col, int level, vector<tuple<int, int, int>>& path) {
    // Base cases: Check if the current cell is out of bounds or blocked
    if (row < 0 || row >= grid.height() || col < 0 || col >= grid.width() || level < 0 || level >= grid.depth() || grid.at(row, col, level) == 0) {
        return false;
    }

    // Check if the current cell is the destination (bottom-right-back corner)
    if (row == grid.height() - 1 && col == grid.width() - 1 && level == grid.depth() - 1) {
        path.push_back({row, col, level});
        return true;
    }

    // Check if we've already visited this cell (mark it as visited)
    if (grid.at(row, col, level) == 2) {
        return false;
    }

    // Mark this cell as visited
    grid.at(row, col, level) = 2;
    path.push_back({row, col, level});

    // Explore all adjacent cells (no diagonal moves allowed)
    // Move down
    if (find_maze_path(grid, row + 1, col, level, path)) return true;
    // Move up
    if (find_maze_path(grid, row - 1, col, level, path)) return true;
    // Move right
    if (find_maze_path(grid, row, col + 1, level, path)) return true;
    // Move left
    if (find_maze_path(grid, row, col - 1, level, path)) return true;
    // Move to the next level
    if (find_maze_path(grid, row, col, level + 1, path)) return true;
    // Move to the previous level
    if (find_maze_path(grid, row, col, level - 1, path)) return true;

    // If none of the moves work, backtrack (remove the current cell from the path)
    path.pop_back();
    return false;
}

int main(int argc, char* argv[]) {
    // Check if the correct number of arguments are provided
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input_maze_file> <output_solution_file>" << endl;
        return 1;
    }

    // Open the input file
    ifstream infile(argv[1]);
    if (!infile) {
        cerr << "Error: Unable to open input file " << argv[1] << endl;
        return 1;
    }

    // Read the maze from the input file
    Grid maze;
    infile >> maze;

    // Close the input file
    infile.close();

    // Store the path
    vector<tuple<int, int, int>> path;

    // Call the recursive function to find the path from (0, 0, 0)
    bool solution_found = find_maze_path(maze, 0, 0, 0, path);

    // Open the output file
    ofstream outfile(argv[2]);
    if (!outfile) {
        cerr << "Error: Unable to open output file " << argv[2] << endl;
        return 1;
    }

    // Write the solution or "NO SOLUTION" to the output file
    if (solution_found) {
        outfile << "SOLUTION" << endl;
        for (const auto& coord : path) {
            outfile << get<0>(coord) << " " << get<1>(coord) << " " << get<2>(coord) << endl;
        }
    } else {
        outfile << "NO SOLUTION" << endl;
    }

    // Close the output file
    outfile.close();

    return 0;
}

