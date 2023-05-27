

var Config = {
  minTaskTime_default: 2,
  maxTaskTime_default: 6,
  minTaskTime: 2,
  maxTaskTime: 6,

  processorConfig: {
    min: 1,
    max: 6,
    default: 1
  },

  taskConfig: {
    min: 3,
    max: 50,
    default: 10
  },

  cpuTime: 0,
  elapsedTime: 0,

  update: function(taskTime, totalProcessorTime) {
    this.cpuTime += taskTime;
    if(this.elapsedTime < totalProcessorTime) {
      this.elapsedTime = totalProcessorTime;
    }
    $("#cpuTime").fadeOut(100, function() {
      $(this).text(Config.cpuTime.toFixed(2)).fadeIn(100);
    });
    $("#elapsedTime").fadeOut(100, function() {
      $(this).text(Config.elapsedTime.toFixed(2)).fadeIn(100);
    });
  }
};

class Processor {
  static processors = [];
  constructor(id, totalTime) {
    this.id = id;
    this.totalTime = totalTime;
  }
}

class Task {
  static tasks = [];
  constructor(id, time) {
    this.id = id;
    this.time = time;
  }
}

function initProcessors(numProcessors) {

  $(".pdcProcessor").remove();
  Processor.processors = [];
  for(let i=1; i<=numProcessors; i++) {
    p = new Processor(i, 0);
    Processor.processors.push(p);
    addProcessor(p);
  }
}

function initTasks(numTasks) {

  $(".pdcTask").remove();
  Task.tasks = [];
  $("#queue").html();
  for(let i=1; i<=numTasks; i++) {
    t = new Task(i, random(Config.minTaskTime,Config.maxTaskTime));
    Task.tasks.push(t);
    addTask(t);
  }
}

function init() {}

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

async function start() {

  Config.update(0, 0);
  var numProcessors = parseValidateFormInput("#numberOfProcessors", Config.processorConfig);
  var numTasks = parseValidateFormInput("#numberOfTasks", Config.taskConfig);
  var timeOfTasks = $("#timeOfTasks").val();
  if(timeOfTasks === "Random") {
    Config.minTaskTime = Config.minTaskTime_default;
    Config.maxTaskTime = Config.maxTaskTime_default;
  } else {
    timeOfTasks = parseInt(timeOfTasks);
    Config.minTaskTime = timeOfTasks;
    Config.maxTaskTime = timeOfTasks;
  }

  //1. create number of tasks
  initTasks(numTasks);
  //2. create processors
  initProcessors(numProcessors);
  //3. kick of each processor after a pause
  await new Promise((resolve, reject) => setTimeout(resolve, 2000.0));
  for(let i=0; i<numProcessors; i++) {
    startProcessor(Processor.processors[i]);
  }

}

async function startProcessor(p) {

  console.log(`staritng processor ${p}`)
  let pId = p.id;
  let pIndex = p.id-1;
  while(true) {

    let task = Task.tasks.shift();
    if(!task) {
      return;
    }
    console.log(task);
    $(`#task${task.id}`).toggle( "slide",
      function() {
        $(this).remove();
        card = createTaskCard(task, "Running");
        $(`#processorQueue${pId}`).append(card);
        $(`#task${task.id}`).hide().fadeIn(500);
      }
    );

    await new Promise((resolve, reject) => setTimeout(resolve, task.time * 1000.0));
    completeTask(task);
    Processor.processors[pIndex].totalTime += task.time;
    $(`#processorTime${p.id}`).text(`${Processor.processors[pIndex].totalTime.toFixed(2)}`);
    Config.update(task.time, Processor.processors[pIndex].totalTime);


  }
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
    <div id="task${task.id}" class="card mb-3 pdcTask" style="max-width: 10rem; padding: 0px; margin: 5px;">
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
      <div id="task${task.id}" class="card mb-3 pdcTask" style="max-width: 10rem; padding: 0px; margin: 5px;">
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

function addProcessor(processor) {
  var div = createProcessorDiv(processor);
  $(div).hide().appendTo("#mainContainer").fadeIn(500);
  //$("#mainContainer").append(div).hide().fadeIn();
}

function createProcessorDiv(processor) {

  var color = ( (processor.id) % 2 == 0) ? "rgba(123,167,204,.25)" : "rgba(141,179,211,0.5)";
  return `
  <div id="processor${processor.id}" class="container pdcProcessor" style="background-color: ${color}">
    <h3>Processor ${processor.id}</h3>
    <p>Total Time: <span id="processorTime${processor.id}">0</span> seconds</p>
    <div id="processorQueue${processor.id}" class="row row-cols-1 row-cols-md-2 g-2" style="min-height: 4em;"></div>
  </div>`;
}
