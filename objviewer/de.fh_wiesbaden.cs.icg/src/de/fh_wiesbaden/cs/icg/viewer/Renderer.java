package de.fh_wiesbaden.cs.icg.viewer;

import java.awt.Dimension;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.io.File;
import java.nio.IntBuffer;
import java.util.ArrayList;

import javax.media.opengl.GL;
import javax.media.opengl.GLAutoDrawable;
import javax.media.opengl.GLEventListener;
import javax.media.opengl.glu.GLU;
import javax.vecmath.Point2f;

import com.sun.opengl.util.BufferUtil;

import de.fh_wiesbaden.cs.icg.renderable.CoordinateSystem;
import de.fh_wiesbaden.cs.icg.renderable.Renderable;
import de.fh_wiesbaden.cs.icg.renderable.Volume;

/**
 * 
 * 
 * @author Michael Frey
 */
public class Renderer implements GLEventListener, KeyListener, MouseListener,
		MouseMotionListener {
	/**
	 * A status flag if a coordinate system should be drawn or not
	 */
	private boolean showCoordinateSystem = true;
	/**
	 * A status flag if the z buffer should be active or not
	 */
	private boolean buffer = true;
	/**
	 * A variable for the wire frame render mode
	 */
	public static final int RENDER_MODE_WIRE_FRAME = 0;
	/**
	 * A variable for the flat shading render mode
	 */
	public static final int RENDER_MODE_FLAT = 1;
	/**
	 * A variable for the gouraud shading render mode
	 */
	public static final int RENDER_MODE_GOURAUD = 2;
	/**
	 * A variable for the wire frame on gouraud shading render mode
	 */
	public static final int RENDER_MODE_WIRE_FRAME_ON_GOURAUD = 3;
	/**
	 * A variable for the zoom mode
	 */
	public static final int VIEW_MODE_ZOOM = 4;
	/**
	 * A variable for the roll mode
	 */
	public static final int VIEW_MODE_ROLL = 5;
	/**
	 * A variable for the rotate mode
	 */
	public static final int VIEW_MODE_ROTATE = 6;
	/**
	 * A variable for the selection mode
	 */
	public static final int VIEW_MODE_SELECTION = 7;
	/**
	 * A variable which indicates the current render mode
	 */
	private int currentRenderMode = 0;
	/**
	 * A variable which indicates the current view mode
	 */
	private int currentViewMode = 6;
	/**
	 * A camera
	 */
	private Camera camera = new Camera();
	/**
	 *  The start point of the mouse movement
	 */
	private Point2f start = new Point2f();
	/**
	 * The end point of the mouse movement 
	 */
	private Point2f end = new Point2f();
	/**
	 *  The clicked button of the mouse movement
	 */
	private int clickedButton;
	/**
	 * A first light source
	 */
	private Light one = new Light(GL.GL_LIGHT0);
	/**
	 * A second light source
	 */
	private Light two = new Light(GL.GL_LIGHT1);
	/**
	 * The dimensions of the window 
	 */
	private Dimension resolution = new Dimension(800, 600);
	/**
	 * The selection buffer size 
	 */
	private int selectionBufferSize = 512;
	/**
	 * The selection volume object 
	 */
	private Volume selectionVolume;
	/**
	 * A list of renderable objects
	 */
	private ArrayList<Renderable> renderableObjects = new ArrayList<Renderable>();
	/**
	 * A variable which indicates if the ctrl key is pressed
	 */
	private boolean ctrlPressed = false;
	/**
	 * A variable which indicates if the alt key is pressed
	 */
	private boolean altPressed = false;
	/**
	 * A coordinate system for orientation
	 */
	private CoordinateSystem coordinateSystem = new CoordinateSystem();
	/**
	 * A variable which indicates if picking is on
	 */
	private boolean picking;
	/**
	 * A variable which indicates if picking is on
	 */
	private boolean umbrellaPressed;
	/**
	 * A variable which indicates if picking is on
	 */
	private Shader shader = new Shader();
	/**
	 * A variable which indicates if a shader should be rendered
	 */
	private boolean renderShader;
	/**
	 * A variable which represents the lambda factor of the umbrella operator
	 */
	private float lambda = 1.0f;
		
	/**
	 * The constructor of the class initializes the selection volume
	 * and the light of the scenary 
	 */
	public Renderer(Dimension dimension){
		// Save the resolution of the application
		this.resolution = dimension;
		// Create a new selection volume
		this.selectionVolume = new Volume(-10.0f,10.0f, -5.0f,5.0f, 0.0f,10.0f);
/*
		one.getPosition().set(0.0f, 15.0f, 0.0f);
		one.getDirection().set(0.0f, -1.0f, 0.0f);
		one.setCutOffAngle(10.0f);
		one.getAmbient().set(0.2f, 0.2f, 0.2f, 0.8f);
		one.getDiffuse().set(1.0f, 1.0f, 0.0f, 0.8f);
		one.getSpecular().set(0.1f, 0.1f, 0.1f, 1.0f);
		one.setConstantAttenuation(0.8f);
		one.setLinearAttenuation(0.8f);
		one.setQuadraticAttenuation(0.8f);
		one.setSpotLightExponent(128f);
		
		two.getPosition().set(0.0f, -15.0f, 0.0f);
		two.getDirection().set(0.0f, 1.0f, 0.0f);
		two.setCutOffAngle(5.0f);
		two.getAmbient().set(0.1f, 0.1f, 0.1f, 0.8f);
		two.getDiffuse().set(0.0f, 1.0f, 1.0f, 0.8f);
		two.getSpecular().set(1.0f, 1.0f, 1.0f, 1.0f);
		two.setConstantAttenuation(1.0f);
		two.setLinearAttenuation(1.0f);
		two.setQuadraticAttenuation(1.0f);
		two.setSpotLightExponent(128f);
		*/
	}
	
	/**
	 * The function will be invoked when a key has been pressed
	 * 
	 * @param e
	 *            A key event which occured
	 */
	@Override
	public void keyPressed(KeyEvent e) {
		// Set to view mode roll
		if (e.getKeyCode() == KeyEvent.VK_T) {
			this.setViewMode(VIEW_MODE_ROLL);
		// Set to view mode zoom
		} else if (e.getKeyCode() == KeyEvent.VK_Z) {
			this.setViewMode(VIEW_MODE_ZOOM);
		// Set to view mode rotate
		} else if (e.getKeyCode() == KeyEvent.VK_R) {
			this.setViewMode(VIEW_MODE_ROTATE);
		// Set to view mode selection
		} else if (e.getKeyCode() == KeyEvent.VK_S) {
			this.setViewMode(VIEW_MODE_SELECTION);
		//	
		} else if (e.getKeyCode() == KeyEvent.VK_CONTROL) {
			this.ctrlPressed = true;
		} else if (e.getKeyCode() == KeyEvent.VK_LEFT){
			this.updateRenderableObjectsPosition(-1.0f, 0.0f, 0.0f);
		} else if (e.getKeyCode() == KeyEvent.VK_RIGHT){
			this.updateRenderableObjectsPosition(1.0f, 0.0f, 0.0f);
		} else if (e.getKeyCode() == KeyEvent.VK_UP){
			this.updateRenderableObjectsPosition(0.0f, 1.0f, 0.0f);
		} else if (e.getKeyCode() == KeyEvent.VK_DOWN){
			this.updateRenderableObjectsPosition(0.0f, -1.0f, 0.0f);
		} else if (e.getKeyCode() == KeyEvent.VK_ALT){
			this.altPressed = true;
		} else if (e.getKeyCode() == KeyEvent.VK_U){
			this.umbrellaPressed = true;
		}
	}

	public void updateRenderableObjectsPosition(float x, float y, float z){
		for (int i=0; i<this.renderableObjects.size(); i++) {
			if(this.renderableObjects.get(i).selected()){
				if(this.altPressed){
					this.renderableObjects.get(i).update(x, z, y);
				}else{
					this.renderableObjects.get(i).update(x, y, z);
				}
			}
		}
	}
	
	/**
	 * The function realizes the selection functionality. It is called
	 * while the application is in selection mode.
	 * 
	 * @param drawable A GLAutodrawable object
	 */
	private void select(GLAutoDrawable drawable){
		// Get the OpenGL object
		GL gl = drawable.getGL();
		// Create a new buffer
		int[] buffer = new int[this.selectionBufferSize];
		// Store the hits in a variable
		int hits;
		// Create a selection buffer
		IntBuffer selectionBuffer = BufferUtil.newIntBuffer(this.selectionBufferSize);
		// Specify the array to be used for the returned hit records with
		gl.glSelectBuffer(this.selectionBufferSize, selectionBuffer);
		// Enter selection mode by specifying GL_SELECT
		gl.glRenderMode(GL.GL_SELECT);
		// Initialize the name stack 
		gl.glInitNames();
		// Pushes a name on the stack
		gl.glPushName(0);
		//
		gl.glPushMatrix();
		// Define the viewing volume
		gl.glMatrixMode(GL.GL_PROJECTION);
		// Load the identity matrix
		gl.glLoadIdentity();
		// Define the selection volume
		gl.glOrtho(this.selectionVolume.leftRightPoint.x, this.selectionVolume.leftRightPoint.y, 
				this.selectionVolume.bottomTopPoint.x, this.selectionVolume.bottomTopPoint.y,
				this.selectionVolume.nearFarPoint.x, this.selectionVolume.nearFarPoint.y);
		// Set the matrix mode to model view
		gl.glMatrixMode(GL.GL_MODELVIEW);
		// Load the identity matrix
		gl.glLoadIdentity();
		// Draw the objects and store their name on the stack
		for (int i=0; i<this.renderableObjects.size(); i++) {
			// Store the objects onto the name stack
			gl.glLoadName(i);
			// Draw the object
			this.renderableObjects.get(i).draw(drawable);
		}
		gl.glPopMatrix();
		gl.glFlush();
		// Hits are written in the selection buffer while glRenderMode(...) is called
		hits = gl.glRenderMode(GL.GL_RENDER);
		// Write the hits into the buffer
		selectionBuffer.get(buffer);
		// Process the hits
		this.process(hits, buffer, false);
	}
	
	/**
	 * The function processes the hit records on the name stack. Each hit 
	 * record consists of four items, in order:
	 * 
	 *   - The number of names on the name stack when the hit occurred
	 *   
	 *   - Both the minimum and maximum window-coordinate z values of 
	 *     all vertices of the primitives that intersected the viewing
	 *     volume since the last recorded hit. These two values, which 
	 *     lie in the range [0,1], are each multiplied by 232-1 and 
	 *     rounded to the nearest unsigned integer.
	 *     
	 *   - The contents of the name stack at the time of the hit, with 
	 *     the bottom most element first.
	 * 
	 * @param hits The number of hits
	 * @param buffer 
	 * @param picking True if picking is enabled 
	 */
	private void process(int hits, int buffer[], boolean picking){		
		// If the ctrl key is not pressed the selection status of all objects is set to false
		if (!this.ctrlPressed) {
			for (Renderable r : this.renderableObjects) {
				r.setSelected(false);
			}
		}
	
		int pickedIndex = 0;
		if (picking && hits > 1) {
			for (int i = 1; i < hits; i++) {
				if (buffer[i * 4 + 1] < buffer[pickedIndex * 4 + 1]) {
					pickedIndex = i;
				}
			}
		}
				
		for (int i = 0; i < hits; i++) {
	    	/**
	    	 * The buffer is constructed in the following way:
	    	 *   
	    	 *   buffer[0] - Number of hits
	    	 *   buffer[1] - Minimum window z coordinate of all vertices
	    	 *   buffer[2] - Maximum window z coordinate of all vertices
	    	 *   buffer[3] - Name of the object on the stack
	    	 */
	    	if (this.renderableObjects.get(buffer[i*4+3]).selected() && this.ctrlPressed){
	    		renderableObjects.get(buffer[i*4+3]).setSelected(false);
	    	} else { 
	    		renderableObjects.get(buffer[i*4+3]).setSelected(true);
	    	}
	    }
		
	}
	
	/**
	 * The function realizes the picking mode. Basically it is a selection
	 * mode which is triggered by a input device.
	 * 
	 */
	private void pick(GLAutoDrawable drawable){
		// Get the GL object
		GL gl = drawable.getGL();
		// Create a new GLU object
		GLU glu = new GLU();
		// Create a new hit record buffer
		int[] buffer = new int[this.selectionBufferSize];
		// Create a selection buffer
		IntBuffer selectBuffer = BufferUtil.newIntBuffer(this.selectionBufferSize);
		// Store the hits in a variable
		int hits;
		// Define a viewport
		int viewport[] = new int[4];
		// Store the current viewport coordinates in the viewport variable
		gl.glGetIntegerv(GL.GL_VIEWPORT, viewport, 0);
		// Set the selection buffer
		gl.glSelectBuffer(this.selectionBufferSize, selectBuffer);
		// Switch to select render mode
		gl.glRenderMode(GL.GL_SELECT);
		// Initialize the name stack
		gl.glInitNames();
		//
		gl.glPushName(-1);
		// Switch to projection matrix mode
		gl.glMatrixMode(GL.GL_PROJECTION);
		gl.glPushMatrix();
		gl.glLoadIdentity();
		// Creates a 5x5 pixel picking region near cursor location
		glu.gluPickMatrix(
				(double) this.start.x,
				(double) (viewport[3] - this.start.y), 
				5.0, 5.0, viewport, 0);
        //
		glu.gluPerspective(60.0, 1.0, 0.1, 1000.0);
		//
        this.camera.lookAt(glu);
		// Render the objects into selection buffer
        gl.glMatrixMode(GL.GL_MODELVIEW);
		for (int i=0; i<this.renderableObjects.size(); i++) {
			// Store the name of the object on the name stack
			gl.glLoadName(i);
			this.renderableObjects.get(i).draw(drawable);
		}
		gl.glPopMatrix();
		gl.glFlush();
		// Hits are written into the selection buffer while glRenderMode(...) is called
		hits = gl.glRenderMode(GL.GL_RENDER);
		// Get the selection buffer
		selectBuffer.get(buffer);
		// Process the hits
		process(hits, buffer, true);
	}
	

	@Override
	public void mousePressed(MouseEvent e) {
		this.clickedButton = e.getButton();
		this.start.set(e.getX(), e.getY());

		// If the right mouse button is pressed toggle the picking status
		if(e.getButton() == MouseEvent.BUTTON3){
			this.picking = true;
		}
	}
	
	@Override
	public void display(GLAutoDrawable drawable) {
		// Get the OpenGL object
		GL gl = drawable.getGL();	
		// Enable or disable the depth test
		if (buffer) {
			// Enable the depth test
			gl.glEnable(GL.GL_DEPTH_TEST);
		} else {
			// Disable the depth test
			gl.glDisable(GL.GL_DEPTH_TEST);
		}
		//  Indicates the buffers currently enabled for color writing and depth buffer enabled
		gl.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);
		// Load the identity matrix
		gl.glLoadIdentity();
		// Set the camera
		this.setCamera(drawable);
		// Set the current render mode
		this.setRenderMode(drawable);
		
		gl.glEnable(GL.GL_LIGHT0);
		float[] lightPosition1 = {0.0f, 15.0f, 18.0f, 1.0f};
		float[] lightDiffuse1 = {1.0f, 1.0f, 1.0f, 1.0f};
		gl.glLightfv(GL.GL_LIGHT0, GL.GL_POSITION, lightPosition1, 0);
		gl.glLightfv(GL.GL_LIGHT0, GL.GL_DIFFUSE, lightDiffuse1, 0);
		
		/*
		gl.glMaterialf(GL.GL_FRONT_AND_BACK, GL.GL_SHININESS, 76.8f);
		gl.glMaterialfv(GL.GL_FRONT_AND_BACK, GL.GL_SPECULAR, Geometry.vector4ToFloat(1f, 0f, 0f, 1.0f));
		gl.glMaterialfv(GL.GL_FRONT_AND_BACK, GL.GL_DIFFUSE, Geometry.vector4ToFloat(0.0f, 1.00f, 0.0f, 1.0f));
		gl.glMaterialfv(GL.GL_FRONT_AND_BACK, GL.GL_AMBIENT, Geometry.vector4ToFloat(0.0f, 0.0f, 1.00f, 1.0f));
		
		// Draw the light
        this.one.draw(drawable);
        this.two.draw(drawable);
		*/
		gl.glColor3f(1.0f, 1.0f, 1.0f);
		gl.glLineWidth(1.0f);
		
		for(int i = 0; i < this.renderableObjects.size(); i++){
			// Set the current render mode
			this.renderableObjects.get(i).setRenderMode(this.currentRenderMode);
			// Draw the object
			this.renderableObjects.get(i).draw(drawable);
			
			if(this.renderableObjects.get(i).selected()){
				if(this.umbrellaPressed){
					this.renderableObjects.get(i).weightedUmbrellaOperator(this.lambda);
				}
			}
		}
		
		// Show coordinate system
		if (showCoordinateSystem) {
			gl.glLineWidth(1.0f);
			this.coordinateSystem.draw(drawable);
		}
		// Picking is enabled
		if (this.picking){
			this.pick(drawable);
			// Reset the status variable
			this.picking = false;
		}
		// Selection is enabled 
		if (this.currentViewMode == VIEW_MODE_SELECTION) {
			// Draw the selection volume
			this.selectionVolume.draw(drawable);
			// Call the select method
			this.select(drawable);
		}
				
		if(this.renderShader){
			gl.glPushMatrix();
			this.shader.loadShader(drawable);
			gl.glPopMatrix();
			this.renderShader = false;
		}
		
		gl.glFlush();
		drawable.swapBuffers();
	}

	/**
	 * The function is called by the drawable when the display mode or the
	 * display device associated with the GLAutoDrawable has changed.
	 * 
	 * @param drawable
	 *            A object which performs the OpenGL rendering
	 * @param modeChanged
	 *            If the mode has changed
	 * @param deviceChanged
	 *            If the device has changed
	 */
	@Override
	public void displayChanged(GLAutoDrawable drawable, boolean modeChanged,
			boolean deviceChanged) {
		/**
		 * Implementations are not required to implement this method as stated
		 * by Sun Microsystem Java API.
		 */
	}

	/**
	 * The function is called immediately after the OpenGL context is
	 * initialized. It performs one-time OpenGL initialization such as setup of
	 * lights and display lists.
	 * 
	 * @param drawable
	 *            A object which performs the OpenGL rendering
	 */
	@Override
	public void init(GLAutoDrawable drawable) {
		// Get the 'OpenGL object'
		GL gl = drawable.getGL();
		// Set the color
		gl.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		// Set the matrix mode to model view matrix
		gl.glMatrixMode(GL.GL_MODELVIEW);
		/** If enabled, use the current lighting parameters to compute the vertex color or index. Otherwise, simply
		associate the current color or index with each vertex */
		gl.glEnable(GL.GL_LIGHTING);
		// Define the first light
		//gl.glEnable(GL.GL_LIGHT0);
		// Define a second light
		//gl.glEnable(GL.GL_LIGHT1);
		// Enable material parameters
		gl.glEnable(GL.GL_COLOR_MATERIAL);
		// Passes the depth test if the incoming depth value is less than the stored depth value.
		gl.glDepthFunc(GL.GL_LESS);
		// The default z mapping
		gl.glDepthRange(0.0, 1.0);
	}

	/**
	 * Set the render mode of the application. Valid vales are
	 * 
	 * - wire frame on gouraud shading
	 * - wire frame
	 * - flat shading
	 * - gouraud shading
	 */
	private void setRenderMode(GLAutoDrawable drawable) {
		// Get the GL object
		GL gl = drawable.getGL();
		// Determine the current render mode
		if (this.currentRenderMode == RENDER_MODE_FLAT) {
			// Sets the polygon rasterization to front and back and fill
			gl.glPolygonMode(GL.GL_FRONT_AND_BACK, GL.GL_FILL);
			// Set shading model to gouraud shading
			gl.glShadeModel(GL.GL_FLAT);
			return;
		} else if (this.currentRenderMode == RENDER_MODE_GOURAUD) {
			// Sets the polygon rasterization to front and back and fill
			gl.glPolygonMode(GL.GL_FRONT_AND_BACK, GL.GL_FILL);
			// Set shading model to gouraud shading
			gl.glShadeModel(GL.GL_SMOOTH);
			return;
		} else if (this.currentRenderMode == RENDER_MODE_WIRE_FRAME) {
			// Sets the polygon rasterization to front and back and lines 
			gl.glPolygonMode(GL.GL_FRONT_AND_BACK, GL.GL_LINE);
			// Set shading model to gouraud shading
			gl.glShadeModel(GL.GL_SMOOTH);
			return;
		} else if (this.currentRenderMode == RENDER_MODE_WIRE_FRAME_ON_GOURAUD) {
			// Sets the polygon rasterization to front and back and fill 
			gl.glPolygonMode(GL.GL_FRONT_AND_BACK, GL.GL_FILL);
			// Set shading model to gouraud shading
			gl.glShadeModel(GL.GL_SMOOTH);
			return;
		}
	}

	/**
	 * The function sets the camera 
	 */
	private void setCamera(GLAutoDrawable drawable) {
		// Get the GL object
		GL gl = drawable.getGL();
		// Create a GLU object
		GLU glu = new GLU();
		// Set the matrix mode to gl projection for glPerspective
		gl.glMatrixMode(GL.GL_PROJECTION);
		// Load the identity matrix
		gl.glLoadIdentity();
		// Set the frustum in world coordinates
		/**
		 * Sets up a perspective projection matrix which specifies
		 * a viewing frustum into the world coordinate system. 
		 */
		glu.gluPerspective(60.0, 1.0, 0.1, 1000.0);
		// Look at a specific point 
		this.camera.lookAt(glu);
	}

	/**
	 * 
	 * @param gLDrawable
	 *            The GLDrawable object.
	 * @param x
	 *            The X Coordinate of the viewport rectangle.
	 * @param y
	 *            The Y coordinate of the viewport rectanble.
	 * @param width
	 *            The new width of the window.
	 * @param height
	 *            The new height of the window.
	 */
	@Override
	public void reshape(GLAutoDrawable drawable, int x, int y, int width,
			int height) {
		// Get the GL object
		GL gl = drawable.getGL();
		// Set the viewport to the new width and height
		gl.glViewport(0, 0, width, height);
		// Set the matrix mode to gl projection for glOrtho
		gl.glMatrixMode(GL.GL_PROJECTION);
		// Load the identity matrix
		gl.glLoadIdentity();
		/** 
		 * Multiply the current matrix with an orthographic matrix
		 * The first and second parameter specifies the coordinates 
		 * for the left and right vertical clipping planes. The third
		 * and fourth parameter specifies the coordinates for the 
		 * bottom and top horizontal clipping planes. The last two
		 * parameters specify the distances to the nearer and farther 
		 * depth clipping planes
		 */
		gl.glOrtho(-3, 4, -3, 4, -11, 11);
	}

	@Override
	public void mouseDragged(MouseEvent e) {
		if (this.clickedButton == MouseEvent.BUTTON1) {
			this.end.set(e.getX(), e.getY());
			
			if (this.currentViewMode == Renderer.VIEW_MODE_ZOOM) {
				this.setViewModeZoom();
				return;
			} else if (this.currentViewMode == Renderer.VIEW_MODE_ROTATE) {
				this.setViewModeRotate();
				return;
			} else if (this.currentViewMode == Renderer.VIEW_MODE_ROLL) {
				this.setViewModeRoll();
				return;
			}
		}
	}

	/**
	 * The function "rolls the object" around.
	 */
	private void setViewModeRoll() {
		double angle;
		if (this.start.x < this.end.x) {
			angle = 1 * 0.3 * this.end.distance(this.start) / this.resolution.width;
		} else {
			angle = -1 * 0.3 * this.end.distance(this.start) / this.resolution.width;
		}
		// If the angle is greater than 360 or -360 the angle is set to 0
		if (angle >= 2 * Math.PI || angle <= -2 * Math.PI) {
			angle = 0.0;
		}
		// Roll the camera
		this.camera.roll(angle);
	}
	
	/**
	 * The function "rotates around the object"
	 */
	private void setViewModeRotate() {
		double angle;
		float x = this.end.x - this.start.x;
		float y = this.end.y - this.start.y;

		if ((Math.abs(x) > Math.abs(y) && this.end.x < this.start.x)
				|| (Math.abs(x) <= Math.abs(y) && this.end.y < this.start.y)) {
			angle = 1 * 0.8 * this.end.distance(this.start) / this.resolution.width;
		} else {
			angle = -1 * 0.8 * this.end.distance(this.start) / this.resolution.width;
		}
		// If the angle is greater than 360 or -360 the angle is set to 0
		if (angle >= 2 * Math.PI || angle <= -2 * Math.PI) {
			angle = 0.0;
		}
		// If the distance between the two x values is greater than the distance between the two y values
		if (Math.abs(x) > Math.abs(y)) { 
			// Rotate around up
			this.camera.rotateAroundUp(angle);
		} else { 
			// Rotate on top
			this.camera.rotateTop(angle);
		}

	}

	/**
	 * The functions zooms to a object 
	 */
	private void setViewModeZoom() {
		if (this.start.y < this.end.y) {
			// Zoom into the object
			this.camera.zoom(1 + (this.end.distance(this.start) / this.resolution.width));
		} else {
			// Zoom out from the object
			this.camera.zoom(1 - (this.end.distance(this.start) / this.resolution.width));
		}
	}
	
	/**
	 * The function display or hides a coordinate system for orientation on the
	 * OpenGL window.
	 */
	public void showCoordinateSystem() {
		this.showCoordinateSystem = !this.showCoordinateSystem;
	}

	/**
	 * The function enables or disables the z buffer.
	 */
	public void setBuffer() {
		this.buffer = !this.buffer;
	}

	/**
	 * The function sets the current render mode to a new render mode
	 * 
	 * @param renderMode
	 *            The new render mode
	 */
	public void setCurrenRenderMode(int renderMode) {
		this.currentRenderMode = renderMode;
	}

	/**
	 * The function sets the current view mode to a new view mode
	 * 
	 * @param viewMode
	 *            The new view mode
	 */
	public void setViewMode(int viewMode) {
		this.currentViewMode = viewMode;
	}
	
	/**
	 * The function returns a reference to one of the
	 * two light sources
	 * 
	 * @param index The index of the light source
	 * @return The corresponding light source
	 */
	public Light getLight(int index){
		if(index == 0){
			return this.one;
		}
		return this.two;
	}
	
	public void addRenderableObject(Renderable object){
		this.renderableObjects.add(object);
	}
	
	public Volume getSelectionVolume(){
		return this.selectionVolume;
	}
	
	public Volume setSelectionVolume(Volume volume){
		return this.selectionVolume = volume;
	}
	
	@Override
	public void mouseMoved(MouseEvent e) {}
	
	@Override
	public void mouseReleased(MouseEvent e) {}

	@Override
	public void mouseExited(MouseEvent e) {}
	
	@Override
	public void mouseEntered(MouseEvent e) {}
	
	
	/**
	 * The function will be invoked when a key has been released
	 * 
	 * @param e
	 *            A key event which occured
	 */
	@Override
	public void keyReleased(KeyEvent e) {
		if(e.getKeyCode() == KeyEvent.VK_CONTROL){
			this.ctrlPressed = false;
		}else if(e.getKeyCode() == KeyEvent.VK_ALT){
			this.altPressed = false;
		}else if(e.getKeyCode() == KeyEvent.VK_U){
			this.umbrellaPressed = false;
		}
	}

	/**
	 * The function will be invoked when a key has been typed
	 * 
	 * @param e
	 *            A key event which occured
	 */
	@Override
	public void keyTyped(KeyEvent e) {}

	/**
	 * The function will be invoked when the mouse has been clicked on a
	 * component.
	 */
	@Override
	public void mouseClicked(MouseEvent e) {}

	public void setVertexShaderFile(File file) {
		this.shader.setVertexShaderFile(file);
	}

	public void setFragmentShaderFile(File file) {
		this.shader.setFragmentShaderFile(file);
	}

	public void renderShader() {
		this.renderShader = true;
	}

	public float getLambda() {
		return lambda;
	}

	public void setLambda(float lambda) {
		this.lambda = lambda;
	}
}
