package de.fh_wiesbaden.cs.icg.viewer.gui;

import java.io.File;

import javax.swing.filechooser.FileFilter;

/**
 * This class restricts a JFileChooser to show only glsl files.
 * 
 * @author Michael Frey
 */
public class ShaderFileFilter extends FileFilter {
	/**
	 * The function tests whether or not the specified file is accepted by this
	 * filter.
	 * 
	 * @param f
	 *            The file which should be checked
	 * 
	 * @return True if the file was accepted, false otherwise
	 */
	public boolean accept(File f) {
		return f.isDirectory() || f.getName().toLowerCase().endsWith(".glsl");
	}

	/**
	 * The description of this filter
	 * 
	 * @return The description of the filter
	 */
	public String getDescription() {
		return ".glsl files";
	}
}