<?php
  //tenta conectar no banco
  try{
      $HOST = "localhost"; //nome do host
      $BANCO = "bdnodemcu"; //nome do banco
      $USUARIO = "root";
      $SENHA = "";
      
      //função para conectar
      $PDO = new PDO("mysql:host=" . $HOST . ";dbname=" . $BANCO . ";charset=utf8", $USUARIO, $SENHA);

      //echo "Conectado com sucesso!";
    } catch (PDOException $erro){
      echo "<h1>Erro de conexão!</h1>";
      echo "Detalhes: " . $erro->getMessage();
      //echo "conexão_erro";
    }
?>