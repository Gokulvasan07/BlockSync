<?php
include('var/www/html/TechFreaks/config/app.php');
class User
{
    private $conn;
    public function __construct()
    {
        $db = new DatabaseConnection();
        // die(var_dump($db->));
        // die(var_dump($db->conn->ping()));
        $this->conn = $db->conn;
    }
    public function userExist($username)
    {
        $query = "SELECT email FROM users WHERE username='$username' LIMIT 1";
        $result = $this->conn->query($query);
        return $result;
    }
    public function emailExist($email)
    {
        $query = "SELECT email FROM users WHERE email='$email' LIMIT 1";
        $result = $this->conn->query($query);
        return $result;
    }
    public function signUp($name, $username, $email, $password, $verify_token)
    {
        $query = "INSERT INTO `TechFreaks`.`users` (`name`, `username`, `email`, `password`, `ver_token`) VALUES ('$name', '$username', '$email', '$password', '$verify_token');";
        $result = $this->conn->query($query);
        return $result;
    }
}
