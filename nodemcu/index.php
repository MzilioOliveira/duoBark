<!DOCTYPE html>
<html lang="pt-br">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <meta http-equiv="X-UA-Compatible" content="ie=edge">
  <meta http-equiv="refresh" content="9; url=index.php">
  <title>Monitoramento DuoBark</title>
  <link rel="shortcut icon" href="favicon.png">
	<link rel="stylesheet" href="bootstrap.min.css">
  <link rel="stylesheet" href="dashboard.css">

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
  $sql2 = "SELECT * FROM tbdados ORDER BY id DESC LIMIT 15;";

  $stmt = $PDO->prepare($sql);
  $stmt->execute();
  
  $stmt2 = $PDO->prepare($sql2);
	$stmt2->execute();
	
  $chartData_hora = [];
  $chartTemp = [];

	while($linha = $stmt->fetch(PDO::FETCH_ASSOC)){

    $chartData_hora[] = "'".$linha['data_hora']."'";
    $chartTemp[] = $linha['Tlm35'];
  }
?> 

<body>
    <nav class="navbar navbar-dark fixed-top bg-dark flex-md-nowrap p-0 shadow">
      <a class="navbar-brand col-sm-3 col-md-2 mr-0" href="#">duoBark Dashboard</a>
      <!--<input class="form-control form-control-dark w-100" type="text" placeholder="Search" aria-label="Search">
      <ul class="navbar-nav px-3">
        <li class="nav-item text-nowrap">
          <a class="nav-link" href="#">Sign out</a>
        </li>
      </ul>-->
      <a href="http://www.duobark.com/" target="_blank"><img src="logo-duo-2-5.png" id="logo-duo"></a>
    </nav>

    <div class="container-fluid">
      <div class="row">
        <nav class="col-md-2 d-none d-md-block bg-light sidebar">
          <div class="sidebar-sticky">
            <ul class="nav flex-column">
              <li class="nav-item">
                <a class="nav-link active" href="#">
                  <span data-feather="home"></span>
                  Dashboard <span class="sr-only">(current)</span>
                </a>
              </li>
              <li class="nav-item">
                <a class="nav-link" href="#">
                  <span data-feather="file"></span>
                  Orders
                </a>
              </li>
              <li class="nav-item">
                <a class="nav-link" href="#">
                  <span data-feather="shopping-cart"></span>
                  Products
                </a>
              </li>
              <li class="nav-item">
                <a class="nav-link" href="#">
                  <span data-feather="users"></span>
                  Customers
                </a>
              </li>
              <li class="nav-item">
                <a class="nav-link" href="#">
                  <span data-feather="bar-chart-2"></span>
                  Reports
                </a>
              </li>
              <li class="nav-item">
                <a class="nav-link" href="#">
                  <span data-feather="layers"></span>
                  Integrations
                </a>
              </li>
            </ul>

            <h6 class="sidebar-heading d-flex justify-content-between align-items-center px-3 mt-4 mb-1 text-muted">
              <span>Saved reports</span>
              <a class="d-flex align-items-center text-muted" href="#">
                <span data-feather="plus-circle"></span>
              </a>
            </h6>
            <ul class="nav flex-column mb-2">
              <li class="nav-item">
                <a class="nav-link" href="#">
                  <span data-feather="file-text"></span>
                  Current month
                </a>
              </li>
              <li class="nav-item">
                <a class="nav-link" href="#">
                  <span data-feather="file-text"></span>
                  Last quarter
                </a>
              </li>
              <li class="nav-item">
                <a class="nav-link" href="#">
                  <span data-feather="file-text"></span>
                  Social engagement
                </a>
              </li>
              <li class="nav-item">
                <a class="nav-link" href="#">
                  <span data-feather="file-text"></span>
                  Year-end sale
                </a>
              </li>
            </ul>
          </div>
        </nav>

        <main role="main" class="col-md-9 ml-sm-auto col-lg-10 px-4">
          <div class="d-flex justify-content-between flex-wrap flex-md-nowrap align-items-center pt-3 pb-2 mb-3 border-bottom">
            <h1 class="h2">Dashboard</h1>
            <div class="btn-toolbar mb-2 mb-md-0">
              <div class="btn-group mr-2">
                <button class="btn btn-sm btn-outline-secondary">Share</button>
                <button class="btn btn-sm btn-outline-secondary">Export</button>
              </div>
              <button class="btn btn-sm btn-outline-secondary dropdown-toggle">
                <span data-feather="calendar"></span>
                This week
              </button>
            </div>
          </div>

          <canvas class="my-4 w-100" id="grafico" width="900" height="380"></canvas>
          <!--<div style="width:auto; padding:15px;">
			      <canvas id="canvas"></canvas>
	        </div>-->
          <h2>Tabela de Dados</h2>
          <div class="table-responsive">
            <table class="table table-striped table-sm">
              <thead>
                <tr>
                  <th>Data/Hora</th>
                  <th>Ax</th>
                  <th>Ay</th>
                  <th>Az</th>
                  <th>Gx</th>
                  <th>Gy</th>
                  <th>Gz</th>
                </tr>
              </thead>
              <tbody>
                <?php
                  while($linha = $stmt2->fetch(PDO::FETCH_OBJ)){

                    $timestamp = strtotime($linha->data_hora);
                    $dataTabela = date('d/m/Y H:i:s', $timestamp);
              
                    echo "<tr>";
                    echo "<td>" . $dataTabela . "</td>";
                    echo "<td>" . $linha->Ax . "</td>";
                    echo "<td>" . $linha->Ay . "</td>";
                    echo "<td>" . $linha->Ax . "</td>";
                    echo "<td>" . $linha->Gx . "</td>";
                    echo "<td>" . $linha->Gy . "</td>";
                    echo "<td>" . $linha->Gz . "</td>";
                    echo "</tr>";
                  }
                ?>
              </tbody>
            </table>
          </div>
        </main>
      </div>
    </div>
	
	<!-- Bootstrap core JavaScript
    ================================================== -->
    <!-- Placed at the end of the document so the pages load faster -->
    <script src="jquery-3.3.1.slim.min.js"></script>
    <script>window.jQuery || document.write('<script src="jquery-3.3.1.slim.min.js"><\/script>')</script>
    <script src="popper.min.js"></script>
    <script src="bootstrap.min.js"></script>

    <!-- Icons -->
    <script src="feather.min.js"></script>
    <script>
      feather.replace()
    </script>

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
							min: 0,
							max: 50
						}
					}]
				}
			}
		};

		window.onload = function() {
			var ctx = document.getElementById('grafico').getContext('2d');
			window.myLine = new Chart(ctx, config);
		};
	</script>
</body>
</html>