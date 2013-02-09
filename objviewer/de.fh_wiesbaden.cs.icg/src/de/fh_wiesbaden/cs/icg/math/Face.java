package de.fh_wiesbaden.cs.icg.math;

/**
 * This class implements geometrical faces as described in the following obj
 * file specification:
 * 
 * http://local.wasp.uwa.edu.au/~pbourke/dataformats/obj/
 * 
 * @author Michael Frey
 */
public class Face {
	/**
	 * A reference number for a geometric vertex
	 */
	private int[] vertex = new int[3];
	/**
	 * A reference number for a texture vertex
	 */
	private int[] texture = new int[3];
	/**
	 * A reference number for a vertex normal
	 */
	private int[] normal = new int[3];

	/**
	 * 
	 * 
	 */
	public Face(final int[] geometric, final int[] texture, final int[] normals) {
		this.vertex = geometric;
		this.texture = texture;
		this.normal = normals;
	}

	public int[] getVertex() {
		return vertex;
	}

	public void setVertex(int[] vertex) {
		this.vertex = vertex;
	}

	public int[] getTexture() {
		return texture;
	}

	public void setTexture(int[] texture) {
		this.texture = texture;
	}

	public int[] getNormal() {
		return normal;
	}

	public void setNormal(int[] normal) {
		this.normal = normal;
	}
	
	public int[] getEdge(int index) {
		int[] ret = new int[2];
		switch (index) {
		case 0:
		case 3:
			ret[0] = vertex[0];
			ret[1] = vertex[1];
			return ret;
		case 1:
			ret[0] = vertex[1];
			ret[1] = vertex[2];
			return ret;
		case 2:
			ret[0] = vertex[2];
			ret[1] = vertex[0];
			return ret;
		}
		
		return null;
	}

}
