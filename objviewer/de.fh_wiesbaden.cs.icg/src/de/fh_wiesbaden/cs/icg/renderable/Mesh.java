package de.fh_wiesbaden.cs.icg.renderable;

import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Vector;

import javax.media.opengl.GL;
import javax.media.opengl.GLAutoDrawable;
import javax.vecmath.Vector3f;

import com.sun.opengl.util.BufferUtil;

import de.fh_wiesbaden.cs.icg.math.Edge;
import de.fh_wiesbaden.cs.icg.math.Face;
import de.fh_wiesbaden.cs.icg.math.Geometry;
import de.fh_wiesbaden.cs.icg.math.Normal;
import de.fh_wiesbaden.cs.icg.math.Vertex;
import de.fh_wiesbaden.cs.icg.viewer.Renderer;

import javax.vecmath.Point3f;

/**
 * The class implements a mesh structure which is a OpenGL representation of a
 * *.obj file.
 * 
 * @author Michael Frey
 */
public class Mesh implements Renderable {
	/**
	 * A vertex array
	 */
	ArrayList<Vertex> vertexList;
	/**
	 * A normal array
	 */
	ArrayList<Normal> normalList;
	/**
	 * A face array
	 */
	ArrayList<Face> faceList;
	/**
	 * A list of points for the winged edge data structure. Tuples of the next
	 * point in the edge or the corresponding face are stored in this list for
	 * every vertex/point.
	 */
	private List<HashMap<Vertex, Face>> pointList;
	/**
	 * A vertex buffer
	 */
	private FloatBuffer vertex = null;
	/**
	 * A normal buffer
	 */
	private FloatBuffer normal = null;
	/**
	 * A face buffer
	 */
	private IntBuffer face = null;
	/**
	 * Set the render mode of the object initially to wire frame
	 */
	private int currentRenderMode = Renderer.RENDER_MODE_WIRE_FRAME;
	/**
	 * The selection status of the object
	 */
	private boolean selected = false;
	/**
	 * A new x coordinate
	 */
	private float x = 0.0f;
	/**
	 * A new y coordinate
	 */
	private float y = 0.0f;
	/**
	 * A new z coordinate
	 */
	private float z = 0.0f;
	/**
	 * A new z coordinate
	 */
	private List<Edge> edges = new ArrayList<Edge>();
	/**
	 * A new z coordinate
	 */
	private HashMap<Integer, Edge> hashEdges = new HashMap<Integer, Edge>();
	
	/**
	 * The constructor initializes important attributes and builds the winged
	 * edge data structure.
	 * 
	 * @param v A list with vertices
	 * @param n A list with normals
	 * @param f A list with faces
	 */
	public Mesh(final ArrayList<Vertex> v, final ArrayList<Normal> n,
			final ArrayList<Face> f) {
		// Set the vertex list
		this.vertexList = v;
		// Set the normal list
		this.normalList = n;
		// Set the face list
		this.faceList = f;
		// Initialize the point list
		this.pointList = new ArrayList<HashMap<Vertex, Face>>(this.vertexList.size());
		// Create the winged edge data structure
		this.createWingedEdgeHash();
		//
		for (Vertex vertex : this.vertexList) {
			vertex.setFirstEdge(vertex.getInvolvedEdges().get(0));
		}
		// Set the border edge status
		for (Edge wingedEdge : this.hashEdges.values()) {
			if (wingedEdge.getE() == null) {
				wingedEdge.getQ().setFirstEdge(wingedEdge);
				wingedEdge.getQ().setPartOfBorderEdge(true);
				continue;
			}
			if (wingedEdge.getF() == null) {
				wingedEdge.getP().setFirstEdge(wingedEdge);
				wingedEdge.getP().setPartOfBorderEdge(true);
				continue;
			}
		}
		// Statistic information about the winged edge data structure
		System.out.println("[[Mesh]] Edges: " + this.hashEdges.size() + " Border Edges: " + this.getHashBorderEdges().size());
		//
		int maxSize = this.pointList.get(0).size();
		int minSize = this.vertexList.size();
		int sum = 0;
		for (HashMap<Vertex, Face> m : this.pointList) {
			if (m.size() > maxSize)
				maxSize = m.size();
			if (m.size() < minSize)
				minSize = m.size();
			sum += m.size();
		}
		/*
		System.out.println("[[Mesh]] Total Number of point list entries (min/avg/max): "
						+ minSize
						+ "/"
						+ (sum / this.pointList.size())
						+ "/"
						+ maxSize);
						*/
		// Create the discrete mean curvature 
		//this.createDiscreteMeanCurvature();
	}

