package de.fh_wiesbaden.cs.icg.viewer.gui;

import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.util.ArrayList;
import java.util.List;

import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import de.fh_wiesbaden.cs.icg.viewer.Renderer;

/**
 * The class implements a frame which shows settings for the umbrella
 * operator. 
 * 
 * @author Michael Frey
 */
public class UmbrellaFrame extends Frame {
	/**
	 * A reference to the renderer control
	 */
	private Renderer renderer;
	/**
	 * 
	 */
	private Control control;
	
	public UmbrellaFrame(Renderer r){
		// The width of the window
		int width = 300;
		// The height of the window
		int height = 480;
		// Set the default size of the window
		this.setSize(width, height);
		// Set the reference to the renderer
		this.renderer = r;
		this.control = new Control(this.renderer);
		// Add a new panel
		// this.add(lightMenu, BorderLayout.SOUTH);
		GridBagLayout gbl = new GridBagLayout();
		this.setLayout(gbl);

		int line = 0;

		Frame.addComponent(this, gbl, new JLabel("Configure Lambda"), 0, line++,
				1, 1, 0, 0, GridBagConstraints.BOTH);
		Frame.addComponent(this, gbl, this.control, 0, line++, 1, 1, 0, 0,
				GridBagConstraints.BOTH);
	}
	
	class Control extends JPanel implements ChangeListener {	
		/**
		 * 
		 */
		private static final long serialVersionUID = -3645000214056825469L;
		private CoordSpinner lambdaSpinner;
		/**
		 * 
		 */
		private Renderer renderer;
		
		public Control(Renderer r){
			this.lambdaSpinner = new CoordSpinner(r.getLambda(), 0.0f, 1.0f, 0.1f);
			this.lambdaSpinner.addChangeListener(this);
			GridBagLayout gbl = new GridBagLayout();
			this.setLayout(gbl);
			this.renderer = r;
			int i=0;
		
			UmbrellaFrame.addComponent(this, gbl, new JLabel("Operator: "), 0, i++, 5, 1, 0, 0, GridBagConstraints.NONE);
			UmbrellaFrame.addComponent(this, gbl, this.lambdaSpinner, 1, i++, 1, 1, 0, 0, GridBagConstraints.NONE);
		}
		
		@Override
		public void stateChanged(ChangeEvent e) {
			this.renderer.setLambda(Float.parseFloat(this.lambdaSpinner.getValue().toString()));
		}
	}

	
}
