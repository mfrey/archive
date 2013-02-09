package de.fh_wiesbaden.cs.icg.viewer.gui;


import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.util.ArrayList;
import java.util.List;

import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import de.fh_wiesbaden.cs.icg.renderable.Volume;
import de.fh_wiesbaden.cs.icg.viewer.Renderer;

/**
 * The class implements a frame which shows data for the selection control 
 * mechanism. Basically the frame shows six spinners (three pairs) for each
 * coordinate of the selection box.
 * 
 * @author Michael Frey
 */
public class SelectionFrame extends Frame {
	/**
	 * A generated serial version uid
	 */
	private static final long serialVersionUID = 2040628467774600664L;
	/**
	 * A reference to the renderer class
	 */
	private Renderer renderer;

	private Control selCtrl;

	public SelectionFrame(Renderer r) {
		// The width of the window
		int width = 320;
		// The height of the window
		int height = 200;
		// Set the default size of the window
		this.setSize(width, height);
		// A reference to the renderer class
		this.renderer = r;

		selCtrl = new Control(this.renderer.getSelectionVolume());

		GridBagLayout gbl = new GridBagLayout();
		this.setLayout(gbl);

		int line = 0;

		Frame.addComponent(this, gbl, new JLabel("Selection-Volume"), 0, line++,
				1, 1, 0, 0, GridBagConstraints.BOTH);
		Frame.addComponent(this, gbl, this.selCtrl, 0, line++, 1, 1, 0, 0,
				GridBagConstraints.BOTH);

	}
	
	class Control extends JPanel implements ChangeListener {
		private static final long serialVersionUID = 1L;
		
		private List<CoordControl2> coordControls = new ArrayList<CoordControl2>();
		
		public Control(Volume sv) {		
			coordControls.add(new CoordControl2(sv.getLeftRightPoint()));
			coordControls.add(new CoordControl2(sv.getBottomTopPoint()));
			coordControls.add(new CoordControl2(sv.getNearFarPoint()));
			
			GridBagLayout gbl = new GridBagLayout();
			this.setLayout(gbl);
			
			int i=0;
			
			SelectionFrame.addComponent(this, gbl, new JLabel("Left/Right:"), 0, i++, 5, 1, 0, 0, GridBagConstraints.NONE);
			SelectionFrame.addComponent(this, gbl, coordControls.get(0),      1, i++, 1, 1, 0, 0, GridBagConstraints.NONE);
			
			SelectionFrame.addComponent(this, gbl, new JLabel("Bottom/Top:"), 0, i++, 5, 1, 0, 0, GridBagConstraints.NONE);
			SelectionFrame.addComponent(this, gbl, coordControls.get(1),      1, i++, 1, 1, 0, 0, GridBagConstraints.NONE);
			
			SelectionFrame.addComponent(this, gbl, new JLabel("zNear/zFar:"), 0, i++, 5, 1, 0, 0, GridBagConstraints.NONE);
			SelectionFrame.addComponent(this, gbl, coordControls.get(2),      1, i++, 1, 1, 0, 0, GridBagConstraints.NONE);
		}

		@Override
		public void stateChanged(ChangeEvent e) {		
		}

	}
}
