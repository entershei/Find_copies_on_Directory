Find_copies_on_Directory 
-------------------------------------------------------

Concurrent program that finds file duplicates in the given directory.
Written in C++ and uses QT.


 ### Run
 `cmake . && make Find_copies && ./Find_copies`

### Files descriptions

Sources:
 - `main.cpp`
 - `mainwindow.h`, `mainwindow.cpp` — main window of application.
 - `group_identical_files.h`, `group_identical_files.cpp` — find duplicates with the same content.
	  
 
Other files:
 - `mainwindow.ui` — XML-file with the window descriptions. 
 - `CMakeLis.txt`
 - `dirdemo.pro` — for qmake. 

