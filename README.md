
🚀 Getting Started

Requirements
	•	CMake (>= 3.10)
	•	GCC / Clang (or another C compiler)
	•	(Optional) Valgrind for memory checking

Build

mkdir build && cd build
cmake ..
make

Run

After compilation, executables will be in build/ or cmake-build-debug/. Example:

./dessin_relief
./lines
./minimal


⸻

🧪 Testing & Debugging
	•	A valgrind_suppressions.supp file is provided to help with memory leak checking.
	•	Run:

valgrind --suppressions=misc/valgrind_suppressions.supp ./dessin_relief


⸻

📖 Learning Outcomes

This project demonstrates:
	•	How to set up a graphics pipeline in C/C++
	•	Using CMake for modern project management
	•	Integration of external assets (fonts, images)
	•	Debugging with Valgrind and suppression files

