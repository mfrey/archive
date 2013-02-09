package de.fh_wiesbaden.cs.icg.renderable;

import javax.media.opengl.GLAutoDrawable;

/**
 * A interface for renderable objects
 *
 * @author Michael Frey
 */
public interface Renderable {
	/**
	 * The function draws the graphical object 
	 * 
	 * @param drawable A drawable object
	 * 
	 */
	public void draw(GLAutoDrawable drawable);
	/**
	 * The function returns the select status of a graphical object 
	 * 
	 * @return The select status of a graphical object
	 * 
	 */
	public boolean selected();
	/**
	 * The function sets the select status of the graphical object 
	 * 
	 * @param mode The new select status of the object
	 * 
	 */
	public void setSelected(boolean value);
	
	/**
	 * The function sets the render mode of the graphical object 
	 * 
	 * @param mode The new render mode of the object
	 * 
	 */
	public void setRenderMode(int mode);
	
	/**
	 * The function updates the position of the graphical object 
	 * 
	 * @param x The new x coordinate of the object
	 * @param y The new y coordinate of the object
	 * @param z The new z coordinate of the object
	 * 
	 */
	public void update(float x, float y, float z);
	
	/**
	 * The function adds the weighted umbrella operation to the
	 * graphical object
	 * 
	 * @param lambda The scale factor of the weighted umbrella operation
	 */
	public void weightedUmbrellaOperator(float lambda);
}
