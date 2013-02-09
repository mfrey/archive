package de.fh_wiesbaden.cs.icg.renderable;

import java.nio.ByteBuffer;
import java.nio.FloatBuffer;

import javax.media.opengl.GL;
import javax.media.opengl.GL2;
import javax.media.opengl.GLAutoDrawable;
import javax.vecmath.Point2f;

import com.jogamp.common.nio.Buffers;

public class Volume {
	public Point2f leftRightPoint;
	public Point2f bottomTopPoint;
	public Point2f nearFarPoint;
	
	public Volume(float left, float right, float bottom, float top,
			float near, float far){
		this.leftRightPoint = new Point2f(left, right);
		this.bottomTopPoint = new Point2f(bottom, top);
		this.nearFarPoint = new Point2f(near, far);
	}
	
	public void draw(GLAutoDrawable drawable){
		GL2 gl = drawable.getGL().getGL2();
		
		float[] vertices = new float[] {
			this.leftRightPoint.x, this.bottomTopPoint.y, -1*this.nearFarPoint.x,
			this.leftRightPoint.y, this.bottomTopPoint.y, -1*this.nearFarPoint.x,
			this.leftRightPoint.x, this.bottomTopPoint.x, -1*this.nearFarPoint.x,
			this.leftRightPoint.y, this.bottomTopPoint.x, -1*this.nearFarPoint.x,
			
			this.leftRightPoint.x, this.bottomTopPoint.y, -1*this.nearFarPoint.y,
			this.leftRightPoint.y, this.bottomTopPoint.y, -1*this.nearFarPoint.y,
			this.leftRightPoint.x, this.bottomTopPoint.x, -1*this.nearFarPoint.y,
			this.leftRightPoint.y, this.bottomTopPoint.x, -1*this.nearFarPoint.y,
		};
		
		byte[] indices = new byte[] {
				2,3,1,0,
				3,7,5,1,
				0,1,5,4,
				2,0,4,6,
				2,6,7,3,
				6,4,5,7
		};

		
		FloatBuffer vertexBuf = Buffers.newDirectFloatBuffer(vertices.length);
		vertexBuf.put(vertices);
		vertexBuf.rewind();
		
		ByteBuffer indexBuf = Buffers.newDirectByteBuffer(indices.length);
		indexBuf.put(indices);
		indexBuf.rewind();
		
		
		
		gl.glEnableClientState(GL2.GL_VERTEX_ARRAY);
		gl.glEnableClientState(GL2.GL_INDEX_ARRAY);
		
		gl.glVertexPointer(3, GL.GL_FLOAT, 0, vertexBuf);
		
		gl.glPolygonMode(GL.GL_FRONT_AND_BACK, GL2.GL_LINE);
		gl.glColor3f(1.0f, 1.0f, 1.0f);
		gl.glDrawElements(GL2.GL_QUADS, indices.length, GL.GL_UNSIGNED_BYTE, indexBuf);
		
		gl.glDisableClientState(GL2.GL_INDEX_ARRAY);
		gl.glDisableClientState(GL2.GL_VERTEX_ARRAY);
	}

	public Point2f getLeftRightPoint() {
		return leftRightPoint;
	}

	public void setLeftRightPoint(Point2f leftRightPoint) {
		this.leftRightPoint = leftRightPoint;
	}

	public Point2f getBottomTopPoint() {
		return bottomTopPoint;
	}

	public void setBottomTopPoint(Point2f bottomTopPoint) {
		this.bottomTopPoint = bottomTopPoint;
	}

	public Point2f getNearFarPoint() {
		return nearFarPoint;
	}

	public void setNearFarPoint(Point2f nearFarPoint) {
		this.nearFarPoint = nearFarPoint;
	}
}
