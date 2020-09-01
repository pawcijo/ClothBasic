
# How to build ? :

##
 **Windows MinGW :**

**cd build**

**cmake -DCMAKE_BUILD_TYPE=Release .. -G "MinGW Makefiles"**

**mingw32-make**

##
 **Linuks :**

**cd build**

**For Release:  cmake -DCMAKE_BUILD_TYPE=Release ..**

**For Debug: cmake -DCMAKE_BUILD_TYPE=Debug ..**

**make**

##
  **Windows Visual studio:**

**cd build**

**cmake -G "Visual Studio 15 2017" ..**


### Tutorial

**Press Q and E to move subData cube.<br/>**
**Press M to add force at the last row of the cloth.<br/>**
**Press P to show last row info (need fix)<br/>**
**WSAD and mouse to move around camera.<br/>**


### Used libs and versions :

 * **glew-1.13.0**
 * **glfw-3.2.1** 
 * **glm-0.9.7.1**
 * **tinyxml2 7.0.1**
