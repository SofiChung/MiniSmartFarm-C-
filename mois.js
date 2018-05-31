var http= require('http');
var b = require('bonescript');
var mois = 0;

setInterval(check, 100);

var server = http.createServer(requestListener);
server.listen(1337);

function requestListener(req, res){
    res.writeHead(200, {'Content-Type':'text/html'});
    res.write('<head><meta charset="utf-8" http-equiv="refresh" content="3"></head>');
    res.write('<body>');
    res.write('<B>안녕하세요! 이 곳에서 화분의 습도를 확인할 수 있습니다!</B> <br><br> ')
    res.write('현재 화분의 습도 : <B> ' + (mois*100).toFixed(2) + '% </B> <br>');
    res.write('습도가 <B>20% 이하</B>가 되면 화분에 물을 주세요.^^ <br> ')
    res.write('</body>');
    res.end();
}

console.log('Server running at 1337');

function check(){
    b.analogRead('P9_33', checkMois);
}

function checkMois(x){
    mois = x.value;
}