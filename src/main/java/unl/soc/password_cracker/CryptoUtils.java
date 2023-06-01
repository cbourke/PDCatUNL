package unl.soc.password_cracker;

import java.security.MessageDigest;
import java.security.Security;

import org.bouncycastle.jce.provider.BouncyCastleProvider;

public class CryptoUtils {

	static {
		Security.addProvider(new BouncyCastleProvider());
	}

	private static String byteToHex(byte num) {
		char[] hexDigits = new char[2];
		hexDigits[0] = Character.forDigit((num >> 4) & 0xF, 16);
		hexDigits[1] = Character.forDigit((num & 0xF), 16);
		return new String(hexDigits);
	}

	private static String encodeHexString(byte[] byteArray) {
		StringBuilder hexString = new StringBuilder();
		for (int i = 0; i < byteArray.length; i++) {
			hexString.append(byteToHex(byteArray[i]));
		}
		return hexString.toString();
	}

	/**
	 * Computes the SHA-256 digest/hash of the given <code>input</code>; returns
	 * the hash as a lower case hexadecimal string.
	 */
	public static String sha256(String input) {

		MessageDigest messageDigest;
		try {
			messageDigest = MessageDigest.getInstance("SHA-256");
		} catch (Exception e) {
			throw new RuntimeException(e);
		}
		byte[] hashedString = messageDigest.digest(input.getBytes());
		return "0x" + encodeHexString(hashedString);
	}
}