	/**
	 * The function sets the render mode of the graphical object
	 */
	public void setRenderMode(int mode) {
		this.currentRenderMode = mode;
	}

	/**
	 * The function draws the object on to the screen
	 * 
	 * @param drawable
	 */
	@Override
	public void draw(GLAutoDrawable drawable) {
		// Get the OpenGL object
		GL gl = drawable.getGL();
		// Enable vertex arrays
		gl.glEnableClientState(GL.GL_VERTEX_ARRAY);
		// Set matrix mode to model view matrix
		gl.glMatrixMode(GL.GL_MODELVIEW);
		// Create the vertex buffer
		//if (this.vertex == null) {
			this.vertex = BufferUtil.newFloatBuffer(3 * this.vertexList.size());
			for (Vertex v : this.vertexList) {
				this.vertex.put(v.x); this.vertex.put(v.y); this.vertex.put(v.z);
			}
			this.vertex.rewind();
		//}
		// Set the vertex pointer
		gl.glVertexPointer(3, GL.GL_FLOAT, 0, this.vertex);
		// If a normal array exists
		if (this.normalList != null) {
			// Create the normal buffer
			if (this.normal == null) {
				this.normal = BufferUtil.newFloatBuffer(3 * this.vertexList.size());
				for (Vertex vertex : this.vertexList) {
					Normal n = vertex.getNormal();
					if(n != null){
						this.normal.put(n.x); this.normal.put(n.y); this.normal.put(n.z);
					}else{
						this.normal.put(0.0f); this.normal.put(0.0f); this.normal.put(0.0f);
					}
				}
				this.normal.rewind();
			}
			// Enable the normal array
			gl.glEnableClientState(GL.GL_NORMAL_ARRAY);
			// Set the normal pointer
			gl.glNormalPointer(GL.GL_FLOAT, 0, this.normal);
		}
		// Reset the color
		gl.glColor3f(1.0f, 1.0f, 1.0f);
		// Set the line width
		gl.glLineWidth(1.0f);
		// Push matrix for the translate
		gl.glPushMatrix();
		// Move the object
		gl.glTranslatef(this.x, this.y, this.z);
		// Create the face buffer
		if (this.face == null) {
			this.face = BufferUtil.newIntBuffer(3 * this.faceList.size());
			for (Face f : this.faceList) {
				this.face.put(f.getVertex());
			}
			this.face.rewind();
		}
		// Draw
		if (this.currentRenderMode == Renderer.RENDER_MODE_WIRE_FRAME_ON_GOURAUD) {
			gl.glEnable(GL.GL_POLYGON_OFFSET_FILL);
			gl.glPolygonOffset(1.0f, 1.0f);
			gl.glColor3f(1.0f, 1.0f, 1.0f);
			// Draw the object
			gl.glDrawElements(GL.GL_TRIANGLES, this.face.limit(),
					GL.GL_UNSIGNED_INT, this.face);
			gl.glDisable(GL.GL_POLYGON_OFFSET_FILL);

			gl.glPolygonMode(GL.GL_FRONT_AND_BACK, GL.GL_LINE);
			if (this.selected) {
				gl.glColor3f(1.0f, 0.0f, 0.0f);
			} else {
				gl.glColor3f(0.5f, 0.5f, 0.5f);
			}
			gl.glDrawElements(GL.GL_TRIANGLES, this.face.limit(),
					GL.GL_UNSIGNED_INT, this.face);
		} else {
			if (selected) {
				gl.glColor3f(1.0f, 0.0f, 0.0f);
			}
			gl.glDrawElements(GL.GL_TRIANGLES, this.face.limit(),
					GL.GL_UNSIGNED_INT, this.face);
		}
		// Pop matrix
		gl.glPopMatrix();
		
		if (this.normalList != null) {
			gl.glDisableClientState(GL.GL_NORMAL_ARRAY);
		}
		
		// Disable the vertex array
		gl.glDisableClientState(GL.GL_VERTEX_ARRAY);
		// Render the border edges
		this.renderBorderEdges(drawable);
		// Render the discrete mean curvature
		//this.renderDiscreteMeanCurvature(drawable);
	}

