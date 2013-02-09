package de.fh_wiesbaden.cs.icg.math;

import java.nio.FloatBuffer;
import java.util.Vector;

import javax.vecmath.Matrix3d;
import javax.vecmath.Tuple3d;
import javax.vecmath.Vector3f;

import com.jogamp.common.nio.Buffers;

/**
 * The class implements a number of helper functions for mathematical geometric
 * operations.
 * 
 * @author Michael Frey
 */
public class Geometry {
	/**
	 * The function implements a rotation operation for a point on a certain
	 * axis and angle.
	 * 
	 * @param point
	 *            The point which should be rotated
	 * @param axis
	 *            The axis to be rotated around
	 * @param angle
	 *            The angle of the rotation
	 * 
	 */
	public static void rotate(Tuple3d point, Tuple3d axis, double angle) {
		double sinus = Math.sin(angle);
		double cosinus = Math.cos(angle);

		double inv = 1.0 - cosinus;
		/**
		 * The following matrix is build up the three standard matrices for
		 * rotation. So far, the calculation for this matrix is:
		 * 
		 * rotation = rotation_z(a) * rotation (y) * rotation_x(a)
		 * 
		 */
		Matrix3d rotation = new Matrix3d(
		// First column of the matrix
				cosinus + axis.x * axis.x * inv, axis.x * axis.y * inv
						- axis.z * sinus, axis.x * axis.z * inv + axis.y
						* sinus,
				// Second column of the matrix
				axis.x * axis.y * inv + axis.z * sinus, cosinus + axis.y
						* axis.y * inv, axis.y * axis.z * inv - axis.x
						* sinus,
				// Third column of the matrix
				axis.z * axis.x * inv - axis.y * sinus, axis.z * axis.y
						* inv + axis.x * sinus, cosinus + axis.z * axis.z
						* inv);
		// Create a new matrix
		Matrix3d matrix = new Matrix3d();
		// Set the one dimensional matrix
		matrix.setColumn(0, point.x, point.y, point.z);
		// Multiply the matrix with the vector
		rotation.mul(matrix);
		/**
		 * Values are:
		 * 
		 * m00 - The first matrix element in the first row m10 - The first
		 * matrix element in the second row m20 - The first matrix element in
		 * the third row
		 */
		point.set(rotation.m00, rotation.m10, rotation.m20);
	}

	/***
	 * The function transforms a vector to a float buffer which is needed by
	 * certain OpenGL methods.
	 * 
	 * @param x
	 *            The x value of the vector
	 * @param y
	 *            The y value of the vector
	 * @param z
	 *            The z value of the vector
	 * @param w
	 *            The alpha value of the vector
	 * 
	 * @return The content of the vector transformed into a float buffer
	 */
	public static FloatBuffer vector4ToFloat(float x, float y, float z, float w) {
		FloatBuffer result = Buffers.newDirectFloatBuffer(4);
		result.put(x);
		result.put(y);
		result.put(z);
		result.put(w);
		result.rewind();
		return result;
	}

	/***
	 * The function transforms a vector to a float buffer which is needed by
	 * certain OpenGL methods.
	 * 
	 * @param x
	 *            The x value of the vector
	 * @param y
	 *            The y value of the vector
	 * @param z
	 *            The z value of the vector
	 * 
	 * @return The content of the vector transformed into a float buffer
	 */
	public static FloatBuffer vector3ToFloat(float x, float y, float z) {
		FloatBuffer result = Buffers.newDirectFloatBuffer(3);
		result.put(x);
		result.put(y);
		result.put(z);
		result.rewind();
		return result;
	}

	
	/**
	 * The function determines the angle between to vectors. 
	 * 
	 * @param a The first vertex
	 * @param b The second vertex
	 * @param p The third vertex
	 * 
	 * @return The angle between the vector b-p and b-a
	 */
	public static double determineAngle(final Vertex a, final Vertex b, final Vertex p){
		// Determine the vector from b to a 
		Vector3f x = new Vector3f(a.x - b.x, a.y - b.y, a.z - b.z);
		// Determine the vector from b to p 
		Vector3f y = new Vector3f(p.x - b.x, p.y - b.y, p.z - b.z);
		// Determine the angle
		float angle = (((x.x * y.x) + (x.y * y.y) + (x.z * y.z))  / (x.lengthSquared() * y.lengthSquared()));
		// Return the result
		return Math.acos(angle);
	}
	
	/**
	 * The function calculates the area of polygon net of a vertex p
	 * 
	 * @param p The vertex p 
	 * @param neighborlist The neighbors of the vector p
	 * 
	 * @return The size of the area of the polygon net of the vertex p
	 */
	public static float determineArea(final Vertex p, final Vector<Vertex> neighborList){
		float result = 0.0f;
		
		for(int i = 0; i < (neighborList.size() - 1); i++){
			// Create the vector qi
			Vertex a = new Vertex(neighborList.get(i));
			// Subtract the vector p from qi
			a.sub(p);
			// Create the vector qi+1
			Vertex b = new Vertex(neighborList.get(i+1));
			// Subtract the vector p from qi+1
			b.sub(p);
			// Create a result vector
			Vertex c = new Vertex();
			// Store the result of the cross product
			c.cross(a, b);
			// Add the result to the overall result
			result += c.lengthSquared();
		}
		
		return (result * (1/6));
	}

	/**
	 * The function calculates the cotangens of a given value
	 * x
	 * 
	 * @param x The parameter of the cotangens
	 * 
	 * @return The cotangens of the parameter x
	 */
	public static double cot(final double x){
		return (1 / Math.tan(x));
	}
}
