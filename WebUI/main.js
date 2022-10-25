const automode = document.getElementById('m_auto');
const manmode = document.getElementById('m_man');
const s_pressure = document.getElementById('pressure'); //slider
const s_speed = document.getElementById('speed');       //slider

const g_pressure = document.getElementById('get_press');
const g_speed = document.getElementById('get_speed');
const g_co = document.getElementById('get_co');
const g_temp = document.getElementById('get_temp');
const g_rh = document.getElementById('get_rh');
const canvas = document.getElementById('dataChart');
const filter = document.getElementById('data');
const submit = document.getElementById('btn_login');
const log_out = document.getElementById('btn_log_out');
let data_miss = false;
let user;
let pointerX = -1;
let pointerY = -1;
let lastX = 0;
let lastY = 0;
let counter = 0;
let lastSpeed = 0;
let notSettled = 0;

const socket = io();

socket.on('connection');
socket.on('data', (data) =>{
    if(data.auto === true){
        s_pressure.value = data.setpoint;
        document.getElementById('set_press').value = s_pressure.value + ' Pa';
    }
    if(data.auto === false){
        s_speed.value = data.setpoint;
        document.getElementById('set_speed').value = s_speed.value + ' %';
    }
    g_pressure.value = data.pressure;
    g_co.value = data.co2;
    g_rh.value = data.rh;
    g_speed.value = data.speed;
    g_temp.value = data.temp;    
    updateChart();   
    checkData(); 
    checkFan();
    circleColor();
});

socket.on('pwd', (data) =>{
    if(data){
        automode.checked = true;
        document.getElementById('login-form').style.display = "block";
        document.getElementById('pwd-warn').style.display = "block";
    }
});

socket.on('user', (data) =>{
    user = data;
    localStorage.setItem('user', data);
    sessionStorage.setItem('loggedIn', 'true');
});

s_pressure.addEventListener('input', e =>{
    e.preventDefault();
    sendPressure();
});

s_speed.addEventListener('input', e =>{
    e.preventDefault();
    sendSpeed();
});

automode.addEventListener('click', () =>{   
    document.getElementById('login-form').style.display = "none";

    s_pressure.disabled = false; 
    document.getElementById('pr_div').style.opacity = 1;

    s_speed.disabled = true;    
    document.getElementById('sp_div').style.opacity = 0.4;    
})

manmode.addEventListener('click', () =>{
    if(!sessionStorage.getItem('loggedIn')){
        document.getElementById('login-form').style.display = "block";        
    }
    else{        
        document.getElementById('sp_div').style.opacity = 1;
        s_speed.disabled = false;

        document.getElementById('pr_div').style.opacity = 0.4;  
        s_pressure.disabled = true; 
    }
})

filter.addEventListener('change', e =>{
    e.preventDefault();
    updateChart();
})

submit.addEventListener('click', e =>{
    if(document.getElementById('username').value && document.getElementById('password').value){
        document.getElementById('login-form').style.display = "none";        
    }
    else{
        e.preventDefault();
        document.getElementById('form-warn').style.display = "block";
    }        
});

document.getElementById('password').addEventListener('click', ()=>{
    document.getElementById('form-warn').style.display = "none";
});

log_out.addEventListener('click', ()=>{
    localStorage.clear();
    sessionStorage.clear();
})

window.addEventListener('beforeunload', (e)=>{    
    if(document.cookie){    
        log_out.click();             
    }  
});

function circleColor(){
    if(g_pressure.value > 115){
        g_pressure.style.borderColor = "red";
    }
    else{
        g_pressure.style.borderColor = "black";
    }
    if(g_speed.value > 95){
        g_speed.style.borderColor = 'red';
    }
    else{
        g_speed.style.borderColor = 'black';
    }
    if(g_co.value > 800){
        g_co.style.borderColor = 'red';
    }
    else{
        g_co.style.borderColor = 'black';
    }
    if(g_temp.value < 15 || g_temp.value >30){
        g_temp.style.borderColor = 'red';
    }
    else{
        g_temp.style.borderColor = 'black';
    }
    if(g_rh.value < 20 || g_rh.value > 70){
        g_rh.style.borderColor = "red";
    }
    else{
        g_rh.style.borderColor = "black";
    }
}

function checkFan(){
    if(lastSpeed !== g_speed.value){
        notSettled += 1;
        lastSpeed = g_speed.value;
    }
    else{
        notSettled = 0;
        document.getElementById('mon-warn').style.display = 'none';
    }   
    if(notSettled > 12){
        document.getElementById('mon-warn').style.display = 'block';
    } 
}

function logOutUser(){
    if(document.cookie){
        log_out.click();        
    }    
}

document.onmousemove = function(event) {
	pointerX = event.pageX;
	pointerY = event.pageY;
}
setInterval(activityCheck, 1000);

function activityCheck() {
    if(document.cookie){
        if(pointerX - lastX === 0 && pointerY - lastY === 0){
            counter = counter + 1;
        }
        else{
            lastX = pointerX;
            lastY = pointerY;
            counter = 0;
        }
    }
    if(counter > 60){
        log_out.click();
    }
}

