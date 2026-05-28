# Aerofoil
Generation of NACA4 Aerofoils.
<img width="800" height="500" alt="scr" src="https://github.com/user-attachments/assets/800d56b8-6840-4e4d-a992-1ce885fa6477" />
<img width="800" height="500" alt="scr" src="https://github.com/user-attachments/assets/435d4f52-6b9c-4585-80f5-57fd46e1910d" />
## Description
Using the formulas given airfoiltools (http://airfoiltools.com/airfoil/naca4digit), this program generates a set of points corresponding to that shape. 
## Compilation
Only tested on linux. Create a build folder and compile using CMake. As there are no sub modules at the moment, you will need the following packages already installed for compilation,
* glm
* SDL3
* SDL3 image
* SDL3 ttf
