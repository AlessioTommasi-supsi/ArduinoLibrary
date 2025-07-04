#include "viewGraph.h"
#include <set> 

String viewGraph::html = "";
String viewGraph::generateHTML()
{
    html = viewGeneric::defaultCssHeader("Graph View");

    html += "<h1>Graph View</h1>";

    html += "<h2>Registri:</h2>";
    std::vector<int> addresses = SystemState::getInstance()->getAllRegisterAddress();
    int i = 0;
    for (int addr : addresses)
    {
        addresses[i] += 1000; //operazione di shift
    }
    
    String apiFetch = "getRegisterValues";
    String apiFetchParam = "address";

    html += generateGraph(addresses, apiFetch, apiFetchParam);

    html += viewGeneric::defaultFooter();

    return html;
}


String viewGraph::generateGraph(std::vector<int> addresses, String apiFetch, String apiFetchParam)
{
    String var_html = "";

    // Only show the selector if there are multiple addresses
    if (addresses.size() > 1) {
        // Menu a tendina per selezionare l'indirizzo del registro
        var_html += "<div style='margin-bottom: 10px; text-align: center;'>";
        var_html += "<label for='register-select' style='font-weight: bold; margin-right: 8px; font-size: 14px;'>Seleziona:</label>";
        var_html += "<select id='register-select' onchange='updateGraph()' style='padding: 4px 8px; border: 1px solid #ccc; border-radius: 4px; font-size: 14px;'>";

        std::set<int> seen;

        for (int addr : addresses)
        {
            if (seen.find(addr) != seen.end()) {
                // Se l'indirizzo è già stato aggiunto, salto la generazione dell'option.
                continue;
            }
            seen.insert(addr);

            if (addr > 0 && addr < 1000) {
                var_html += "<option value='" + String(addr) + "'>" + String(addr) + "</option>";
            } else if (addr <= 0) 
            {
                var_html += "<option value='" + String(addr) + "'>Multiplex:" + String(-addr) + "</option>";
            }
            else if (addr >= 1000 && addr < 2000) {
                var_html += "<option value='" + String(addr - 1000) + "'>ModbusValue:" + String(addr - 1000) + "</option>";
            } else if (addr == 999) {
                // TEST MODE - special case
                var_html += "<option value='999'>🧪 TEST Mode</option>";
            } else {
                var_html += "<option value='" + String(addr) + "'>Errore inserimento!" + String(addr - 3000) + "</option>";
            }
        }

        var_html += "</select>";
        var_html += "</div>";
    }

    // Canvas che si adatta perfettamente al container esistente
    var_html += "<canvas id='myChart'></canvas>";

    // Script JavaScript ottimizzato per il container esistente
    var_html += R"(
<script>
let chartData = [];

function resizeCanvas() {
    const canvas = document.getElementById('myChart');
    if (canvas && canvas.parentElement) {
        const container = canvas.parentElement;
        const rect = container.getBoundingClientRect();
        
        // Imposta dimensioni esatte del container
        canvas.width = rect.width;
        canvas.height = rect.height;
        
        // Ridisegna se ci sono dati
        if (chartData.length > 0) {
            drawChart(canvas, chartData);
        }
    }
}

function drawChart(canvas, data) {
    const ctx = canvas.getContext('2d');
    const width = canvas.width;
    const height = canvas.height;
    const padding = 40;
    const chartWidth = width - 2 * padding;
    const chartHeight = height - 2 * padding;
    
    // Pulisce il canvas
    ctx.clearRect(0, 0, width, height);
    
    if (data.length === 0) {
        ctx.fillStyle = '#666';
        ctx.font = '14px Arial';
        ctx.textAlign = 'center';
        ctx.fillText('Nessun dato disponibile', width / 2, height / 2);
        return;
    }
    
    // Scala dinamica per Y
    const minValue = Math.min(...data);
    const maxValue = Math.max(...data);
    const valueRange = maxValue - minValue || 1;
    
    // Griglia di sfondo
    ctx.strokeStyle = '#f0f0f0';
    ctx.lineWidth = 1;
    for (let i = 0; i <= 5; i++) {
        const y = padding + (chartHeight * i / 5);
        ctx.beginPath();
        ctx.moveTo(padding, y);
        ctx.lineTo(width - padding, y);
        ctx.stroke();
    }
    
    // Assi principali
    ctx.strokeStyle = '#333';
    ctx.lineWidth = 2;
    ctx.beginPath();
    ctx.moveTo(padding, padding);
    ctx.lineTo(padding, height - padding);
    ctx.lineTo(width - padding, height - padding);
    ctx.stroke();
    
    // Etichette Y (più compatte)
    ctx.fillStyle = '#333';
    ctx.font = '11px Arial';
    ctx.textAlign = 'right';
    for (let i = 0; i <= 5; i++) {
        const value = minValue + (valueRange * i / 5);
        const y = height - padding - (chartHeight * i / 5);
        ctx.fillText(value.toFixed(1), padding - 5, y + 3);
    }
    
    // Linea del grafico
    ctx.strokeStyle = '#4CAF50';
    ctx.lineWidth = 2;
    ctx.beginPath();
    
    const stepX = chartWidth / Math.max(data.length - 1, 1);
    for (let i = 0; i < data.length; i++) {
        const x = padding + (stepX * i);
        const normalizedValue = (data[i] - minValue) / valueRange;
        const y = height - padding - (chartHeight * normalizedValue);
        
        if (i === 0) {
            ctx.moveTo(x, y);
        } else {
            ctx.lineTo(x, y);
        }
    }
    ctx.stroke();
    
    // Punti (solo se dataset non troppo grande)
    if (data.length <= 30) {
        ctx.fillStyle = '#2196F3';
        for (let i = 0; i < data.length; i++) {
            const x = padding + (stepX * i);
            const normalizedValue = (data[i] - minValue) / valueRange;
            const y = height - padding - (chartHeight * normalizedValue);
            
            ctx.beginPath();
            ctx.arc(x, y, 2, 0, 2 * Math.PI);
            ctx.fill();
        }
    }
    
    // Valore corrente (in alto a sinistra)
    if (data.length > 0) {
        const lastValue = data[data.length - 1];
        ctx.fillStyle = '#2196F3';
        ctx.font = 'bold 12px Arial';
        ctx.textAlign = 'left';
        ctx.fillText('Ultimo: ' + lastValue.toFixed(2), padding, 20);
    }
}

function updateGraph() {
    const selectElement = document.getElementById('register-select');
    const address = selectElement ? selectElement.value : 0;
    
    fetch('/)";
    
    var_html += apiFetch + "?" + apiFetchParam + "=' + address)";
    
    var_html += R"(
        .then(response => response.json())
        .then(data => {
            chartData = data;
            const canvas = document.getElementById('myChart');
            if (canvas) {
                drawChart(canvas, data);
            }
        })
        .catch(error => {
            console.error('Error:', error);
            const canvas = document.getElementById('myChart');
            if (canvas) {
                const ctx = canvas.getContext('2d');
                ctx.clearRect(0, 0, canvas.width, canvas.height);
                ctx.fillStyle = '#f44336';
                ctx.font = '14px Arial';
                ctx.textAlign = 'center';
                ctx.fillText('Errore nel caricamento dei dati', canvas.width / 2, canvas.height / 2);
            }
        });
}

// Inizializzazione quando il DOM è pronto
document.addEventListener('DOMContentLoaded', function() {
    // Aspetta un frame per essere sicuri che il layout sia completo
    requestAnimationFrame(() => {
        resizeCanvas();
        updateGraph();
        
        // Aggiornamento automatico ogni 2 secondi
        setInterval(updateGraph, 2000);
        
        // Ridimensionamento della finestra
        window.addEventListener('resize', () => {
            setTimeout(resizeCanvas, 100);
        });
    });
});
</script>
)";

    return var_html;
}