	/**
	 * This function draws the border edges of the parsed *.obj object.
	 * 
	 * @param drawable
	 */
	public void renderBorderEdges(GLAutoDrawable drawable) {
		// Get the OpenGL object
		GL gl = drawable.getGL();
		// Get the border edges
		Vector<Edge> borderEdges = this.getHashBorderEdges();
		// Push the model view matrix
		gl.glPushMatrix();
		// Set the line width to 5.0f
		gl.glLineWidth(5.0f);
		gl.glTranslatef(this.x, this.y, this.z);
		// Set the color to green
		gl.glColor3f(0.0f, 1.0f, 0.0f);
		for (Edge wingedEdge : borderEdges) {
			// Draw lines
			gl.glBegin(GL.GL_LINE_STRIP);
			// Get the P edge
			Vertex p = wingedEdge.getP();
			// Get the Q edge
			Vertex q = wingedEdge.getQ();
			// Draw the P edge
			gl.glVertex3f(p.x, p.y, p.z);
			// Draw the Q edge
			gl.glVertex3f(q.x, q.y, q.z);
			gl.glEnd();
		}
		// Pop the model view matrix
		gl.glPopMatrix();
	}

	/**
	 * This function draws the border edges of the parsed *.obj object.
	 * 
	 * @param drawable
	 */
	public void renderDiscreteMeanCurvature(GLAutoDrawable drawable) {
		// Get the OpenGL object
		GL gl = drawable.getGL();
		// Get the border edges
		Vector<Edge> borderEdges = this.getHashBorderEdges();
		// Push the model view matrix
		gl.glPushMatrix();
		// Set the line width to 5.0f
		gl.glLineWidth(5.0f);
		gl.glTranslatef(this.x, this.y, this.z);
		// Set the color to green
		gl.glColor3f(0.0f, 1.0f, 0.0f);
		for (Edge wingedEdge : borderEdges) {
			// Draw lines
			gl.glBegin(GL.GL_LINE_STRIP);
			// Get the P edge
			Vertex p = wingedEdge.getP();
			// Get the Q edge
			Vertex k = wingedEdge.getK();
			// Draw the P edge
			gl.glVertex3f(p.x, p.y, p.z);
			// Draw the K edge
			gl.glVertex3f(k.x, k.y, k.z);
			gl.glEnd();
		}
		// Pop the model view matrix
		gl.glPopMatrix();
	}
	
	
	public Vector<Edge> getHashBorderEdges() {
		Vector<Edge> borderEdges = new Vector<Edge>();
		for (Edge we : this.hashEdges.values()) {
			if (we.getE() == null || we.getF() == null) {
				borderEdges.add(we);
			}
		}
		return borderEdges;
	}
	
