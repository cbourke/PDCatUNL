package unl.soc.producer_consumer;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class RequestManager {

	private static final Logger LOGGER = LogManager.getLogger(RequestManager.class);
	private static final BlockingQueue<Request> REQUEST_QUEUE = new ArrayBlockingQueue<>(10000);

	/**
	 * An executor pool to execute all {@link RequestMonitor}s,
	 * {@link RequestGenerator}s and one {@link RequestMonitor}.
	 */
	private static final ExecutorService POOL = Executors
			.newFixedThreadPool(Simulation.NUM_REQUEST_HANDLERS + Simulation.NUM_REQUEST_GENERATORS + 1);

	public static void initialize() {

		LOGGER.info("initalizing...");
		RequestMonitor rm = new RequestMonitor(REQUEST_QUEUE);
		POOL.submit(rm);

		LOGGER.info(String.format("creating %d request generators...", Simulation.NUM_REQUEST_GENERATORS));
		for (int i = 0; i < Simulation.NUM_REQUEST_GENERATORS; i++) {
			RequestGenerator generator = new RequestGenerator((i + 1) * 10,
					Simulation.REQUEST_GENERATION_INTERVAL_SEC_MIN, Simulation.REQUEST_GENERATION_INTERVAL_SEC_MAX);
			POOL.submit(generator);
		}

		LOGGER.info(String.format("creating %d request handlers...", Simulation.NUM_REQUEST_HANDLERS));
		for (int i = 0; i < Simulation.NUM_REQUEST_HANDLERS; i++) {
			RequestHandler handler = new RequestHandler((i + 1));
			POOL.submit(handler);
		}
		LOGGER.info("initalization DONE");
	}

	public static void submit(Request req) {
		LOGGER.info(String.format("Request #%d enqueued", req.getId()));
		try {
			REQUEST_QUEUE.put(req);
		} catch (InterruptedException e) {
			throw new RuntimeException(e);
		}
	}

	public static Request handle() {
		try {
			return REQUEST_QUEUE.take();
		} catch (InterruptedException e) {
			throw new RuntimeException(e);
		}
	}

	public static void shutdown() {
		LOGGER.info("Shutting down...");
		POOL.shutdownNow();
		LOGGER.info("Shutdown complete.");

	}
}
