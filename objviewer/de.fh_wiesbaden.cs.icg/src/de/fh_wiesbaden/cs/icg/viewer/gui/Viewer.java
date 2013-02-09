package de.fh_wiesbaden.cs.icg.viewer.gui;

import java.awt.BorderLayout;

import java.awt.Dimension;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;

import javax.media.opengl.*;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;

import com.sun.opengl.util.Animator;

import de.fh_wiesbaden.cs.icg.viewer.Renderer;
import de.fh_wiesbaden.cs.icg.viewer.parser.Parser;

/**
 * The class implements the graphical user interface of the OpenGL viewer
 * 
 * @author Michael Frey
 */
public class Viewer extends JFrame implements ActionListener {
	/**
	 * A file chooser dialog for reading a configuration file
	 */
	private JFileChooser objectFileChooser;
	/**
	 * A file chooser dialog for reading a configuration file
	 */
	private JFileChooser shaderFileChooser;
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	/**
	 * A GLCanvas object which allows to render OpenGL content in a swing object
	 */
	private GLCanvas canvas;
	/**
	 * 
	 */
	private Renderer renderer;
	/**
	 * 
	 */
	private Animator animator;

	private SelectionFrame selectionFrame;
	
	private LightFrame lightFrame;
	
	private UmbrellaFrame umbrellaFrame;
	
