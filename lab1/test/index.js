let assert = require('assert');
const XMLHttpRequest = require("xmlhttprequest").XMLHttpRequest;

let request = new XMLHttpRequest();

let url = "http://localhost:8080"
describe('Hello world:', ()=> {
    it('should return hello world', () => {
       request.open("GET", url + '/hello', false);
       request.send();
       assert.equal(request.responseText, " Hello world!");
    });
});

describe('Sum:', ()=> {
    it('should return sum of int numbers', () => {
       request.open("GET", url + '/sum?a=3&b=4', false);
       request.send();
       assert.equal(request.responseText, "7");
    });
});

describe('Sum check missign param:', ()=> {
    it('should return error', () => {
       request.open("GET", url + '/sum?a=3&', false);
       request.send();
       assert.equal(request.responseText, "error");
    });
});
