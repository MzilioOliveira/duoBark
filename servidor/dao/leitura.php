<?php
  if(!function_exists('returnGraficoData_Hora')){
    function returnGraficoData_Hora(){  
      include('conexao.php');
      $sql = "SELECT TIME(data_hora) AS data_hora FROM tbdados ORDER BY id DESC LIMIT 4;";
      $stmt = $PDO->prepare($sql);
      $stmt->execute();
      $chartData_hora = [];

      while($linha = $stmt->fetch(PDO::FETCH_ASSOC)){
        $chartData_hora[] = "'".$linha['data_hora']."'";
      }
      echo join(",",array_reverse($chartData_hora));
    }
  }

  if(!function_exists('returnGraficoTemp')){
    function returnGraficoTemp(){  
      include('conexao.php');
      $sql = "SELECT Tlm35 FROM tbdados ORDER BY id DESC LIMIT 4;";
      $stmt = $PDO->prepare($sql);
      $stmt->execute();
      $chartTemp = [];

      while($linha = $stmt->fetch(PDO::FETCH_ASSOC)){
        $chartTemp[] = "'".$linha['Tlm35']."'";
      }
      echo join(",",array_reverse($chartTemp));
    }
  }

  if(!function_exists('tabelaDados')){
    function tabelaDados(){
      include('conexao.php');
      $sql = "SELECT * FROM tbdados ORDER BY id DESC LIMIT 15;";
      $stmt = $PDO->prepare($sql);
      $stmt->execute();

      while($linha = $stmt->fetch(PDO::FETCH_OBJ)){
        $timestamp = strtotime($linha->data_hora);
        $dataTabela = date('d/m/Y H:i:s', $timestamp);
        echo "<tr>";
        echo "<td id='data'>" . $dataTabela . "</td>";
        echo "<td>" . $linha->Ax . "</td>";
        echo "<td>" . $linha->Ay . "</td>";
        echo "<td>" . $linha->Ax . "</td>";
        echo "<td>" . $linha->Gx . "</td>";
        echo "<td>" . $linha->Gy . "</td>";
        echo "<td>" . $linha->Gz . "</td>";
        echo "</tr>";
      }
    }
  }

  if(!function_exists('teste')){
    function teste(){  
      require_once('conexao.php');
      $stmt = $PDO->prepare('SELECT Gx FROM tbdados ORDER BY RAND() LIMIT 1');
        if ($stmt->execute()){
          $teste = $stmt->fetchObject();
          echo json_encode($teste);
        }
    }
  }

  if(isset($_POST['action']) && !empty($_POST['action'])) {
    $action = $_POST['action'];
    switch($action) {
        case 'test' : teste();break;
    }
  }
?>