package de.fh_wiesbaden.cs.icg.viewer;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;

import javax.media.opengl.GL;
import javax.media.opengl.GL2;
import javax.media.opengl.GLAutoDrawable;

/**
 * The class implements a shader load mechanism for the 
 * OpenGL Viewer.
 * 
 * @author Michael Frey
 */
public class Shader {
    /**
     * The vertex shader program
     */
	private String vertexShaderProgram = new String("");
    /**
     * The fragment shader program
     */
	private String fragmentShaderProgram = new String("");
    /**
     * The vertex shader file
     */
	private File vertexShaderFile;
    /**
     * The fragment shader file
     */
	private File fragmentShaderFile;
	
    /**
     * 
     */
    public static void validateShaderExtensions(GL gl) {
        if(!gl.isExtensionAvailable("GL_ARB_vertex_shader") && !gl.isExtensionAvailable("GL_ARB_fragment_shader")){
            System.out.println("ARB_vertex_shader extension is not supported.");
            System.out.println("ARB_fragment_shader extension is not supported.");
        } else if (gl.isExtensionAvailable("GL_ARB_vertex_shader") && gl.isExtensionAvailable("GL_ARB_vertex_shader")){
            System.out.println("ARB_vertex_shader extension is supported continuing!");
            System.out.println("ARB_fragment_shader extension is supported continuing!");
        }
    }
    
    /**
     * 
     */
    public String readShaderSourceFile(final File file) {
        String src = null;
        try {
            FileReader fr = new FileReader(file);
            int size = (int) file.length();
            char buff[] = new char[size];
            int len =  fr.read(buff);
            src = new String(buff,0,len);
            fr.close();
        } catch(IOException e) {
            System.err.println(e);
            System.exit(-1);
        }
        return src;
    }
    
    
    /**
     * 
     */
    public void printShaderInfoLog(GL2 gl, int id){
    	//
    	int[] check = new int[1];
    	//
        gl.glGetObjectParameterivARB(id, GL.GL_OBJECT_INFO_LOG_LENGTH_ARB, check, 0);
        int logLength = check[0];
        if ( logLength <= 1 ) {
            System.out.println("[[Shader]] Shader Object Info Log is Clean");
            return;
        }
        byte[] compilecontent = new byte[logLength+1];
        
        gl.glGetInfoLogARB(id,logLength,check,0,compilecontent,0);
        String infolog = new String(compilecontent);
        System.err.println("\nInfo Log of Shader Object ID: " + id);
        System.err.println("--------------------------");
        System.err.println(infolog);
        System.err.println("--------------------------");	
    }
    
    /**
     *  
     */
    public void setVertexShaderFile(File file){
    	this.vertexShaderFile = file;
    	//
    	if(this.fragmentShaderFile != null){
    		this.vertexShaderProgram = this.readShaderSourceFile(vertexShaderFile);
    		this.fragmentShaderProgram = this.readShaderSourceFile(fragmentShaderFile);
    	}
    }
    
    /**
     *  
     */
    public void setFragmentShaderFile(File file){
    	this.fragmentShaderFile = file;
    	//
    	if(this.vertexShaderFile != null){
    		this.vertexShaderProgram = this.readShaderSourceFile(vertexShaderFile);
    		this.fragmentShaderProgram = this.readShaderSourceFile(fragmentShaderFile);
    	}
    }
    
    /**
     *  
     */
	public void loadShader(GLAutoDrawable drawable) {
		// Get the OpenGL object
		GL2 gl = drawable.getGL().getGL2(); 
		// Create the vertex shader 
		int vertexShader = gl.glCreateShader(GL.GL_VERTEX_SHADER);
		// Create the fragment shader
		int fragmentShader = gl.glCreateShader(GL.GL_FRAGMENT_SHADER);
		// Get the shader source for the vertex shader
		gl.glShaderSource(vertexShader, 1, new String[]{vertexShaderProgram}, new int[]{vertexShaderProgram.length()},0);
		// Compile the vertex shader program
		gl.glCompileShader(vertexShader);
		// Print information about the vertex shader program
		this.printShaderInfoLog(gl, vertexShader);
		// Get the shader source for the fragment shader
		gl.glShaderSource(fragmentShader, 1, new String[]{fragmentShaderProgram}, new int[]{fragmentShaderProgram.length()},0);
		// Compile the fragment shader program
		gl.glCompileShader(fragmentShader);
		// Print information about the fragment shader program
		this.printShaderInfoLog(gl, fragmentShader);
		// Create the overall shader program
		int shaderProgram = gl.glCreateProgram();
		// Attach the vertex shader to the shader program
		gl.glAttachShader(shaderProgram, vertexShader);
		// Attach the fragment shader to the shader program
		gl.glAttachShader(shaderProgram, fragmentShader);
		// Link the shader
		gl.glLinkProgram(shaderProgram);
		// Validate the shader
		gl.glValidateProgram(shaderProgram);
		// Use the shader
		gl.glUseProgram(shaderProgram);
	}
}