	/**
	 * The function calculates the discrete mean curvature of the graphical
	 * object. The description of the formula can be found in
	 *  
	 *  	2009-SS-ICG-TeilC-v1, page 68.
	 * 
	 */
	public void createDiscreteMeanCurvature(){
		// For each edge in the winged edge
		for (Edge wingedEdge : this.hashEdges.values()){
			// Calculate the area of a given point p and it's neighbors
			float area = Geometry.determineArea(wingedEdge.getP(), this.getNeighbours(wingedEdge.getP()));
			// Determine the mean curvature normal of a given point p, it's neighbors and the area
			Vertex vector = this.determineMeanCurvatureNormal(wingedEdge.getP(), this.getNeighbours(wingedEdge.getP()), area);
			// Set the K vector
			wingedEdge.setK(vector);
		}
	}
	
	/**
	 * The function determines the mean curvature normal. The discrete mean cur
	 * 
	 * @param p The vertex p
	 * @param neighborList The neighbors of the vertex p
	 * @param a The size of the area of a given point p and a neighbor list
	 * 
	 * @return 
	 */
	private Vertex determineMeanCurvatureNormal(final Vertex p, final Vector<Vertex> neighborList, float a){
		// Createt a result vector
		Vertex result = new Vertex(0.0f, 0.0f, 0.0f);
		// Iterate through the neighbor list
		for(int i = 0; i < (neighborList.size() - 2); i++){
			try {
				// Determine the angle between two neighbors
				double alpha = Geometry.determineAngle(neighborList.get(i), neighborList.get(i+1), p);
				// Determine the angle between two other neighbors
				double beta  = Geometry.determineAngle(neighborList.get(i+1), neighborList.get(i+2), p);
				// Determine the angle
				double angle = Geometry.cot(alpha) + Geometry.cot(beta);
				// Create a new vertex
				Vertex vertex = new Vertex(p.x, p.y, p.z);
				// 
				vertex.sub(neighborList.get(i));
				
				vertex.x = vertex.x * new Double(angle).floatValue();
				vertex.y = vertex.y * new Double(angle).floatValue();
				vertex.z = vertex.z * new Double(angle).floatValue();
				
				result.add(vertex);
			} catch (IndexOutOfBoundsException e){
				e.printStackTrace();
			}
		}
		
		result.x = result.x * (1/(2 * a));
		result.y = result.y * (1/(2 * a));
		result.z = result.z * (1/(2 * a));
	
		return result;
	}

	/**
	 * The function returns true if the object is selected otherwise false
	 * 
	 * @return The selection status of the object
	 */
	@Override
	public boolean selected() {
		return this.selected;
	}

	/**
	 * The function sets the selection status of the object.
	 * 
	 * @param status
	 *            The new selection status of the object
	 */
	@Override
	public void setSelected(boolean status) {
		this.selected = status;
	}

	/**
	 * The function returns a list of neighbored vertices
	 * of a given vertex
	 * 
	 * @param The vertex whose neighbors should be determined
	 * 
	 * @return A list with the neighbors of the vertex
	 */
	public Vector<Vertex> getNeighbours(Vertex v) {
		Edge F = v.getFirstEdge(), E = F;
		Vector<Vertex> n = new Vector<Vertex>();
		n.add(E.nextVertex(v));
		while ((E != null) && (E.nextEdge(v) != F)) {
			E = E.nextEdge(v);
			if (E != null) {
				if (n.contains(E.nextVertex(v))) {
					// additional break-condition
					break; 
				} else {
					n.add(E.nextVertex(v));
				}
			}
		}
		return n;
	}

	/**
	 * The function returns the face vertices of a given edge and a
	 * given vertex
	 * 
	 * @param firstEdge
	 * @param v 
	 * 
	 * @return A vertex vector with vertices which correspond to a face 
	 */
	public Vector<Vertex> getFaceVertices(Edge firstEdge, Vertex v) {
		Edge ei = firstEdge;
		Vector<Vertex> vertices = new Vector<Vertex>();

		while (ei.nextEdge(v) != firstEdge && ei != null) {
			vertices.add(v);
			v = ei.nextVertex(v);
			ei = ei.nextEdge(v);
			if (ei == firstEdge)
				break;
		}
		return vertices;
	}

