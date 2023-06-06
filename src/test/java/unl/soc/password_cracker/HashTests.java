package unl.soc.password_cracker;

import org.junit.jupiter.api.Assertions;
import org.junit.Test;

/**
 * A suite of tests for the {@link CryptoUtils} class.
 * 
 */
public class HashTests {

	/**
	 * Tests that the {@link CryptoUtils#sha256(String)} method correctly hashes the
	 * string <code>"password1234"</code>.
	 * 
	 */
	@Test
	public void hashTest01() {

		String input = "password1234";
		String expectedOutput = "0xb9c950640e1b3740e98acb93e669c65766f6670dd1609ba91ff41052ba48c6f3";
		String actualOutput = CryptoUtils.sha256(input);

		Assertions.assertEquals(expectedOutput, actualOutput);

	}

}