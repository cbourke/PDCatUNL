<?php

function random_float($min = 0, $max = 1, $includeMax = false) {
    $result = $min + mt_rand(0, (mt_getrandmax() - ($includeMax ? 0 : 1))) / mt_getrandmax() * ($max - $min);
    return round($result, 2);
}

//support CORS:
header("Access-Control-Allow-Origin: *");
header("Access-Control-Allow-Headers: Origin, X-Requested-With, Content-Type, Accept");
header("Access-Control-Allow-Methods: GET, OPTIONS");
header("Access-Control-Max-Age: 1728000");

//response will be JSON
header('Content-Type: application/json');

//and support JSONP:
$callBack = null;
if($_GET && array_key_exists("callback", $_GET)) {
  $callBack = $_GET["callback"];
}

//defaults, otherwise get the GET parameters
$delay = random_float(2, 6);

if($_GET) {
  $delay = urlencode($_GET["delay"] ? $_GET["delay"] : 5);
}

sleep($delay);

$response = "{\"delay\":".$delay.",\"message\":\"Hello!\"}";

if($callBack != null) {
  $response = $callBack . "(" . $response . ")";
}

echo $response;


?>