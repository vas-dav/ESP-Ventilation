'use strict'

const express = require('express')
const path = require('path')
const app = express()
const util = require("util");
const fs = require("fs");
const bodyParser = require("body-parser");
const server = require('http').createServer(app);
const io = require('socket.io')(server);
const mqtt = require('mqtt');
const session = require('express-session');
const bcrypt = require('bcrypt')

const client = mqtt.connect('mqtt://127.0.0.1:1883', {clientId: 'node_server', clean: true})
let newData;
let user;
let msg;
let sesUser;
let sesStart;

app.use(express.static(path.join(__dirname, '/')));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({extended: true}));
app.use(session({    
    secret: 'esp vent',
    resave: false,
    saveUninitialized: false,
    name: 'sid',
    cookie: { 
        httpOnly: false 
    }
  }))


const writeFile = util.promisify(fs.writeFile);
const readFile = util.promisify(fs.readFile);

function read(filePath) {
    return readFile(path.resolve(__dirname, filePath)).then(data => JSON.parse(data));
}

function write(data, filePath) {
    return writeFile(path.resolve(__dirname, filePath), JSON.stringify(data, null, 2))
        .catch(err => console.error(err));
}

function getTime(){
    let today = new Date();
    let day = ("0" + today.getDate()).slice(-2);
    let month = ("0" + (today.getMonth() + 1)).slice(-2);
    let hours = ("0" + today.getHours()).slice(-2);
    let mins = ("0" + today.getMinutes()).slice(-2);
    let secs = ("0" + today.getSeconds()).slice(-2);
    let timestamp = (day + '.' + month + '.' + today.getFullYear() + ' ' + hours + '.' + mins + '.' + secs).toString();
    return timestamp;
}

client.on('connect', ()=>{
    console.log('MQTT client connected: '+ client.connected);       
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
        client.publish("controller/settings", JSON.stringify(arg), { qos: 2, retain: false }, (error)=>{
            if(error){
                console.log(error);
            }
        })
    });   
});

client.on('message', async (topic, message) =>{
    newData = JSON.parse(message);
    console.log(newData);
    io.emit('data', newData);  

    let info = [];
    try {
        info = await read('data.json');
    } catch (e) { console.log(e); }
    let now = getTime();
    newData['ts'] = now;
    info.push(newData);
    write(info, 'data.json');    
});


app.get('/', (req, res)=>{
    res.sendFile(path.join(__dirname + '/index.html'));
})

app.get('/data', async (req, res) => {
    try {
        const data = await read('data.json');
        res.json(data);
    } catch (e) {
        res.status(404).send(e);
    }
});

app.get('/register', (req,res)=>{
    res.send(`
    <h1>Register</h1>
    <form method='post' action='/register' />
        <input type='text' name='name' placeholder='Username' required />
        <input type='password' name='password' placeholder='Password' required />
        <input type='submit' />
    </form>
    `);
});

app.post('/', async (req,res)=>{
    let pwd;
    const { username, password } = req.body;
    try {
        let users = await read('user_db.json');
        users.forEach((user)=>{
            if(username === user.name){
                pwd = user.password;
            }
        })
    } catch (e) { console.log(e); }

    if(bcrypt.compareSync(password, pwd)){  
        let now = getTime();
        req.session.userId = username;        
        req.session.startTime = now;
        console.log('Password is correct');
        user = req.session.userId;
        io.emit('user', user); 
        sesUser = user;
        sesStart = req.session.startTime;
        res.status(205);
    }
    else{
        msg = 'wrong';
        io.emit('pwd', msg); 
        console.log('Wrong password');
        res.status(205);
    }    
})

app.post('/register', async (req,res) =>{
    let users = [];
    let hashed;
    const { name, password } = req.body;
    if(name && password){
        try {
            users = await read('user_db.json');
        } catch (e) { console.log(e); }
        const exists = users.some(user => user.name === name);
        if(!exists){
            try{
                hashed = await bcrypt.hash(password, 12);
            }catch(e){
                console.log(e);
                res.redirect('/register');
            }

            let newUser = {
                "id": users.length +1,
                "name": name,
                "password": hashed
            }
            users.push(newUser);
            write(users, 'user_db.json');
        }
        else{
            console.log('User exists already. No registeration done.');
        }
    }
    res.redirect('/register')
})

app.post('/logout',async (req, res) =>{
    let now = getTime();
    req.session.endTime = now;
    let sesEnd = req.session.endTime;    
    let log = [];
    try {
        log = await read('user_log.json');
    } catch (e) { console.log(e); }
    
    let newLog = {
        "id": log.length +1,
        "UserId": sesUser,
        "Login": sesStart,
        "Logout": sesEnd
    }
    log.unshift(newLog);
    write(log, 'user_log.json');

    req.session.destroy(err => {
        if(err){
            return res.redirect('/');
        }
        res.clearCookie('sid');
        res.redirect('/');
    });
    
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