function checkMode(){
    if(document.cookie && sessionStorage.getItem('loggedIn')){
        document.getElementById('user').style.display = "block";
        document.getElementById('user').innerHTML = 'Signed in user: ' + localStorage.getItem('user');
        document.getElementById('btn_log_out').style.display = "block";

        manmode.checked = true;
        s_pressure.disabled = true; 
        document.getElementById('pr_div').style.opacity = 0.4; 
        s_speed.disabled = false;        
    }
    else{
        automode.checked = true;
        s_speed.disabled = true; 
        document.getElementById('sp_div').style.opacity = 0.4; 
        s_pressure.disabled = false;       
        document.getElementById('user').style.display = "none"; 
        document.getElementById('btn_log_out').style.display = "none";
    }
}

function checkData(){
    if(data_miss){
        document.getElementById('chart_warning').style.display = "block";
    }
    if(!data_miss){
        document.getElementById('chart_warning').style.display = "none";
    }    
}

function sendPressure(){
    let press = { auto: true, pressure: parseInt(s_pressure.value) }
    socket.emit('setting', press);
}

function sendSpeed(){
    let speed = { auto: false, speed: parseInt(s_speed.value) }
    socket.emit('setting', speed);
}

const data = {
        datasets: [{
            label: 'CO2',
            yAxisID: 'y0',
            backgroundColor: 
                'rgba(247, 37, 133, 0.9)', //red
            borderColor: 
                'rgba(247, 37, 133, 1)', //red
            tension: 0.4,
            pointRadius: 1
        },
        {
        label: 'Pressure',
        yAxisID: 'y1',
            backgroundColor: 
                'rgba(114, 9, 183, 0.9)', //purple
            borderColor: 
                'rgba(114, 9, 183, 1)', //purple
            tension: 0.4,
            pointRadius: 1
        },
        {
        label: 'Relative humidity',
        yAxisID: 'y2',
            backgroundColor:
                'rgba(67, 97, 238, 0.9)', //blue
            borderColor: 
                'rgba(67, 97, 238, 1)', //blue               
            tension: 0.4,
            pointRadius: 1
        }]
    };

const config = {
    type: 'line',
    data,
    options: {
        scales: {
            /*x: {
                ticks: {
                    display: false
                }
                },*/
            y0: { // co2
                type: 'linear',                
                position: 'left',                
                id: 'y-0',
                beginAtZero: true,
                min: 0, 
                max: 5000,
                ticks: {
                    stepSize: 500,                 
                    color: 'rgba(247, 37, 133, 1)' //red
                }
                },
            y1: { //press
                type: 'linear',
                beginAtZero: true,
                min: 0,
                max: 120,
                position: 'left',
                id: 'y-1',
                ticks: {
                    stepSize: 10,  
                    color: 'rgba(114, 9, 183, 1)' //purple
                    }
                },
            y2: { //rh
                type: 'linear',                
                position: 'left',
                id: 'y-2',
                beginAtZero: true,
                min: 0, 
                max: 100,
                ticks: {
                    stepSize: 10,                                    
                    color: 'rgba(67, 97, 238, 1)' //blue
                }
                }           
            
        
        }//,
        //aspectRatio: 1
    }
};

const myChart = new Chart(canvas, config);

function updateChart(){ 
    async function fetchData(){   
        let datasize = 0;
        let data_per_min = 12;     
        let datapoints;      
        await fetch('/data')
        .then(res => res.json())
        .then(data =>{
            console.log(data.length);
            datapoints = data.filter((elem, index)=>{            
                switch(parseInt(filter.value))
                {
                    case 0: //all
                        datasize = data.length;
                    break;
                    case 1: //10min
                        datasize = 10*data_per_min;
                    break;
                    case 2: //30min
                        datasize = 30*data_per_min;
                    break;
                    case 3: //1 hour
                        datasize = 60*data_per_min;
                    break;            
                }  
                if(datasize > data.length){
                    data_miss = true;  
                } 
                else{
                    data_miss = false;
                }                    
                return index >= data.length -datasize;            
            });      
        });
        
        return datapoints; 
    };

    fetchData().then(datapoints => {
        const time = datapoints.map((time) => {
            return time.ts;
        });
        const co = datapoints.map((time) => {            
            return time.co2;
        });
        const pressure = datapoints.map((time) => {
            return time.pressure;
        });
        const rh = datapoints.map((time) => {
            return time.rh;
        });
        myChart.config.data.labels = time;
        myChart.config.data.datasets[0].data = co;
        myChart.config.data.datasets[1].data = pressure;
        myChart.config.data.datasets[2].data = rh;
        myChart.update();
        checkData();
    });
};

fetch('user_log.json')
.then((res)=>{
    return res.json();
})
.then((logs)=>{
    let placeholder = document.getElementById('table-output');
    let out = '';
    for(let log of logs){
        out += `
            <tr>
                <td>${log.UserId}</td>
                <td>${log.Login}</td>
                <td>${log.Logout}</td>
            </tr>
        `
    }
    placeholder.innerHTML = out;
});

function getStartValues(){
    fetch('data.json')
    .then((res) =>{
        return res.json();
    })
    .then((data) =>{
        if(data.length === 0){
            g_pressure.value = 0;
            g_co.value = 0;
            g_rh.value = 0;
            g_speed.value = 0;
            g_temp.value = 0; 
        }
        else{
            const last = data[data.length -1];
            g_pressure.value = last.pressure;
            g_co.value = last.co2;
            g_rh.value = last.rh;
            g_speed.value = last.speed;
            g_temp.value = last.temp; 
        }
    });
}
