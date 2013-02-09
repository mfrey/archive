package de.fh_wiesbaden.cs.icg.math;

/**
 * The class edge implements "parts" of a winged edge structure. Further
 * information about the winged edge data structure can be found here:
 * 
 *  http://www.baumgart.org/winged-edge/winged-edge.html
 * 
 * @author Michael Frey
 */
public class Edge {
	/**
	 * First point 
	 */
	private Vertex P = null;
	/**
	 * Second point
	 */
	private Vertex Q = null;
	/**
	 * First Edge
	 */
	private Edge E = null;
	/**
	 * Second Edge
	 */
	private Edge F = null;
	/**
	 * The discrete mean curvature
	 */
	private Vertex K = null;
	
	
	public Edge(Vertex p, Vertex q){
		this.P = p;
		this.P.addInvolvedEdge(this);
		this.Q = q;		
		this.Q.addInvolvedEdge(this);
	}
		
	public Vertex nextVertex(Vertex v) {
		return (P == v) ? Q : P;
	}
	
	public Edge nextEdge(Vertex v) {
		return (P == v) ? E : F;
	}

	public Vertex getP() {
		return P;
	}

	public void setP(Vertex p) {
		P = p;
	}

	public Vertex getQ() {
		return Q;
	}

	public void setQ(Vertex q) {
		Q = q;
	}

	public Edge getE() {
		return E;
	}

	public void setE(Edge e) {
		E = e;
	}

	public Edge getF() {
		return F;
	}

	public void setF(Edge f) {
		F = f;
	}

	public Vertex getK() {
		return K;
	}

	public void setK(Vertex k) {
		K = k;
	}
	
}
