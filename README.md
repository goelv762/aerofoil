# Aerofoil
Rendering of NACA4 Aerofoils (future plans of using vortex / source panel method for simulations).
<img width="800" height="500" alt="scr" src="https://github.com/user-attachments/assets/f308ba11-2895-45d8-bf94-0bf7526e728a" />


## Description
Using the formulas given airfoiltools (http://airfoiltools.com/airfoil/naca4digit), this program generates a set of points corresponding to that shape. 
## Compilation
Only tested on linux. Create a build folder and compile using CMake. As there are no sub modules at the moment, you will need the following packages already installed for compilation,
* glm
* SDL3
* SDL3 image
* SDL3 ttf
