package unl.soc.producer_consumer;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.net.URL;
import java.util.Map;

import javax.net.ssl.HttpsURLConnection;

/**
 * A "delay" request that make a simple HTTPS (get) request to a server that
 * delays for either a specified amount of time (<code>delayMs</code>) or a
 * random amount of time between 2-5 seconds.
 * 
 */
public class HttpDelayRequest extends Request {

	private static final Logger LOGGER = LogManager.getLogger(HttpDelayRequest.class);
	private static final Gson GSON = new GsonBuilder().setPrettyPrinting().create();

	private static final String BASE_URL = "https://cse.unl.edu/~cbourke/PDCatUNL/delay.php";

	private final Double delaySec;

	public HttpDelayRequest(int id) {
		this(id, null);
	}

	public HttpDelayRequest(int id, Double delaySec) {
		super(id);
		this.delaySec = delaySec;
	}

	public double executeAction() {
		LOGGER.info(String.format("Request #%d executing action...", this.getId()));
		StringBuilder sb = new StringBuilder();

		try {

			String httpsURL = this.delaySec == null ? BASE_URL : String.format(BASE_URL + "?delay=%.2f", this.delaySec);

			URL myUrl = new URL(httpsURL);
			HttpsURLConnection conn = (HttpsURLConnection) myUrl.openConnection();
			InputStream is = conn.getInputStream();
			InputStreamReader isr = new InputStreamReader(is);
			BufferedReader br = new BufferedReader(isr);

			String line = null;

			while ((line = br.readLine()) != null) {
				sb.append(line);
			}

			br.close();
		} catch (Exception e) {
			throw new RuntimeException(e);
		}

		String jsonString = sb.toString();
		@SuppressWarnings("unchecked")
		Map<String, Object> map = GSON.fromJson(jsonString, Map.class);
		double delay = Double.parseDouble(map.get("delay").toString());
		LOGGER.info(String.format("Request #%d complete", this.getId()));
		return delay;
	}

}
