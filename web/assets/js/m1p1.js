
function delayAsynch() {

  $("#loadingImage").show();
  $("#asynchArea").html("");

$.ajax({
    //dataType: "jsonp",
    url: "https://cse.unl.edu/~cbourke/PDCatUNL/delay.php",
    data: {
      //delay: 5
    },
    success: function(json) {
      console.log(json);
      $("#loadingImage").hide();
      $("#asynchArea").hide().html(`<p>Asynchronous Process Done!</p>  <p>Elapsed Time: ${json.delay} seconds</p>`).fadeIn();

    },
		error: function() {
      $("#asynchArea").html("<p>Error loading data</p>");
    }
  });
}

function update(option) {
  $("#actionArea").hide("fade").promise().done(
    function() {
      $("#actionArea").html("<p>Option " + option + " selected.</p>").show( "fade" );
    }
  );
}

function init() {
  $("#loadingImage").hide();
  $("#synchLoadingImage").hide();
}

function delayBusy() {
  $("#synchLoadingImage").show();
  $("#synchArea").html("<p>Busy Delay Started...</p>").promise().then(
    function() {
      var n = 4000000000;
      var x = 0;
      for(var i=0; i<n; i++) {
        x++;
      }
      console.log(x);
      $("#synchArea").html("<p>Busy Delay Done!</p>");
      $("#synchLoadingImage").hide();
    }
  );

}
