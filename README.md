# Aerofoil
Rendering of NACA4 Aerofoils with source panel method for rendering stream lines.
<img width="800" height="500" alt="scr" src="https://github.com/user-attachments/assets/cf22a157-1312-472c-9c66-2ea3524e6edd" />


## Description
Using the formulas given airfoiltools (http://airfoiltools.com/airfoil/naca4digit), this program generates a set of points corresponding to that shape. Using the source panel method, we can use these descrete points to simulate how wind would flow around the aerofoil. By querying the velocity vector field and using euler integration, we can map out the path of these stream lines.
## Compilation
Only tested on linux. Create a build folder and compile using CMake. As there are no sub modules at the moment, you will need the following packages already installed for compilation,
* glm
* SDL3
* SDL3 image
* SDL3 ttf
