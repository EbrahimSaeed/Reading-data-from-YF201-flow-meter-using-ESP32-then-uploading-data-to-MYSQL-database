
<?php

$servername = "localhost";
$username = "esp32";
$password = "microcontrollerslab@123";
$database_name = "Fuel_Monitoring_System";
$time = $_GET["time"] ;
$flowMilliLitres1 = $_GET["flowMilliLitres1"];
$totalMilliLitres1 = $_GET["totalMilliLitres1"];
$flowMilliLitres2 = $_GET["flowMilliLitres2"];
$totalMilliLitres2 = $_GET["totalMilliLitres2"];

   // Create MySQL connection fom PHP to MySQL server
$connection = new mysqli($servername, $username, $password, $database_name);
   // Check connection
if ($connection->connect_error) {
   die("MySQL connection failed: " . $connection->connect_error);
}
$sql = "INSERT INTO data (ts,sensor1_flowRate, sensor1_flow, sensor2_flowRate, sensor2_flow FLOAT) VALUES ($time, $flowMilliLitres1, $totalMilliLitres1, $flowMilliLitres2, $totalMilliLitres2)";

if ($connection->query($sql) === TRUE) {
   echo "New record created successfully";
    } 
else {
  echo "Error: " . $sql . " => " . $connection->error;
   }
  
$connection->close();

?>