package unl.soc;

import java.util.Random;

public class Utils {

	public static final Random SOURCE = new Random();

	public static double randomRange(double rangeMin, double rangeMax) {
		return rangeMin + (rangeMax - rangeMin) * SOURCE.nextDouble();
	}

}
