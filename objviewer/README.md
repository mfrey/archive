objviewer
=========
objviewer is a file viewer for Wavefront's Advanced Visualizer [object files][1] (*.obj) written in Java. 
The viewer was part of a integrated computer graphics (ICG) course in my first term as a master student 
in the summer of 2009. The viewer uses a [winged edge][2] data structure for the obj files.


Installation from git
---------------------
1. Clone the repository:

  		git clone git://github.com/mfrey/objviewer.git

2. Install the dependencies for your distribution (packages names can differ):

		* jogl
		* java3d

3. 

Usage
-----

The 'z' key activates the zoom mode which scales the vector between the camera and the object. The actual 
zoom is triggered using the left mouse button. The 'r' key allows to rotate the view-up vector between 
camera and the object. Using the 't' key allows to rotate ...

The coordinate system can be enabled/disabled using the 'settings' menu in top area of the application 
window. 



License
-------
tba.


[1]:http://paulbourke.net/dataformats/obj/
[2]:http://en.wikipedia.org/wiki/Winged_edge