String viewGraph::initCirularProgressBarGraph()
{
    String var_circle_progressbar_html = "";
    var_circle_progressbar_html += "<div class='circle_progressbar_chart-container' style='font-family: Raleway, sans-serif; display: flex; flex-wrap: wrap; justify-content: space-around; gap: 20px; padding: 20px;'>";
    
    // Definisco le funzioni JavaScript globali una sola volta
    var_circle_progressbar_html += "<script>";
    var_circle_progressbar_html += "if (typeof drawCircularProgress === 'undefined') {";
    var_circle_progressbar_html += "  function drawCircularProgress(canvas, percentage) {";
    var_circle_progressbar_html += "    const ctx = canvas.getContext('2d');";
    var_circle_progressbar_html += "    const centerX = canvas.width / 2;";
    var_circle_progressbar_html += "    const centerY = canvas.height / 2;";
    var_circle_progressbar_html += "    const radius = 70;";
    var_circle_progressbar_html += "    const lineWidth = 15;";
    var_circle_progressbar_html += "    ";
    var_circle_progressbar_html += "    ctx.clearRect(0, 0, canvas.width, canvas.height);";
    var_circle_progressbar_html += "    ";
    var_circle_progressbar_html += "    ctx.beginPath();";
    var_circle_progressbar_html += "    ctx.arc(centerX, centerY, radius, 0, 2 * Math.PI);";
    var_circle_progressbar_html += "    ctx.lineWidth = lineWidth;";
    var_circle_progressbar_html += "    ctx.strokeStyle = '#ddd';";
    var_circle_progressbar_html += "    ctx.stroke();";
    var_circle_progressbar_html += "    ";
    var_circle_progressbar_html += "    const startAngle = -Math.PI / 2;";
    var_circle_progressbar_html += "    const endAngle = startAngle + (2 * Math.PI * percentage / 100);";
    var_circle_progressbar_html += "    ";
    var_circle_progressbar_html += "    ctx.beginPath();";
    var_circle_progressbar_html += "    ctx.arc(centerX, centerY, radius, startAngle, endAngle);";
    var_circle_progressbar_html += "    ctx.lineWidth = lineWidth;";
    var_circle_progressbar_html += "    ctx.strokeStyle = '#4caf50';";
    var_circle_progressbar_html += "    ctx.lineCap = 'round';";
    var_circle_progressbar_html += "    ctx.stroke();";
    var_circle_progressbar_html += "  }";
    var_circle_progressbar_html += "  ";
    var_circle_progressbar_html += "  function updateCircularChart(apiUrl, canvasId, labelId) {";
    var_circle_progressbar_html += "    fetch(apiUrl)";
    var_circle_progressbar_html += "      .then(response => response.json())";
    var_circle_progressbar_html += "      .then(data => {";
    var_circle_progressbar_html += "        const percentage = (data.used / data.total) * 100;";
    var_circle_progressbar_html += "        const canvas = document.getElementById(canvasId);";
    var_circle_progressbar_html += "        if (canvas) {";
    var_circle_progressbar_html += "          drawCircularProgress(canvas, percentage);";
    var_circle_progressbar_html += "        }";
    var_circle_progressbar_html += "        const label = document.getElementById(labelId);";
    var_circle_progressbar_html += "        if (label) {";
    var_circle_progressbar_html += "          label.innerText = percentage.toFixed(1) + '%';";
    var_circle_progressbar_html += "        }";
    var_circle_progressbar_html += "      })";
    var_circle_progressbar_html += "      .catch(error => console.error('Error fetching data:', error));";
    var_circle_progressbar_html += "  }";
    var_circle_progressbar_html += "}";
    var_circle_progressbar_html += "</script>";
    
    return var_circle_progressbar_html;
}

