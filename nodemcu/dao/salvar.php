<?php
  include('conexao.php');

  $Ax = $_GET['Ax'];
  $Ay = $_GET['Ay'];
  $Az = $_GET['Az'];
  $Gx = $_GET['Gx'];
  $Gy = $_GET['Gy'];
  $Gz = $_GET['Gz'];
  $Tlm35 = $_GET['Tlm35'];

  //query
  $sql = "INSERT INTO tbdados (Ax, Ay, Az, Gx, Gy, Gz, Tlm35) VALUES (:Ax, :Ay, :Az, :Gx, :Gy, :Gz, :Tlm35)";

  //statement
  $stmt = $PDO->prepare($sql);

  $stmt->bindParam(':Ax', $Ax);
  $stmt->bindParam(':Ay', $Ay);
  $stmt->bindParam(':Az', $Az);
  $stmt->bindParam(':Gx', $Gx);
  $stmt->bindParam(':Gy', $Gy);
  $stmt->bindParam(':Gz', $Gz);
  $stmt->bindParam(':Tlm35', $Tlm35);

  if($stmt->execute()){
    echo "salvo com sucesso!";
  }else{
    echo "erro ao salvar!";
  }
?>