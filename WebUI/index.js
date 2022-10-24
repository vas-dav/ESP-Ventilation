'use strict'

const express = require('express')
const path = require('path')
const app = express()
const util = require("util");
const fs = require("fs");
const bodyParser = require("body-parser");
const server = require('http').createServer(app);
const io = require('socket.io')(server);
const mqtt = require('mqtt')
//const WebSocket = require('ws');

//const wss = new WebSocket.Server({server});
const client = mqtt.connect('mqtt://127.0.0.1:1883', {clientId: 'node_server', clean: true})
let newData;

app.use(express.static(path.join(__dirname, '/')));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({extended: false}));


const writeFile = util.promisify(fs.writeFile);
const readFile = util.promisify(fs.readFile);

function read(filePath) {
    return readFile(path.resolve(__dirname, filePath)).then(data => JSON.parse(data));
}

function write(data, filePath) {
    return writeFile(path.resolve(__dirname, filePath), JSON.stringify(data, null, 2))
        .catch(err => console.error(err));
}

app.get('/', (req, res)=>{
    
    res.sendFile(path.join(__dirname + '/index.html'));
})

client.on('connect', ()=>{
    console.log('MQTT client connect: '+client.connected);       
});

client.subscribe('controller/status', (err)=>{
    if(err){
        console.log('Subscription failed')
    }
});

io.on('connection', (socket)=>{
    console.log("User " + socket.id + " connected");    
    socket.on('setting', (arg)=> {
        let data = JSON.stringify(arg);
        console.log(data);
        client.publish("controller/settings", JSON.stringify(arg), { qos: 2, retain: false }, (error)=>{
            if(error){
                console.log(error);
            }
        })
    });           
});

client.on('message',async (topic, message) =>{
    newData = JSON.parse(message.toString());
    io.emit('data', newData);
    console.log(newData);  
    //console.log(message)
    //io.on('connection', (socket)=>{
        //let send = JSON.stringify(newdata);
        
    //});   

    let info = [];
    try {
        info = await read('data.json');
    } catch (e) {
    }
    let today = new Date();
    let day = ("0" + today.getDate()).slice(-2);
    let month = ("0" + (today.getMonth() + 1)).slice(-2);
    let hours = ("0" + today.getHours()).slice(-2);
    let mins = ("0" + today.getMinutes()).slice(-2);
    let secs = ("0" + today.getSeconds()).slice(-2);
    let timestamp = (day + '.' + month + '.' + today.getFullYear() + ' ' + hours + '.' + mins + '.' + secs).toString();
    //console.log(timestamp);
    newData['ts'] = timestamp;
    info.push(newData);
    write(info, 'data.json');    
});



app.get('/data', async (req, res) => {
    try {
        const data = await read('data.json');
        res.json(data);
    } catch (e) {
        res.status(404).send(e);
    }
});


server.listen(3000, () => console.log('Server listening on port 3000'));