String viewGraph::endCirularProgressBarGraph()
{
    String var_circle_progressbar_html = "";
    var_circle_progressbar_html += "</div>";
    return var_circle_progressbar_html;
}

String viewGraph::generateCirularProgressBarGraph(String circle_progressbar_label, float circle_progressbar_used_value, float circle_progressbar_total_value, String apiFetchData, int timeToUpdate)
{
    String var_circle_progressbar_html = "";
    float percentage = (circle_progressbar_used_value / circle_progressbar_total_value) * 100;
    
    var_circle_progressbar_html += "<div class='circle_progressbar_chart-wrapper' style='display: flex; flex-direction: column; align-items: center; width: 45%; max-width: 300px; min-width: 150px; position: relative;'>";
    var_circle_progressbar_html += "  <div class='circle_progressbar_label' style='font-size: 1.2rem; margin-bottom: 10px;'>" + circle_progressbar_label + "</div>";
    var_circle_progressbar_html += "  <canvas id='circle_progressbar_chart_" + circle_progressbar_label + "' class='circle_progressbar_chart' style='width: 200px; height: 200px;' width='200' height='200'></canvas>";
    var_circle_progressbar_html += "  <div class='circle_progressbar_center-label' id='label" + circle_progressbar_label + "' style='position: absolute; top: 50%; left: 50%; transform: translate(-50%, -50%); font-size: 2rem; font-weight: bold; color: #000; text-shadow: 1px 1px 2px rgba(0, 0, 0, 0.2);'>" + String((int)percentage) + "%</div>";
    var_circle_progressbar_html += "  <div class='circle_progressbar_percentage' style='font-size: 1rem; margin-top: 10px;'>Utilizzato: " + String(circle_progressbar_used_value) + " / Totale: " + String(circle_progressbar_total_value) + "</div>";
    var_circle_progressbar_html += "</div>";
    
    var_circle_progressbar_html += "<script>";
    var_circle_progressbar_html += "document.addEventListener('DOMContentLoaded', function() {";
    var_circle_progressbar_html += "  const canvas = document.getElementById('circle_progressbar_chart_" + circle_progressbar_label + "');";
    var_circle_progressbar_html += "  if (canvas && typeof drawCircularProgress === 'function') {";
    var_circle_progressbar_html += "    const percentage = " + String(percentage) + ";";
    var_circle_progressbar_html += "    drawCircularProgress(canvas, percentage);";
    var_circle_progressbar_html += "  }";
    var_circle_progressbar_html += "  ";
    var_circle_progressbar_html += "  if (" + String(timeToUpdate) + " > 0) {";
    var_circle_progressbar_html += "    setInterval(function() {";
    var_circle_progressbar_html += "      if (typeof updateCircularChart === 'function') {";
    var_circle_progressbar_html += "        updateCircularChart('" + apiFetchData + "', 'circle_progressbar_chart_" + circle_progressbar_label + "', 'label" + circle_progressbar_label + "');";
    var_circle_progressbar_html += "      }";
    var_circle_progressbar_html += "    }, " + String(timeToUpdate) + ");";
    var_circle_progressbar_html += "  }";
    var_circle_progressbar_html += "});";
    var_circle_progressbar_html += "</script>";
    
    return var_circle_progressbar_html;
}

