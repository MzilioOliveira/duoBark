<!DOCTYPE html>
<html lang="pt-br">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <meta http-equiv="X-UA-Compatible" content="ie=edge">
  <meta http-equiv="refresh" content="9; url=index.php">
  <title>Monitoramento DuoBark</title>

  <link rel="stylesheet" href="bootstrap.min.css">
  <link rel="stylesheet" href="estilo.css">
</head>
<body>
<div class="container">

  <div class="areaPesquisa">
    <form action="" method="POST">
      <input type="text" name="data" placeholder="mês/ano">
      <input type="submit" name="submit" value="Buscar">
    </form>
  </div>

  <?php
    include('conexao.php');

    if($_SERVER['REQUEST_METHOD'] == 'POST'){
      $dataPesquisa = $_POST['data'];

      $dataArray = explode("/", $dataPesquisa);
      $dataPesquisa = $dataArray[1] . "-" .$dataArray[0];

      $sql = "SELECT * FROM tbdados WHERE data_hora LIKE '%" . $dataPesquisa . "%'";
    }else{
      $dataAtual = date('Y-m');
      
      $sql = "SELECT * FROM tbdados WHERE data_hora LIKE '%" . $dataAtual . "%'";
    }

    $stmt = $PDO->prepare($sql);
    $stmt->execute();

    echo "<table border=\"1\">";

    echo "<tr>
    <th>AcX</th>
    <th>AcY</th>
    <th>AcZ</th>
    <th>Gx</th>
    <th>Gy</th>
    <th>Gz</th>
    <th>TempLM35</th>
    <th>Data/Hora</th>
    </tr>";
    
    while($linha = $stmt->fetch(PDO::FETCH_OBJ)){

      $timestamp = strtotime($linha->data_hora);
      $dataTabela = date('d/m/Y H:i:s', $timestamp);

      echo "<tr>";
      echo "<td>" . $linha->Ax . "</td>";
      echo "<td>" . $linha->Ay . "</td>";
      echo "<td>" . $linha->Ax . "</td>";
      echo "<td>" . $linha->Gx . "</td>";
      echo "<td>" . $linha->Gy . "</td>";
      echo "<td>" . $linha->Gz . "</td>";
      echo "<td>" . $linha->Tlm35 . "</td>";
      echo "<td>" . $dataTabela . "</td>";
      echo "</tr>";
    }

    echo "</table>";
  ?>
</div>
    <script src="jquery-3.2.1.slim.min.js"></script>
    <script src="popper.min.js"></script>
    <script src="bootstrap.min.js"></script>
</body>
</html>