	/**
	 * The function returns a list of border edges. A border edge has at least
	 * one edge which is "null".
	 * 
	 * @return A list of border edges
	 */
	public Vector<Edge> getBorderEdges() {
		Vector<Edge> borderEdges = new Vector<Edge>();
		for (Edge wingedEdge : this.edges) {
			if (wingedEdge.getE() == null || wingedEdge.getF() == null) {
				borderEdges.add(wingedEdge);
			}
		}
		return borderEdges;
	}

	/**
	 * The function updates the position of an object
	 * 
	 * @param x The new x coordinate
	 * @param y The new y coordinate
	 * @param z The new z coordinate
	 */
	public void update(float x, float y, float z) {
		this.x += x;
		this.y += y;
		this.z += z;
	}
	
	/**
	 * The function creates the winged edge hash list.
	 * 
	 */
	public void createWingedEdgeHash() {
		int csum = 0;
		// Initialize the point list
		for (int i = 0; i < this.vertexList.size(); i++) {
			this.pointList.add(new HashMap<Vertex, Face>(5));
		}
		// Count the hash collisions
		int collisions = 0;
		// Iterate through the face list
		for (Face f : this.faceList) {
			// Iterate through every edge of the corresponding face
			for (int i = 0; i < 3; i++) {
				// Get the edge
				int[] edge = f.getEdge(i);
				// Get the P edge
				Vertex pi = this.vertexList.get(edge[0]);
				// Get the Q edge
				Vertex qj = this.vertexList.get(edge[1]);
				// Create a hash value
				int hash = this.getEdgeHash(pi, qj);
				// Test if there is already such an hash
				if (!hashEdges.containsKey(hash)) {
					Edge test = hashEdges.put(hash, new Edge(pi, qj));
					if (test != null) {
						// Increase the number of collisions
						collisions++;
					}
				}
			}
		}

		// For each face
		for (Face face : this.faceList) {
			// Iterate through every edge of the corresponding face
			for (int i = 0; i < 3; i++) {
				int[] edge = face.getEdge(i); // current edge <pi,qj>
				// Get the P edge
				Vertex pi = this.vertexList.get(edge[0]);
				// Get the Q edge
				Vertex qj = this.vertexList.get(edge[1]);
				// Point list of the edge P
				HashMap<Vertex, Face> pointListP = this.pointList.get(edge[0]); 
				// Point list of the edge Q
				HashMap<Vertex, Face> pointListQ = this.pointList.get(edge[1]); 
				// If a point list has a key pi or qi
				if(pointListQ.containsKey(pi) || pointListP.containsKey(qj)) {
					// Determine the next edge in the face
					int[] nextEdge = face.getEdge(i + 1); 
					// Find the edge in the list
					Edge e = this.findHashedEdge(edge[0], edge[1]); 
					// Link it to the next edge
					if (e != null){
						e.setE(this.findHashedEdge(nextEdge[0], nextEdge[1])); 
					}else{
						System.err.println("There is a vertex in the point list but not in in the edge list");
					}

					if(pointListQ.containsKey(pi)){
						// Remove it from the point list
						pointListQ.remove(pi); 
					}
					if (pointListP.containsKey(qj)){
						// Remove it from the point list
						pointListP.remove(qj); 
					}
				} else {
					// Find the winged edge in the winged edge hash
					Edge e = this.findHashedEdge(edge[0], edge[1]);
					// Link to next edge
					int[] nextEdge = face.getEdge(i + 1);
					// 
					e.setF(this.findHashedEdge(nextEdge[0], nextEdge[1]));
					// Save into the point list
					Face prev = pointListP.put(qj, face);
					if (prev != null) {
						csum += 1;
					}
				}
			}
		}
	}

