
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
const start = document.getElementById('start-time');
const end = document.getElementById('end-time');
const reset = document.getElementById('btn_reset');

let user;
let pointerX = -1;
let pointerY = -1;
let lastX = 0;
let lastY = 0;
let counter = 0;
let lastSpeed = 0;
let notSettled = 0;
let day = new Date();
day = new Date(day.getTime() - day.getTimezoneOffset()*60000);
let today = day.toISOString().slice(0,16);
start.value = today.replace('T', ' ') + ':00';
end.value = today.replace('T', ' ') + ':00';
let start_time = new Date(start.value).getTime();
let end_time = new Date(end.value).getTime();

const socket = io();

socket.on('connection');
socket.on('data', (data) =>{
    if(data.error === true){
        document.getElementById('mon-warn').style.display = 'block';
    }
    else{
        document.getElementById('mon-warn').style.display = 'none';
    }
    g_pressure.value = data.pressure;
    g_co.value = data.co2;
    g_rh.value = data.rh;
    g_speed.value = data.speed;
    g_temp.value = data.temp;    
    updateChart();   
    circleColor();
});

socket.on('pwd', (data) =>{
    if(data){
        sessionStorage.setItem('reload', true);
        document.location.reload();
        
    }
});

socket.on('user', (data) =>{
    user = data;
    localStorage.setItem('user', data);
    sessionStorage.setItem('loggedIn', 'true');
});

reset.addEventListener('click', e =>{
    e.preventDefault();
    start.value = today.replace('T', ' ') + ':00';
    end.value = today.replace('T', ' ') + ':00';
    start_time = new Date(start.value).getTime();
    end_time = new Date(end.value).getTime();

    //start.value = today.replace('T', ' ') + ':00';
    //end.value = today.replace('T', ' ') + ':00';
    updateChart();
})

start.addEventListener('change', e =>{
    e.preventDefault();
    start_time = start.value + ':00';
    start_time = new Date(start_time).getTime();   
    updateChart(); 
});

end.addEventListener('change', e =>{
    e.preventDefault();
    end_time = end.value + ':00';
    end_time = new Date(end_time).getTime();
    updateChart();
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
    if(!sessionStorage.getItem('loggedIn')){
        document.getElementById('login-form').style.display = "block";   
        document.getElementsByClassName('modes').style.display = "none";
        document.getElementsByClassName('set_values').style.display = "none";           
    }
    else{
        document.getElementById('login-form').style.display = "none";   
        s_pressure.disabled = false; 
        document.getElementById('pr_div').style.opacity = 1;

        s_speed.disabled = true;    
        document.getElementById('sp_div').style.opacity = 0.4;  
    }
      
})

manmode.addEventListener('click', () =>{
    if(!sessionStorage.getItem('loggedIn')){
        document.getElementById('login-form').style.display = "block";  
        document.getElementById('sp_div').style.display = "none";  
        document.getElementById('pr_div').style.display = "none";          
        document.getElementById('m_auto').style.display = "none";
        document.getElementById('m_man').style.display = "none";
        document.getElementById('auto_label').style.display = "none"; 
        document.getElementById('man_label').style.display = "none";          
    }
    else{   
        document.getElementById('login-form').style.display = "none";       
        document.getElementById('sp_div').style.opacity = 1;
        s_speed.disabled = false;

        document.getElementById('pr_div').style.opacity = 0.4;  
        s_pressure.disabled = true; 
    }
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

log_out.addEventListener('click', () =>{
    console.log('log out clicked');
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
    if(sessionStorage.getItem('loggedIn')){
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

function checkUser(){
    if(sessionStorage.getItem('reload')){
        document.getElementById('login-form').style.display = "block";
        document.getElementById('pwd-warn').style.display = "block";
        sessionStorage.removeItem('reload');
    }
    if(document.cookie && sessionStorage.getItem('loggedIn')){
        document.getElementById('login-form').style.display = "none";  
        document.getElementById('user').style.display = "block";
        document.getElementById('user').innerHTML = 'Signed in user: ' + localStorage.getItem('user');
        document.getElementById('btn_log_out').style.display = "block";
        document.getElementById('user').style.display = "block";
        document.getElementById('user').innerHTML = 'Signed in user: ' + localStorage.getItem('user');
        document.getElementById('btn_log_out').style.display = "block";
        document.getElementById('user-table').style.width = "45%";
        document.getElementById('chart-cont').style.width = "50%";

        if(manmode.checked = true){        
            s_pressure.disabled = true; 
            document.getElementById('pr_div').style.opacity = 0.4; 
            s_speed.disabled = false; 
        }  
        if(automode.checked = true){
            s_speed.disabled = true; 
            document.getElementById('sp_div').style.opacity = 0.4; 
            s_pressure.disabled = false;       
        }     
    }
    else{
        document.getElementById('login-form').style.display = "block";  
        document.getElementById('sp_div').style.display = "none";  
        document.getElementById('pr_div').style.display = "none";          
        document.getElementById('m_auto').style.display = "none";
        document.getElementById('m_man').style.display = "none";
        document.getElementById('auto_label').style.display = "none"; 
        document.getElementById('man_label').style.display = "none";  
        document.getElementById('user-table').style.display = "none";       
        document.getElementById('chart-cont').style.width = "95%";   
    }
}

function checkMode(){
    if(document.cookie && sessionStorage.getItem('loggedIn')){
        automode.checked = true;
        s_speed.disabled = true; 
        document.getElementById('sp_div').style.opacity = 0.4; 
        document.getElementById('pr_div').style.opacity = 1; 
        s_pressure.disabled = false;         
    }
    else{
        document.getElementById('login-form').style.display = "block";  
        document.getElementById('sp_div').style.display = "none";  
        document.getElementById('pr_div').style.display = "none";          
        document.getElementById('m_auto').style.display = "none";
        document.getElementById('m_man').style.display = "none";
        document.getElementById('auto_label').style.display = "none"; 
        document.getElementById('man_label').style.display = "none";  
        document.getElementById('user-table').style.display = "none";        
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

function updateChart(){ 
    async function fetchData(){  
        const response = await fetch('data.json');
        const data = await response.json();
        if(data.length !== 0){
            let min = data[0].ts;
            let max = data[data.length-1].ts;
            start.min = min.slice(0, 16);
            start.max = max.slice(0, 16);
            end.min = min.slice(0, 16);
            end.max = max.slice(0, 16);
        }
        const datapoints = data.filter(d => {
            if((end_time-start_time) !== 0){             
                return (new Date(d.ts).getTime() >= start_time && new Date(d.ts).getTime() <= end_time)      
            }
            else{
                return data;
            }
        })
        return datapoints;    
    };

    fetchData().then(datapoints => {
        if(datapoints.length === 0)
        {
            document.getElementById('chart_warn').style.display = "block";
        }
        const time = datapoints.map((index) => {
            return index.ts;
        });
        const co = datapoints.map((index) => {            
            return index.co2;
        });
        const pressure = datapoints.map((index) => {
            return index.pressure;
        });
        const rh = datapoints.map((index) => {
            return index.rh;
        });
        myChart.config.data.labels = time;
        myChart.config.data.datasets[0].data = co;
        myChart.config.data.datasets[1].data = pressure;
        myChart.config.data.datasets[2].data = rh;
        myChart.update();
    });
};

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
            
        
        },
        aspectRatio: 2
    }
};

const myChart = new Chart(canvas, config);

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

async function getStartValues(){
    await fetch('data.json')
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
