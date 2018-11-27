const express = require('express');
const XMLHttpRequest = require("xmlhttprequest").XMLHttpRequest;
const app = express();
var path = require("path");

function sleep(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}
app.set('view engine', 'ejs');

app.get('/', function (req, res) {
  res.send('Hello World!');
});

app.get('/tokens', function (req, res) {
  let xhr = new XMLHttpRequest();
  //await sleep(2000);
  const code = req.query.code;
  xhr.open('GET', "http://localhost:8080/getToken?client_id=1&client_secret=123&code=" + code + '&callback_url=/test', false);
  xhr.send();
  console.log(xhr.status, "STATUS");
  console.log(xhr.responseText, "STATUS");

  const location = xhr.getResponseHeader("Location");
  console.log(location);

  const body = JSON.parse(xhr.responseText);
  res.render("tokens", { authtoken: body["authtoken"], refreshtoken: body["refreshtoken"], location: location });
});

app.get('/auth', function (req, res) {
  res.render("auth", { code: req.query.code });
});
http://localhost:8080/getToken?client_id=1&client_secret=123&code=664redirect_uri=/lol

app.get('/start', async function (req, res) {
  let xhr = new XMLHttpRequest();
  //await sleep(2000);
  xhr.open('GET', 'http://localhost:8080/getAuthCode?client_id=1&callback_url=/auth', false);
  xhr.send();
  const location = xhr.getResponseHeader("Location");
  console.log(location);
  res.location(location);
  res.status(302);
  res.send("start auth");

});

app.listen(4000, function () {
  console.log('Example app listening on port 4000!');
});


//отдаем ему токен на право записи от юзера 1 и пробуем комментить
//авторизашн код желателен время жизни - туду нью табл