String viewGraph::generateCirularProgressBarGraph(String circle_progressbar_label, float circle_progressbar_used_value, float circle_progressbar_total_value)
{
    String var_circle_progressbar_html = "";
    float percentage = (circle_progressbar_used_value / circle_progressbar_total_value) * 100;
    
    var_circle_progressbar_html += "<div class='circle_progressbar_chart-wrapper' style='display: flex; flex-direction: column; align-items: center; width: 45%; max-width: 300px; min-width: 150px; position: relative;'>";
    var_circle_progressbar_html += "  <div class='circle_progressbar_label' style='font-size: 1.2rem; margin-bottom: 10px;'>" + circle_progressbar_label + "</div>";
    var_circle_progressbar_html += "  <canvas id='circle_progressbar_chart_" + circle_progressbar_label + "' class='circle_progressbar_chart' style='width: 200px; height: 200px;' width='200' height='200'></canvas>";
    var_circle_progressbar_html += "  <div class='circle_progressbar_center-label' id='label" + circle_progressbar_label + "' style='position: absolute; top: 50%; left: 50%; transform: translate(-50%, -50%); font-size: 2rem; font-weight: bold; color: #000; text-shadow: 1px 1px 2px rgba(0, 0, 0, 0.2);'>" + String((int)percentage) + "%</div>";
    var_circle_progressbar_html += "  <div class='circle_progressbar_percentage' style='font-size: 1rem; margin-top: 10px;'>Utilizzato: " + String(circle_progressbar_used_value) + " / Totale: " + String(circle_progressbar_total_value) + "</div>";
    var_circle_progressbar_html += "</div>";
    
    var_circle_progressbar_html += "<script>";
    var_circle_progressbar_html += "document.addEventListener('DOMContentLoaded', function() {";
    var_circle_progressbar_html += "  const canvas = document.getElementById('circle_progressbar_chart_" + circle_progressbar_label + "');";
    var_circle_progressbar_html += "  if (canvas && typeof drawCircularProgress === 'function') {";
    var_circle_progressbar_html += "    const percentage = " + String(percentage) + ";";
    var_circle_progressbar_html += "    drawCircularProgress(canvas, percentage);";
    var_circle_progressbar_html += "  }";
    var_circle_progressbar_html += "});";
    var_circle_progressbar_html += "</script>";

    return var_circle_progressbar_html;
}

