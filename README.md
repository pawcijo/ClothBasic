
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

### Used libs and versions :

 * **glew-1.13.0**
 * **glfw-3.2.1** 
 * **glm-0.9.7.1**
 * **tinyxml2 7.0.1**
