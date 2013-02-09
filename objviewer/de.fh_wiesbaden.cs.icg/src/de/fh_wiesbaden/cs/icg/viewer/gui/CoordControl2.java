package de.fh_wiesbaden.cs.icg.viewer.gui;

import java.awt.FlowLayout;
import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.vecmath.*;

public class CoordControl2
extends JPanel
implements ChangeListener {
	private static final long serialVersionUID = 1L;
	private Tuple2f coord;
	
	private CoordSpinner [] spinners = new CoordSpinner[2];
	
	public CoordControl2(Tuple2f coord) {
		super();
		this.coord = coord;
		this.setLayout(new FlowLayout(FlowLayout.LEFT));
		this.spinners[0] = new CoordSpinner(this.coord.getX());
		this.spinners[1] = new CoordSpinner(this.coord.getY());
		
		for (int i=0; i< spinners.length; i++) {
			this.add(spinners[i]);
			spinners[i].addChangeListener(this);
		}
	}

	@Override
	public void stateChanged(ChangeEvent e) {
		this.coord.set(
				Float.parseFloat(spinners[0].getValue().toString()),
				Float.parseFloat(spinners[1].getValue().toString())
		);
		
	}
	

}