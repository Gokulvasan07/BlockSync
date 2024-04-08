<?php
define('DB_HOST', 'mysql.selfmade.ninja');
define('DB_USER', 'mhemanthkmr');
define('DB_PASSWORD', 'hemanth123');
define('DB_DATABASE', 'mhemanthkmr_api');
define('SITE_URL', 'http://localhost/EthicElectronics/');
include_once('database.Class.php');
$db = new DatabaseConnection;
$con = $db->conn;
// $con = mysqli_connect("localhost","mhemanthkmr","hemanth123","TechFreaks");
function baseurl($slug)
{
    echo SITE_URL . $slug;
}
// baseurl('index.php');
function validateInput($dbcon, $input)
{
    return mysqli_real_escape_string($dbcon, $input);
}
