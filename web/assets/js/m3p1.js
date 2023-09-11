

var Config = {

  active: false,

  minTaskTime_default: 2,
  maxTaskTime_default: 6,
  minTaskTime: 2,
  maxTaskTime: 6,

  producerConfig: {
    min: 1,
    max: 5,
    default: 1
  },

  consumerConfig: {
    min: 1,
    max: 5,
    default: 1
  },

  taskConfig: {
    min: 3,
    max: 50,
    default: 10
  },

  cpuTime: 0,
  elapsedTime: 0,

  queue: [],
  ids: [],

  totalTasksCompleted: 0,

  update: function() {
    var s = $("#queueSize").text();
    if(Config.queue.length != s) {
      $("#queueSize").fadeOut(100, function() {
        $(this).text(Config.queue.length).fadeIn(100);
      });
    }
    var t = $("#totalTasksCompleted").text();
    if(Config.totalTasksCompleted != t) {
      $("#totalTasksCompleted").fadeOut(100, function() {
        $(this).text(Config.totalTasksCompleted).fadeIn(100);
      });
    }
  }
};

class Producer {
  static producers = [];
  constructor(id) {
    this.id = id;
  }
}

class Consumer {
  static consumers = [];
  constructor(id) {
    this.id = id;
  }
}

class Task {
  constructor(id, time) {
    this.id = id;
    this.time = time;
  }
}

function initProducers(numProducers) {

  $(".producer").remove();
  Producer.producers = [];
  for(let i=1; i<=numProducers; i++) {
    p = new Producer(i);
    Producer.producers.push(p);
    addProducer(p);
  }
}

function initConsumers(numConsumers) {

  $(".consumer").remove();
  Consumer.consumers = [];
  for(let i=1; i<=numConsumers; i++) {
    p = new Consumer(i);
    Consumer.consumers.push(p);
    addConsumer(p);
  }
}

function parseValidateFormInput(id, config) {
  var x = $(id).val();
  var v = parseInt(x);
  if(isNaN(v)) {
    v = config.default;
  }
  if(v < config.min) {
    v = config.min;
  } else if(v > config.max) {
    v = config.max;
  }
  $(id).val(v);
  return v;
}

function stop() {
  Config.active = false;
}

async function start() {

  Config.update();
  Config.active = true;
  Config.queue = [];
  Config.ids = [];
  Config.totalTasksCompleted = 0;
  $("#queueDiv").html("");

  var numProducers = parseValidateFormInput("#numberOfProducers", Config.producerConfig);
  var numConsumers = parseValidateFormInput("#numberOfConsumers", Config.consumerConfig);

  //1. create producers/consumers/queue
  initProducers(numProducers);
  initConsumers(numConsumers);

  //ait for 2 seconds to start the simulation
  await new Promise((resolve, reject) => setTimeout(resolve, 2000.0));

  for(var i=0; i<Producer.producers.length; i++) {
    startProducer(Producer.producers[i]);
  }

  for(var i=0; i<Consumer.consumers.length; i++) {
    startConsumer(Consumer.consumers[i]);
  }

  update();

}

async function startConsumer(c) {

  console.log(`Consumer ${c.id} starting...`);
  $(`#consumer-${c.id}-status`).text(`Status: waiting`).hide().fadeIn();
  while(Config.active) {

    //risky but ok
    console.log(`Consumer ${c.id} checking for task...`);
    if(Config.queue.length > 0) {
      //take the next one...
      let task = Config.queue.shift();
      $(`#consumer-${c.id}-status`).text(`Status: dequeuing`).hide().fadeIn();
      $(`#task${task.id}`).hide('slide', {direction: 'right'}, 1000);
      //wait on the animation; cannot depend on callbacks since this
      // is async
      await new Promise((resolve, reject) => setTimeout(resolve, 1010.0));
      $(`#task${task.id}`).remove();
      let card = createTaskCard(task, "Running");

      $(`#consumer-${c.id}-main`).append(card);
      $(`#consumer-${c.id}-status`).text(`Status: running`).hide().fadeIn();
      $(`#task${task.id}`).hide().fadeIn(500);
      await new Promise((resolve, reject) => setTimeout(resolve, 500.0));

      await new Promise((resolve, reject) => setTimeout(resolve, task.time * 1000.0));

      completeTask(task);
      await new Promise((resolve, reject) => setTimeout(resolve, 1000.0));
      $(`#task${task.id}`).fadeOut(1500, function() {
        $(this).remove();
      });
      await new Promise((resolve, reject) => setTimeout(resolve, 1500.0));
      Config.totalTasksCompleted++;

      $(`#consumer-${c.id}-status`).text(`Status: waiting`).hide().fadeIn();

    } else {
      console.log(`Consumer ${c.id} waiting for next task...`);
      var sleepTimeSec = random(1, 2);
      await new Promise((resolve, reject) => setTimeout(resolve, sleepTimeSec * 1000.0));
    }
  }
  console.log(`Consumer ${c.id} terminating...`);
  $(`#consumer-${c.id}-status`).removeClass("loading").text(`Status: terminated`).hide().fadeIn();
}

