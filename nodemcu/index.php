<!DOCTYPE html>
<html lang="pt-br">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <meta http-equiv="X-UA-Compatible" content="ie=edge">
  <meta http-equiv="refresh" content="9; url=index.php">
  <title>Monitoramento DuoBark</title>
  <link rel="shortcut icon" href="favicon.png">
  
  <script src="Chart.bundle.min.js"></script>
	<script src="utils.js"></script>
	<style>
		canvas{
			-moz-user-select: none;
			-webkit-user-select: none;
			-ms-user-select: none;
		}
	</style>
</head>

<body>
<?php 
	include('conexao.php');

  $sql = "SELECT TIME(data_hora) AS data_hora, Tlm35 FROM tbdados ORDER BY id DESC LIMIT 4;";

  $stmt = $PDO->prepare($sql);
	$stmt->execute();
	
  $chartData_hora = [];
  $chartTemp = [];

	while($linha = $stmt->fetch(PDO::FETCH_ASSOC)){

    $chartData_hora[] = "'".$linha['data_hora']."'";
    $chartTemp[] = $linha['Tlm35'];
  }
?> 

	<div style="width:75%;">
		<canvas id="canvas"></canvas>
	</div>
	<script>
		var config = {
			type: 'line',
			data: {
        labels: [<?php echo join(",",$chartData_hora)?>],
				datasets: [{
					label: 'Temperatura',
					backgroundColor: window.chartColors.red,
					borderColor: window.chartColors.red,
          data: [<?php echo join(",",$chartTemp)?>],
					fill: false,
				}]
			},
			options: {
				responsive: true,
				title: {
					display: true
				},
				scales: {
					yAxes: [{
						ticks: {
							min: -10,
							max: 100
						}
					}]
				}
			}
		};

		window.onload = function() {
			var ctx = document.getElementById('canvas').getContext('2d');
			window.myLine = new Chart(ctx, config);
		};
	</script>
</body>

</html>