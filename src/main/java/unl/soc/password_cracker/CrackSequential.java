package unl.soc.password_cracker;

/**
 * A brute-force, sequential password cracker that performs a dictionary attack
 * (given the SHA-256 hashed password) on all {@link Dictionary#WORDS} with 2
 * digit suffixes (ex: <code>"password42"</code>.
 *
 */
public class CrackSequential {

	public static void main(String args[]) {

		if (args.length != 1) {
			System.err.println("Usage: CrackSequential password\n");
			System.exit(1);
		}

		String hashedPassword = args[0];
		// 2 digit suffixes
		int m = 100;
		int counter = 0;

		long start = System.currentTimeMillis(), end;
		long nStart = System.nanoTime(), nEnd;
		for (String word : Dictionary.WORDS) {

			for (int i = 0; i < m; i++) {
				String password = String.format("%s%02d", word, i);
				String hash = CryptoUtils.sha256(password);
				counter++;
				// System.out.println(password + " => " + hash);
				if (hash.equals(hashedPassword)) {
					end = System.currentTimeMillis();
					nEnd = System.nanoTime();
					System.out.printf("Cracked password: %s => %s\n", hashedPassword, password);
					System.out.printf("Hashes Computed: %d\n", counter);
					System.out.printf("Total Time: %f seconds\n", (end - start) / 1000.0);
					System.out.printf("CPU(?) Time: %f seconds\n", (nEnd - nStart) / 1.0e9);
					System.exit(0);
				}
			}
		}

		end = System.currentTimeMillis();
		nEnd = System.nanoTime();
		System.out.printf("Password not found!\n");
		System.out.printf("Hashes Computed: %d\n", counter);
		System.out.printf("Total Time: %f seconds\n", (end - start) / 1000.0);
		System.out.printf("CPU(?) Time: %f seconds\n", (nEnd - nStart) / 1.0e9);

	}

}
