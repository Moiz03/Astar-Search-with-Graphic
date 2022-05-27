# AstarSearch-Graphic-Representation

## Description:
   A* is a graph traversal and path search algorithm, which is often used in many fields of computer science due to its completeness, optimality, and optimal efficiency. One major practical drawback is its O(b^d) space complexity, as it stores all generated nodes in memory.
   
   This is graphical representaion of A* Search Algorithm

## How to Run:
1. First install SFML in LINUX using command: 
   * `sudo apt-get install libsfml-dev`
2. And then compile like this: 
   * `g++ code.cpp -o code -std=c++11 -lsfml-window -lsfml-system -lsfml-graphics`
   * `./code`

### Colors:
	1. Blue : Source
	2. Green : Destination
	3. Pink : Final Path
	4. Yellow : where it searched 
	5. Black : Blocked / Walls
