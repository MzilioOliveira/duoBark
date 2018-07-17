function atualizar(){
  $.ajax({ url: '../dao/leitura.php',
  data: {action: 'test'},
  type: 'post',
  dataType: "json",
  success: function(teste) {
    $('#teste').html('<p>' + teste.Gx + '</p>');
    }
  });
}
setInterval("atualizar()", 10000);
$(function() {
  atualizar();
});