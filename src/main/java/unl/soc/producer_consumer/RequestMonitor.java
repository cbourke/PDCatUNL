package unl.soc.producer_consumer;

import java.util.concurrent.BlockingQueue;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

/**
 * Monitors the job/request queue in {@link RequestManager} and reports its size
 * every 5 seconds.
 *
 */
public class RequestMonitor implements Runnable {

	private static final Logger LOGGER = LogManager.getLogger(RequestMonitor.class);
	private final BlockingQueue<Request> queue;

	public RequestMonitor(BlockingQueue<Request> queue) {
		this.queue = queue;
	}

	@Override
	public void run() {

		while (true) {
			try {
				LOGGER.info(String.format("%d requests awaiting execution", queue.size()));
				Thread.sleep(5000);
			} catch (InterruptedException e) {
				LOGGER.warn(e);
				throw new RuntimeException(e);
			}
		}

	}

}
