package unl.soc.password_cracker;

import java.util.Iterator;
import java.util.List;
import java.util.concurrent.Callable;

/**
 * A {@link Callable} password cracker. Runs until the password is found, all
 * possible words are exhausted, or the shared {{@link #ACTIVE} flag is set to
 * <code>false</code>.
 *
 */
public class CallableCracker implements Callable<String> {

	private static boolean ACTIVE = true;
	private static int M = 100;

	private final int id;
	private final String targetHash;
	private final List<String> words;

	public CallableCracker(int id, List<String> words, String targetHash) {
		this.id = id;
		this.words = words;
		this.targetHash = targetHash;
	}

	@Override
	public String call() throws Exception {
		int counter = 0;
		System.out.printf("Thread %d starting; processing %s thru %s (%d total)...\n", this.id, words.get(0),
				words.get(words.size() - 1), this.words.size());

		Iterator<String> iter = words.iterator();
		while (CallableCracker.ACTIVE && iter.hasNext()) {
			String word = iter.next();

			for (int j = 0; j < M; j++) {
				String password = String.format("%s%02d", word, j);
				String hash = CryptoUtils.sha256(password);
				counter++;
				if (hash.equals(this.targetHash)) {
					System.out.printf("Signalling other threads to terminate...\n");
					CallableCracker.ACTIVE = false;
					System.out.printf("Cracked password: %s => %s\n", targetHash, password);
					System.out.printf("Hashes Computed: %d\n", counter);
					return password;
				}
			}
		}
		if (CallableCracker.ACTIVE) {
			System.out.printf("Password not found!\n");
			System.out.printf("Hashes Computed: %d\n", counter);
		}

		return null;
	}

}
