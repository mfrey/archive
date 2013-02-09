package de.fh_wiesbaden.cs.icg.viewer;

import javax.media.opengl.glu.GLU;
import javax.vecmath.Point3d;
import javax.vecmath.Vector3d;

import de.fh_wiesbaden.cs.icg.math.Geometry;

/**
 * 
 */
public class Camera {
	/**
	 * The position of the eye point
	 */
	private Point3d eye;
	/**
	 * The position of the reference point
	 */
	private Point3d center;
	/**
	 * The direction of the view up vector of the camera
	 */
	private Vector3d viewup;

	/**
	 * 
	 */
	public Camera() {
		this.eye = new Point3d(21.0, 11.0, 21.0);
		this.center = new Point3d(0.0, 0.0, 0.0);
		this.viewup = new Vector3d(0.0, 1.0, 0.0);
	}

	/**
	 * The constructor initializes the attributes of the class.
	 * 
	 * @param e
	 *            The coordinates of the eye
	 * @param c
	 *            The coordinates of the reference point
	 * @param v
	 *            The coordinates of the view up vector of the camera
	 */
	public Camera(Point3d e, Point3d c, Vector3d v) {
		// Set the eye coordinates
		this.eye = e;
		// Set the center
		this.center = c;
		// Set the view up vector of the camera
		this.viewup = v;
	}

	/**
	 * The function sets the existing coordinates of the reference point to new
	 * coordinates
	 * 
	 * @param c
	 *            The coordinates of the new reference point
	 */
	public void setCenter(Point3d c) {
		this.center = c;
	}

	/**
	 * The function sets the existing coordinates of the eye to new coordinates
	 * 
	 * @param e
	 *            The coordinates of the new eye
	 */
	public void setEye(Point3d e) {
		this.eye = e;
	}

	/**
	 * The function sets the existing coordinates of the view up vector of the
	 * camera to new coordinates
	 * 
	 * @param v
	 *            The coordinates of the new view up vector
	 */
	public void setViewUp(Vector3d v) {
		this.viewup = v;
	}

	public void lookAt(GLU glu) {
		glu.gluLookAt(this.eye.getX(), this.eye.getY(), this.eye.getZ(),
				this.center.getX(), this.center.getY(), this.center.getZ(),
				this.viewup.getX(), this.viewup.getY(), this.viewup.getZ());
	}

	/**
	 * The function scales the eye on a given factor. The 
	 * effect is comparable to a zoom.
	 * 
	 * @param zoomFactor The scale factor
	 * 
	 */
	public void zoom(float zoomFactor) {
		this.eye.scale(zoomFactor);
	}

	public void roll(double angle) {
		// Create a new camera view up vector
		Point3d cameraUp = new Point3d(this.viewup);
		// Add the eye position to it
		cameraUp.add(this.eye);
		// Define a new 'axis'
		Vector3d axis = new Vector3d(this.eye);
		// Normalize the vector
		axis.normalize();
		// Rotate around those axis with the new camera up vector and the angle
		Geometry.rotate(cameraUp, axis, angle);
		// Sub the eye from the camera up vector
		this.viewup.sub(cameraUp, this.eye);
		// Normalize the vector
		this.viewup.normalize();
	}

	/**
	 * The function rotates the eye and view up vector of
	 * the camera with a given angle.
	 * 
	 * @param angle The rotation angle
	 */
	public void rotateAroundUp(double angle) {
		Geometry.rotate(this.eye, this.viewup, angle);
	}

	/**
	 * 
	 */
	public void rotateTop(double angle) {
		// Define a axis the camera should rotate around
		Vector3d axis = new Vector3d(); 
		// Build the cross product between the view up and the eye vector
		axis.cross(this.viewup, new Vector3d(this.eye));
		// Normalize the vector
		axis.normalize();
		// Define a new camera up 'point'
		Point3d cameraUp = new Point3d(this.viewup);
		cameraUp.add(this.eye);
		// Save previous the distance between eye and origin
		double previousDistance = this.eye.distance(new Point3d(0.0, 0.0, 0.0));
		// Rotate eye
		Geometry.rotate(this.eye, axis, angle);
		// Rotate 
		Geometry.rotate(cameraUp, axis, angle);
		// Revert the operation
		this.viewup.sub(cameraUp, this.eye);
		// Normalize the vector
		this.viewup.normalize();
		// Calculate the new distance between eye and origin
		double newDistance = this.eye.distance(new Point3d(0.0, 0.0, 0.0));
		// If the eye has moved
		if (newDistance > previousDistance) {
			double scale = previousDistance / newDistance;
			this.eye.scale(scale);
		}
	}
}
