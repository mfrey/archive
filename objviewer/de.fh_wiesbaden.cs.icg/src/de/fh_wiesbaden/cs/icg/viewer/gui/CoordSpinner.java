package de.fh_wiesbaden.cs.icg.viewer.gui;
import javax.swing.*;
import java.awt.event.*;

public class CoordSpinner
extends JSpinner
implements MouseWheelListener {
	private static final long serialVersionUID = 1L;
	private static final float MIN_VAL = -100f;
	private static final float MAX_VAL = +100f;
	private static final float STEP = +0.5f;
	private SpinnerNumberModel model;
	
	public CoordSpinner(float value, float min, float max, float step) {
		super();
		try {
			this.model = new SpinnerNumberModel(value, min, max, step);
			this.setModel(this.model);
			this.addMouseWheelListener(this);
			
		} catch (Exception e) {}
	}
	
	public CoordSpinner(float value) {
		super();
		try {
			this.model = new SpinnerNumberModel(value, MIN_VAL, MAX_VAL, STEP);
			this.setModel(this.model);
			this.addMouseWheelListener(this);
			
		} catch (Exception e) {}
	}

	@Override
	public void mouseWheelMoved(MouseWheelEvent e) {
		int absCount = Math.abs(e.getWheelRotation());
		for (int i=0; i<absCount; i++) {
			try {
				if (e.getWheelRotation() < 0) {
					this.setValue(this.getModel().getNextValue());
				} else {
					this.setValue(this.getModel().getPreviousValue());
				}
			} catch (IllegalArgumentException ex) { }
		}		
	}
}