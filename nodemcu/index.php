<!DOCTYPE html>
<html lang="pt-br">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <meta http-equiv="X-UA-Compatible" content="ie=edge">
  <meta http-equiv="refresh" content="9; url=index.php">
  <title>Monitoramento DuoBark</title>

  <link rel="stylesheet" href="estilo.css">

  <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
</head>
<body>

<?php 
	include('conexao.php');

	$sql = "SELECT TIME(data_hora) AS data_hora, Tlm35 FROM tbdados ORDER BY id DESC LIMIT 4;";

  $stmt = $PDO->prepare($sql);
	$stmt->execute();
	
  $chartValues = [];
	while($linha = $stmt->fetch(PDO::FETCH_ASSOC)){
    $chartValues[] = "'".$linha['data_hora']."', ".$linha['Tlm35'];
  }
?> 

<div class="container">
  <div class="row">
		<div class="col-md-6">
    <div id="curve_chart" style="width: auto; height: 400px"></div>
    </div>
	</div>
</div>

<script type="text/javascript">
      google.charts.load('current', {'packages':['corechart']});
      google.charts.setOnLoadCallback(drawChart);

      function drawChart() {
        var data = google.visualization.arrayToDataTable([
          ['Hora', 'Temperatura'],
          [<?php echo join('],[',$chartValues) ?>],
        ]);

        var options = {
          curveType: 'function',
          legend: { position: 'bottom' }
        };

        var chart = new google.visualization.LineChart(document.getElementById('curve_chart'));

        chart.draw(data, options);
      }
    </script>
</body>
</html>