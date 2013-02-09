package de.fh_wiesbaden.cs.icg.renderable;

import javax.media.opengl.GL;
import javax.media.opengl.GLAutoDrawable;

//import com.sun.opengl.util.GLUT;
import com.jogamp.opengl.util.gl2.GLUT;

public class CoordinateSystem implements Renderable {
	private float AXIS_LENGTH = 10.0f;
	@Override
	public void draw(GLAutoDrawable drawable) {
		// Get the OpenGL object
		GL gl = drawable.getGL();
		GLUT glut = new GLUT();

		gl.glBegin(GL.GL_LINES);
			gl.glColor3f(1.0f, 0.0f, 0.0f);
			gl.glVertex3f(-1*AXIS_LENGTH, 0.0f, 0.0f);
			gl.glVertex3f(AXIS_LENGTH, 0.0f, 0.0f);			

			gl.glColor3f(0.0f, 1.0f, 0.0f);
			gl.glVertex3f(0.0f, -1*AXIS_LENGTH, 0.0f);
			gl.glVertex3f(0.0f, AXIS_LENGTH, 0.0f);
			
			gl.glColor3f(0.0f, 0.0f, 1.0f);
			gl.glVertex3f(0.0f, 0.0f, -1*AXIS_LENGTH);
			gl.glVertex3f(0.0f, 0.0f, AXIS_LENGTH);
		gl.glEnd();
		
		gl.glPushMatrix();
		gl.glTranslatef(AXIS_LENGTH, 0.0f, 0.0f);
		gl.glColor3f(1.0f, 0.0f, 0.0f);
		gl.glRasterPos2f(0, 0);
		glut.glutBitmapString(GLUT.BITMAP_9_BY_15, new String("+x"));
		gl.glTranslatef(-0.5f, 0f, 0f);
		gl.glRotatef(90, 0.0f, 1.0f, 0.0f);
			gl.glBegin(GL.GL_TRIANGLES);
				glut.glutSolidCone(0.25f, 0.75f, 10, 10);
			gl.glEnd();
		gl.glPopMatrix();
		
		gl.glPushMatrix();
		gl.glTranslatef(0.0f, AXIS_LENGTH, 0.0f);
		gl.glColor3f(0.0f, 1.0f, 0.0f);
		gl.glRasterPos2f(0, 0);
		glut.glutBitmapString(GLUT.BITMAP_9_BY_15, new String("+y"));
		gl.glTranslatef(0f, -0.5f, 0f);
		gl.glRotatef(-90, 1.0f, 0.0f, 0.0f);
			gl.glBegin(GL.GL_TRIANGLES);
				glut.glutSolidCone(0.25f, 0.75f, 10, 10);
			gl.glEnd();
		gl.glPopMatrix();
		
		gl.glPushMatrix();
		gl.glTranslatef(0.0f, 0.0f, AXIS_LENGTH);
		gl.glColor3f(0.0f, 0.0f, 1.0f);
		gl.glRasterPos2f(0, 0);
		glut.glutBitmapString(GLUT.BITMAP_9_BY_15, new String("+z"));
		gl.glTranslatef(0f, 0f, -0.5f);
		//gl.glRotatef(-90, 1.0f, 0.0f, 0.0f);
			gl.glBegin(GL.GL_TRIANGLES);
				glut.glutSolidCone(0.25f, 0.75f, 10, 10);
			gl.glEnd();
		gl.glPopMatrix();
	}

	@Override
	public boolean selected() {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public void setRenderMode(int mode) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void setSelected(boolean value) {
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
