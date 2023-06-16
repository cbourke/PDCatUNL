package unl.soc.producer_consumer;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import unl.soc.Utils;

/**
 * Generates {@link HttpDelayRequest}s (using the default
 * {@link HttpDelayRequest} parameters) at random intervals between
 * {@link #intervalSecLow} and {@link #intervalSecHigh} seconds (default: 2-5
 * seconds) and submits them for processing to the {@link RequestManager}.
 *
 */
public class RequestGenerator implements Runnable {

	private static final Logger LOGGER = LogManager.getLogger(RequestGenerator.class);

	private final int id;
	private final double intervalSecLow;
	private final double intervalSecHigh;

	public RequestGenerator(int id) {
		this(id, 2.0, 5.0);
	}

	public RequestGenerator(int id, double intervalSecLow, double intervalSecHigh) {
		this.id = id;
		this.intervalSecLow = intervalSecLow;
		this.intervalSecHigh = intervalSecHigh;
	}

	@Override
	public void run() {
		LOGGER.info(String.format("Request generator %d starting...", this.id));
		try {
			while (true) {
				long intervalMs = (long) (1000.0 * Utils.randomRange(intervalSecLow, intervalSecHigh));
				LOGGER.debug(String.format("Request generator %d sleeping for %d ms ...", this.id, intervalMs));
				Thread.sleep(intervalMs);
				int requestId = Utils.SOURCE.nextInt(1000);
				LOGGER.info(String.format("Request generator %d submitting new request # %d", this.id, requestId));
				Request req = new HttpDelayRequest(requestId);
				RequestManager.submit(req);
			}
		} catch (Exception e) {
			LOGGER.warn(e);
			throw new RuntimeException(e);
		}

	}

}
