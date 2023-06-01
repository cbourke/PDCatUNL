package unl.soc.password_cracker;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

/**
 * A class that holds utility methods.
 *
 */
public class Dictionary {

	/**
	 * A list of dictionary words.
	 */
	public static final List<String> WORDS = new ArrayList<>(247406);

	static {
		try (Scanner s = new Scanner(new File("data/dictionary.txt"))) {
			while (s.hasNext()) {
				WORDS.add(s.nextLine().trim());
			}
		} catch (FileNotFoundException e) {
			throw new RuntimeException(e);
		}
	}
}
