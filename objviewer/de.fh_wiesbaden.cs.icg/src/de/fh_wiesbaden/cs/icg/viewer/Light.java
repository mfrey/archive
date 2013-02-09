package de.fh_wiesbaden.cs.icg.viewer;

import javax.media.opengl.GL;
import javax.media.opengl.GLAutoDrawable;
import javax.vecmath.Color4f;
import javax.vecmath.Point3f;
import javax.vecmath.Vector3f;

import com.sun.opengl.util.GLUT;

import de.fh_wiesbaden.cs.icg.math.Geometry;
import de.fh_wiesbaden.cs.icg.renderable.Renderable;

public class Light implements Renderable {
	/**
	 * Position of the light
	 */
	private Point3f position = new Point3f();
	/**
	 * Direction of the light
	 */
	private Vector3f direction = new Vector3f();
	/**
	 * Ambient RGBA intensity of light
	 */
	private Color4f ambient = new Color4f();
	/**
	 * Diffuse RGBA intensity of light
	 */
	private Color4f diffuse = new Color4f();
	/**
	 * Specular RGBA intensity of light
	 */
	private Color4f specular = new Color4f();
	/**
	 * Light cutoff angle
	 */
	private float cutOffAngle = 90.0f;
	/**
	 * Light exponent
	 */
	private float spotLightExponent = 0.0f;
	/**
	 * Constant attenuation factor
	 */
	private float constantAttenuation = 0.0f;
	/**
	 * Linear attenuation factor
	 */
	private float linearAttenuation = 0.0f;
	/**
	 * Quadratic attenuation factor
	 */
	private float quadraticAttenuation = 0.0f;
	/**
	 * The index of the light
	 */
	private int light;

	/**
	 * The constructor sets the index of the light which can be GL_LIGHT0,
	 * GL_LIGHT1, ... , or GL_LIGHT7
	 * 
	 * @param index
	 *            The index of the light
	 */
	public Light(final int index) {
		this.light = index;
	}

	/**
	 * 
	 */
	@Override
	public void draw(GLAutoDrawable drawable) {
		// Get thte OpenGL object
		GL gl = drawable.getGL();	
		GLUT glut = new GLUT();
		// Set the constant attenuation
		gl.glLightf(this.light, GL.GL_CONSTANT_ATTENUATION,
				this.constantAttenuation);
		// Set the linear attenuation
		gl.glLightf(this.light, GL.GL_LINEAR_ATTENUATION,
				this.linearAttenuation);
		// Set the quadratic attenuation
		gl.glLightf(this.light, GL.GL_QUADRATIC_ATTENUATION,
				this.quadraticAttenuation);
		// Set the ambient light
		gl
				.glLightfv(this.light, GL.GL_AMBIENT, Geometry.vector4ToFloat(
						this.ambient.x, this.ambient.y, this.ambient.z,
						this.ambient.w));
		// Set the diffuse light
		gl
				.glLightfv(this.light, GL.GL_DIFFUSE, Geometry.vector4ToFloat(
						this.diffuse.x, this.diffuse.y, this.diffuse.z,
						this.diffuse.w));
		// Set the specular light
		gl.glLightfv(this.light, GL.GL_SPECULAR, Geometry.vector4ToFloat(
				this.specular.x, this.specular.y, this.specular.z,
				this.specular.w));
		// Set the cut off angle
		gl.glLightf(this.light, GL.GL_SPOT_CUTOFF, this.cutOffAngle);
		// Set the spot exponent
		gl.glLightf(this.light, GL.GL_SPOT_EXPONENT, this.spotLightExponent);
		// Set the position of the light
		gl.glLightfv(this.light, GL.GL_POSITION, Geometry.vector4ToFloat(
				this.position.x, this.position.y, this.position.z, 1.0f));
		// Set the direction of the light
		gl.glLightfv(this.light, GL.GL_SPOT_DIRECTION, Geometry.vector3ToFloat(
				this.direction.x, this.direction.y, this.direction.z));
		
		// glutwirecone
		//glut.glutWireCone((double)this.cutOffAngle, 1.0, 10, 10);
	}

	public Point3f getPosition() {
		return position;
	}

	public void setPosition(Point3f position) {
		this.position = position;
	}

	public Vector3f getDirection() {
		return direction;
	}

	public void setDirection(Vector3f direction) {
		this.direction = direction;
	}

	public Color4f getAmbient() {
		return ambient;
	}

	public void setAmbient(Color4f ambient) {
		this.ambient = ambient;
	}

	public Color4f getDiffuse() {
		return diffuse;
	}

	public void setDiffuse(Color4f diffuse) {
		this.diffuse = diffuse;
	}

	public Color4f getSpecular() {
		return specular;
	}

	public void setSpecular(Color4f specular) {
		this.specular = specular;
	}

	public float getCutOffAngle() {
		return cutOffAngle;
	}

	public void setCutOffAngle(float cutOffAngle) {
		this.cutOffAngle = cutOffAngle;
	}

	public float getSpotLightExponent() {
		return spotLightExponent;
	}

	public void setSpotLightExponent(float spotLightExponent) {
		this.spotLightExponent = spotLightExponent;
	}

	public float getConstantAttenuation() {
		return constantAttenuation;
	}

	public void setConstantAttenuation(float constantAttenuation) {
		this.constantAttenuation = constantAttenuation;
	}

	public float getLinearAttenuation() {
		return linearAttenuation;
	}

	public void setLinearAttenuation(float linearAttenuation) {
		this.linearAttenuation = linearAttenuation;
	}

	public float getQuadraticAttenuation() {
		return quadraticAttenuation;
	}

	public void setQuadraticAttenuation(float quadraticAttenuation) {
		this.quadraticAttenuation = quadraticAttenuation;
	}

	@Override
	public boolean selected() {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public void setSelected(boolean value) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void setRenderMode(int mode) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void update(float x, float y, float z) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void weightedUmbrellaOperator(float lambda) {
		// TODO Auto-generated method stub
		
	}

}
