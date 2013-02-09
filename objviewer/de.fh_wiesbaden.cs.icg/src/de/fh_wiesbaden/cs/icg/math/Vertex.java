package de.fh_wiesbaden.cs.icg.math;

import java.util.ArrayList;
import java.util.List;

import javax.vecmath.Tuple3d;
import javax.vecmath.Tuple3f;
import javax.vecmath.Vector3d;
import javax.vecmath.Vector3f;

public class Vertex extends Vector3f {
	/**
	 * A generated serial version id 
	 */
	private static final long serialVersionUID = -5532952631538599576L;
	private Edge firstEdge = null;
	private boolean isPartOfBorderEdge = false;
	private List<Edge> involvedEdges = new ArrayList<Edge>();
	private Normal normal = null;
	
	
	public Vertex() {
		super();
	}
	
	public Vertex(float[] v) {
		super(v);
	}
    
	public Vertex(float x, float y, float z) {
		super(x, y, z);
	}

	public Vertex(Tuple3d t1) {
		super(t1);
	}

	public Vertex(Tuple3f t1) {
		super(t1);
	}

	public Vertex(Vector3d v1) {
		super(v1);
	}

	public Vertex(Vector3f v1) {
		super(v1);
	}
	
	public Edge getFirstEdge() {
		return firstEdge;
	}

	public void setFirstEdge(Edge firstEdge) {
		this.firstEdge = firstEdge;
	}
	
	public void addInvolvedEdge(Edge we) {
		this.involvedEdges.add(we);
	}

	/**
	 * @return the isPartOfBorderEdge
	 */
	public boolean isPartOfBorderEdge() {
		return isPartOfBorderEdge;
	}

	/**
	 * @param isPartOfBorderEdge the isPartOfBorderEdge to set
	 */
	public void setPartOfBorderEdge(boolean isPartOfBorderEdge) {
		this.isPartOfBorderEdge = isPartOfBorderEdge;
	}

	/**
	 * @return the involvedEdges
	 */
	public List<Edge> getInvolvedEdges() {
		return involvedEdges;
	}

	public Normal getNormal() {
		return normal;
	}

	public void setNormal(Normal normal) {
		this.normal = normal;
	}
	
	
}
