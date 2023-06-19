package unl.soc.producer_consumer;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

/**
 * Continuously and asynchronously handles requests from the
 * {@link RequestManager}, reporting (an estimate) of the time each request
 * takes to executes.
 *
 */
public class RequestHandler implements Runnable {

	private static final Logger LOGGER = LogManager.getLogger(RequestHandler.class);
	private final int id;

	public RequestHandler(int id) {
		this.id = id;
	}

	@Override
	public void run() {

		LOGGER.info(String.format("RequestHandler #%d starting...", this.id));

		while (true) {
			Request req = RequestManager.handle();
			LOGGER.info(String.format("RequestHandler #%d executing Request #%d ...", this.id, req.getId()));
			double delaySeconds = req.executeAction();
			LOGGER.info(String.format("RequestHandler #%d finished Request #%d (%.2f seconds)", this.id, req.getId(),
					delaySeconds));
		}

	}

}
