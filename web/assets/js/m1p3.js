
async function disolveSeq() {

  for(let i=100; i>=0; i--) {
    var op = i / 100.0;
    $("#seqImage").css({ opacity: op });
  }
  await new Promise((resolve, reject) => setTimeout(resolve, 2000.0));
  $("#seqImage").css({ opacity: 1 }).hide().fadeIn(1000);

}

async function disolveAsync() {

  for(let i=100; i>=0; i--) {
    var op = i / 100.0;
    $("#asyncImage").css({ opacity: op });
    await new Promise((resolve, reject) => setTimeout(resolve, 10.0));
  }
  await new Promise((resolve, reject) => setTimeout(resolve, 2000.0));
  $("#asyncImage").css({ opacity: 1 }).hide().fadeIn(1000);


}
