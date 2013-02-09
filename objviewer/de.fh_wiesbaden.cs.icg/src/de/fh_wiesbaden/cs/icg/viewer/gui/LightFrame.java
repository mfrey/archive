package de.fh_wiesbaden.cs.icg.viewer.gui;


import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.util.ArrayList;
import java.util.List;

import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import de.fh_wiesbaden.cs.icg.viewer.Light;
import de.fh_wiesbaden.cs.icg.viewer.Renderer;

/**
 * The class implements a frame which shows data for the light control 
 * mechanism. 
 * 
 * @author Michael Frey
 */
public class LightFrame extends Frame {
	/**
	 * A auto generated serial version uid
	 */
	private static final long serialVersionUID = 8598193448903436906L;
	/**
	 * A reference to the renderer control
	 */
	private Renderer renderer;
	/**
	 * 
	 */
	private Control control;
	
	/**
	 * 
	 */
	public LightFrame(Renderer r){
		// The width of the window
		int width = 300;
		// The height of the window
		int height = 480;
		// Set the default size of the window
		this.setSize(width, height);
		// Set the reference to the renderer
		this.renderer = r;
		// Create a new control object
		this.control = new Control(this.renderer);

		GridBagLayout gbl = new GridBagLayout();
		this.setLayout(gbl);

		int line = 0;

		Frame.addComponent(this, gbl, new JLabel("Configure light"), 0, line++,
				1, 1, 0, 0, GridBagConstraints.BOTH);
		Frame.addComponent(this, gbl, this.control, 0, line++, 1, 1, 0, 0,
				GridBagConstraints.BOTH);
	}
	
	class Control extends JPanel implements ChangeListener {
		/**
		 * A auto generated serial version uid
		 */
		private static final long serialVersionUID = -4246624495595441063L;

		
		private List<CoordControl3> coordControls = new ArrayList<CoordControl3>();
		private CoordSpinner cutOffSpinnerOne;
		private CoordSpinner cutOffSpinnerTwo;

		private Light ls;
		
		public Control(Renderer renderer){
			this.ls = renderer.getLight(0);
			coordControls.add(new CoordControl3(renderer.getLight(0).getPosition()));
			coordControls.add(new CoordControl3(renderer.getLight(0).getDirection()));
			coordControls.add(new CoordControl3(renderer.getLight(1).getPosition()));
			coordControls.add(new CoordControl3(renderer.getLight(1).getDirection()));

			/*
			coordControls.add(new CoordControl2(sv.getLeftRightPoint()));
			coordControls.add(new CoordControl2(sv.getBottomTopPoint()));
			coordControls.add(new CoordControl2(sv.getNearFarPoint()));
			*/
			GridBagLayout gbl = new GridBagLayout();
			this.setLayout(gbl);
			
			int i=0;
			cutOffSpinnerOne = new CoordSpinner(renderer.getLight(0).getCutOffAngle());
			cutOffSpinnerOne.addChangeListener(this);
			cutOffSpinnerTwo = new CoordSpinner(renderer.getLight(1).getCutOffAngle());
			cutOffSpinnerTwo.addChangeListener(this);
			
			SelectionFrame.addComponent(this, gbl, new JLabel("Spotlight 1: "), 0, i++, 5, 1, 0, 0, GridBagConstraints.NONE);
			SelectionFrame.addComponent(this, gbl, new JLabel("Position: "), 0, i++, 5, 1, 0, 0, GridBagConstraints.NONE);
			SelectionFrame.addComponent(this, gbl, coordControls.get(0),      1, i++, 1, 1, 0, 0, GridBagConstraints.NONE);
			SelectionFrame.addComponent(this, gbl, new JLabel("Direction: "), 0, i++, 5, 1, 0, 0, GridBagConstraints.NONE);
			SelectionFrame.addComponent(this, gbl, coordControls.get(1),      1, i++, 1, 1, 0, 0, GridBagConstraints.NONE);
			SelectionFrame.addComponent(this, gbl, new JLabel("Cut-Off:"), 0, i++, 1, 1, 0, 0, GridBagConstraints.NONE);
			SelectionFrame.addComponent(this, gbl, cutOffSpinnerOne,          1, i++, 1, 1, 0, 0, GridBagConstraints.NONE);
			SelectionFrame.addComponent(this, gbl, new JLabel("Spotlight 2: "), 0, i++, 5, 1, 0, 0, GridBagConstraints.NONE);
			SelectionFrame.addComponent(this, gbl, coordControls.get(2),      1, i++, 1, 1, 0, 0, GridBagConstraints.NONE);
			SelectionFrame.addComponent(this, gbl, new JLabel("Direction: "), 0, i++, 5, 1, 0, 0, GridBagConstraints.NONE);
			SelectionFrame.addComponent(this, gbl, coordControls.get(3),      1, i++, 1, 1, 0, 0, GridBagConstraints.NONE);
			SelectionFrame.addComponent(this, gbl, new JLabel("Cut-Off:"), 0, i++, 1, 1, 0, 0, GridBagConstraints.NONE);
			SelectionFrame.addComponent(this, gbl, cutOffSpinnerTwo,          1, i++, 1, 1, 0, 0, GridBagConstraints.NONE);
			/*
			SelectionFrame.addComponent(this, gbl, new JLabel("zNear/zFar:"), 0, i++, 5, 1, 0, 0, GridBagConstraints.NONE);
			SelectionFrame.addComponent(this, gbl, coordControls.get(2),      1, i++, 1, 1, 0, 0, GridBagConstraints.NONE);
			*/
		}
		
		@Override
		public void stateChanged(ChangeEvent e) {
			this.ls.setCutOffAngle(Float.parseFloat(this.cutOffSpinnerOne.getValue().toString()));
			
		}
		
	}
}
