<?php
  try{
      $HOST = "localhost";
      $BANCO = "bdnodemcu";
      $USUARIO = "root";
      $SENHA = "";
      
      //função para conectar
      $PDO = new PDO("mysql:host=" . $HOST . ";dbname=" . $BANCO . ";charset=utf8", $USUARIO, $SENHA);

    }catch (PDOException $erro){
      echo "<h1>Erro de conexão!</h1>";
      echo "Detalhes: " . $erro->getMessage();
    }
?>