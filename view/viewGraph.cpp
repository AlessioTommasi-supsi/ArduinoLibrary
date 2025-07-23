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
    
    // Riserva memoria per evitare continue riallocazioni
    var_html.reserve(2048);

    // Creo il contenitore del grafico con dimensioni responsive
    var_html += "<div id='graphWrapper' style='width: 100%; height: 100%; min-height: 400px; position: relative; padding: 20px; box-sizing: border-box;'>";

    // Only show the selector if there are multiple addresses
    if (addresses.size() > 1) {
        var_html += generateSelectorHTML(addresses);
    }
    
    // Aggiungo bottone export CSV per i grafici
    var_html += "<div style='text-align: center; margin: 10px 0;'>";
    var_html += "<button onclick=\"exportCurrentGraphData()\" style='background-color: #2196F3; color: white; padding: 8px 16px; border: none; border-radius: 5px; cursor: pointer; font-size: 14px;'>📊 Export Graph CSV</button>";
    var_html += "</div>";

    // Canvas responsive all'interno del contenitore
    var_html += "<canvas id='myChart' style='width: 100%; height: 100%; display: block; border: 1px solid #ddd; border-radius: 8px;'></canvas>";

    var_html += "</div>";

    // JavaScript diviso in parti più piccole
    var_html += generateBasicJavaScript();
    var_html += generateDrawFunctionJS();
    var_html += generateUpdateFunctionJS(apiFetch, apiFetchParam);
    var_html += generateGraphExportJS(apiFetch, apiFetchParam);
    var_html += generateInitializationJS();

    return var_html;
}

String viewGraph::generateSelectorHTML(std::vector<int> addresses)
{
    String html = "";
    html.reserve(512);
    
    html += "<div style='margin-bottom: 10px; text-align: center;'>";
    html += "<label for='register-select' style='font-weight: bold; margin-right: 8px; font-size: 14px;'>Seleziona:</label>";
    html += "<select id='register-select' onchange='updateGraph()' style='padding: 4px 8px; border: 1px solid #ccc; border-radius: 4px; font-size: 14px;'>";

    std::set<int> seen;
    for (int addr : addresses)
    {
        if (seen.find(addr) != seen.end()) {
            continue;
        }
        seen.insert(addr);

        if (addr > 0 && addr < 1000) {
            html += "<option value='" + String(addr) + "'>" + String(addr) + "</option>";
        } else if (addr <= 0) {
            html += "<option value='" + String(addr) + "'>Multiplex:" + String(-addr) + "</option>";
        } else if (addr >= 1000 && addr < 2000) {
            html += "<option value='" + String(addr - 1000) + "'>ModbusValue:" + String(addr - 1000) + "</option>";
        } else if (addr == 999) {
            html += "<option value='999'>🧪 TEST Mode</option>";
        } else {
            //caso di scritture non valide non le inserisco
            //html += "<option value='" + String(addr) + "'>Errore inserimento!" + String(addr - 3000) + "</option>";
        }
    }

    html += "</select></div>";
    return html;
}

String viewGraph::generateBasicJavaScript()
{
    return "<script>let chartData = [];</script>";
}

String viewGraph::generateDrawFunctionJS()
{
    String js = "";
    js.reserve(1024);
    
    js += "<script>";
    js += "function resizeCanvas() {";
    js += "const canvas = document.getElementById('myChart');";
    js += "if (canvas && canvas.parentElement) {";
    js += "const container = canvas.parentElement;";
    js += "const rect = container.getBoundingClientRect();";
    js += "canvas.width = rect.width - 40;";  // Sottrae il padding
    js += "canvas.height = rect.height - 40;";
    js += "if (chartData.length > 0) { drawChart(canvas, chartData); }";
    js += "}}";
    
    js += "function drawChart(canvas, data) {";
    js += "const ctx = canvas.getContext('2d');";
    js += "const width = canvas.width;";
    js += "const height = canvas.height;";
    js += "const padding = 40;";
    js += "const chartWidth = width - 2 * padding;";
    js += "const chartHeight = height - 2 * padding;";
    js += "ctx.clearRect(0, 0, width, height);";
    js += "if (data.length === 0) {";
    js += "ctx.fillStyle = '#666';";
    js += "ctx.font = '14px Arial';";
    js += "ctx.textAlign = 'center';";
    js += "ctx.fillText('Nessun dato disponibile', width / 2, height / 2);";
    js += "return;";
    js += "}";
    js += "const minValue = Math.min(...data);";
    js += "const maxValue = Math.max(...data);";
    js += "const valueRange = maxValue - minValue || 1;";
    js += "ctx.strokeStyle = '#f0f0f0';";
    js += "ctx.lineWidth = 1;";
    js += "for (let i = 0; i <= 5; i++) {";
    js += "const y = padding + (chartHeight * i / 5);";
    js += "ctx.beginPath();";
    js += "ctx.moveTo(padding, y);";
    js += "ctx.lineTo(width - padding, y);";
    js += "ctx.stroke();";
    js += "}";
    js += "ctx.strokeStyle = '#333';";
    js += "ctx.lineWidth = 2;";
    js += "ctx.beginPath();";
    js += "ctx.moveTo(padding, padding);";
    js += "ctx.lineTo(padding, height - padding);";
    js += "ctx.lineTo(width - padding, height - padding);";
    js += "ctx.stroke();";
    js += "ctx.fillStyle = '#333';";
    js += "ctx.font = '11px Arial';";
    js += "ctx.textAlign = 'right';";
    js += "for (let i = 0; i <= 5; i++) {";
    js += "const value = minValue + (valueRange * i / 5);";
    js += "const y = height - padding - (chartHeight * i / 5);";
    js += "ctx.fillText(value.toFixed(1), padding - 5, y + 3);";
    js += "}";
    js += "ctx.strokeStyle = '#4CAF50';";
    js += "ctx.lineWidth = 2;";
    js += "ctx.beginPath();";
    js += "const stepX = chartWidth / Math.max(data.length - 1, 1);";
    js += "for (let i = 0; i < data.length; i++) {";
    js += "const x = padding + (stepX * i);";
    js += "const normalizedValue = (data[i] - minValue) / valueRange;";
    js += "const y = height - padding - (chartHeight * normalizedValue);";
    js += "if (i === 0) { ctx.moveTo(x, y); } else { ctx.lineTo(x, y); }";
    js += "}";
    js += "ctx.stroke();";
    js += "if (data.length <= 30) {";
    js += "ctx.fillStyle = '#2196F3';";
    js += "for (let i = 0; i < data.length; i++) {";
    js += "const x = padding + (stepX * i);";
    js += "const normalizedValue = (data[i] - minValue) / valueRange;";
    js += "const y = height - padding - (chartHeight * normalizedValue);";
    js += "ctx.beginPath();";
    js += "ctx.arc(x, y, 2, 0, 2 * Math.PI);";
    js += "ctx.fill();";
    js += "}}";
    js += "if (data.length > 0) {";
    js += "const lastValue = data[data.length - 1];";
    js += "ctx.fillStyle = '#2196F3';";
    js += "ctx.font = 'bold 12px Arial';";
    js += "ctx.textAlign = 'left';";
    js += "ctx.fillText('Ultimo: ' + lastValue.toFixed(2), padding, 20);";
    js += "}}";
    js += "</script>";
    
    return js;
}

