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

s_pressure.addEventListener('input', e =>{
    e.preventDefault();
    updateChart();
})

automode.addEventListener('click', () =>{
    s_pressure.disabled = false; 
    document.getElementById('set_press').disabled = false;
    s_speed.disabled = true;
    document.getElementById('set_speed').disabled = true;
})

manmode.addEventListener('click', () =>{
    s_pressure.disabled = true; 
    document.getElementById('set_press').disabled = true;
    s_speed.disabled = false;
    document.getElementById('set_speed').disabled = false;
})
    
const data = {
        //labels: ['Red', 'Blue', 'Yellow', 'Green', 'Purple', 'Orange'],
        datasets: [{
            label: 'Temperature',
            yAxisID: 'y0',
            //data: [12, 19, 3, 5, 2, 3],
            backgroundColor: [
                'rgba(255, 99, 132, 1)'//,
             //   'rgba(54, 162, 235, 0.2)',
             //   'rgba(255, 206, 86, 0.2)',
             //   'rgba(75, 192, 192, 0.2)',
             //   'rgba(153, 102, 255, 0.2)',
             //   'rgba(255, 159, 64, 0.2)'
            ],
            borderColor: [
                'rgba(255, 99, 132, 1)'//,
                //'rgba(54, 162, 235, 1)',
                //'rgba(255, 206, 86, 1)',
                //'rgba(75, 192, 192, 1)',
                //'rgba(153, 102, 255, 1)',
                //'rgba(255, 159, 64, 1)'
            ],
            //borderWidth: 1,
            tension: 0.05,
            pointRadius: 1
        },
        {
        label: 'Pressure',
        yAxisID: 'y1',
            //data: [12, 19, 3, 5, 2, 3],
            backgroundColor: [
            //    'rgba(255, 99, 132, 1)'//,
                'rgba(54, 162, 235, 1)',
             //   'rgba(255, 206, 86, 0.2)',
             //   'rgba(75, 192, 192, 0.2)',
             //   'rgba(153, 102, 255, 0.2)',
             //   'rgba(255, 159, 64, 0.2)'
            ],
            borderColor: [
             //   'rgba(255, 99, 132, 1)'//,
                'rgba(54, 162, 235, 1)',
                //'rgba(255, 206, 86, 1)',
                //'rgba(75, 192, 192, 1)',
                //'rgba(153, 102, 255, 1)',
                //'rgba(255, 159, 64, 1)'
            ],
            //borderWidth: 1,
            tension: 0.05,
            pointRadius: 1
        },
        {
        label: 'Fan speed',
        yAxisID: 'y2',
            //data: [12, 19, 3, 5, 2, 3],
            backgroundColor: [
             //   'rgba(255, 99, 132, 1)'//,
             //   'rgba(54, 162, 235, 0.2)',
                'rgba(255, 206, 86, 1)',
             //   'rgba(75, 192, 192, 0.2)',
             //   'rgba(153, 102, 255, 0.2)',
             //   'rgba(255, 159, 64, 0.2)'
            ],
            borderColor: [
             //   'rgba(255, 99, 132, 1)'//,
                //'rgba(54, 162, 235, 1)',
                'rgba(255, 206, 86, 1)',
                //'rgba(75, 192, 192, 1)',
                //'rgba(153, 102, 255, 1)',
                //'rgba(255, 159, 64, 1)'
            ],
            //borderWidth: 1,
            tension: 0.05,
            pointRadius: 1
        }]
    };

const config = {
    type: 'line',
    data,
    options: {
        scales: {
            x: {
                ticks: {
                    display: false
                }
                },
            y0: { // temp
                type: 'linear',                
                position: 'left',                
                id: 'y-0',
                beginAtZero: true,
                min: 0, 
                max:50,
                ticks: {
                    stepSize: 5,                 
                    color: 'rgba(255, 99, 132, 1)'
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
                    color: 'rgba(54, 162, 235, 1)'
                    }
                },
            y2: { //speed
                type: 'linear',                
                position: 'left',
                id: 'y-2',
                beginAtZero: true,
                min: 0, 
                max: 100,
                ticks: {
                    stepSize: 10,                                    
                    color: 'rgba(255, 206, 86, 1)'
                }
                }           
            
        
        }
        //aspectRatio: 1
    }
};

const myChart = new Chart(canvas, config);

function updateChart(){
    async function fetchData(){
        const response = await fetch('data.json');
        const datapoints = await response.json();
        console.log(datapoints);
        return datapoints;
    };

    fetchData().then(datapoints => {
        const time = datapoints.map((time, index) => {
            return time.nr;
        });
        const temp = datapoints.map((temp, index) => {
            return temp.temp;
        });
        const pressure = datapoints.map((temp, index) => {
            return temp.pressure;
        });
        const fanspeed = datapoints.map((temp, index) => {
            return temp.speed;
        });
        console.log(time);
        console.log(temp);

        myChart.config.data.labels = time;
        myChart.config.data.datasets[0].data = temp;
        myChart.config.data.datasets[1].data = pressure;
        myChart.config.data.datasets[2].data = fanspeed;
        myChart.update();
    })

    
};