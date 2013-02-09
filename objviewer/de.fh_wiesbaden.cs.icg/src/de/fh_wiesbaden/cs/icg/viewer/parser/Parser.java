package de.fh_wiesbaden.cs.icg.viewer.parser;


import de.fh_wiesbaden.cs.icg.renderable.*;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;

import de.fh_wiesbaden.cs.icg.math.Face;
import de.fh_wiesbaden.cs.icg.math.Normal;
import de.fh_wiesbaden.cs.icg.math.Vertex;

/**
 * This class implements a object file parser
 * 
 * @author Michael Frey
 */
public class Parser {
	private File file;
	private Mesh mesh;

	public Parser(final File f) {
		this.file = f;
	}

	/**
	 * The function reads a object file. There is an official specification
	 * about the object file format which could be found here:
	 * 
	 * http://local.wasp.uwa.edu.au/~pbourke/dataformats/obj/
	 * 
	 * @throws IOException
	 * @throws FileNotFoundException
	 */
	public void read() {
		try {
			BufferedReader reader = new BufferedReader(
					new FileReader(this.file));

			ArrayList<Vertex> vertexList;
			ArrayList<Normal> normalList;
			ArrayList<Face> faceList;

			try {
				vertexList = new ArrayList<Vertex>();
				normalList = new ArrayList<Normal>();
				faceList = new ArrayList<Face>();

				String line = null;

				while ((line = reader.readLine()) != null) {
					String[] result = line.split(" ");

					if (result.length > 0) {
						if (result[0].equals("v")) {
							try {
								vertexList.add(new Vertex(Float
										.parseFloat(result[1]), Float
										.parseFloat(result[2]), Float
										.parseFloat(result[3])));
							} catch (Exception e) {
								e.printStackTrace();
							}
						} else if (result[0].equals("f")) {											
							// A temporary array for storing vertex references
							int[] vertex = new int[3];
							// A temporary array for storing texture references
							int[] texture = new int[3];
							// A normal array for storing normals
							int[] normal = new int[3];

							for (int i = 0; i < 3; i++) {
								// Split the string after the '/'
								String split[] = result[i + 1].split("/");

								if (split.length > 0) {
									if (split[0].length() > 0) {
										vertex[i] = Integer.parseInt(split[0]) - 1;
									} else {
										vertex[i] = 1;
									}
								}

								if (split.length > 1) {
									if (split[1].length() > 0) {
										texture[i] = Integer.parseInt(split[1]) - 1;
									} else {
										texture[i] = 1;
									}
								}

								if (split.length > 2) {
									if (split[2].length() > 0) {
										normal[i] = Integer.parseInt(split[2]) - 1;
									} else {
										normal[i] = 1;
									}
								}
								if(normalList.size() > 0){
									vertexList.get(vertex[i]).setNormal(normalList.get(normal[i]));
								}
							}
							
							faceList.add(new Face(vertex, texture, normal));
						} else if (result[0].equals("vn")) {
							try {
								normalList.add(new Normal(Float
										.parseFloat(result[1]), Float
										.parseFloat(result[2]), Float
										.parseFloat(result[3])));
							} catch (Exception e) {
								System.out.println("1 " + result);
								// e.printStackTrace();
							}
						} else if (result[0].equals("#")
								|| result[0].equals("vt")) {

						} else {

						}
					}
				}

				
				System.out.println("[[Parser]] Read *.obj file: " + file.getName());
				System.out.println("[[Parser]] Vertices: " + vertexList.size()
						+ " Normals: " + normalList.size() + " Faces: "
						+ faceList.size());
				if(normalList.size() > 0){
					this.mesh = new Mesh(vertexList, normalList, faceList);
				}else{
					this.mesh = new Mesh(vertexList, null, faceList);
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}

	}

	public Mesh getMesh(){
		return this.mesh;
	}
}