async function update() {
  while(Config.active) {
    await new Promise((resolve, reject) => setTimeout(resolve, 1000.0));
    Config.update();
  }
}

async function startProducer(p) {

  let pId = p.id;
  let pIndex = p.id-1;
  console.log(`Producer ${pId} starting...`);
  while(Config.active) {

    //1. sleep for a random time
    var sleepTimeSec = random(2, 6);
    console.log(`Producer ${pId} sleeping for ${sleepTimeSec} seconds`);
    // change status
    $(`#producer-${pId}-status`).text(`Status: sleeping (${sleepTimeSec}s)`).hide().fadeIn();
    await new Promise((resolve, reject) => setTimeout(resolve, sleepTimeSec * 1000.0));

    //2. briefly change status to producing
    $(`#producer-${pId}-status`).text("Status: producing").hide().fadeIn();
    await new Promise((resolve, reject) => setTimeout(resolve, 2000.0));

    //3. Produce Task and display
    var taskId = Math.floor(Math.random() * 1000);
    while(Config.ids.indexOf(taskId) >= 0) {
      taskId = Math.floor(Math.random() * 1000);
    }
    Config.ids.push(taskId);
    var taskTime = random(2, 6);
    let task = new Task(taskId, taskTime);
    console.log(`Producer ${pId} producing task #${taskId}`);
    let card = createTaskCard(task, "Pending");
    $(`#producer-${pId}-status`).text("Status: enqueuing").hide().fadeIn();
    $(`#producer-${pId}-main`).html(card);
    $(`#task${task.id}`).hide().fadeIn(500);

    //pause before we enqueue...
    await new Promise((resolve, reject) => setTimeout(resolve, 2000.0));
    //enqueue
    //$(`#task${task.id}`).fadeOut(5000).remove();
    $(`#task${task.id}`).hide('slide', {direction: 'right'}, 1000,
      function() {
        $(this).remove();
        Config.queue.push(task);
        $(`#queueDiv`).prepend(card);
        //$(`#task${task.id}`).hide().fadeIn(500);
        $(`#task${task.id}`).hide().slideDown(700);
      });

  }
  console.log(`Producer ${pId} ending...`);
  $(`#producer-${pId}-status`).removeClass("loading").text("Status: terminated").hide().fadeIn();

}

function completeTask(task) {
  $(`#task${task.id}`).fadeOut(100, function() {
    $(`#task${task.id} div.card-header`).removeClass("text-bg-success").addClass("text-bg-light");
    $(`#task${task.id} button`).remove();
    $(`#task${task.id}`).append("<div class='card-footer text-bg-light text-center'>✅ Complete</div>");
    $(`#task${task.id}`).fadeIn(100);
  });
}

function random(min, max) {
  r = min + Math.random() * (max - min);
  return Math.round(r * 100) / 100;
}


function createTaskCard(task, status = "Pending") {

  var card = ""
  if(status === "Pending") {
   card = `
    <div id="task${task.id}" class="card pdcTask" style="width: 10rem; padding: 0px; margin: 5px;">
      <div class="card-header text-bg-primary" style="margin: 0px;">Task #${task.id}</div>
      <div class="card-body">
        <p class="card-text">Time: ${task.time.toFixed(2)} s</p>
      </div>
      <button class="btn btn-primary" type="button" disabled>
        <span class="spinner-grow spinner-grow-sm" role="status" aria-hidden="true"></span>
        Pending...
      </button>
      <span class="visually-hidden">Pending...</span>
    </div>`;
  } else if(status === "Running") {
     card = `
      <div id="task${task.id}" class="card pdcTask" style="width: 10rem; padding: 0px; margin: 5px;">
        <div class="card-header text-bg-success" style="margin: 0px;">Task #${task.id}</div>
        <div class="card-body">
          <p class="card-text">Time: ${task.time.toFixed(2)} s</p>
        </div>
        <button class="btn btn-success" type="button" disabled>
          <span class="spinner-border spinner-grow-sm" role="status" aria-hidden="true"></span>
          Running...
        </button>
        <span class="visually-hidden">Running...</span>
      </div>`;
    }
  return card

}

function addTask(task) {

  var card = createTaskCard(task, "Pending");
  $(card).hide().appendTo("#queue").fadeIn(100*task.id);
}

function addProducer(producer) {
  var div = `
    <div id="producer-${producer.id}" class="producer">
      <h4>Producer ${producer.id}</h4>
      <p id="producer-${producer.id}-status" class="loading">Status: starting</p>
      <div class="row justify-content-end">
        <div class="col" id="producer-${producer.id}-main"></div>
      </div>
    </div>
    `;
  $(div).hide().appendTo("#producerDiv").fadeIn(500);
}

function addConsumer(consumer) {
  var div = `
    <div id="consumer-${consumer.id}" class="consumer">
      <h4>Consumer ${consumer.id}</h4>
      <p id="consumer-${consumer.id}-status" class="loading">Status: starting</p>
      <div class="row justify-content-center">
        <div class="col" id="consumer-${consumer.id}-main"></div>
      </div>
    </div>
    `;
  $(div).hide().appendTo("#consumerDiv").fadeIn(500);
}