String viewGraph::generateUpdateFunctionJS(String apiFetch, String apiFetchParam)
{
    String js = "";
    js.reserve(512);
    
    js += "<script>";
    js += "function updateGraph() {";
    js += "const selectElement = document.getElementById('register-select');";
    js += "const address = selectElement ? selectElement.value : 0;";
    js += "fetch('" + apiFetch + "?" + apiFetchParam + "=' + address)";
    js += ".then(response => response.json())";
    js += ".then(data => {";
    js += "chartData = data;";
    js += "const canvas = document.getElementById('myChart');";
    js += "if (canvas) { drawChart(canvas, data); }";
    js += "})";
    js += ".catch(error => {";
    js += "console.error('Error:', error);";
    js += "const canvas = document.getElementById('myChart');";
    js += "if (canvas) {";
    js += "const ctx = canvas.getContext('2d');";
    js += "ctx.clearRect(0, 0, canvas.width, canvas.height);";
    js += "ctx.fillStyle = '#f44336';";
    js += "ctx.font = '14px Arial';";
    js += "ctx.textAlign = 'center';";
    js += "ctx.fillText('Errore nel caricamento dei dati', canvas.width / 2, canvas.height / 2);";
    js += "}});";
    js += "}";
    js += "</script>";
    
    return js;
}

String viewGraph::generateGraphExportJS(String apiFetch, String apiFetchParam)
{
    String js = "";
    js.reserve(512);
    
    js += "<script>";
    js += "function exportCurrentGraphData() {";
    js += "const selectElement = document.getElementById('register-select');";
    js += "const address = selectElement ? selectElement.value : 0;";
    js += "fetch('" + apiFetch + "?" + apiFetchParam + "=' + address)";
    js += ".then(response => response.json())";
    js += ".then(data => {";
    js += "if (data.length === 0) {";
    js += "alert('Nessun dato disponibile per l\\'esportazione.');";
    js += "return;";
    js += "}";
    js += "let csv = 'Index,Value\\n';";
    js += "data.forEach((value, index) => {";
    js += "csv += index + ',' + value + '\\n';";
    js += "});";
    js += "const csvFile = new Blob([csv], { type: 'text/csv' });";
    js += "const downloadLink = document.createElement('a');";
    js += "downloadLink.download = 'graph_data_' + address + '.csv';";
    js += "downloadLink.href = window.URL.createObjectURL(csvFile);";
    js += "downloadLink.style.display = 'none';";
    js += "document.body.appendChild(downloadLink);";
    js += "downloadLink.click();";
    js += "document.body.removeChild(downloadLink);";
    js += "})";
    js += ".catch(error => console.error('Error exporting data:', error));";
    js += "}";
    js += "</script>";
    
    return js;
}

String viewGraph::generateInitializationJS()
{
    String js = "";
    js.reserve(512);
    
    js += "<script>";
    js += "document.addEventListener('DOMContentLoaded', function() {";
    js += "requestAnimationFrame(() => {";
    js += "resizeCanvas();";
    js += "updateGraph();";
    js += "setInterval(updateGraph, 10000);";
    js += "window.addEventListener('resize', () => {";
    js += "setTimeout(resizeCanvas, 100);";
    js += "});";
    js += "});";
    js += "});";
    js += "</script>";
    
    return js;
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

