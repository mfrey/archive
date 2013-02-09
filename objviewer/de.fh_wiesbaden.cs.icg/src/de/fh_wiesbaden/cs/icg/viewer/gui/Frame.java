package de.fh_wiesbaden.cs.icg.viewer.gui;

import java.awt.Component;
import java.awt.Container;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;

import javax.swing.JFrame;

/**
 * 
 */
public abstract class Frame extends JFrame {
	/**
	 * A generates serial version uid
	 */
	private static final long serialVersionUID = 4872434034609128093L;

	public static void addComponent(Container cont, GridBagLayout gbl,
			Component c, int x, int y, int width, int height, double weightx,
			double weighty, int fill) {
		final GridBagConstraints GBC = new GridBagConstraints();
		GBC.fill = GridBagConstraints.NONE;
		GBC.gridx = x;
		GBC.gridy = y;
		GBC.gridwidth = width;
		GBC.gridheight = height;
		GBC.weightx = weightx;
		GBC.weighty = weighty;
		GBC.insets = new Insets(0, 0, 10, 0);
		GBC.fill = fill;
		GBC.anchor = GridBagConstraints.LINE_START;
		gbl.setConstraints(c, GBC);
		cont.add(c);
	}
}