	/**
	 * 
	 * 
	 */
	public Viewer() {
		// Set the default close operation
		this.setDefaultCloseOperation(EXIT_ON_CLOSE);
		// The width of the window
		int width = 1024;
		// The height of the window
		int height = 768;
		// Set the default size of the window
		this.setSize(width, height);
		// Create a new renderer object for interaction
		this.renderer = new Renderer(new Dimension(width, height));
		//
		this.selectionFrame = new SelectionFrame(this.renderer);
		//
		this.lightFrame = new LightFrame(this.renderer);
		//
		this.umbrellaFrame = new UmbrellaFrame(this.renderer);
		//
		canvas = new GLCanvas();
		//
		canvas.addGLEventListener(renderer);
		//
		canvas.addMouseListener(renderer);
		//
		canvas.addMouseMotionListener(renderer);
		//
		canvas.addKeyListener(renderer);
		// Create a new file chooser
		objectFileChooser = new JFileChooser();
		// Create a new *.obj file filter
		OBJFileFilter fileFilter = new OBJFileFilter();
		// Set the file filter
		objectFileChooser.setFileFilter(fileFilter);
		//
		this.shaderFileChooser = new JFileChooser();
		ShaderFileFilter shaderFileFilter = new ShaderFileFilter();
		this.shaderFileChooser.setFileFilter(shaderFileFilter);
		
		
		// Create a new JMenuBar
		JMenuBar menuBar = this.initializeMenu();
		// Set the menu bar
		this.setJMenuBar(menuBar);
		//
		this.add(canvas, BorderLayout.CENTER);
		//
		animator = new Animator(canvas);
		//
		this.addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent e) {
				new Thread(new Runnable() {
					public void run() {
						animator.stop();
						System.exit(0);
					}
				}).start();
			}
		});
		animator.start();
	}

	/**
	 * The method creates a new JMenuBar object for the graphical user
	 * interface.
	 * 
	 * @return A JMenuBar object for the graphical user interface
	 */
	private JMenuBar initializeMenu() {
		// Create a new menu bar
		JMenuBar menuBar = new JMenuBar();
		// Create a file menu entry
		JMenu fileMenu = new JMenu("File");
		// Create a settings menu entry
		JMenu settingsMenu = new JMenu("Settings");
		// Add the file menu
		menuBar.add(fileMenu);
		// Add the edit menu
		menuBar.add(settingsMenu);
		// Add a open file entry
		JMenuItem item = new JMenuItem("Open *.obj file");
		// Add a action listener to the open file action
		item.addActionListener(new OpenFileAction());
		// Add the open entry to the file menu
		fileMenu.add(item);
		//
		item = new JMenuItem("Open vertex shader file");
		item.addActionListener(new OpenVertexShaderFileAction());
		// Add the open entry to the file menu
		fileMenu.add(item);
		item = new JMenuItem("Open fragment shader file");
		item.addActionListener(new OpenFragmentShaderFileAction());
		// Add the open entry to the file menu
		fileMenu.add(item);
		
		// Add a exit entry
		item = new JMenuItem("Exit");
		// Add the exit entry to the file menu
		fileMenu.add(item);
		item.addActionListener(this);
		// Show the the coordinate system option
		item = new JMenuItem("Enable/disable coordinate system");
		// Add the coordinate system entry to the settings system
		settingsMenu.add(item);
		// Set the action listener to it
		item.addActionListener(this);
		// Show the the coordinate system option
		item = new JMenuItem("Enable/disable z buffer");
		// Add the z buffer entry to the settings system
		settingsMenu.add(item);
		// Set the action listener to it
		item.addActionListener(this);
		
		// Show the the render shader option
		item = new JMenuItem("Render Shader");
		// Add the z buffer entry to the settings system
		settingsMenu.add(item);
		// Set the action listener to it
		item.addActionListener(this);
		
		// Create a new sub menu item
		JMenu configure = new JMenu("Configure");
		// Show the the configure selection option
		item = new JMenuItem("Light");
		// Add the z buffer entry to the settings system
		configure.add(item);
		// Set the action listener to it
		item.addActionListener(this);
		// Show the the configure selection option
		item = new JMenuItem("Selection");
		// Add the z buffer entry to the settings system
		configure.add(item);
		// Set the action listener to it
		item.addActionListener(this);
		// Show the the umbrella operater lambda option
		item = new JMenuItem("Umbrella Operator Lambda");
		// Add the z buffer entry to the settings system
		configure.add(item);
		// Set the action listener to it
		item.addActionListener(this);
		
		// Create a new sub menu item
		JMenu display = new JMenu("Display");
		item = new JMenuItem("Wireframe");
		// Set the action listener to it
		item.addActionListener(this);
		display.add(item);
		item = new JMenuItem("Flat shading");
		// Set the action listener to it
		item.addActionListener(this);
		display.add(item);
		item = new JMenuItem("Gouraud shading");
		// Set the action listener to it
		item.addActionListener(this);
		display.add(item);
		item = new JMenuItem("Wireframe on Gouraud shading");
		// Set the action listener to it
		item.addActionListener(this);
		display.add(item);
		//
		settingsMenu.add(configure);
		// Add the display sub menu entry to the settings menu
		settingsMenu.add(display);
		// Return the menu bar
		return menuBar;
	}

	/**
	 * The class implements a open file action.
	 */
	private class OpenFileAction implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			// Handle open button action.
			int returnVal = objectFileChooser.showOpenDialog(Viewer.this);
			// If a file was selected
			if (returnVal == JFileChooser.APPROVE_OPTION) {
				// Open the file
				File file = objectFileChooser.getSelectedFile();
				// Pass the file to the parser
				Parser parser = new Parser(file);
				// Read the configuration file
				parser.read();
				// Add the parsed object to the renderer
				renderer.addRenderableObject(parser.getMesh());
			} else {

			}
		}
	}
	
	/**
	 * The class implements a open vertex shader file action.
	 */
	private class OpenVertexShaderFileAction implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			// Handle open button action.
			int returnVal = shaderFileChooser.showOpenDialog(Viewer.this);
			// If a file was selected
			if (returnVal == JFileChooser.APPROVE_OPTION) {
				// Open the file
				File file = shaderFileChooser.getSelectedFile();
				renderer.setVertexShaderFile(file);
			} else {

			}
		}
	}
	
	/**
	 * The class implements a open fragment shader file action.
	 */
	private class OpenFragmentShaderFileAction implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			// Handle open button action.
			int returnVal = shaderFileChooser.showOpenDialog(Viewer.this);
			// If a file was selected
			if (returnVal == JFileChooser.APPROVE_OPTION) {
				// Open the file
				File file = shaderFileChooser.getSelectedFile();
				// Add the parsed object to the renderer
				renderer.setFragmentShaderFile(file);
			} else {

			}
		}
	}

	/**
     * 
     */
	public static void main(String[] args) {
		// Determine the size of the screen
		// Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
		// Set the look and feel of the application to the system 'default'
		try {
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		} catch (InstantiationException e) {
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			e.printStackTrace();
		} catch (UnsupportedLookAndFeelException e) {
			e.printStackTrace();
		}
		// Create a new OpenGL viewer
		Viewer viewer = new Viewer();
		// Show the window
		viewer.setVisible(true);
	}

	/**
	 * The function is invoked when an action occurs.
	 * 
	 * @param e
	 *            A semantic event which indicates that a component-defined
	 *            action has occured
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		JMenuItem source = (JMenuItem) (e.getSource());

		if (source.getText().equals("Enable/disable coordinate system")) {
			// Enable or disable the coordinate system
			this.renderer.showCoordinateSystem();
		} else if (source.getText().equals("Enable/disable z buffer")) {
			// Enable or disable the z buffer
			this.renderer.setBuffer();
		} else if (source.getText().equals("Wireframe")) {
			// Set the render mode to wire frame
			this.renderer.setCurrenRenderMode(Renderer.RENDER_MODE_WIRE_FRAME);
		} else if (source.getText().equals("Flat shading")) {
			// Set the render mode to flat shading
			this.renderer.setCurrenRenderMode(Renderer.RENDER_MODE_FLAT);
		} else if (source.getText().equals("Gouraud shading")) {
			// Set the render mode to gouraud shading
			this.renderer.setCurrenRenderMode(Renderer.RENDER_MODE_GOURAUD);
		} else if (source.getText().equals("Wireframe on Gouraud shading")) {
			// Set the render mode to wire frame on gouraud shading
			this.renderer
					.setCurrenRenderMode(Renderer.RENDER_MODE_WIRE_FRAME_ON_GOURAUD);
		} else if (source.getText().equals("Light")) {
			this.lightFrame.setVisible(true);
		} else if (source.getText().equals("Selection")) {
			this.selectionFrame.setVisible(true);
		} else if (source.getText().equals("Exit")){
			System.exit(0);
		} else if (source.getText().equals("Render Shader")) {
			this.renderer.renderShader();
		} else if (source.getText().equals("Umbrella Operator Lambda")) {
			this.umbrellaFrame.setVisible(true);
		}


	}
}
