function atualizar(){
  $.post('../dao/leitura.php', {action: 'test'}, function (teste) {  
    $('#teste').html('<p>' + teste.Gx + '</p>');
  }, 'JSON');
}
setInterval("atualizar()", 100);
$(function() {
  atualizar();
});