	/**
	 * The function creates a hash from two vertices. Bascially the hash
	 * function looks like the following:
	 * 
	 * (Vertex 1 XOR Vertex 2) OR (Vertex 2 XOR Vertex 1)
	 */
	public int getEdgeHash(Vertex v1, Vertex v2) {
		return ((v1.hashCode() ^ v2.hashCode()) | (v2.hashCode() ^ v1
				.hashCode()));
	}

	/**
	 * The function searches for a edge in the winged edge hash
	 * 
	 * @param p
	 * @param q
	 * 
	 * @return The corresponding winged edge
	 */
	public Edge findHashedEdge(final int p, final int q) {
		return this.hashEdges.get(this.getEdgeHash(this.vertexList.get(p),
				this.vertexList.get(q)));
	}

	/**
	 * The function implements the weighted umbrella operator. The operator
	 * is defined as
	 * 
	 *  U_{G}(p) = \frac{1}{\sum_{i=0}^{n-1} \omega_{i}} \sum_{i=0}^{n-1} \omega_{i}p_{i} - p 
	 *  
	 * @param lambda A scale factor for the weighted umbrella operator
	 * 
	 */
	public void weightedUmbrellaOperator(final float lambda) {
		System.out.println("[[Mesh]] Weighted Umbrella Operator called");
			
		// Iterate through the vertex list
		for (Vertex v : this.vertexList) {
			// If the vertex is part of a border edge skip the loop
			if(v.isPartOfBorderEdge()){
				continue;
			}
			// Determine the neighbors of the vertex
			List<Vertex> neighbors = this.getNeighbours(v);
			// If the neighbor list is empty skip the loop
			if(neighbors.isEmpty()){
				continue;
			}
			// Create a new point p which is another representation of the vertex v
			Point3f p = new Point3f(v);
			// Create a 'umbrella' vector
			Vector3f umbrella = new Vector3f();
			// Create a variable which stores the total weight
			float totalWeight = 0.0f;
			// For each neighbor pi of a vertex p 
			for (Vertex neighborVertex : neighbors) { 
				// If the vertex in the neighbor list is empty skip the loop
				if (neighborVertex == null) {
					continue;
				}
				// Create a new point for calculations
				Point3f pi = new Point3f(neighborVertex);
				// The weight is the distance between pi and p
				float weight = p.distance(pi); 
				/**
				 * Formula isp_{i} - p or 
				 * (pi - p)
				 */
				pi.sub(p); 						// p_i - p
				/**
				 * Formula is \omega_{i}p_{i} - p or 
				 * wi * (pi - p)
				 */
				pi.scale(weight); 				// w_i * (p_i - p)
				/**
				 * Formula is \sum_{i=0}^{n-1} \omega_{i}p_{i} - p 
				 * Ug(v) = Ug(v) + (wi * (pi - p))
				 */
				umbrella.add(pi); 				
				// Add the weight to the total weight
				totalWeight += weight;
			}
			/**
			 * Scale the umbrella vector, whereas the formula is
			 * 
			 * \frac{1}{\sum_{i=0}^{n-1}\omega_{i}} * \sum_{i=0}^{n-1} \omega_{i}p_{i} - p 
			 * 
			 * or in a simpler way
			 * 
			 * Ug(p) = 1/totalWeight * Ug(p)
			 *  
			 */
			umbrella.scale(1.0f/totalWeight);
			/**
			 * Scale the umbrella operator for the new edge. The formula is
			 * 
			 *  pnew = p + lambda * umbrella
			 *  
			 * This operation represents lambda * umbrella
			 */
			umbrella.scale(lambda); 	
			/**
			 * Add the umbrella operator to the old edge. 
			 * 
			 *  pnew = p + lambda * umbrella
			 */
			v.add(umbrella); 	

		}
	}
}
