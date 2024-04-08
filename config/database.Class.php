<?php
class DatabaseConnection
{
    public function __construct()
    {
        $conn = new mysqli(DB_HOST, DB_USER, DB_PASSWORD, DB_DATABASE);
        // die(print_r($conn));
        if($conn->connect_error)
        {
            die("<h1>Database Connection Failed</h1>");
        }
        // echo "Data Base Connected Successfully";
        return $this->conn = $conn;
    }
}
?>