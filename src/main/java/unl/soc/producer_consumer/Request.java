package unl.soc.producer_consumer;

/**
 * An abstract class that models a "request": an action that requires some
 * non-trivial amount of computational resources.
 * 
 */
public abstract class Request {

	/**
	 * The (possibly non-unique) identifier of this request.
	 */
	private final int id;

	/**
	 * A generic "action" that the request performs. Returns an (estimate) of the
	 * time (seconds) it took to execute.
	 * 
	 * @return
	 */
	public abstract double executeAction();

	public Request(int id) {
		this.id = id;
	}

	public int getId() {
		return this.id;
	}

}
