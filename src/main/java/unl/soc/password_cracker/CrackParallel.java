package unl.soc.password_cracker;

import java.lang.management.ManagementFactory;
import java.lang.management.ThreadMXBean;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

import unl.soc.Dictionary;

/**
 * A brute-force, parallel password cracker that performs a dictionary attack
 * (given the SHA-256 hashed password) on all {@link Dictionary#WORDS} with 2
 * digit suffixes (ex: <code>"password42"</code>.
 * 
 * Launches a collection of threads (running {@link CallableCracker} instances)
 * and divides all dictionary words evenly among each thread.
 *
 */
public class CrackParallel {

	public static int NUM_THREADS = 4;

	public static String callableCrack(String hash) {

		// create an executor to run our threads
		ExecutorService pool = Executors.newFixedThreadPool(NUM_THREADS);

		// split the dictionary into a (roughly) equal partition:
		int rangeSize = Dictionary.WORDS.size() / NUM_THREADS;
		List<CallableCracker> workers = new ArrayList<>();
		for (int i = 0; i < NUM_THREADS - 1; i++) {
			List<String> subDictionary = Dictionary.WORDS.subList(i * rangeSize,
					Math.min((i + 1) * rangeSize, Dictionary.WORDS.size()));
			workers.add(new CallableCracker((i + 1), subDictionary, hash));
		}
		List<String> subDictionary = Dictionary.WORDS.subList((NUM_THREADS - 1) * rangeSize, Dictionary.WORDS.size());
		workers.add(new CallableCracker(NUM_THREADS, subDictionary, hash));

		// now collect all the futures
		List<Future<String>> results = new ArrayList<>();
		long nStart = System.nanoTime(), nEnd;
		try {
			// start all threads
			results = pool.invokeAll(workers);
		} catch (InterruptedException e) {
			throw new RuntimeException(e);
		}

		String password = null;
		for (Future<String> f : results) {
			try {
				String temp = f.get();
				if (temp != null) {
					password = temp;
					break;
				}
			} catch (InterruptedException | ExecutionException e) {
				throw new RuntimeException(e);
			}
		}
		nEnd = System.nanoTime();
		System.out.printf("Total Wall Clock Time: %.2f seconds\n", (nEnd - nStart) / 1.0e9);

		ThreadMXBean threadMXBean = ManagementFactory.getThreadMXBean();
		long nano = 0l;
		for (long id : threadMXBean.getAllThreadIds()) {
			nano += threadMXBean.getThreadCpuTime(id);
		}
		System.out.printf("Total CPU time:        %.2f seconds", nano / 1.0e9);

		pool.shutdown();
		return password;
	}

	public static void main(String args[]) {

		if (args.length == 0 || args.length > 2) {
			System.err.println("Usage: CrackParallel password [numThreads]\n");
			System.exit(1);
		}
		if (args.length == 2) {
			NUM_THREADS = Integer.parseInt(args[1]);
		}

		String hashedPassword = args[0];

		callableCrack(hashedPassword);

	}

}
